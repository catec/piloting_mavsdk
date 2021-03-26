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
MAVLinkInspectionTransferRoboticVehicle::upload_items_async(const TasksPlan &plan, ResultAndAckCallback callback)
{
    auto ptr = std::make_shared<UploadWorkItem>(
        _sender, _message_handler, _timeout_handler, plan, callback);

    _work_queue.push_back(ptr);

    return std::weak_ptr<WorkItem>(ptr);
}

std::weak_ptr<MAVLinkInspectionTransfer::WorkItem>
MAVLinkInspectionTransferRoboticVehicle::download_items_async(
    const uint16_t mission_id, const uint16_t count, ResultAndPlanCallback callback)
{
    auto ptr = std::make_shared<DownloadWorkItem>(
        _sender, _message_handler, _timeout_handler, mission_id, count, callback);

    _work_queue.push_back(ptr);

    return std::weak_ptr<WorkItem>(ptr);
}

MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::UploadWorkItem(Sender& sender,
    MAVLinkMessageHandler& message_handler,
    TimeoutHandler& timeout_handler,
    const TasksPlan &plan,
    ResultAndAckCallback callback) :
    WorkItem(sender, message_handler, timeout_handler),
    _plan(plan),
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
        MAVLINK_MSG_ID_INSPECTION_TASKS_ACK,
        [this](const mavlink_message_t& message) { process_inspection_ack(message); },
        this);

    _retries_done = 0;
    _step = Step::SendCount;
    _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);

    _started = true;
    if (_plan.items.size() == 0) {
        send_count();
        return;
    }

    int count = 0;
    for (const auto& item : _plan.items) {
        if (count++ != item.seq) {
            callback_and_reset(Result::InvalidSequence);
            return;
        }
    }

    _next_sequence = 0;

    _message_handler.register_one(
        MAVLINK_MSG_ID_INSPECTION_TASKS_READ,
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
    mavlink_msg_inspection_tasks_count_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        _plan.mission_id,
        _plan.items.size());

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
    mavlink_msg_inspection_tasks_ack_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        INSPECTION_TASKS_CANCELLED);

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

    mavlink_inspection_tasks_read_t inspection_tasks_read;
    mavlink_msg_inspection_tasks_read_decode(&message, &inspection_tasks_read);

    _step = Step::SendItems;

    if (_next_sequence < inspection_tasks_read.seq) {
        // We should not go back to a previous one.
        // TODO: figure out if we should error here.
        LogWarn() << "inspection_tasks_read: sequence incorrect";
        return;

    } else if (_next_sequence > inspection_tasks_read.seq) {
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

    _next_sequence = inspection_tasks_read.seq;
    send_inspection_item();
}

void MAVLinkInspectionTransferRoboticVehicle::UploadWorkItem::send_inspection_item()
{
    if (_next_sequence >= _plan.items.size()) {
        LogErr() << "send_inspection_item: sequence out of bounds";
        return;
    }

    const auto item = _plan.items[_next_sequence];

    mavlink_message_t message;
    mavlink_msg_inspection_tasks_item_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        _next_sequence,
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

    mavlink_inspection_tasks_ack_t inspection_tasks_ack;
    mavlink_msg_inspection_tasks_ack_decode(&message, &inspection_tasks_ack);

    _timeout_handler.remove(_cookie);

    switch (inspection_tasks_ack.type) {
        case INSPECTION_TASKS_ACCEPTED:
            _ack = Ack::Accepted;
            break;
        case INSPECTION_TASKS_ERROR:
            _ack = Ack::Error;
            break;
        case INSPECTION_TASKS_UNSUPPORTED:
            _ack = Ack::Unsupported;
            break;
        case INSPECTION_TASKS_NO_SPACE:
            _ack = Ack::NoSpace;
            break;
        case INSPECTION_TASKS_INVALID:
            _ack = Ack::Invalid;
            break;
        case INSPECTION_TASKS_INVALID_PARAM1:
            _ack = Ack::InvalidParam1;
            break;
        case INSPECTION_TASKS_INVALID_PARAM2:
            _ack = Ack::InvalidParam2;
            break;
        case INSPECTION_TASKS_INVALID_PARAM3:
            _ack = Ack::InvalidParam3;
            break;
        case INSPECTION_TASKS_INVALID_PARAM4:
            _ack = Ack::InvalidParam4;
            break;
        case INSPECTION_TASKS_INVALID_PARAM5:
            _ack = Ack::InvalidParam5;
            break;
        case INSPECTION_TASKS_INVALID_PARAM6:
            _ack = Ack::InvalidParam6;
            break;
        case INSPECTION_TASKS_INVALID_PARAM7:
            _ack = Ack::InvalidParam7;
            break;
        case INSPECTION_TASKS_INVALID_SEQUENCE:
            _ack = Ack::InvalidSequence;
            break;
        case INSPECTION_TASKS_CANCELLED:
            _ack = Ack::Cancelled;
            break;
        default:
            _ack = Ack::Unknown;
            break;
    }

    if (_next_sequence == _plan.items.size()) {
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
    const uint16_t mission_id,
    const uint16_t count,
    ResultAndPlanCallback callback) :
    WorkItem(sender, message_handler, timeout_handler),
    _callback(callback),
    _expected_count(count)
{
    _plan.mission_id = mission_id;
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
        MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM,
        [this](const mavlink_message_t& message) { process_inspection_item_int(message); },
        this);

    if (_expected_count == 0) {
        send_ack_and_finish();
        return;
    }

    _plan.items.clear();
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
    mavlink_msg_inspection_tasks_read_pack(
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

void MAVLinkInspectionTransferRoboticVehicle::DownloadWorkItem::send_ack_and_finish()
{
    mavlink_message_t message;
    mavlink_msg_inspection_tasks_ack_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        INSPECTION_TASKS_ACCEPTED);

    if (!_sender.send_message(message)) {
        callback_and_reset(Result::ConnectionError);
        return;
    }

    // We do not wait on anything coming back after this.
    callback_and_reset(Result::Success);
}

void MAVLinkInspectionTransferRoboticVehicle::DownloadWorkItem::send_cancel_and_finish()
{
    mavlink_message_t message;
    mavlink_msg_inspection_tasks_ack_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        INSPECTION_TASKS_CANCELLED);

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

    mavlink_inspection_tasks_item_t inspection_tasks_item;
    mavlink_msg_inspection_tasks_item_decode(&message, &inspection_tasks_item);

    _plan.items.push_back(TasksItem{inspection_tasks_item.seq,
                                    inspection_tasks_item.command,
                                    inspection_tasks_item.autocontinue,
                                    inspection_tasks_item.param1,
                                    inspection_tasks_item.param2,
                                    inspection_tasks_item.param3,
                                    inspection_tasks_item.param4,
                                    inspection_tasks_item.x,
                                    inspection_tasks_item.y,
                                    inspection_tasks_item.z});

    if (_next_sequence + 1 == _expected_count) {
        _timeout_handler.remove(_cookie);
        send_ack_and_finish();

    } else {
        _next_sequence = inspection_tasks_item.seq + 1;
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
        _callback(result, _plan);
    }
    _callback = nullptr;
    _done = true;
}

} // namespace mavsdk
