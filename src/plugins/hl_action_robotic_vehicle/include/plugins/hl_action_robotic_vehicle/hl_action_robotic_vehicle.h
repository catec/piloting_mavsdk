#pragma once

#include <memory>

#include "plugin_base.h"
#include "hl_action_base.h"

namespace mavsdk {

class System;
class HLActionRoboticVehicleImpl;

/**
 * @brief Enable hl_actions.
 */
class HLActionRoboticVehicle : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto hl_action = HLActionRoboticVehicle(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit HLActionRoboticVehicle(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto hl_action = HLActionRoboticVehicle(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit HLActionRoboticVehicle(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~HLActionRoboticVehicle();

    /**
     * @brief Sets the hl_action list to be uploaded if requested by gRCS
     *
     * This function is non-blocking.
     */
    void set_upload_hl_action(HLActionBase::HLActionList hl_action_list) const;

    /**
     * @brief Upload a list of hl_action items to the system.
     *
     * Every time the robotic vehicle hl_action list changes,
     * it must be updated through the method 'set_upload_hl_action'
     *
     * This function is non-blocking.
     */
    void upload_hl_action_async(
        const HLActionBase::ResultAckCallback callback,
        HLActionBase::HLActionList hl_action_list = {});

    /**
     * @brief Cancel an ongoing hl_action upload.
     *
     * This function is blocking.
     *
     * @return Result of request.
     */
    HLActionBase::Result cancel_hl_action_upload() const;
    /**
     * @brief Copy constructor.
     */
    HLActionRoboticVehicle(const HLActionRoboticVehicle& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const HLActionRoboticVehicle& operator=(const HLActionRoboticVehicle&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<HLActionRoboticVehicleImpl> _impl;
};

} // namespace mavsdk
