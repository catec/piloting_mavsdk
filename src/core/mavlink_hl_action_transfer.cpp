#include <algorithm>
#include "mavlink_hl_action_transfer.h"
#include "log.h"

namespace mavsdk {

MAVLinkHLActionTransfer::MAVLinkHLActionTransfer(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    _sender(sender),
    _message_handler(message_handler),
    _timeout_handler(timeout_handler)
{}

MAVLinkHLActionTransfer::~MAVLinkHLActionTransfer() {}

void MAVLinkHLActionTransfer::do_work()
{
    LockedQueue<WorkItem>::Guard work_queue_guard(_work_queue);
    auto work = work_queue_guard.get_front();

    if (!work) {
        return;
    }

    if (!work->has_started()) {
        work->start();
    }
    if (work->is_done()) {
        work_queue_guard.pop_front();
    }
}

bool MAVLinkHLActionTransfer::is_idle()
{
    LockedQueue<WorkItem>::Guard work_queue_guard(_work_queue);
    return (work_queue_guard.get_front() == nullptr);
}

MAVLinkHLActionTransfer::WorkItem::WorkItem(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    _sender(sender),
    _message_handler(message_handler),
    _timeout_handler(timeout_handler)
{}

MAVLinkHLActionTransfer::WorkItem::~WorkItem() {}

bool MAVLinkHLActionTransfer::WorkItem::has_started()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _started;
}

bool MAVLinkHLActionTransfer::WorkItem::is_done()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _done;
}

} // namespace mavsdk
