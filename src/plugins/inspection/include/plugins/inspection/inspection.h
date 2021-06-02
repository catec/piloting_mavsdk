#pragma once

#include <memory>

#include "plugin_base.h"
#include "inspection_base.h"

namespace mavsdk {

class System;
class InspectionImpl;

/**
 * @brief Enable waypoint list.
 */
class Inspection : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto inspection = Inspection(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Inspection(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto inspection = Inspection(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Inspection(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~Inspection();

    /**
     * @brief Upload a list of waypoint items to the system.
     *
     * This function is non-blocking. See 'upload_inspection' for the blocking counterpart.
     */
    void upload_inspection_async(
        InspectionBase::WaypointList list,
        const InspectionBase::ResultAckCallback callback);

    /**
     * @brief Upload a list of waypoint items to the system.
     *
     * This function is blocking. See 'upload_inspection_async' for the non-blocking counterpart.
     *
     * @return Result of request.
     */
    std::pair<InspectionBase::Result, InspectionBase::Ack>
    upload_inspection(InspectionBase::WaypointList list) const;

    /**
     * @brief Cancel an ongoing inspection upload.
     *
     * This function is blocking.
     *
     * @return Result of request.
     */
    InspectionBase::Result cancel_inspection_upload() const;

    /**
     * @brief Download a list of waypoint items from the system (asynchronous).
     *
     * This function is non-blocking. See 'download_inspection' for the blocking counterpart.
     */
    void download_inspection_async(const InspectionBase::DownloadInspectionCallback callback);

    /**
     * @brief Download a list of waypoint items from the system (asynchronous).
     *
     * This function is blocking. See 'download_inspection_async' for the non-blocking counterpart.
     *
     * @return Result of request.
     */
    std::pair<InspectionBase::Result, InspectionBase::WaypointList> download_inspection() const;

    /**
     * @brief Cancel an ongoing inspection download.
     *
     * This function is blocking.
     *
     * @return Result of request.
     */
    InspectionBase::Result cancel_inspection_download() const;

    /**
     * @brief Sets the waypoint item index to go to.
     *
     * By setting the current index to 0, the inspection is restarted from the beginning. If it
     * is set to a specific index of a waypoint item, the inspection will be set to this item.
     *
     * This function is non-blocking.
     */
    void set_current_inspection_item(uint16_t index) const;

    /**
     * @brief Subscribe to 'InspectionProgress' updates.
     */
    void subscribe_inspection_progress(InspectionBase::InspectionProgressCallback callback);

    /**
     * @brief Poll for 'InspectionProgress' (blocking).
     *
     * @return One InspectionProgress update.
     */
    InspectionBase::InspectionProgress inspection_progress() const;

    /**
     * @brief Copy constructor.
     */
    Inspection(const Inspection& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const Inspection& operator=(const Inspection&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<InspectionImpl> _impl;
};

} // namespace mavsdk
