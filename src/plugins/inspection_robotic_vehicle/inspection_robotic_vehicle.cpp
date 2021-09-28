#include "inspection_robotic_vehicle_impl.h"
#include "plugins/inspection_robotic_vehicle/inspection_robotic_vehicle.h"

namespace mavsdk {

InspectionRoboticVehicle::InspectionRoboticVehicle(System& system) :
    PluginBase(),
    _impl{new InspectionRoboticVehicleImpl(system)}
{}

InspectionRoboticVehicle::InspectionRoboticVehicle(std::shared_ptr<System> system) :
    PluginBase(),
    _impl{new InspectionRoboticVehicleImpl(system)}
{}

InspectionRoboticVehicle::~InspectionRoboticVehicle() {}

void InspectionRoboticVehicle::set_upload_inspection(InspectionBase::WaypointList list) const
{
    _impl->set_upload_inspection(list);
}

void InspectionRoboticVehicle::upload_inspection_async(
    const InspectionBase::ResultAckCallback callback, InspectionBase::WaypointList list)
{
    _impl->upload_inspection_async(list, callback);
}

InspectionBase::Result InspectionRoboticVehicle::cancel_inspection_upload() const
{
    return _impl->cancel_inspection_upload();
}

void InspectionRoboticVehicle::download_inspection_async(
    InspectionBase::DownloadInspectionCallback callback)
{
    _impl->download_inspection_async(callback);
}

InspectionBase::Result InspectionRoboticVehicle::send_download_ack(
    const InspectionBase::Ack& ack)
{
    return _impl->send_download_ack(ack);
}

InspectionBase::Result InspectionRoboticVehicle::cancel_inspection_download() const
{
    return _impl->cancel_inspection_download();
}

void InspectionRoboticVehicle::subscribe_inspection_set_current(
    InspectionBase::InspectionSetCurrentCallback callback)
{
    _impl->inspection_set_current_async(callback);
}

void InspectionRoboticVehicle::update_current_inspection_item(const uint16_t item_seq)
{
    _impl->update_current_inspection_item(item_seq);
}

void InspectionRoboticVehicle::update_reached_inspection_item(const uint16_t item_seq)
{
    _impl->update_reached_inspection_item(item_seq);
}

} // namespace mavsdk
