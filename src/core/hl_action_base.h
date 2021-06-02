#pragma once

#include <string>
#include <functional>
#include <cmath>
#include <vector>

namespace mavsdk {

/**
 * @brief Base class for every hl_action.
 */
class HLActionBase {
public:
    /**
     * @brief Default Constructor.
     */
    HLActionBase() = default;

    /**
     * @brief Default Destructor.
     */
    virtual ~HLActionBase() = default;

    /**
     * @brief Type representing a hl_action item.
     */
    struct HLActionItem {
        uint16_t index{uint16_t(NAN)};
        uint16_t command{uint16_t(NAN)};
        std::string name{};
        std::string description{};
    };

    /**
     * @brief Equal operator to compare two `HLActionBase::HLActionItem` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool
    operator==(const HLActionBase::HLActionItem& lhs, const HLActionBase::HLActionItem& rhs);

    /**
     * @brief Stream operator to print information about a `HLActionBase::HLActionItem`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, HLActionBase::HLActionItem const& hl_action_item);

    /**
     * @brief HLAction list
     */
    struct HLActionList {
        std::vector<HLActionItem> items{}; /**< @brief The hl_action items */
    };

    /**
     * @brief Equal operator to compare two `HLActionBase::HLActionList` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool
    operator==(const HLActionBase::HLActionList& lhs, const HLActionBase::HLActionList& rhs);

    /**
     * @brief Stream operator to print information about a `HLActionBase::HLAction`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, HLActionBase::HLActionList const& list);

    /**
     * @brief Possible ACKs returned for action requests.
     */
    enum class Ack { Accepted, Error, NoSpace, InvalidSequence, Cancelled, Unknown };

    /**
     * @brief Stream operator to print information about a `HLActionBase::Ack`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, HLActionBase::Ack const& ack);

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
        TransferCancelled, /**< @brief HLAction transfer has been cancelled. */
    };

    /**
     * @brief Stream operator to print information about a `HLActionBase::Result`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, HLActionBase::Result const& result);

    /**
     * @brief Callback type for asynchronous HLAction ACK calls.
     */
    using ResultAckCallback = std::function<void(Result, Ack)>;

    /**
     * @brief Callback type for download_hl_action_async.
     */
    using DownloadHLActionListCallback = std::function<void(Result, HLActionList)>;

    /**
     * @brief Copy constructor (object is not copyable).
     */
    HLActionBase(const HLActionBase&) = delete;

    /**
     * @brief Assign operator (object is not copyable).
     */
    const HLActionBase& operator=(const HLActionBase&) = delete;
};

} // namespace mavsdk
