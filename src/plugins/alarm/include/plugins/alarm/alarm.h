#pragma once

#include <memory>

#include "plugin_base.h"
#include "alarm_base.h"

namespace mavsdk {

class System;
class AlarmImpl;

/**
 * @brief Enable alarms.
 */
class Alarm : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto alarm = Alarm(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Alarm(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto alarm = Alarm(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Alarm(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~Alarm();

    /**
     * @brief Download a list of alarm items from the system (asynchronous).
     *
     * This function is non-blocking. See 'download_alarm' for the blocking counterpart.
     */
    void download_alarm_async(const AlarmBase::DownloadAlarmListCallback callback);

    /**
     * @brief Download a list of alarm items from the system (asynchronous).
     *
     * This function is blocking. See 'download_alarm_async' for the non-blocking counterpart.
     *
     * @return Result of request.
     */
    std::pair<AlarmBase::Result, AlarmBase::AlarmList> download_alarm() const;

    /**
     * @brief Cancel an ongoing alarm download.
     *
     * This function is blocking.
     *
     * @return Result of request.
     */
    AlarmBase::Result cancel_alarm_download() const;

    /**
     * @brief Subscribe to 'AlarmStatus' updates.
     */
    void subscribe_alarm_status(AlarmBase::AlarmStatusCallback callback);

    /**
     * @brief Copy constructor.
     */
    Alarm(const Alarm& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const Alarm& operator=(const Alarm&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<AlarmImpl> _impl;
};

} // namespace mavsdk
