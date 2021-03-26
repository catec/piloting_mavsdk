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
#include "mavlink_alarm_transfer.h"
#include "timeout_handler.h"
#include "locked_queue.h"
#include "log.h"

namespace mavsdk {

class MAVLinkAlarmTransferRoboticVehicle : public MAVLinkAlarmTransfer {
public:
    class UploadWorkItem : public WorkItem {
    public:
        UploadWorkItem(
            Sender& sender,
            MAVLinkMessageHandler& message_handler,
            TimeoutHandler& timeout_handler,
            const AlarmList& alarm_list,
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
        void send_alarm_item();
        void send_cancel_and_finish();

        void process_alarm_read(const mavlink_message_t& message);
        void process_alarm_ack(const mavlink_message_t& message);
        void process_timeout();
        void callback_and_reset(Result result);

        enum class Step {
            SendCount,
            SendItems,
        } _step{Step::SendCount};

        AlarmList _alarm_list{};
        Ack _ack{Ack::Unknown};
        ResultAndAckCallback _callback{nullptr};
        std::size_t _next_sequence{0};
        void* _cookie{nullptr};
        unsigned _retries_done{0};
    };

    MAVLinkAlarmTransferRoboticVehicle(
        Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler);

    ~MAVLinkAlarmTransferRoboticVehicle();

    std::weak_ptr<WorkItem>
    upload_items_async(const AlarmList& alarm_list, ResultAndAckCallback callback);

    // Non-copyable
    MAVLinkAlarmTransferRoboticVehicle(const MAVLinkAlarmTransferRoboticVehicle&) = delete;
    const MAVLinkAlarmTransferRoboticVehicle&
    operator=(const MAVLinkAlarmTransferRoboticVehicle&) = delete;
};

} // namespace mavsdk
