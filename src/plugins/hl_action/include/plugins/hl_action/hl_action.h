#pragma once

#include <memory>

#include "plugin_base.h"
#include "hl_action_base.h"

namespace mavsdk {

class System;
class HLActionImpl;

/**
 * @brief Enable hl_actions.
 */
class HLAction : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto hl_action = HLAction(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit HLAction(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto hl_action = HLAction(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit HLAction(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~HLAction();

    /**
     * @brief Download a list of hl_action items from the system (asynchronous).
     *
     * This function is non-blocking. See 'download_hl_action' for the blocking counterpart.
     */
    void download_hl_action_async(const HLActionBase::DownloadHLActionListCallback callback);

    /**
     * @brief Download a list of hl_action items from the system (asynchronous).
     *
     * This function is blocking. See 'download_hl_action_async' for the non-blocking counterpart.
     *
     * @return Result of request.
     */
    std::pair<HLActionBase::Result, HLActionBase::HLActionList> download_hl_action() const;

    /**
     * @brief Cancel an ongoing hl_action download.
     *
     * This function is blocking.
     *
     * @return Result of request.
     */
    HLActionBase::Result cancel_hl_action_download() const;

    /**
     * @brief Copy constructor.
     */
    HLAction(const HLAction& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const HLAction& operator=(const HLAction&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<HLActionImpl> _impl;
};

} // namespace mavsdk
