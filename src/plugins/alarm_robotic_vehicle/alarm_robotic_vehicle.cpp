#include "alarm_robotic_vehicle_impl.h"
#include "plugins/alarm_robotic_vehicle/alarm_robotic_vehicle.h"

namespace mavsdk {

AlarmRoboticVehicle::AlarmRoboticVehicle(System& system) :
    PluginBase(),
    _impl{new AlarmRoboticVehicleImpl(system)}
{}

AlarmRoboticVehicle::AlarmRoboticVehicle(std::shared_ptr<System> system) :
    PluginBase(),
    _impl{new AlarmRoboticVehicleImpl(system)}
{}

AlarmRoboticVehicle::~AlarmRoboticVehicle() {}

void AlarmRoboticVehicle::set_upload_alarm(AlarmBase::AlarmList alarms_list) const
{
    _impl->set_upload_alarm(alarms_list);
}

void AlarmRoboticVehicle::upload_alarm_async(
    const AlarmBase::ResultAckCallback callback, AlarmBase::AlarmList alarms_list)
{
    _impl->upload_alarm_async(alarms_list, callback);
}

AlarmBase::Result AlarmRoboticVehicle::cancel_alarm_upload() const
{
    return _impl->cancel_alarm_upload();
}

void AlarmRoboticVehicle::send_alarm_status(const AlarmBase::AlarmStatus & alarm_status)
{
    _impl->send_alarm_status(alarm_status);
}

} // namespace mavsdk
