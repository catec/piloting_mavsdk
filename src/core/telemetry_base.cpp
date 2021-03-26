#include <iomanip>

#include "telemetry_base.h"

namespace mavsdk {

bool operator==(const TelemetryBase::PositionNed& lhs, const TelemetryBase::PositionNed& rhs)
{
    return ((std::isnan(rhs.north_m) && std::isnan(lhs.north_m)) || rhs.north_m == lhs.north_m) &&
           ((std::isnan(rhs.east_m) && std::isnan(lhs.east_m)) || rhs.east_m == lhs.east_m) &&
           ((std::isnan(rhs.down_m) && std::isnan(lhs.down_m)) || rhs.down_m == lhs.down_m);
}

std::ostream& operator<<(std::ostream& str, TelemetryBase::PositionNed const& value)
{
    str << std::setprecision(15);
    str << "position_ned:" << '\n' << "{\n";
    str << "    north_m: " << value.north_m << '\n';
    str << "    east_m: " << value.east_m << '\n';
    str << "    down_m: " << value.down_m << '\n';
    str << '}';
    return str;
}

bool operator==(const TelemetryBase::VelocityNed& lhs, const TelemetryBase::VelocityNed& rhs)
{
    return ((std::isnan(rhs.north_m_s) && std::isnan(lhs.north_m_s)) ||
            rhs.north_m_s == lhs.north_m_s) &&
           ((std::isnan(rhs.east_m_s) && std::isnan(lhs.east_m_s)) ||
            rhs.east_m_s == lhs.east_m_s) &&
           ((std::isnan(rhs.down_m_s) && std::isnan(lhs.down_m_s)) || rhs.down_m_s == lhs.down_m_s);
}

std::ostream& operator<<(std::ostream& str, TelemetryBase::VelocityNed const& value)
{
    str << std::setprecision(15);
    str << "velocity_ned:" << '\n' << "{\n";
    str << "    north_m_s: " << value.north_m_s << '\n';
    str << "    east_m_s: " << value.east_m_s << '\n';
    str << "    down_m_s: " << value.down_m_s << '\n';
    str << '}';
    return str;
}

bool operator==(const TelemetryBase::PositionVelocityNed& lhs, const TelemetryBase::PositionVelocityNed& rhs)
{
    return (lhs.system_id == rhs.system_id) && (lhs.component_id == rhs.component_id) &&
           (rhs.stamp_ms == lhs.stamp_ms) && (rhs.position == lhs.position) && (rhs.velocity == lhs.velocity);
}

std::ostream& operator<<(std::ostream& str, TelemetryBase::PositionVelocityNed const& value)
{
    str << std::setprecision(15);
    str << "position_velocity_ned:" << '\n' << "{\n";
    str << "    system_id: " << unsigned(value.system_id) << '\n';
    str << "    component_id: " << unsigned(value.component_id) << '\n';
    str << "    stamp_ms: " << value.stamp_ms << '\n';
    str << "    position: " << value.position << '\n';
    str << "    velocity: " << value.velocity << '\n';
    str << '}';
    return str;
}

bool operator==(const TelemetryBase::Quaternion& lhs, const TelemetryBase::Quaternion& rhs)
{
    return ((std::isnan(rhs.w) && std::isnan(lhs.w)) || rhs.w == lhs.w) &&
           ((std::isnan(rhs.x) && std::isnan(lhs.x)) || rhs.x == lhs.x) &&
           ((std::isnan(rhs.y) && std::isnan(lhs.y)) || rhs.y == lhs.y) &&
           ((std::isnan(rhs.z) && std::isnan(lhs.z)) || rhs.z == lhs.z);
}

std::ostream& operator<<(std::ostream& str, TelemetryBase::Quaternion const& value)
{
    str << std::setprecision(15);
    str << "quaternion:" << '\n' << "{\n";
    str << "    w: " << value.w << '\n';
    str << "    x: " << value.x << '\n';
    str << "    y: " << value.y << '\n';
    str << "    z: " << value.z << '\n';
    str << '}';
    return str;
}

bool operator==(const TelemetryBase::AngularVelocityBody& lhs, const TelemetryBase::AngularVelocityBody& rhs)
{
    return ((std::isnan(rhs.roll_rad_s) && std::isnan(lhs.roll_rad_s)) ||
            rhs.roll_rad_s == lhs.roll_rad_s) &&
           ((std::isnan(rhs.pitch_rad_s) && std::isnan(lhs.pitch_rad_s)) ||
            rhs.pitch_rad_s == lhs.pitch_rad_s) &&
           ((std::isnan(rhs.yaw_rad_s) && std::isnan(lhs.yaw_rad_s)) ||
            rhs.yaw_rad_s == lhs.yaw_rad_s);
}

std::ostream& operator<<(std::ostream& str, TelemetryBase::AngularVelocityBody const& value)
{
    str << std::setprecision(15);
    str << "angular_velocity_body:" << '\n' << "{\n";
    str << "    roll_rad_s: " << value.roll_rad_s << '\n';
    str << "    pitch_rad_s: " << value.pitch_rad_s << '\n';
    str << "    yaw_rad_s: " << value.yaw_rad_s << '\n';
    str << '}';
    return str;
}

bool operator==(const TelemetryBase::Attitude& lhs, const TelemetryBase::Attitude& rhs)
{
    return (lhs.system_id == rhs.system_id) && (lhs.component_id == rhs.component_id) &&
           (rhs.stamp_ms == lhs.stamp_ms) && (lhs.quaternion_angle == rhs.quaternion_angle) &&
           (lhs.angular_velocity == rhs.angular_velocity);
}

std::ostream& operator<<(std::ostream& str, TelemetryBase::Attitude const& value)
{
    str << std::setprecision(15);
    str << "attitude:" << '\n' << "{\n";
    str << "    system_id: " << unsigned(value.system_id) << '\n';
    str << "    component_id: " << unsigned(value.component_id) << '\n';
    str << "    stamp_ms: " << value.stamp_ms << '\n';
    str << "    quaternion_angle: " << value.quaternion_angle << '\n';
    str << "    angular_velocity: " << value.angular_velocity << '\n';
    str << '}';
    return str;
}

std::ostream& operator<<(std::ostream& str, TelemetryBase::TextStatusType const& value)
{
    switch (value) {
        case TelemetryBase::TextStatusType::Info:
            return str << "Info";
        case TelemetryBase::TextStatusType::Warning:
            return str << "Warning";
        case TelemetryBase::TextStatusType::Error:
            return str << "Error";
        default:
            return str << "Unknown";
    }
}

bool operator==(const TelemetryBase::TextStatus& lhs, const TelemetryBase::TextStatus& rhs)
{
    return (rhs.type == lhs.type) && (rhs.text == lhs.text);
}

std::ostream& operator<<(std::ostream& str, TelemetryBase::TextStatus const& value)
{
    str << std::setprecision(15);
    str << "text_status:" << '\n' << "{\n";
    str << "    type: " << value.type << '\n';
    str << "    text: " << value.text << '\n';
    str << '}';
    return str;
}

} // namespace mavsdk
