#pragma once

#include "mavlink_sender.h"
#include "timeout_handler.h"
#include "locked_queue.h"

namespace mavsdk {

class MAVLinkCommandTransfer {
public:
    class WorkItem {
    public:
        WorkItem(
            Sender& sender,
            MAVLinkMessageHandler& message_handler,
            TimeoutHandler& timeout_handler);
        virtual ~WorkItem();
        virtual void start() = 0;
        virtual void cancel() = 0;
        bool has_started();
        bool is_done();

        WorkItem(const WorkItem&) = delete;
        WorkItem(WorkItem&&) = delete;
        WorkItem& operator=(const WorkItem&) = delete;
        WorkItem& operator=(WorkItem&&) = delete;

    protected:
        Sender& _sender;
        MAVLinkMessageHandler& _message_handler;
        TimeoutHandler& _timeout_handler;
        bool _started{false};
        bool _done{false};
        std::mutex _mutex{};
    };

    static constexpr double timeout_s = 0.5;
    static constexpr unsigned retries = 4;

    MAVLinkCommandTransfer(
        Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler);
    virtual ~MAVLinkCommandTransfer();

    void do_work();
    bool is_idle();

    // Non-copyable
    MAVLinkCommandTransfer(const MAVLinkCommandTransfer&) = delete;
    const MAVLinkCommandTransfer& operator=(const MAVLinkCommandTransfer&) = delete;

protected:
    Sender& _sender;
    MAVLinkMessageHandler& _message_handler;
    TimeoutHandler& _timeout_handler;

    LockedQueue<WorkItem> _work_queue{};
};

} // namespace mavsdk
