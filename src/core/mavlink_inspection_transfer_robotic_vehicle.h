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

class MAVLinkInspectionTransferRoboticVehicle : public MAVLinkInspectionTransfer {
public:
    class UploadWorkItem : public WorkItem {
    public:
        UploadWorkItem(
            Sender& sender,
            MAVLinkMessageHandler& message_handler,
            TimeoutHandler& timeout_handler,
            const WaypointList& list,
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
        void process_inspection_ack(const mavlink_message_t& message);
        void process_timeout();
        void callback_and_reset(Result result);

        enum class Step {
            SendCount,
            SendItems,
        } _step{Step::SendCount};

        WaypointList _list{};
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
            const uint16_t count,
            ResultAndListCallback callback);

        virtual ~DownloadWorkItem();
        void start() override;
        void cancel() override;

        DownloadWorkItem(const DownloadWorkItem&) = delete;
        DownloadWorkItem(DownloadWorkItem&&) = delete;
        DownloadWorkItem& operator=(const DownloadWorkItem&) = delete;
        DownloadWorkItem& operator=(DownloadWorkItem&&) = delete;

    private:
        void request_item();
        void send_cancel_and_finish();
        void process_inspection_item_int(const mavlink_message_t& message);
        void process_timeout();
        void callback_and_reset(Result result);

        WaypointList _list{};
        ResultAndListCallback _callback{nullptr};
        void* _cookie{nullptr};
        std::size_t _next_sequence{0};
        std::size_t _expected_count{0};
        unsigned _retries_done{0};
    };

    MAVLinkInspectionTransferRoboticVehicle(
        Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler);

    ~MAVLinkInspectionTransferRoboticVehicle();

    std::weak_ptr<WorkItem>
    upload_items_async(const WaypointList& list, ResultAndAckCallback callback);

    std::weak_ptr<WorkItem>
    download_items_async(const uint16_t count, ResultAndListCallback callback);

    // Non-copyable
    MAVLinkInspectionTransferRoboticVehicle(const MAVLinkInspectionTransferRoboticVehicle&) =
        delete;
    const MAVLinkInspectionTransferRoboticVehicle&
    operator=(const MAVLinkInspectionTransferRoboticVehicle&) = delete;
};

} // namespace mavsdk
