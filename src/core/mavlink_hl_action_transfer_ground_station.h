#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include "mavlink_address.h"
#include "mavlink_include.h"
#include "mavlink_message_handler.h"
#include "mavlink_hl_action_transfer.h"
#include "timeout_handler.h"
#include "locked_queue.h"
#include "log.h"

namespace mavsdk {

class MAVLinkHLActionTransferGroundStation : public MAVLinkHLActionTransfer {
public:
    class DownloadWorkItem : public WorkItem {
    public:
        DownloadWorkItem(
            Sender& sender,
            MAVLinkMessageHandler& message_handler,
            TimeoutHandler& timeout_handler,
            ResultAndHLActionListCallback callback);

        virtual ~DownloadWorkItem();
        void start() override;
        void cancel() override;

        DownloadWorkItem(const DownloadWorkItem&) = delete;
        DownloadWorkItem(DownloadWorkItem&&) = delete;
        DownloadWorkItem& operator=(const DownloadWorkItem&) = delete;
        DownloadWorkItem& operator=(DownloadWorkItem&&) = delete;

    private:
        void request_list();
        void request_item();
        void send_ack_and_finish();
        void send_cancel_and_finish();

        void process_hl_action_count(const mavlink_message_t& message);
        void process_hl_action_item(const mavlink_message_t& message);
        void process_timeout();
        void callback_and_reset(Result result);

        enum class Step {
            RequestList,
            RequestItem,
        } _step{Step::RequestList};

        HLActionList _hl_action_list{};
        ResultAndHLActionListCallback _callback{nullptr};
        void* _cookie{nullptr};
        std::size_t _next_sequence{0};
        std::size_t _expected_count{0};
        unsigned _retries_done{0};
    };

    MAVLinkHLActionTransferGroundStation(
        Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler);

    ~MAVLinkHLActionTransferGroundStation();

    std::weak_ptr<WorkItem> download_items_async(ResultAndHLActionListCallback callback);

    // Non-copyable
    MAVLinkHLActionTransferGroundStation(const MAVLinkHLActionTransferGroundStation&) = delete;
    const MAVLinkHLActionTransferGroundStation&
    operator=(const MAVLinkHLActionTransferGroundStation&) = delete;
};

} // namespace mavsdk
