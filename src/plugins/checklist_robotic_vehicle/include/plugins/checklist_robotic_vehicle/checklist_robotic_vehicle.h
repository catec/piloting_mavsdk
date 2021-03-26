#pragma once

#include <memory>

#include "plugin_base.h"
#include "checklist_base.h"

namespace mavsdk {

class System;
class ChecklistRoboticVehicleImpl;

/**
 * @brief Enable checklists.
 */
class ChecklistRoboticVehicle : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto checklist = ChecklistRoboticVehicle(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit ChecklistRoboticVehicle(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto checklist = ChecklistRoboticVehicle(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit ChecklistRoboticVehicle(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~ChecklistRoboticVehicle();

    /**
     * @brief Sets the checklist to be uploaded if requested by gRCS
     *
     * This function is non-blocking.
     */
    void set_upload_checklist(ChecklistBase::Checklist checklist) const;

    /**
     * @brief Upload a list of check items to the system.
     *
     * Every time the robotic vehicle checklist changes,
     * it must be updated through the method 'set_upload_checklist'
     *
     * This function is non-blocking.
     */
    void upload_checklist_async(
        const ChecklistBase::ResultAckCallback callback, ChecklistBase::Checklist checklist = {});

    /**
     * @brief Cancel an ongoing checklist upload.
     *
     * This function is blocking.
     *
     * @return Result of request.
     */
    ChecklistBase::Result cancel_checklist_upload() const;

    /**
     * @brief Copy constructor.
     */
    ChecklistRoboticVehicle(const ChecklistRoboticVehicle& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const ChecklistRoboticVehicle& operator=(const ChecklistRoboticVehicle&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<ChecklistRoboticVehicleImpl> _impl;
};

} // namespace mavsdk
