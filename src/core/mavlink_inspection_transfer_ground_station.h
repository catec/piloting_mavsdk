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
#include "mavlink_inspection_transfer.h"
#include "timeout_handler.h"
#include "locked_queue.h"
#include "log.h"

namespace mavsdk {

class MAVLinkInspectionTransferGroundStation : public MAVLinkInspectionTransfer {
public:
    class UploadWorkItem : public WorkItem {
    public:
        UploadWorkItem(
            Sender& sender,
            MAVLinkMessageHandler& message_handler,
            TimeoutHandler& timeout_handler,
            const TasksPlan& plan,
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
        void send_inspection_item();
        void send_cancel_and_finish();

        void process_inspection_read(const mavlink_message_t& message);
        void process_inspection_request(const mavlink_message_t& message);
        void process_inspection_request_int(const mavlink_message_t& message);
        void process_inspection_ack(const mavlink_message_t& message);
        void process_timeout();
        void callback_and_reset(Result result);

        enum class Step {
            SendCount,
            SendItems,
        } _step{Step::SendCount};

        TasksPlan _plan{};
        Ack _ack{Ack::Unknown};
        ResultAndAckCallback _callback{nullptr};
        std::size_t _next_sequence{0};
        void* _cookie{nullptr};
        unsigned _retries_done{0};
    };

    class DownloadWorkItem : public WorkItem {
    public:
        DownloadWorkItem(
            Sender& sender,
            MAVLinkMessageHandler& message_handler,
            TimeoutHandler& timeout_handler,
            ResultAndPlanCallback callback);

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

        void process_inspection_count(const mavlink_message_t& message);
        void process_inspection_item(const mavlink_message_t& message);
        void process_timeout();
        void callback_and_reset(Result result);

        enum class Step {
            RequestList,
            RequestItem,
        } _step{Step::RequestList};

        TasksPlan _plan{};
        ResultAndPlanCallback _callback{nullptr};
        void* _cookie{nullptr};
        std::size_t _next_sequence{0};
        std::size_t _expected_count{0};
        unsigned _retries_done{0};
    };

    MAVLinkInspectionTransferGroundStation(
        Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler);

    ~MAVLinkInspectionTransferGroundStation();

    std::weak_ptr<WorkItem> upload_items_async(const TasksPlan& plan, ResultAndAckCallback callback);

    std::weak_ptr<WorkItem> download_items_async(ResultAndPlanCallback callback);

    // Non-copyable
    MAVLinkInspectionTransferGroundStation(const MAVLinkInspectionTransferGroundStation&) = delete;
    const MAVLinkInspectionTransferGroundStation&
    operator=(const MAVLinkInspectionTransferGroundStation&) = delete;
};

} // namespace mavsdk
