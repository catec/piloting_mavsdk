#pragma once

#include "plugins/inspection_robotic_vehicle/inspection_robotic_vehicle.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"
#include "mavlink_inspection_transfer.h"

namespace mavsdk {

class InspectionRoboticVehicleImpl : public PluginImplBase {
public:
    explicit InspectionRoboticVehicleImpl(System& system);
    explicit InspectionRoboticVehicleImpl(std::shared_ptr<System> system);
    ~InspectionRoboticVehicleImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    void set_upload_inspection(const InspectionBase::WaypointList& list);
    void upload_inspection_async(
        const InspectionBase::WaypointList& list,
        const InspectionBase::ResultAckCallback& callback);
    InspectionBase::Result cancel_inspection_upload();

    void download_inspection_async(InspectionBase::DownloadInspectionCallback callback);
    InspectionBase::Result send_download_ack(const InspectionBase::Ack& ack);
    InspectionBase::Result cancel_inspection_download();

    void inspection_set_current_async(InspectionBase::InspectionSetCurrentCallback callback);
    void update_current_inspection_item(const uint16_t item_seq);
    void update_reached_inspection_item(const uint16_t item_seq);

    // Non-copyable
    InspectionRoboticVehicleImpl(const InspectionRoboticVehicleImpl&) = delete;
    const InspectionRoboticVehicleImpl& operator=(const InspectionRoboticVehicleImpl&) = delete;

private:
    void upload_inspection();
    void download_inspection(const std::string& plan_uuid, const uint32_t sync_id, const uint16_t count);
    void process_inspection_set_current(const mavlink_message_t& message);

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
        InspectionBase::ResultAckCallback upload_callback{nullptr};
        InspectionBase::DownloadInspectionCallback download_callback{nullptr};
        InspectionBase::InspectionSetCurrentCallback set_current_callback{nullptr};
        InspectionBase::WaypointList list{};
        std::weak_ptr<MAVLinkInspectionTransfer::WorkItem> last_upload{};
        std::weak_ptr<MAVLinkInspectionTransfer::WorkItem> last_download{};
    } _inspection_data{};

    void* _timeout_cookie{nullptr};
};

} // namespace mavsdk
