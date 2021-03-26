#pragma once

#include <memory>

#include "plugin_base.h"
#include "telemetry_base.h"

namespace mavsdk {

class System;
class TelemetryRoboticVehicleImpl;

/**
 * @brief Allow users to set vehicle telemetry and state information
 */
class TelemetryRoboticVehicle : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto telemetry_robotic_vehicle = TelemetryRoboticVehicle(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit TelemetryRoboticVehicle(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto telemetry_robotic_vehicle = TelemetryRoboticVehicle(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit TelemetryRoboticVehicle(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~TelemetryRoboticVehicle();

    /**
     * @brief Send 'PositionVelocityNed' updates.
     */
    void send_local_position_ned(const TelemetryBase::PositionVelocityNed&);

    /**
     * @brief Send 'Attitude' updates.
     */
    void send_attitude(const TelemetryBase::Attitude&);

    /**
     * @brief Send 'TextStatus' updates.
     */
    void send_text_status(const TelemetryBase::TextStatus&);

    /**
     * @brief Copy constructor.
     */
    TelemetryRoboticVehicle(const TelemetryRoboticVehicle& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const TelemetryRoboticVehicle& operator=(const TelemetryRoboticVehicle&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<TelemetryRoboticVehicleImpl> _impl;
};

} // namespace mavsdk
