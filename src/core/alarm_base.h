#pragma once

#include <string>
#include <functional>
#include <cmath>
#include <vector>

namespace mavsdk {

/**
 * @brief Base class for every alarm.
 */
class AlarmBase {
public:
    /**
     * @brief Default Constructor.
     */
    AlarmBase() = default;

    /**
     * @brief Default Destructor.
     */
    virtual ~AlarmBase() = default;

    /**
     * @brief Type representing a alarm item.
     */
    struct AlarmItem {
        uint16_t index{uint16_t(NAN)};
        std::string name{};
        std::string description{};
    };

    /**
     * @brief Equal operator to compare two `AlarmBase::AlarmItem` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(const AlarmBase::AlarmItem& lhs, const AlarmBase::AlarmItem& rhs);

    /**
     * @brief Stream operator to print information about a `AlarmBase::AlarmItem`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, AlarmBase::AlarmItem const& alarm_item);

    /**
     * @brief Alarm list
     */
    struct AlarmList {
        std::vector<AlarmItem> items{}; /**< @brief The alarm items */
    };

    /**
     * @brief Equal operator to compare two `AlarmBase::AlarmList` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(const AlarmBase::AlarmList& lhs, const AlarmBase::AlarmList& rhs);

    /**
     * @brief Stream operator to print information about a `AlarmBase::AlarmList`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, AlarmBase::AlarmList const& alarm_list);

    /**
     * @brief Possible ACKs returned for action requests.
     */
    enum class Ack {
        Accepted,
        Error,
        NoSpace,
        InvalidSequence,
        Cancelled,
        Unknown
    };

    /**
     * @brief Stream operator to print information about a `AlarmBase::Ack`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, AlarmBase::Ack const& ack);

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
        TransferCancelled, /**< @brief Alarm transfer has been cancelled. */
    };

    /**
     * @brief Stream operator to print information about a `AlarmBase::Result`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, AlarmBase::Result const& result);

    /**
     * @brief Alaram status types.
     */
    enum class AlarmStatusType {
        Ok,
        Warning,
        Error
    };

    /**
     * @brief Stream operator to print information about a
     * `TelemetryBase::AlarmStatusType`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, AlarmBase::AlarmStatusType const& value);

    /**
     * @brief AlarmStatus information type.
     */
    struct AlarmStatus {
        uint32_t stamp_ms{}; /**< @brief Timestamp (ms) */
        uint16_t index{}; /**< @brief Alarm index */
        AlarmStatusType status{}; /**< @brief Alarm status type */
        uint16_t errors_count{}; /**< @brief Errors count */
        uint16_t warns_count{}; /**< @brief Warnings count */
    };

    /**
     * @brief Equal operator to compare two `TelemetryBase::AlarmStatus` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(const AlarmBase::AlarmStatus& lhs, const AlarmBase::AlarmStatus& rhs);

    /**
     * @brief Stream operator to print information about a `TelemetryBase::AlarmStatus`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, AlarmBase::AlarmStatus const& value);

    /**
     * @brief Callback type for asynchronous Alarm ACK calls.
     */
    using ResultAckCallback = std::function<void(Result, Ack)>;

    /**
     * @brief Callback type for download_alarm_async.
     */
    using DownloadAlarmListCallback = std::function<void(Result, AlarmList)>;

    /**
     * @brief Callback type for subscribe_alarm_status.
     */
    using AlarmStatusCallback = std::function<void(AlarmStatus)>;

    /**
     * @brief Copy constructor (object is not copyable).
     */
    AlarmBase(const AlarmBase&) = delete;

    /**
     * @brief Assign operator (object is not copyable).
     */
    const AlarmBase& operator=(const AlarmBase&) = delete;
};

} // namespace mavsdk
