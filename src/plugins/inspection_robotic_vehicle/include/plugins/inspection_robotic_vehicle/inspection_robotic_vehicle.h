#pragma once

#include <memory>

#include "plugin_base.h"
#include "inspection_base.h"

namespace mavsdk {

class System;
class InspectionRoboticVehicleImpl;

/**
 * @brief Enable waypoint list.
 */
class InspectionRoboticVehicle : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto inspection = InspectionRoboticVehicle(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit InspectionRoboticVehicle(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto inspection = InspectionRoboticVehicle(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit InspectionRoboticVehicle(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~InspectionRoboticVehicle();

    /**
     * @brief Sets the waypoint list to be uploaded if requested by gRCS
     *
     * This function is non-blocking.
     */
    void set_upload_inspection(InspectionBase::WaypointList list) const;

    /**
     * @brief Upload a list of waypoint items to the system.
     *
     * Every time the robotic vehicle waypoint list changes,
     * it must be updated through the method 'set_upload_inspection'
     *
     * This function is non-blocking.
     */
    void upload_inspection_async(
        const InspectionBase::ResultAckCallback callback,
        InspectionBase::WaypointList list = {});

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
     * This function is non-blocking.
     */
    void download_inspection_async(InspectionBase::DownloadInspectionCallback callback);

    /**
     * @brief Cancel an ongoing inspection download.
     *
     * This function is blocking.
     *
     * @return Result of request.
     */
    InspectionBase::Result cancel_inspection_download() const;

    /**
     * @brief Subscribe to inspection set current command.
     *
     * The callback must be answered immediately using the method 'update_current_inspection_item'
     */
    void subscribe_inspection_set_current(InspectionBase::InspectionSetCurrentCallback callback);

    /**
     * @brief Update current inspection item.
     */
    void update_current_inspection_item(const uint16_t item_seq);

    /**
     * @brief Update reached inspection item.
     */
    void update_reached_inspection_item(const uint16_t item_seq);

    /**
     * @brief Copy constructor.
     */
    InspectionRoboticVehicle(const InspectionRoboticVehicle& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const InspectionRoboticVehicle& operator=(const InspectionRoboticVehicle&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<InspectionRoboticVehicleImpl> _impl;
};

} // namespace mavsdk
