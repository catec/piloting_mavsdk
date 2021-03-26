#include <algorithm>
#include "mavlink_checklist_transfer_robotic_vehicle.h"
#include "log.h"

namespace mavsdk {

MAVLinkChecklistTransferRoboticVehicle::MAVLinkChecklistTransferRoboticVehicle(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    MAVLinkChecklistTransfer(sender, message_handler, timeout_handler)
{}

MAVLinkChecklistTransferRoboticVehicle::~MAVLinkChecklistTransferRoboticVehicle() {}

std::weak_ptr<MAVLinkChecklistTransfer::WorkItem>
MAVLinkChecklistTransferRoboticVehicle::upload_items_async(
    const Checklist& checklist, ResultAndAckCallback callback)
{
    auto ptr = std::make_shared<UploadWorkItem>(
        _sender, _message_handler, _timeout_handler, checklist, callback);

    _work_queue.push_back(ptr);

    return std::weak_ptr<WorkItem>(ptr);
}

MAVLinkChecklistTransferRoboticVehicle::UploadWorkItem::UploadWorkItem(
    Sender& sender,
    MAVLinkMessageHandler& message_handler,
    TimeoutHandler& timeout_handler,
    const Checklist& checklist,
    ResultAndAckCallback callback) :
    WorkItem(sender, message_handler, timeout_handler),
    _checklist(checklist),
    _callback(callback)
{}

MAVLinkChecklistTransferRoboticVehicle::UploadWorkItem::~UploadWorkItem()
{
    std::lock_guard<std::mutex> lock(_mutex);
    _message_handler.unregister_all(this);
    _timeout_handler.remove(_cookie);
}

void MAVLinkChecklistTransferRoboticVehicle::UploadWorkItem::start()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _started = true;

    _message_handler.register_one(
        MAVLINK_MSG_ID_CHECK_LIST_ACK,
        [this](const mavlink_message_t& message) { process_checklist_ack(message); },
        this);

    _retries_done = 0;
    _step = Step::SendCount;
    _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);

    _started = true;
    if (_checklist.items.size() == 0) {
        send_count();
        return;
    }

    int count = 0;
    for (const auto& item : _checklist.items) {
        if (count++ != item.index) {
            callback_and_reset(Result::InvalidSequence);
            return;
        }
    }

    _next_sequence = 0;

    _message_handler.register_one(
        MAVLINK_MSG_ID_CHECK_LIST_READ,
        [this](const mavlink_message_t& message) { process_checklist_read(message); },
        this);

    send_count();
}

void MAVLinkChecklistTransferRoboticVehicle::UploadWorkItem::cancel()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _timeout_handler.remove(_cookie);
    send_cancel_and_finish();
}

void MAVLinkChecklistTransferRoboticVehicle::UploadWorkItem::send_count()
{
    LogDebug() << "Sending count";

    mavlink_message_t message;
    mavlink_msg_check_list_count_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        _checklist.items.size());

    if (!_sender.send_message(message)) {
        _timeout_handler.remove(_cookie);
        callback_and_reset(Result::ConnectionError);
        return;
    }

    ++_retries_done;
}

void MAVLinkChecklistTransferRoboticVehicle::UploadWorkItem::send_cancel_and_finish()
{
    mavlink_message_t message;
    mavlink_msg_check_list_ack_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        CHECK_LIST_CANCELLED);

    if (!_sender.send_message(message)) {
        callback_and_reset(Result::ConnectionError);
        return;
    }

    // We do not wait on anything coming back after this.
    callback_and_reset(Result::Cancelled);
}

void MAVLinkChecklistTransferRoboticVehicle::UploadWorkItem::process_checklist_read(
    const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    mavlink_check_list_read_t checklist_read;
    mavlink_msg_check_list_read_decode(&message, &checklist_read);

    _step = Step::SendItems;

    if (_next_sequence < checklist_read.index) {
        // We should not go back to a previous one.
        // TODO: figure out if we should error here.
        LogWarn() << "checklist_read: sequence incorrect";
        return;

    } else if (_next_sequence > checklist_read.index) {
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

    _next_sequence = checklist_read.index;
    send_checklist_item();
}

void MAVLinkChecklistTransferRoboticVehicle::UploadWorkItem::send_checklist_item()
{
    if (_next_sequence >= _checklist.items.size()) {
        LogErr() << "send_checklist_item: sequence out of bounds";
        return;
    }

    const auto item = _checklist.items[_next_sequence];

    mavlink_message_t message;
    mavlink_msg_check_list_item_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        _next_sequence,
        item.name.c_str(),
        item.description.c_str());

    ++_next_sequence;

    if (!_sender.send_message(message)) {
        _timeout_handler.remove(_cookie);
        callback_and_reset(Result::ConnectionError);
        return;
    }

    ++_retries_done;
}

void MAVLinkChecklistTransferRoboticVehicle::UploadWorkItem::process_checklist_ack(
    const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    mavlink_check_list_ack_t checklist_ack;
    mavlink_msg_check_list_ack_decode(&message, &checklist_ack);

    _timeout_handler.remove(_cookie);

    switch (checklist_ack.type) {
        case CHECK_LIST_ACCEPTED:
            _ack = Ack::Accepted;
            break;
        case CHECK_LIST_ERROR:
            _ack = Ack::Error;
            break;
        case CHECK_LIST_NO_SPACE:
            _ack = Ack::NoSpace;
            break;
        case CHECK_LIST_INVALID_SEQUENCE:
            _ack = Ack::InvalidSequence;
            break;
        case CHECK_LIST_CANCELLED:
            _ack = Ack::Cancelled;
            break;
        default:
            _ack = Ack::Unknown;
            break;
    }

    if (_next_sequence == _checklist.items.size()) {
        callback_and_reset(Result::Success);
    } else {
        callback_and_reset(Result::ProtocolError);
    }
}

void MAVLinkChecklistTransferRoboticVehicle::UploadWorkItem::process_timeout()
{
    std::lock_guard<std::mutex> lock(_mutex);

    callback_and_reset(Result::Timeout);
}

void MAVLinkChecklistTransferRoboticVehicle::UploadWorkItem::callback_and_reset(Result result)
{
    if (_callback) {
        _callback(result, _ack);
    }
    _callback = nullptr;
    _done = true;
}

} // namespace mavsdk
