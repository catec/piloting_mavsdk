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

class MAVLinkHLActionTransferRoboticVehicle : public MAVLinkHLActionTransfer {
public:
    class UploadWorkItem : public WorkItem {
    public:
        UploadWorkItem(
            Sender& sender,
            MAVLinkMessageHandler& message_handler,
            TimeoutHandler& timeout_handler,
            const HLActionList& hl_action_list,
            ResultAndAckCallback callback);

        virtual ~UploadWorkItem();
        void start() override;
        void cancel() override;

        UploadWorkItem(const UploadWorkItem&) = delete;
        UploadWorkItem(UploadWorkItem&&) = delete;
        UploadWorkItem& operator=(const UploadWorkItem&) = delete;
        UploadWorkItem& operator=(UploadWorkItem&&) = delete;

    private:
        void send_count();
        void send_hl_action_item();
        void send_cancel_and_finish();

        void process_hl_action_read(const mavlink_message_t& message);
        void process_hl_action_ack(const mavlink_message_t& message);
        void process_timeout();
        void callback_and_reset(Result result);

        enum class Step {
            SendCount,
            SendItems,
        } _step{Step::SendCount};

        HLActionList _hl_action_list{};
        Ack _ack{Ack::Unknown};
        ResultAndAckCallback _callback{nullptr};
        std::size_t _next_sequence{0};
        void* _cookie{nullptr};
        unsigned _retries_done{0};
    };

    MAVLinkHLActionTransferRoboticVehicle(
        Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler);

    ~MAVLinkHLActionTransferRoboticVehicle();

    std::weak_ptr<WorkItem>
    upload_items_async(const HLActionList& hl_action_list, ResultAndAckCallback callback);

    // Non-copyable
    MAVLinkHLActionTransferRoboticVehicle(const MAVLinkHLActionTransferRoboticVehicle&) = delete;
    const MAVLinkHLActionTransferRoboticVehicle&
    operator=(const MAVLinkHLActionTransferRoboticVehicle&) = delete;
};

} // namespace mavsdk
