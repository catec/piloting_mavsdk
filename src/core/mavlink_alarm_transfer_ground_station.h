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

class MAVLinkAlarmTransferGroundStation : public MAVLinkAlarmTransfer {
public:
    class DownloadWorkItem : public WorkItem {
    public:
        DownloadWorkItem(
            Sender& sender,
            MAVLinkMessageHandler& message_handler,
            TimeoutHandler& timeout_handler,
            ResultAndAlarmListCallback callback);

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

        void process_alarm_count(const mavlink_message_t& message);
        void process_alarm_item(const mavlink_message_t& message);
        void process_timeout();
        void callback_and_reset(Result result);

        enum class Step {
            RequestList,
            RequestItem,
        } _step{Step::RequestList};

        AlarmList _alarm_list{};
        ResultAndAlarmListCallback _callback{nullptr};
        void* _cookie{nullptr};
        std::size_t _next_sequence{0};
        std::size_t _expected_count{0};
        unsigned _retries_done{0};
    };

    MAVLinkAlarmTransferGroundStation(
        Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler);

    ~MAVLinkAlarmTransferGroundStation();

    std::weak_ptr<WorkItem> download_items_async(ResultAndAlarmListCallback callback);

    // Non-copyable
    MAVLinkAlarmTransferGroundStation(const MAVLinkAlarmTransferGroundStation&) = delete;
    const MAVLinkAlarmTransferGroundStation&
    operator=(const MAVLinkAlarmTransferGroundStation&) = delete;
};

} // namespace mavsdk
