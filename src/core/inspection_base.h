#pragma once

#include <string>
#include <functional>
#include <cmath>
#include <vector>

namespace mavsdk {

/**
 * @brief Base class for every inspection.
 */
class InspectionBase {
public:
    /**
     * @brief Default Constructor.
     */
    InspectionBase() = default;

    /**
     * @brief Default Destructor.
     */
    virtual ~InspectionBase() = default;

    /**
     * @brief Type representing a waypoint item.
     *
     * A WaypointItem can contain a position and/or actions.
     * Waypoint items are building blocks to assemble a inspection,
     * which can be sent to (or received from) a system.
     * They cannot be used independently.
     */
    struct WaypointItem {
        uint32_t task_id{0};
        uint16_t command{0};
        uint8_t autocontinue{0};
        float param1{0};
        float param2{0};
        float param3{0};
        float param4{0};
        float x{0};
        float y{0};
        float z{0};
    };

    /**
     * @brief Equal operator to compare two `InspectionBase::WaypointItem` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(
        const InspectionBase::WaypointItem& lhs, const InspectionBase::WaypointItem& rhs);

    /**
     * @brief Stream operator to print information about a `InspectionBase::WaypointItem`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, InspectionBase::WaypointItem const& item);

    /**
     * @brief Waypoint list
     */
    struct WaypointList {
        std::vector<WaypointItem> items{}; /**< @brief The waypoint items */
    };

    /**
     * @brief Equal operator to compare two `InspectionBase::WaypointList` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(
        const InspectionBase::WaypointList& lhs, const InspectionBase::WaypointList& rhs);

    /**
     * @brief Stream operator to print information about a `InspectionBase::WaypointList`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, InspectionBase::WaypointList const& list);

    /**
     * @brief Inspection progress type.
     */
    struct InspectionProgress {
        int32_t current{}; /**< @brief Current waypoint item index (0-based) */
        int32_t reached{}; /**< @brief Reached waypoint item index (0-based) */
    };

    /**
     * @brief Equal operator to compare two `InspectionBase::InspectionProgress` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(
        const InspectionBase::InspectionProgress& lhs,
        const InspectionBase::InspectionProgress& rhs);

    /**
     * @brief Stream operator to print information about a `InspectionBase::InspectionProgress`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, InspectionBase::InspectionProgress const& progress);

    /**
     * @brief Possible ACKs returned for action requests.
     */
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

    /**
     * @brief Stream operator to print information about a `InspectionBase::Ack`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, InspectionBase::Ack const& ack);

    /**
     * @brief Possible results returned for action requests.
     */
    enum class Result {
        Unknown, /**< @brief Unknown result. */
        Success, /**< @brief Request succeeded. */
        ConnectionError, /**< @brief Connection error. */
        ProtocolError, /**< @brief Protocol error. */
        Busy, /**< @brief Vehicle is busy. */
        Timeout, /**< @brief Request timed out. */
        InvalidArgument, /**< @brief Invalid argument. */
        TransferCancelled, /**< @brief Inspection transfer has been cancelled. */
    };

    /**
     * @brief Stream operator to print information about a `InspectionBase::Result`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, InspectionBase::Result const& result);

    /**
     * @brief Callback type for asynchronous Inspection calls.
     */
    using ResultCallback = std::function<void(Result)>;

    /**
     * @brief Callback type for asynchronous Inspection ACK calls.
     */
    using ResultAckCallback = std::function<void(Result, Ack)>;

    /**
     * @brief Callback type for download_inspection_async.
     */
    using DownloadInspectionCallback = std::function<void(Result, WaypointList)>;

    /**
     * @brief Callback type for subscribe_inspection_progress.
     */
    using InspectionProgressCallback = std::function<void(InspectionProgress)>;

    /**
     * @brief Callback type for subscribe_inspection_set_current.
     */
    using InspectionSetCurrentCallback = std::function<void(uint16_t)>;

    /**
     * @brief Copy constructor (object is not copyable).
     */
    InspectionBase(const InspectionBase&) = delete;

    /**
     * @brief Assign operator (object is not copyable).
     */
    const InspectionBase& operator=(const InspectionBase&) = delete;
};

} // namespace mavsdk
