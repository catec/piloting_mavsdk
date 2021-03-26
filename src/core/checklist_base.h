#pragma once

#include <string>
#include <functional>
#include <cmath>
#include <vector>

namespace mavsdk {

/**
 * @brief Base class for every checklist.
 */
class ChecklistBase {
public:
    /**
     * @brief Default Constructor.
     */
    ChecklistBase() = default;

    /**
     * @brief Default Destructor.
     */
    virtual ~ChecklistBase() = default;

    /**
     * @brief Type representing a checklist item.
     */
    struct ChecklistItem {
        uint16_t index{uint16_t(NAN)};
        std::string name{};
        std::string description{};
    };

    /**
     * @brief Equal operator to compare two `ChecklistBase::ChecklistItem` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool
    operator==(const ChecklistBase::ChecklistItem& lhs, const ChecklistBase::ChecklistItem& rhs);

    /**
     * @brief Stream operator to print information about a `ChecklistBase::ChecklistItem`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, ChecklistBase::ChecklistItem const& checklist_item);

    /**
     * @brief Checklist
     */
    struct Checklist {
        std::vector<ChecklistItem> items{}; /**< @brief The checklist items */
    };

    /**
     * @brief Equal operator to compare two `ChecklistBase::Checklist` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool
    operator==(const ChecklistBase::Checklist& lhs, const ChecklistBase::Checklist& rhs);

    /**
     * @brief Stream operator to print information about a `ChecklistBase::Checklist`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, ChecklistBase::Checklist const& checklist_plan);

    /**
     * @brief Possible ACKs returned for action requests.
     */
    enum class Ack { Accepted, Error, NoSpace, InvalidSequence, Cancelled, Unknown };

    /**
     * @brief Stream operator to print information about a `ChecklistBase::Ack`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, ChecklistBase::Ack const& ack);

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
        TransferCancelled, /**< @brief Checklist transfer has been cancelled. */
    };

    /**
     * @brief Stream operator to print information about a `ChecklistBase::Result`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, ChecklistBase::Result const& result);

    /**
     * @brief Callback type for asynchronous Checklist ACK calls.
     */
    using ResultAckCallback = std::function<void(Result, Ack)>;

    /**
     * @brief Callback type for download_checklist_async.
     */
    using DownloadChecklistCallback = std::function<void(Result, Checklist)>;

    /**
     * @brief Copy constructor (object is not copyable).
     */
    ChecklistBase(const ChecklistBase&) = delete;

    /**
     * @brief Assign operator (object is not copyable).
     */
    const ChecklistBase& operator=(const ChecklistBase&) = delete;
};

} // namespace mavsdk
