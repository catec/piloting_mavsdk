#pragma once

#include "plugins/telemetry_robotic_vehicle/telemetry_robotic_vehicle.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"

namespace mavsdk {

class System;

class TelemetryRoboticVehicleImpl : public PluginImplBase {
public:
    explicit TelemetryRoboticVehicleImpl(System& system);
    explicit TelemetryRoboticVehicleImpl(std::shared_ptr<System> system);
    ~TelemetryRoboticVehicleImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    void send_local_position_ned(const TelemetryBase::PositionVelocityNed&);
    void send_attitude(const TelemetryBase::Attitude&);
    void send_text_status(const TelemetryBase::TextStatus &);
};

} // namespace mavsdk
