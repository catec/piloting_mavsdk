#include "telemetry_impl.h"
#include "plugins/telemetry/telemetry.h"

namespace mavsdk {

using PositionVelocityNed = TelemetryBase::PositionVelocityNed;
using Attitude = TelemetryBase::Attitude;
using TextStatus = TelemetryBase::TextStatus;
using PositionVelocityNedCallback = TelemetryBase::PositionVelocityNedCallback;
using AttitudeCallback = TelemetryBase::AttitudeCallback;
using TextStatusCallback = TelemetryBase::TextStatusCallback;

Telemetry::Telemetry(System& system) :
    PluginBase(),
    _impl{new TelemetryImpl(system)}
{}

Telemetry::Telemetry(std::shared_ptr<System> system) :
    PluginBase(),
    _impl{new TelemetryImpl(system)}
{}

Telemetry::~Telemetry() {}

void Telemetry::subscribe_position_velocity_ned(PositionVelocityNedCallback callback)
{
    _impl->position_velocity_ned_async(callback);
}

PositionVelocityNed Telemetry::position_velocity_ned() const
{
    return _impl->position_velocity_ned();
}

void Telemetry::subscribe_attitude(AttitudeCallback callback)
{
    _impl->attitude_async(callback);
}

Attitude Telemetry::attitude() const
{
    return _impl->attitude();
}

void Telemetry::subscribe_text_status(TextStatusCallback callback)
{
    _impl->text_status_async(callback);
}

TextStatus Telemetry::text_status() const
{
    return _impl->text_status();
}

} // namespace mavsdk
