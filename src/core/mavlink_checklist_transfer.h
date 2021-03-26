#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include "mavlink_sender.h"
#include "timeout_handler.h"
#include "locked_queue.h"
#include "log.h"

namespace mavsdk {

class MAVLinkChecklistTransfer {
public:
    enum class Result {
        Success,
        ConnectionError,
        Timeout,
        Cancelled,
        InvalidSequence,
        ProtocolError
    };

    enum class Ack { Accepted, Error, NoSpace, InvalidSequence, Cancelled, Unknown };

    struct ChecklistItem {
        uint16_t index;
        std::string name;
        std::string description;

        bool operator==(const ChecklistItem& other) const
        {
            return (index == other.index && name == other.name && description == other.description);
        }
    };

    struct Checklist {
        std::vector<ChecklistItem> items{};

        bool operator==(const Checklist& other) const { return (items == other.items); }
    };

    using ResultAndAckCallback = std::function<void(Result result, Ack ack)>;
    using ResultAndChecklistCallback = std::function<void(Result result, Checklist checklist)>;

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

    MAVLinkChecklistTransfer(
        Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler);

    virtual ~MAVLinkChecklistTransfer();

    void do_work();
    bool is_idle();

    // Non-copyable
    MAVLinkChecklistTransfer(const MAVLinkChecklistTransfer&) = delete;
    const MAVLinkChecklistTransfer& operator=(const MAVLinkChecklistTransfer&) = delete;

protected:
    Sender& _sender;
    MAVLinkMessageHandler& _message_handler;
    TimeoutHandler& _timeout_handler;

    LockedQueue<WorkItem> _work_queue{};
};

} // namespace mavsdk
