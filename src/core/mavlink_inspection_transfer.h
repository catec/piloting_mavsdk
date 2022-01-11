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

class MAVLinkInspectionTransfer {
public:
    enum class Result {
        Success,
        ConnectionError,
        Timeout,
        Cancelled,
        InvalidSequence,
        CurrentInvalid,
        ProtocolError
    };

    enum class Ack {
        Accepted,
        Error,
        Unsupported,
        NoSpace,
        Invalid,
        InvalidParam1,
        InvalidParam2,
        InvalidParam3,
        InvalidParam4,
        InvalidParam5,
        InvalidParam6,
        InvalidParam7,
        InvalidSequence,
        Cancelled,
        Unknown
    };

    struct WaypointItem {
        uint16_t seq;
        uint32_t task_id;
        uint16_t command;
        uint8_t autocontinue;
        float param1;
        float param2;
        float param3;
        float param4;
        float x;
        float y;
        float z;

        bool operator==(const WaypointItem& other) const
        {
            return (
                seq == other.seq && task_id == other.task_id && command == other.command &&
                autocontinue == other.autocontinue && param1 == other.param1 &&
                param2 == other.param2 && param3 == other.param3 && param4 == other.param4 &&
                x == other.x && y == other.y && z == other.z);
        }
    };

    struct WaypointList {
        uint32_t plan_id;
        uint32_t sync_id;
        std::vector<WaypointItem> items{};

        bool operator==(const WaypointList& other) const
        {
            return plan_id == other.plan_id && sync_id == other.sync_id && items == other.items;
        }
    };

    using ResultCallback = std::function<void(Result result)>;
    using ResultAndAckCallback = std::function<void(Result result, Ack ack)>;
    using ResultAndListCallback = std::function<void(Result result, WaypointList list)>;

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

    MAVLinkInspectionTransfer(
        Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler);

    virtual ~MAVLinkInspectionTransfer();

    void do_work();
    bool is_idle();

    // Non-copyable
    MAVLinkInspectionTransfer(const MAVLinkInspectionTransfer&) = delete;
    const MAVLinkInspectionTransfer& operator=(const MAVLinkInspectionTransfer&) = delete;

protected:
    Sender& _sender;
    MAVLinkMessageHandler& _message_handler;
    TimeoutHandler& _timeout_handler;

    LockedQueue<WorkItem> _work_queue{};
};

} // namespace mavsdk
