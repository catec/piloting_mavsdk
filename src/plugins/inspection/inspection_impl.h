#pragma once

#include "plugins/inspection/inspection.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"
#include "mavlink_inspection_transfer.h"

namespace mavsdk {

class InspectionImpl : public PluginImplBase {
public:
    explicit InspectionImpl(System& system);
    explicit InspectionImpl(std::shared_ptr<System> system);
    ~InspectionImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    std::pair<InspectionBase::Result, InspectionBase::Ack>
    upload_inspection(const InspectionBase::WaypointList& list);
    void upload_inspection_async(
        const InspectionBase::WaypointList& list,
        const InspectionBase::ResultAckCallback& callback);
    InspectionBase::Result cancel_inspection_upload();

    std::pair<InspectionBase::Result, InspectionBase::WaypointList> download_inspection();
    void download_inspection_async(const InspectionBase::DownloadInspectionCallback& callback);
    InspectionBase::Result cancel_inspection_download();

    void set_current_inspection_item(uint16_t index);

    InspectionBase::InspectionProgress inspection_progress();
    void inspection_progress_async(InspectionBase::InspectionProgressCallback callback);

    // Non-copyable
    InspectionImpl(const InspectionImpl&) = delete;
    const InspectionImpl& operator=(const InspectionImpl&) = delete;

private:
    void process_inspection_current(const mavlink_message_t& message);
    void process_inspection_item_reached(const mavlink_message_t& message);

    void report_progress();
    void reset_inspection_progress();

    std::vector<MAVLinkInspectionTransfer::WaypointItem>
    convert_to_int_items(const std::vector<InspectionBase::WaypointItem>& items);

    // FIXME: make static
    std::pair<InspectionBase::Result, InspectionBase::WaypointList>
    convert_to_result_and_waypoint_list(
        MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::WaypointList& list);

    // FIXME: make static
    std::pair<InspectionBase::Result, InspectionBase::Ack> convert_to_result_and_ack(
        MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::Ack ack);

    static InspectionBase::Ack convert_ack(MAVLinkInspectionTransfer::Ack ack);

    static InspectionBase::Result convert_result(MAVLinkInspectionTransfer::Result result);

    struct {
        mutable std::recursive_mutex mutex{};
        int last_current_mavlink_inspection_item{-1};
        int last_reached_mavlink_inspection_item{-1};
        InspectionBase::InspectionProgressCallback progress_callback{nullptr};
        int last_current_reported_inspection_item{-1};
        int last_reached_reported_inspection_item{-1};
        std::weak_ptr<MAVLinkInspectionTransfer::WorkItem> last_upload{};
        std::weak_ptr<MAVLinkInspectionTransfer::WorkItem> last_download{};
    } _inspection_data{};

    void* _timeout_cookie{nullptr};
};

} // namespace mavsdk
