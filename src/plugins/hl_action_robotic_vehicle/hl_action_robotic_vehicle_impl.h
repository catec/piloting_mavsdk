#pragma once

#include "plugins/hl_action_robotic_vehicle/hl_action_robotic_vehicle.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"
#include "mavlink_hl_action_transfer.h"

namespace mavsdk {

class HLActionRoboticVehicleImpl : public PluginImplBase {
public:
    explicit HLActionRoboticVehicleImpl(System& system);
    explicit HLActionRoboticVehicleImpl(std::shared_ptr<System> system);
    ~HLActionRoboticVehicleImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    void set_upload_hl_action(const HLActionBase::HLActionList& hl_action_list);
    void upload_hl_action_async(
        const HLActionBase::HLActionList& hl_action_list,
        const HLActionBase::ResultAckCallback& callback);
    HLActionBase::Result cancel_hl_action_upload();

    // Non-copyable
    HLActionRoboticVehicleImpl(const HLActionRoboticVehicleImpl&) = delete;
    const HLActionRoboticVehicleImpl& operator=(const HLActionRoboticVehicleImpl&) = delete;

private:
    void upload_hl_action();

    MAVLinkHLActionTransfer::HLActionList
    convert_to_int_items(const HLActionBase::HLActionList& hl_actions_list_base);

    // FIXME: make static
    std::pair<HLActionBase::Result, HLActionBase::Ack> convert_to_result_and_ack(
        MAVLinkHLActionTransfer::Result result, MAVLinkHLActionTransfer::Ack ack);

    static HLActionBase::Ack convert_ack(MAVLinkHLActionTransfer::Ack ack);

    static HLActionBase::Result convert_result(MAVLinkHLActionTransfer::Result result);

    struct {
        mutable std::recursive_mutex mutex{};
        HLActionBase::ResultAckCallback upload_callback{nullptr};
        HLActionBase::HLActionList hl_action_list{};
        std::weak_ptr<MAVLinkHLActionTransfer::WorkItem> last_upload{};
    } _hl_action_data{};

    void* _timeout_cookie{nullptr};
};

} // namespace mavsdk
