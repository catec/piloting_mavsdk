#include "hl_action_robotic_vehicle_impl.h"
#include "plugins/hl_action_robotic_vehicle/hl_action_robotic_vehicle.h"

namespace mavsdk {

HLActionRoboticVehicle::HLActionRoboticVehicle(System& system) :
    PluginBase(),
    _impl{new HLActionRoboticVehicleImpl(system)}
{}

HLActionRoboticVehicle::HLActionRoboticVehicle(std::shared_ptr<System> system) :
    PluginBase(),
    _impl{new HLActionRoboticVehicleImpl(system)}
{}

HLActionRoboticVehicle::~HLActionRoboticVehicle() {}

void HLActionRoboticVehicle::set_upload_hl_action(HLActionBase::HLActionList hl_action_list) const
{
    _impl->set_upload_hl_action(hl_action_list);
}

void HLActionRoboticVehicle::upload_hl_action_async(
    const HLActionBase::ResultAckCallback callback, HLActionBase::HLActionList hl_action_list)
{
    _impl->upload_hl_action_async(hl_action_list, callback);
}

HLActionBase::Result HLActionRoboticVehicle::cancel_hl_action_upload() const
{
    return _impl->cancel_hl_action_upload();
}

} // namespace mavsdk
