#include <algorithm>
#include "mavlink_checklist_transfer_ground_station.h"
#include "log.h"

namespace mavsdk {
MAVLinkChecklistTransferGroundStation::MAVLinkChecklistTransferGroundStation(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    MAVLinkChecklistTransfer(sender, message_handler, timeout_handler)
{}

MAVLinkChecklistTransferGroundStation::~MAVLinkChecklistTransferGroundStation() {}

std::weak_ptr<MAVLinkChecklistTransfer::WorkItem>
MAVLinkChecklistTransferGroundStation::download_items_async(ResultAndChecklistCallback callback)
{
    auto ptr =
        std::make_shared<DownloadWorkItem>(_sender, _message_handler, _timeout_handler, callback);

    _work_queue.push_back(ptr);

    return std::weak_ptr<WorkItem>(ptr);
}

MAVLinkChecklistTransferGroundStation::DownloadWorkItem::DownloadWorkItem(
    Sender& sender,
    MAVLinkMessageHandler& message_handler,
    TimeoutHandler& timeout_handler,
    ResultAndChecklistCallback callback) :
    WorkItem(sender, message_handler, timeout_handler),
    _callback(callback)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _message_handler.register_one(
        MAVLINK_MSG_ID_CHECK_LIST_COUNT,
        [this](const mavlink_message_t& message) {
            LogDebug() << "Checklist count message received!";
            process_checklist_count(message);
        },
        this);

    _message_handler.register_one(
        MAVLINK_MSG_ID_CHECK_LIST_ITEM,
        [this](const mavlink_message_t& message) { process_checklist_item(message); },
        this);
}

MAVLinkChecklistTransferGroundStation::DownloadWorkItem::~DownloadWorkItem()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _message_handler.unregister_all(this);
    _timeout_handler.remove(_cookie);
}

void MAVLinkChecklistTransferGroundStation::DownloadWorkItem::start()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _checklist.items.clear();
    _started = true;
    _retries_done = 0;
    _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);
    request_list();
}

void MAVLinkChecklistTransferGroundStation::DownloadWorkItem::cancel()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _timeout_handler.remove(_cookie);
    send_cancel_and_finish();
}

void MAVLinkChecklistTransferGroundStation::DownloadWorkItem::request_list()
{
    mavlink_message_t message;
    mavlink_msg_check_list_request_pack(
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

void MAVLinkChecklistTransferGroundStation::DownloadWorkItem::request_item()
{
    mavlink_message_t message;
    mavlink_msg_check_list_read_pack(
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

void MAVLinkChecklistTransferGroundStation::DownloadWorkItem::send_ack_and_finish()
{
    mavlink_message_t message;
    mavlink_msg_check_list_ack_pack(
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

void MAVLinkChecklistTransferGroundStation::DownloadWorkItem::send_cancel_and_finish()
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

void MAVLinkChecklistTransferGroundStation::DownloadWorkItem::process_checklist_count(
    const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    mavlink_check_list_count_t count;
    mavlink_msg_check_list_count_decode(&message, &count);

    if (count.count == 0) {
        LogDebug() << "Checklist count is 0. Sending ACK and finishing";
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

void MAVLinkChecklistTransferGroundStation::DownloadWorkItem::process_checklist_item(
    const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _timeout_handler.refresh(_cookie);

    mavlink_check_list_item_t checklist_item;
    mavlink_msg_check_list_item_decode(&message, &checklist_item);

    _checklist.items.push_back(ChecklistItem{
        checklist_item.index, checklist_item.name, checklist_item.description});

    if (_next_sequence + 1 == _expected_count) {
        _timeout_handler.remove(_cookie);
        send_ack_and_finish();

    } else {
        _next_sequence = checklist_item.index + 1;
        _retries_done = 0;
        request_item();
    }
}

void MAVLinkChecklistTransferGroundStation::DownloadWorkItem::process_timeout()
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

void MAVLinkChecklistTransferGroundStation::DownloadWorkItem::callback_and_reset(Result result)
{
    if (_callback) {
        _callback(result, _checklist);
    }
    _callback = nullptr;
    _done = true;
}

} // namespace mavsdk
