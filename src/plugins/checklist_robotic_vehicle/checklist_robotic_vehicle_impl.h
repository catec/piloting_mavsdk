#pragma once

#include "plugins/checklist_robotic_vehicle/checklist_robotic_vehicle.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"
#include "mavlink_checklist_transfer.h"

namespace mavsdk {

class ChecklistRoboticVehicleImpl : public PluginImplBase {
public:
    explicit ChecklistRoboticVehicleImpl(System& system);
    explicit ChecklistRoboticVehicleImpl(std::shared_ptr<System> system);
    ~ChecklistRoboticVehicleImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    void set_upload_checklist(const ChecklistBase::Checklist& checklist);
    void upload_checklist_async(
        const ChecklistBase::Checklist& checklist,
        const ChecklistBase::ResultAckCallback& callback);
    ChecklistBase::Result cancel_checklist_upload();

    // Non-copyable
    ChecklistRoboticVehicleImpl(const ChecklistRoboticVehicleImpl&) = delete;
    const ChecklistRoboticVehicleImpl& operator=(const ChecklistRoboticVehicleImpl&) = delete;

private:
    void upload_checklist();

    MAVLinkChecklistTransfer::Checklist
    convert_to_int_items(const ChecklistBase::Checklist& checklist_base);

    // FIXME: make static
    std::pair<ChecklistBase::Result, ChecklistBase::Ack> convert_to_result_and_ack(
        MAVLinkChecklistTransfer::Result result, MAVLinkChecklistTransfer::Ack ack);

    static ChecklistBase::Ack convert_ack(MAVLinkChecklistTransfer::Ack ack);

    static ChecklistBase::Result convert_result(MAVLinkChecklistTransfer::Result result);

    struct {
        mutable std::recursive_mutex mutex{};
        ChecklistBase::ResultAckCallback upload_callback{nullptr};
        ChecklistBase::Checklist checklist{};
        std::weak_ptr<MAVLinkChecklistTransfer::WorkItem> last_upload{};
    } _checklist_data{};

    void* _timeout_cookie{nullptr};
};

} // namespace mavsdk
