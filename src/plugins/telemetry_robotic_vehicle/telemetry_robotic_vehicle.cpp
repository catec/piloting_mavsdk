#include "telemetry_robotic_vehicle_impl.h"
#include "plugins/telemetry_robotic_vehicle/telemetry_robotic_vehicle.h"

namespace mavsdk {

using PositionVelocityNed = TelemetryBase::PositionVelocityNed;
using Attitude = TelemetryBase::Attitude;
using TextStatus = TelemetryBase::TextStatus;

TelemetryRoboticVehicle::TelemetryRoboticVehicle(System& system) :
    PluginBase(),
    _impl{new TelemetryRoboticVehicleImpl(system)}
{}

TelemetryRoboticVehicle::TelemetryRoboticVehicle(std::shared_ptr<System> system) :
    PluginBase(),
    _impl{new TelemetryRoboticVehicleImpl(system)}
{}

TelemetryRoboticVehicle::~TelemetryRoboticVehicle() {}


void TelemetryRoboticVehicle::send_local_position_ned(const PositionVelocityNed& local_position_ned)
{
    _impl->send_local_position_ned(local_position_ned);
}

void TelemetryRoboticVehicle::send_attitude(const Attitude& attitude)
{
    _impl->send_attitude(attitude);
}

void TelemetryRoboticVehicle::send_text_status(const TextStatus& text_status)
{
    _impl->send_text_status(text_status);
}

} // namespace mavsdk
