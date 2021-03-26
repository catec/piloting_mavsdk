#pragma once

#include <memory>

#include "plugin_base.h"
#include "alarm_base.h"

namespace mavsdk {

class System;
class AlarmRoboticVehicleImpl;

/**
 * @brief Enable alarms.
 */
class AlarmRoboticVehicle : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto alarm = AlarmRoboticVehicle(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit AlarmRoboticVehicle(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto alarm = AlarmRoboticVehicle(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit AlarmRoboticVehicle(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~AlarmRoboticVehicle();

    /**
     * @brief Sets the alarm list to be uploaded if requested by gRCS
     *
     * This function is non-blocking.
     */
    void set_upload_alarm(AlarmBase::AlarmList alarms_list) const;

    /**
     * @brief Upload a list of alarm items to the system.
     *
     * Every time the robotic vehicle alarm list changes,
     * it must be updated through the method 'set_upload_alarm'
     *
     * This function is non-blocking.
     */
    void upload_alarm_async(
        const AlarmBase::ResultAckCallback callback,
        AlarmBase::AlarmList alarm_list = {});

    /**
     * @brief Cancel an ongoing alarm upload.
     *
     * This function is blocking.
     *
     * @return Result of request.
     */
    AlarmBase::Result cancel_alarm_upload() const;

    /**
     * @brief Send 'AlarmStatus' updates.
     */
    void send_alarm_status(const AlarmBase::AlarmStatus&);

    /**
     * @brief Copy constructor.
     */
    AlarmRoboticVehicle(const AlarmRoboticVehicle& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const AlarmRoboticVehicle& operator=(const AlarmRoboticVehicle&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<AlarmRoboticVehicleImpl> _impl;
};

} // namespace mavsdk
