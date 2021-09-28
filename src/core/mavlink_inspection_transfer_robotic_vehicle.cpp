#include <algorithm>
#include "mavlink_inspection_transfer_robotic_vehicle.h"
#include "log.h"

namespace mavsdk {

MAVLinkInspectionTransferRoboticVehicle::MAVLinkInspectionTransferRoboticVehicle(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    MAVLinkInspectionTransfer(sender, message_handler, timeout_handler)
{}

MAVLinkInspectionTransferRoboticVehicle::~MAVLinkInspectionTransferRoboticVehicle() {}

std::weak_ptr<MAVLinkInspectionTransfer::WorkItem>
MAVLinkInspectionTransferRoboticVehicle::upload_items_async(const WaypointList &list, ResultAndAckCallback callback)
{
    auto ptr = std::make_shared<UploadWorkItem>(
        _sender, _message_handler, _timeout_handler, list, callback);

    _work_queue.push_back(ptr);

    return std::weak_ptr<WorkItem>(ptr);
}

std::weak_ptr<MAVLinkInspectionTransfer::WorkItem>
MAVLinkInspectionTransferRoboticVehicle::download_items_async(
    const uint16_t count, ResultAndListCallback callback)
{
    auto ptr = std::make_shared<DownloadWorkItem>(
        _sender, _message_handler, _timeout_handler, count, callback);

    _work_queue.push_back(ptr);

    return std::weak_ptr<WorkItem>(ptr);
}

MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::UploadWorkItem(Sender& sender,
    MAVLinkMessageHandler& message_handler,
    TimeoutHandler& timeout_handler,
    const WaypointList &list,
    ResultAndAckCallback callback) :
    WorkItem(sender, message_handler, timeout_handler),
    _list(list),
    _callback(callback)
{}

MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::~UploadWorkItem()
{
    std::lock_guard<std::mutex> lock(_mutex);
    _message_handler.unregister_all(this);
    _timeout_handler.remove(_cookie);
}

void MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::start()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _started = true;

    _message_handler.register_one(
        MAVLINK_MSG_ID_WAYPOINT_LIST_ACK,
        [this](const mavlink_message_t& message) { process_inspection_ack(message); },
        this);

    _retries_done = 0;
    _step = Step::SendCount;
    _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);

    _started = true;
    if (_list.items.size() == 0) {
        send_count();
        return;
    }

    int count = 0;
    for (const auto& item : _list.items) {
        if (count++ != item.seq) {
            callback_and_reset(Result::InvalidSequence);
            return;
        }
    }

    _next_sequence = 0;

    _message_handler.register_one(
        MAVLINK_MSG_ID_WAYPOINT_LIST_READ,
        [this](const mavlink_message_t& message) { process_inspection_read(message); },
        this);

    send_count();
}

void MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::cancel()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _timeout_handler.remove(_cookie);
    send_cancel_and_finish();
}

void MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::send_count()
{
    LogDebug() << "Sending count";

    mavlink_message_t message;
    mavlink_msg_waypoint_list_count_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        _list.plan_id,
        _list.items.size());

    if (!_sender.send_message(message)) {
        _timeout_handler.remove(_cookie);
        callback_and_reset(Result::ConnectionError);
        return;
    }

    ++_retries_done;
}

void MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::send_cancel_and_finish()
{
    mavlink_message_t message;
    mavlink_msg_waypoint_list_ack_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        WAYPOINT_LIST_CANCELLED);

    if (!_sender.send_message(message)) {
        callback_and_reset(Result::ConnectionError);
        return;
    }

    // We do not wait on anything coming back after this.
    callback_and_reset(Result::Cancelled);
}

void MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::process_inspection_read(
    const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    mavlink_waypoint_list_read_t waypoint_list_read;
    mavlink_msg_waypoint_list_read_decode(&message, &waypoint_list_read);

    _step = Step::SendItems;

    if (_next_sequence < waypoint_list_read.seq) {
        // We should not go back to a previous one.
        // TODO: figure out if we should error here.
        LogWarn() << "waypoint_list_read: sequence incorrect";
        return;

    } else if (_next_sequence > waypoint_list_read.seq) {
        // We have already sent that one before.
        if (_retries_done >= retries) {
            _timeout_handler.remove(_cookie);
            callback_and_reset(Result::Timeout);
            return;
        }

    } else {
        // Correct one, sending it the first time.
        _retries_done = 0;
    }

    _timeout_handler.refresh(_cookie);

    _next_sequence = waypoint_list_read.seq;
    send_inspection_item();
}

void MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::send_inspection_item()
{
    if (_next_sequence >= _list.items.size()) {
        LogErr() << "send_inspection_item: sequence out of bounds";
        return;
    }

    const auto item = _list.items[_next_sequence];

    mavlink_message_t message;
    mavlink_msg_waypoint_list_item_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        _next_sequence,
        item.task_id,
        item.command,
        item.autocontinue,
        item.param1,
        item.param2,
        item.param3,
        item.param4,
        item.x,
        item.y,
        item.z);

    ++_next_sequence;

    if (!_sender.send_message(message)) {
        _timeout_handler.remove(_cookie);
        callback_and_reset(Result::ConnectionError);
        return;
    }

    ++_retries_done;
}

void MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::process_inspection_ack(
    const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    mavlink_waypoint_list_ack_t waypoint_list_ack;
    mavlink_msg_waypoint_list_ack_decode(&message, &waypoint_list_ack);

    _timeout_handler.remove(_cookie);

    switch (waypoint_list_ack.type) {
        case WAYPOINT_LIST_ACCEPTED:
            _ack = Ack::Accepted;
            break;
        case WAYPOINT_LIST_ERROR:
            _ack = Ack::Error;
            break;
        case WAYPOINT_LIST_UNSUPPORTED:
            _ack = Ack::Unsupported;
            break;
        case WAYPOINT_LIST_NO_SPACE:
            _ack = Ack::NoSpace;
            break;
        case WAYPOINT_LIST_INVALID:
            _ack = Ack::Invalid;
            break;
        case WAYPOINT_LIST_INVALID_PARAM1:
            _ack = Ack::InvalidParam1;
            break;
        case WAYPOINT_LIST_INVALID_PARAM2:
            _ack = Ack::InvalidParam2;
            break;
        case WAYPOINT_LIST_INVALID_PARAM3:
            _ack = Ack::InvalidParam3;
            break;
        case WAYPOINT_LIST_INVALID_PARAM4:
            _ack = Ack::InvalidParam4;
            break;
        case WAYPOINT_LIST_INVALID_PARAM5:
            _ack = Ack::InvalidParam5;
            break;
        case WAYPOINT_LIST_INVALID_PARAM6:
            _ack = Ack::InvalidParam6;
            break;
        case WAYPOINT_LIST_INVALID_PARAM7:
            _ack = Ack::InvalidParam7;
            break;
        case WAYPOINT_LIST_INVALID_SEQUENCE:
            _ack = Ack::InvalidSequence;
            break;
        case WAYPOINT_LIST_CANCELLED:
            _ack = Ack::Cancelled;
            break;
        default:
            _ack = Ack::Unknown;
            break;
    }

    if (_next_sequence == _list.items.size()) {
        callback_and_reset(Result::Success);
    } else {
        callback_and_reset(Result::ProtocolError);
    }
}

void MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::process_timeout()
{
    std::lock_guard<std::mutex> lock(_mutex);

    callback_and_reset(Result::Timeout);
}

void MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::callback_and_reset(Result result)
{
    if (_callback) {
        _callback(result, _ack);
    }
    _callback = nullptr;
    _done = true;
}

MAVLinkInspectionTransferRoboticVehicle::DownloadWorkItem::DownloadWorkItem(Sender& sender,
    MAVLinkMessageHandler& message_handler,
    TimeoutHandler& timeout_handler,
    const uint16_t count,
    ResultAndListCallback callback) :
    WorkItem(sender, message_handler, timeout_handler),
    _callback(callback),
    _expected_count(count)
{
}

MAVLinkInspectionTransferRoboticVehicle::DownloadWorkItem::~DownloadWorkItem()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _message_handler.unregister_all(this);
    _timeout_handler.remove(_cookie);
}

void MAVLinkInspectionTransferRoboticVehicle::DownloadWorkItem::start()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _started = true;

    _message_handler.register_one(
        MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM,
        [this](const mavlink_message_t& message) { process_inspection_item_int(message); },
        this);

    if (_expected_count == 0) {
        callback_and_reset(Result::Success);
        return;
    }

    _list.items.clear();
    _retries_done = 0;
    _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);

    _next_sequence = 0;

    request_item();
}

void MAVLinkInspectionTransferRoboticVehicle::DownloadWorkItem::cancel()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _timeout_handler.remove(_cookie);
    send_cancel_and_finish();
}

void MAVLinkInspectionTransferRoboticVehicle::DownloadWorkItem::request_item()
{
    mavlink_message_t message;
    mavlink_msg_waypoint_list_read_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        _next_sequence);

    if (!_sender.send_message(message)) {
        _timeout_handler.remove(_cookie);
        callback_and_reset(Result::ConnectionError);
        return;
    }

    ++_retries_done;
}

void MAVLinkInspectionTransferRoboticVehicle::DownloadWorkItem::send_cancel_and_finish()
{
    mavlink_message_t message;
    mavlink_msg_waypoint_list_ack_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        WAYPOINT_LIST_CANCELLED);

    if (!_sender.send_message(message)) {
        callback_and_reset(Result::ConnectionError);
        return;
    }

    // We do not wait on anything coming back after this.
    callback_and_reset(Result::Cancelled);
}

void MAVLinkInspectionTransferRoboticVehicle::DownloadWorkItem::process_inspection_item_int(
    const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _timeout_handler.refresh(_cookie);

    mavlink_waypoint_list_item_t waypoint_list_item;
    mavlink_msg_waypoint_list_item_decode(&message, &waypoint_list_item);

    _list.items.push_back(WaypointItem{waypoint_list_item.seq,
                                       waypoint_list_item.task_id,
                                       waypoint_list_item.command,
                                       waypoint_list_item.autocontinue,
                                       waypoint_list_item.param1,
                                       waypoint_list_item.param2,
                                       waypoint_list_item.param3,
                                       waypoint_list_item.param4,
                                       waypoint_list_item.x,
                                       waypoint_list_item.y,
                                       waypoint_list_item.z});

    if (_next_sequence + 1 == _expected_count) {
        _timeout_handler.remove(_cookie);
        callback_and_reset(Result::Success);

    } else {
        _next_sequence = waypoint_list_item.seq + 1;
        _retries_done = 0;
        request_item();
    }
}

void MAVLinkInspectionTransferRoboticVehicle::DownloadWorkItem::process_timeout()
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_retries_done >= retries) {
        callback_and_reset(Result::Timeout);
        return;
    }

    _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);
    request_item();
}

void MAVLinkInspectionTransferRoboticVehicle::DownloadWorkItem::callback_and_reset(Result result)
{
    if (_callback) {
        _callback(result, _list);
    }
    _callback = nullptr;
    _done = true;
}

} // namespace mavsdk
