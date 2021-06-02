#include <algorithm>
#include "mavlink_hl_action_transfer_ground_station.h"
#include "log.h"

namespace mavsdk {
MAVLinkHLActionTransferGroundStation::MAVLinkHLActionTransferGroundStation(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    MAVLinkHLActionTransfer(sender, message_handler, timeout_handler)
{}

MAVLinkHLActionTransferGroundStation::~MAVLinkHLActionTransferGroundStation() {}

std::weak_ptr<MAVLinkHLActionTransfer::WorkItem>
MAVLinkHLActionTransferGroundStation::download_items_async(ResultAndHLActionListCallback callback)
{
    auto ptr =
        std::make_shared<DownloadWorkItem>(_sender, _message_handler, _timeout_handler, callback);

    _work_queue.push_back(ptr);

    return std::weak_ptr<WorkItem>(ptr);
}

MAVLinkHLActionTransferGroundStation::DownloadWorkItem::DownloadWorkItem(
    Sender& sender,
    MAVLinkMessageHandler& message_handler,
    TimeoutHandler& timeout_handler,
    ResultAndHLActionListCallback callback) :
    WorkItem(sender, message_handler, timeout_handler),
    _callback(callback)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _message_handler.register_one(
        MAVLINK_MSG_ID_HL_ACTION_LIST_COUNT,
        [this](const mavlink_message_t& message) {
            LogDebug() << "HLAction count message received!";
            process_hl_action_count(message);
        },
        this);

    _message_handler.register_one(
        MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM,
        [this](const mavlink_message_t& message) { process_hl_action_item(message); },
        this);
}

MAVLinkHLActionTransferGroundStation::DownloadWorkItem::~DownloadWorkItem()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _message_handler.unregister_all(this);
    _timeout_handler.remove(_cookie);
}

void MAVLinkHLActionTransferGroundStation::DownloadWorkItem::start()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _hl_action_list.items.clear();
    _started = true;
    _retries_done = 0;
    _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);
    request_list();
}

void MAVLinkHLActionTransferGroundStation::DownloadWorkItem::cancel()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _timeout_handler.remove(_cookie);
    send_cancel_and_finish();
}

void MAVLinkHLActionTransferGroundStation::DownloadWorkItem::request_list()
{
    mavlink_message_t message;
    mavlink_msg_hl_action_list_request_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id);

    if (!_sender.send_message(message)) {
        _timeout_handler.remove(_cookie);
        callback_and_reset(Result::ConnectionError);
        return;
    }

    ++_retries_done;
}

void MAVLinkHLActionTransferGroundStation::DownloadWorkItem::request_item()
{
    mavlink_message_t message;
    mavlink_msg_hl_action_list_read_pack(
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

void MAVLinkHLActionTransferGroundStation::DownloadWorkItem::send_ack_and_finish()
{
    mavlink_message_t message;
    mavlink_msg_hl_action_list_ack_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        CHECK_LIST_ACCEPTED);

    if (!_sender.send_message(message)) {
        callback_and_reset(Result::ConnectionError);
        return;
    }

    // We do not wait on anything coming back after this.
    callback_and_reset(Result::Success);
}

void MAVLinkHLActionTransferGroundStation::DownloadWorkItem::send_cancel_and_finish()
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

void MAVLinkHLActionTransferGroundStation::DownloadWorkItem::process_hl_action_count(
    const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    mavlink_hl_action_list_count_t count;
    mavlink_msg_hl_action_list_count_decode(&message, &count);

    if (count.count == 0) {
        LogDebug() << "HLAction count is 0. Sending ACK and finishing";
        send_ack_and_finish();
        _timeout_handler.remove(_cookie);
        return;
    }

    _timeout_handler.refresh(_cookie);
    _next_sequence = 0;
    _step = Step::RequestItem;
    _retries_done = 0;
    _expected_count = count.count;
    request_item();
}

void MAVLinkHLActionTransferGroundStation::DownloadWorkItem::process_hl_action_item(
    const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _timeout_handler.refresh(_cookie);

    mavlink_hl_action_list_item_t hl_action_list_item;
    mavlink_msg_hl_action_list_item_decode(&message, &hl_action_list_item);

    _hl_action_list.items.push_back(HLActionItem{hl_action_list_item.index,
                                                 hl_action_list_item.command,
                                                 hl_action_list_item.name,
                                                 hl_action_list_item.description});

    if (_next_sequence + 1 == _expected_count) {
        _timeout_handler.remove(_cookie);
        send_ack_and_finish();
    } else {
        _next_sequence = hl_action_list_item.index + 1;
        _retries_done = 0;
        request_item();
    }
}

void MAVLinkHLActionTransferGroundStation::DownloadWorkItem::process_timeout()
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_retries_done >= retries) {
        callback_and_reset(Result::Timeout);
        return;
    }

    switch (_step) {
        case Step::RequestList:
            _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);
            request_list();
            break;

        case Step::RequestItem:
            _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);
            request_item();
            break;
    }
}

void MAVLinkHLActionTransferGroundStation::DownloadWorkItem::callback_and_reset(Result result)
{
    if (_callback) {
        _callback(result, _hl_action_list);
    }
    _callback = nullptr;
    _done = true;
}

} // namespace mavsdk
