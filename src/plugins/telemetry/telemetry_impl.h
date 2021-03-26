#pragma once

#include "plugins/telemetry/telemetry.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"

namespace mavsdk {

class System;

class TelemetryImpl : public PluginImplBase {
public:
    explicit TelemetryImpl(System& system);
    explicit TelemetryImpl(std::shared_ptr<System> system);
    ~TelemetryImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    TelemetryBase::PositionVelocityNed position_velocity_ned() const;
    TelemetryBase::Attitude attitude() const;
    TelemetryBase::TextStatus text_status() const;

    void position_velocity_ned_async(TelemetryBase::PositionVelocityNedCallback&);
    void attitude_async(TelemetryBase::AttitudeCallback&);
    void text_status_async(TelemetryBase::TextStatusCallback&);

    TelemetryImpl(const TelemetryImpl&) = delete;
    TelemetryImpl& operator=(const TelemetryImpl&) = delete;

private:
    void process_local_position_ned(const mavlink_message_t&);
    void process_attitude_quaternion(const mavlink_message_t&);
    void process_text_status(const mavlink_message_t&);

    // The mutexs are mutable so that the lock can get aqcuired in
    // methods marked const.
    mutable std::mutex _position_velocity_ned_mutex{};
    TelemetryBase::PositionVelocityNed _position_velocity_ned{};

    mutable std::mutex _attitude_mutex{};
    TelemetryBase::Attitude _attitude{};

    mutable std::mutex _text_status_mutex{};
    TelemetryBase::TextStatus _text_status{};

    std::mutex _subscription_mutex{};
    TelemetryBase::PositionVelocityNedCallback _position_velocity_ned_subscription{nullptr};
    TelemetryBase::AttitudeCallback _attitude_subscription{nullptr};
    TelemetryBase::TextStatusCallback _text_status_subscription{nullptr};
};

} // namespace mavsdk
