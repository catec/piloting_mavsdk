#pragma once

#include <memory>

#include "plugin_base.h"
#include "telemetry_base.h"

namespace mavsdk {

class System;
class TelemetryImpl;

/**
 * @brief Allow users to get vehicle telemetry and state information
 */
class Telemetry : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto telemetry = Telemetry(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Telemetry(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto telemetry = Telemetry(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Telemetry(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~Telemetry();

    /**
     * @brief Subscribe to 'PositionVelocityNed' updates.
     */
    void subscribe_position_velocity_ned(TelemetryBase::PositionVelocityNedCallback callback);

    /**
     * @brief Poll for 'PositionVelocityNed' (blocking).
     *
     * @return One PositionVelocityNed update.
     */
    TelemetryBase::PositionVelocityNed position_velocity_ned() const;

    /**
     * @brief Subscribe to 'Attitude' updates.
     */
    void subscribe_attitude(TelemetryBase::AttitudeCallback callback);

    /**
     * @brief Poll for 'Attitude' (blocking).
     *
     * @return One attitude update.
     */
    TelemetryBase::Attitude attitude() const;

    /**
     * @brief Subscribe to 'TextStatus' updates.
     */
    void subscribe_text_status(TelemetryBase::TextStatusCallback callback);

    /**
     * @brief Poll for 'TextStatus' (blocking).
     *
     * @return One TextStatus update.
     */
    TelemetryBase::TextStatus text_status() const;

    /**
     * @brief Copy constructor.
     */
    Telemetry(const Telemetry& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const Telemetry& operator=(const Telemetry&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<TelemetryImpl> _impl;
};

} // namespace mavsdk
