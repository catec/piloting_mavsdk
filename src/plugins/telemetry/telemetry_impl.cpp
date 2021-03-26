#include "telemetry_impl.h"
#include "system.h"
#include "global_include.h"

namespace mavsdk {

using namespace std::placeholders; // for `_1`

TelemetryImpl::TelemetryImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

TelemetryImpl::TelemetryImpl(std::shared_ptr<System> system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

TelemetryImpl::~TelemetryImpl()
{
    _parent->unregister_plugin(this);
}

void TelemetryImpl::init()
{
    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_LOCAL_POSITION_NED,
        std::bind(&TelemetryImpl::process_local_position_ned, this, _1),
        this);

    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_ATTITUDE_QUATERNION,
        std::bind(&TelemetryImpl::process_attitude_quaternion, this, _1),
        this);

    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_TEXT_STATUS,
        std::bind(&TelemetryImpl::process_text_status, this, _1),
        this);
}

void TelemetryImpl::deinit()
{
    _parent->unregister_all_mavlink_message_handlers(this);
}

void TelemetryImpl::enable() {}

void TelemetryImpl::disable() {}

void TelemetryImpl::process_local_position_ned(const mavlink_message_t& message)
{
    mavlink_local_position_ned_t local_position;
    mavlink_msg_local_position_ned_decode(&message, &local_position);

    TelemetryBase::PositionVelocityNed position;
    position.system_id          = message.sysid;
    position.component_id       = message.compid;
    position.stamp_ms           = local_position.time_boot_ms;
    position.position.north_m   = local_position.x;
    position.position.east_m    = local_position.y;
    position.position.down_m    = local_position.z;
    position.velocity.north_m_s = local_position.vx;
    position.velocity.east_m_s  = local_position.vy;
    position.velocity.down_m_s  = local_position.vz;

    {
        std::lock_guard<std::mutex> lock(_position_velocity_ned_mutex);
        _position_velocity_ned = position;
    }

    std::lock_guard<std::mutex> lock(_subscription_mutex);
    if (_position_velocity_ned_subscription) {
        auto temp_callback = _position_velocity_ned_subscription;
        _parent->call_user_callback([temp_callback, position]() { temp_callback(position); });
    }
}

void TelemetryImpl::process_attitude_quaternion(const mavlink_message_t& message)
{
    mavlink_attitude_quaternion_t attitude_quaternion;
    mavlink_msg_attitude_quaternion_decode(&message, &attitude_quaternion);

    TelemetryBase::Attitude attitude;
    attitude.system_id                    = message.sysid;
    attitude.component_id                 = message.compid;
    attitude.stamp_ms                     = attitude_quaternion.time_boot_ms;
    attitude.quaternion_angle.w           = attitude_quaternion.q1;
    attitude.quaternion_angle.x           = attitude_quaternion.q2;
    attitude.quaternion_angle.y           = attitude_quaternion.q3;
    attitude.quaternion_angle.z           = attitude_quaternion.q4;
    attitude.angular_velocity.roll_rad_s  = attitude_quaternion.rollspeed;
    attitude.angular_velocity.pitch_rad_s = attitude_quaternion.pitchspeed;
    attitude.angular_velocity.yaw_rad_s   = attitude_quaternion.yawspeed;

    {
        std::lock_guard<std::mutex> lock(_attitude_mutex);
        _attitude = attitude;
    }

    std::lock_guard<std::mutex> lock(_subscription_mutex);
    if (_attitude_subscription) {
        auto temp_callback = _attitude_subscription;
        _parent->call_user_callback([temp_callback, attitude]() { temp_callback(attitude); });
    }
}

void TelemetryImpl::process_text_status(const mavlink_message_t& message)
{
    mavlink_text_status_t text_status_data;
    mavlink_msg_text_status_decode(&message, &text_status_data);

    TelemetryBase::TextStatusType type;
    switch (text_status_data.severity) {
        case TEXT_STATUS_ERROR:
            type = TelemetryBase::TextStatusType::Error;
            break;
        case TEXT_STATUS_WARNING:
            type = TelemetryBase::TextStatusType::Warning;
            break;
        case TEXT_STATUS_INFO:
            type = TelemetryBase::TextStatusType::Info;
            break;
        default:
            LogWarn() << "Unknown StatusText severity";
            return;
    }

    char text_with_null[sizeof(text_status_data.text) + 1]{};
    memcpy(text_with_null, text_status_data.text, sizeof(text_status_data.text));

    TelemetryBase::TextStatus text_status;
    text_status.type = type;
    text_status.text = text_with_null;

    {
        std::lock_guard<std::mutex> lock(_text_status_mutex);
        _text_status = text_status;
    }

    std::lock_guard<std::mutex> lock(_subscription_mutex);
    if (_text_status_subscription) {
        auto temp_callback = _text_status_subscription;
        _parent->call_user_callback([temp_callback, text_status]() { temp_callback(text_status); });
    }
}

TelemetryBase::PositionVelocityNed TelemetryImpl::position_velocity_ned() const
{
    std::lock_guard<std::mutex> lock(_position_velocity_ned_mutex);
    return _position_velocity_ned;
}

TelemetryBase::Attitude TelemetryImpl::attitude() const
{
    std::lock_guard<std::mutex> lock(_attitude_mutex);
    return _attitude;
}

TelemetryBase::TextStatus TelemetryImpl::text_status() const
{
    std::lock_guard<std::mutex> lock(_text_status_mutex);
    return _text_status;
}

void TelemetryImpl::position_velocity_ned_async(TelemetryBase::PositionVelocityNedCallback& callback)
{
    std::lock_guard<std::mutex> lock(_subscription_mutex);
    _position_velocity_ned_subscription = callback;
}

void TelemetryImpl::attitude_async(TelemetryBase::AttitudeCallback& callback)
{
    std::lock_guard<std::mutex> lock(_subscription_mutex);
    _attitude_subscription = callback;
}

void TelemetryImpl::text_status_async(TelemetryBase::TextStatusCallback& callback)
{
    std::lock_guard<std::mutex> lock(_subscription_mutex);
    _text_status_subscription = callback;
}

} // namespace mavsdk
