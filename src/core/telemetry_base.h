#pragma once

#include <string>
#include <functional>
#include <cmath>
#include <vector>

namespace mavsdk {

/**
 * @brief Base class for every telemetry.
 */
class TelemetryBase {
public:
    /**
     * @brief Default Constructor.
     */
    TelemetryBase() = default;

    /**
     * @brief Default Destructor.
     */
    virtual ~TelemetryBase() = default;

    /**
     * @brief PositionNed message type.
     */
    struct PositionNed {
        float north_m{float(NAN)}; /**< @brief Position along north direction in metres */
        float east_m{float(NAN)}; /**< @brief Position along east direction in metres */
        float down_m{float(NAN)}; /**< @brief Position along down direction in metres */
    };

    /**
     * @brief Equal operator to compare two `TelemetryBase::PositionNed` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(const TelemetryBase::PositionNed& lhs, const TelemetryBase::PositionNed& rhs);

    /**
     * @brief Stream operator to print information about a `TelemetryBase::PositionNed`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, TelemetryBase::PositionNed const& value);

    /**
     * @brief VelocityNed message type.
     */
    struct VelocityNed {
        float north_m_s{}; /**< @brief Velocity along north direction in metres per second */
        float east_m_s{}; /**< @brief Velocity along east direction in metres per second */
        float down_m_s{}; /**< @brief Velocity along down direction in metres per second */
    };

    /**
     * @brief Equal operator to compare two `TelemetryBase::VelocityNed` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(const TelemetryBase::VelocityNed& lhs, const TelemetryBase::VelocityNed& rhs);

    /**
     * @brief Stream operator to print information about a `TelemetryBase::VelocityNed`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, TelemetryBase::VelocityNed const& value);

    /**
     * @brief PoseNed message type.
     */
    struct PositionVelocityNed {
        uint8_t system_id{};
        uint8_t component_id{};
        uint32_t stamp_ms{}; /**< @brief Timestamp (ms) */
        PositionNed position{}; /**< @brief Position (NED) */
        VelocityNed velocity{}; /**< @brief Velocity (NED) */
    };

    /**
     * @brief Equal operator to compare two `TelemetryBase::PositionVelocityNed` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(const TelemetryBase::PositionVelocityNed& lhs, const TelemetryBase::PositionVelocityNed& rhs);

    /**
     * @brief Stream operator to print information about a
     * `TelemetryBase::PositionVelocityNed`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, TelemetryBase::PositionVelocityNed const& value);

    /**
     * @brief Quaternion type.
     *
     * All rotations and axis systems follow the right-hand rule.
     * The Hamilton quaternion product definition is used.
     * A zero-rotation quaternion is represented by (1,0,0,0).
     * The quaternion could also be written as w + xi + yj + zk.
     *
     * For more info see: https://en.wikipedia.org/wiki/Quaternion
     */
    struct Quaternion {
        float w{float(NAN)}; /**< @brief Quaternion entry 0, also denoted as a */
        float x{float(NAN)}; /**< @brief Quaternion entry 1, also denoted as b */
        float y{float(NAN)}; /**< @brief Quaternion entry 2, also denoted as c */
        float z{float(NAN)}; /**< @brief Quaternion entry 3, also denoted as d */
    };

    /**
     * @brief Equal operator to compare two `TelemetryBase::Quaternion` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(const TelemetryBase::Quaternion& lhs, const TelemetryBase::Quaternion& rhs);

    /**
     * @brief Stream operator to print information about a `TelemetryBase::Quaternion`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, TelemetryBase::Quaternion const& value);

    /**
     * @brief Angular velocity type.
     */
    struct AngularVelocityBody {
        float roll_rad_s{float(NAN)}; /**< @brief Roll angular velocity */
        float pitch_rad_s{float(NAN)}; /**< @brief Pitch angular velocity */
        float yaw_rad_s{float(NAN)}; /**< @brief Yaw angular velocity */
    };

    /**
     * @brief Equal operator to compare two `TelemetryBase::AngularVelocityBody` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(const TelemetryBase::AngularVelocityBody& lhs, const TelemetryBase::AngularVelocityBody& rhs);

    /**
     * @brief Stream operator to print information about a
     * `TelemetryBase::AngularVelocityBody`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, TelemetryBase::AngularVelocityBody const& value);

    struct Attitude {
        uint8_t system_id{};
        uint8_t component_id{};
        uint32_t stamp_ms{}; /**< @brief Timestamp (ms) */
        Quaternion quaternion_angle{};
        AngularVelocityBody angular_velocity{};
    };

    /**
     * @brief Equal operator to compare two `TelemetryBase::Attitude` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(const TelemetryBase::Attitude& lhs, const TelemetryBase::Attitude& rhs);

    /**
     * @brief Stream operator to print information about a
     * `TelemetryBase::Attitude`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, TelemetryBase::Attitude const& value);

    /**
     * @brief Text Status types.
     */
    enum class TextStatusType {
        Error, /**< @brief Error. */
        Warning, /**< @brief Warning. */
        Info, /**< @brief Information. */
    };

    /**
     * @brief Stream operator to print information about a
     * `TelemetryBase::TextStatusType`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, TelemetryBase::TextStatusType const& value);

    /**
     * @brief TextStatus information type.
     */
    struct TextStatus {
        TextStatusType type{}; /**< @brief Message type */
        std::string text{}; /**< @brief MAVLink status message */
    };

    /**
     * @brief Equal operator to compare two `TelemetryBase::TextStatus` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool operator==(const TelemetryBase::TextStatus& lhs, const TelemetryBase::TextStatus& rhs);

    /**
     * @brief Stream operator to print information about a `TelemetryBase::TextStatus`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, TelemetryBase::TextStatus const& value);

    /**
     * @brief Callback type for subscribe_position_velocity_ned.
     */
    using PositionVelocityNedCallback = std::function<void(PositionVelocityNed)>;

    /**
     * @brief Callback type for subscribe_attitude.
     */
    using AttitudeCallback = std::function<void(Attitude)>;

    /**
     * @brief Callback type for subscribe_text_status.
     */
    using TextStatusCallback = std::function<void(TextStatus)>;

    /**
     * @brief Copy constructor (object is not copyable).
     */
    TelemetryBase(const TelemetryBase&) = delete;

    /**
     * @brief Assign operator (object is not copyable).
     */
    const TelemetryBase& operator=(const TelemetryBase&) = delete;
};

} // namespace mavsdk
