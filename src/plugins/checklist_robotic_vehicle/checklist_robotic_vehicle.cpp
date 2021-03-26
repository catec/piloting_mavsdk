#include "checklist_robotic_vehicle_impl.h"
#include "plugins/checklist_robotic_vehicle/checklist_robotic_vehicle.h"

namespace mavsdk {

ChecklistRoboticVehicle::ChecklistRoboticVehicle(System& system) :
    PluginBase(),
    _impl{new ChecklistRoboticVehicleImpl(system)}
{}

ChecklistRoboticVehicle::ChecklistRoboticVehicle(std::shared_ptr<System> system) :
    PluginBase(),
    _impl{new ChecklistRoboticVehicleImpl(system)}
{}

ChecklistRoboticVehicle::~ChecklistRoboticVehicle() {}

void ChecklistRoboticVehicle::set_upload_checklist(ChecklistBase::Checklist checklist) const
{
    _impl->set_upload_checklist(checklist);
}

void ChecklistRoboticVehicle::upload_checklist_async(
    const ChecklistBase::ResultAckCallback callback, ChecklistBase::Checklist checklist)
{
    _impl->upload_checklist_async(checklist, callback);
}

ChecklistBase::Result ChecklistRoboticVehicle::cancel_checklist_upload() const
{
    return _impl->cancel_checklist_upload();
}

} // namespace mavsdk
