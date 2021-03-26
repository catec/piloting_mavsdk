#include <algorithm>
#include "mavlink_hl_action_transfer_robotic_vehicle.h"
#include "log.h"

namespace mavsdk {

MAVLinkHLActionTransferRoboticVehicle::MAVLinkHLActionTransferRoboticVehicle(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    MAVLinkHLActionTransfer(sender, message_handler, timeout_handler)
{}

MAVLinkHLActionTransferRoboticVehicle::~MAVLinkHLActionTransferRoboticVehicle() {}

std::weak_ptr<MAVLinkHLActionTransfer::WorkItem>
MAVLinkHLActionTransferRoboticVehicle::upload_items_async(
    const HLActionList& hl_action_list, ResultAndAckCallback callback)
{
    auto ptr = std::make_shared<UploadWorkItem>(
        _sender, _message_handler, _timeout_handler, hl_action_list, callback);

    _work_queue.push_back(ptr);

    return std::weak_ptr<WorkItem>(ptr);
}

MAVLinkHLActionTransferRoboticVehicle::UploadWorkItem::UploadWorkItem(
    Sender& sender,
    MAVLinkMessageHandler& message_handler,
    TimeoutHandler& timeout_handler,
    const HLActionList& hl_action_list,
    ResultAndAckCallback callback) :
    WorkItem(sender, message_handler, timeout_handler),
    _hl_action_list(hl_action_list),
    _callback(callback)
{}

MAVLinkHLActionTransferRoboticVehicle::UploadWorkItem::~UploadWorkItem()
{
    std::lock_guard<std::mutex> lock(_mutex);
    _message_handler.unregister_all(this);
    _timeout_handler.remove(_cookie);
}

void MAVLinkHLActionTransferRoboticVehicle::UploadWorkItem::start()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _started = true;

    _message_handler.register_one(
        MAVLINK_MSG_ID_HL_ACTION_LIST_ACK,
        [this](const mavlink_message_t& message) { process_hl_action_ack(message); },
        this);

    _retries_done = 0;
    _step = Step::SendCount;
    _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);

    _started = true;
    if (_hl_action_list.items.size() == 0) {
        send_count();
        return;
    }

    int count = 0;
    for (const auto& item : _hl_action_list.items) {
        if (count++ != item.index) {
            callback_and_reset(Result::InvalidSequence);
            return;
        }
    }

    _next_sequence = 0;

    _message_handler.register_one(
        MAVLINK_MSG_ID_HL_ACTION_LIST_READ,
        [this](const mavlink_message_t& message) { process_hl_action_read(message); },
        this);

    send_count();
}

void MAVLinkHLActionTransferRoboticVehicle::UploadWorkItem::cancel()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _timeout_handler.remove(_cookie);
    send_cancel_and_finish();
}

void MAVLinkHLActionTransferRoboticVehicle::UploadWorkItem::send_count()
{
    LogDebug() << "Sending count";

    mavlink_message_t message;
    mavlink_msg_hl_action_list_count_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        _hl_action_list.items.size());

    if (!_sender.send_message(message)) {
        _timeout_handler.remove(_cookie);
        callback_and_reset(Result::ConnectionError);
        return;
    }

    ++_retries_done;
}

void MAVLinkHLActionTransferRoboticVehicle::UploadWorkItem::send_cancel_and_finish()
{
    mavlink_message_t message;
    mavlink_msg_hl_action_list_ack_pack(
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

void MAVLinkHLActionTransferRoboticVehicle::UploadWorkItem::process_hl_action_read(
    const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    mavlink_hl_action_list_read_t hl_action_read;
    mavlink_msg_hl_action_list_read_decode(&message, &hl_action_read);

    _step = Step::SendItems;

    if (_next_sequence < hl_action_read.index) {
        // We should not go back to a previous one.
        // TODO: figure out if we should error here.
        LogWarn() << "hl_action_read: sequence incorrect";
        return;

    } else if (_next_sequence > hl_action_read.index) {
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

    _next_sequence = hl_action_read.index;
    send_hl_action_item();
}

void MAVLinkHLActionTransferRoboticVehicle::UploadWorkItem::send_hl_action_item()
{
    if (_next_sequence >= _hl_action_list.items.size()) {
        LogErr() << "send_hl_action_item: sequence out of bounds";
        return;
    }

    const auto item = _hl_action_list.items[_next_sequence];

    mavlink_message_t message;
    mavlink_msg_hl_action_list_item_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        _next_sequence,
        item.command,
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

void MAVLinkHLActionTransferRoboticVehicle::UploadWorkItem::process_hl_action_ack(
    const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    mavlink_hl_action_list_ack_t hl_action_ack;
    mavlink_msg_hl_action_list_ack_decode(&message, &hl_action_ack);

    _timeout_handler.remove(_cookie);

    switch (hl_action_ack.type) {
        case HL_ACTION_LIST_ACCEPTED:
            _ack = Ack::Accepted;
            break;
        case HL_ACTION_LIST_ERROR:
            _ack = Ack::Error;
            break;
        case HL_ACTION_LIST_NO_SPACE:
            _ack = Ack::NoSpace;
            break;
        case HL_ACTION_LIST_INVALID_SEQUENCE:
            _ack = Ack::InvalidSequence;
            break;
        case HL_ACTION_LIST_CANCELLED:
            _ack = Ack::Cancelled;
            break;
        default:
            _ack = Ack::Unknown;
            break;
    }

    if (_next_sequence == _hl_action_list.items.size()) {
        callback_and_reset(Result::Success);
    } else {
        callback_and_reset(Result::ProtocolError);
    }
}

void MAVLinkHLActionTransferRoboticVehicle::UploadWorkItem::process_timeout()
{
    std::lock_guard<std::mutex> lock(_mutex);

    callback_and_reset(Result::Timeout);
}

void MAVLinkHLActionTransferRoboticVehicle::UploadWorkItem::callback_and_reset(Result result)
{
    if (_callback) {
        _callback(result, _ack);
    }
    _callback = nullptr;
    _done = true;
}

} // namespace mavsdk
