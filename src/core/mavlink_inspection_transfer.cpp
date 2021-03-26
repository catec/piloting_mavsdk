#include <algorithm>
#include "mavlink_inspection_transfer.h"
#include "log.h"

namespace mavsdk {

MAVLinkInspectionTransfer::MAVLinkInspectionTransfer(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    _sender(sender),
    _message_handler(message_handler),
    _timeout_handler(timeout_handler)
{}

MAVLinkInspectionTransfer::~MAVLinkInspectionTransfer() {}

void MAVLinkInspectionTransfer::do_work()
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

bool MAVLinkInspectionTransfer::is_idle()
{
    LockedQueue<WorkItem>::Guard work_queue_guard(_work_queue);
    return (work_queue_guard.get_front() == nullptr);
}

MAVLinkInspectionTransfer::WorkItem::WorkItem(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    _sender(sender),
    _message_handler(message_handler),
    _timeout_handler(timeout_handler)
{}

MAVLinkInspectionTransfer::WorkItem::~WorkItem() {}

bool MAVLinkInspectionTransfer::WorkItem::has_started()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _started;
}

bool MAVLinkInspectionTransfer::WorkItem::is_done()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _done;
}

} // namespace mavsdk
