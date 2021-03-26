#include "telemetry_robotic_vehicle_impl.h"
#include "system.h"
#include "global_include.h"

namespace mavsdk {

TelemetryRoboticVehicleImpl::TelemetryRoboticVehicleImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

TelemetryRoboticVehicleImpl::TelemetryRoboticVehicleImpl(std::shared_ptr<System> system) :
    PluginImplBase(system)
{
    _parent->register_plugin(this);
}

TelemetryRoboticVehicleImpl::~TelemetryRoboticVehicleImpl()
{
    _parent->unregister_plugin(this);
}

void TelemetryRoboticVehicleImpl::init() {}

void TelemetryRoboticVehicleImpl::deinit() {}

void TelemetryRoboticVehicleImpl::enable() {}

void TelemetryRoboticVehicleImpl::disable() {}

void TelemetryRoboticVehicleImpl::send_local_position_ned(
    const TelemetryBase::PositionVelocityNed& local_position_ned)
{
    if (_parent->get_own_system_id() != local_position_ned.system_id) {
        LogWarn() << "System Id (" << unsigned(local_position_ned.system_id)
                  << ") not matching our own system id (" << unsigned(_parent->get_own_system_id()) << ")";
        return;
    }

    mavlink_message_t message;
    mavlink_msg_local_position_ned_pack(
        _parent->get_own_system_id(),
        local_position_ned.component_id,
        &message,
        local_position_ned.stamp_ms,
        local_position_ned.position.north_m,
        local_position_ned.position.east_m,
        local_position_ned.position.down_m,
        local_position_ned.velocity.north_m_s,
        local_position_ned.velocity.east_m_s,
        local_position_ned.velocity.down_m_s);

    _parent->send_message(message);
}

void TelemetryRoboticVehicleImpl::send_attitude(const TelemetryBase::Attitude& attitude)
{
    if (_parent->get_own_system_id() != attitude.system_id) {
        LogWarn() << "System Id (" << unsigned(attitude.system_id)
                  << ") not matching our own system id (" << unsigned(_parent->get_own_system_id()) << ")";
        return;
    }

    mavlink_message_t message;
    mavlink_msg_attitude_quaternion_pack(
        _parent->get_own_system_id(),
        attitude.component_id,
        &message,
        attitude.stamp_ms,
        attitude.quaternion_angle.w,
        attitude.quaternion_angle.x,
        attitude.quaternion_angle.y,
        attitude.quaternion_angle.z,
        attitude.angular_velocity.roll_rad_s,
        attitude.angular_velocity.pitch_rad_s,
        attitude.angular_velocity.yaw_rad_s,
        nullptr);

    _parent->send_message(message);
}

void TelemetryRoboticVehicleImpl::send_text_status(
    const TelemetryBase::TextStatus& text_status)
{
    uint8_t severity_type;
    switch (text_status.type) {
        case TelemetryBase::TextStatusType::Error:
            severity_type = TEXT_STATUS_ERROR;
            break;
        case TelemetryBase::TextStatusType::Warning:
            severity_type = TEXT_STATUS_WARNING;
            break;
        case TelemetryBase::TextStatusType::Info:
            severity_type = TEXT_STATUS_INFO;
            break;
        default:
            LogWarn() << "Unknown StatusText severity";
            return;
    }

    mavlink_message_t message;
    mavlink_msg_text_status_pack(
        _parent->get_own_system_id(),
        _parent->get_own_component_id(),
        &message,
        severity_type,
        text_status.text.c_str());

    _parent->send_message(message);
}

} // namespace mavsdk
