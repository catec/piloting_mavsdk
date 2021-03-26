#include <algorithm>
#include "mavlink_checklist_transfer.h"
#include "log.h"

namespace mavsdk {

MAVLinkChecklistTransfer::MAVLinkChecklistTransfer(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    _sender(sender),
    _message_handler(message_handler),
    _timeout_handler(timeout_handler)
{}

MAVLinkChecklistTransfer::~MAVLinkChecklistTransfer() {}

void MAVLinkChecklistTransfer::do_work()
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

bool MAVLinkChecklistTransfer::is_idle()
{
    LockedQueue<WorkItem>::Guard work_queue_guard(_work_queue);
    return (work_queue_guard.get_front() == nullptr);
}

MAVLinkChecklistTransfer::WorkItem::WorkItem(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    _sender(sender),
    _message_handler(message_handler),
    _timeout_handler(timeout_handler)
{}

MAVLinkChecklistTransfer::WorkItem::~WorkItem() {}

bool MAVLinkChecklistTransfer::WorkItem::has_started()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _started;
}

bool MAVLinkChecklistTransfer::WorkItem::is_done()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _done;
}

} // namespace mavsdk
