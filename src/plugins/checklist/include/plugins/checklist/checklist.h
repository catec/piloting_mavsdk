#pragma once

#include <memory>

#include "plugin_base.h"
#include "checklist_base.h"

namespace mavsdk {

class System;
class ChecklistImpl;

/**
 * @brief Enable checklists.
 */
class Checklist : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto checklist = Checklist(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Checklist(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto checklist = Checklist(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Checklist(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~Checklist();

    /**
     * @brief Download a list of check items from the system (asynchronous).
     *
     * This function is non-blocking. See 'download_checklist' for the blocking counterpart.
     */
    void download_checklist_async(const ChecklistBase::DownloadChecklistCallback callback);

    /**
     * @brief Download a list of check items from the system (asynchronous).
     *
     * This function is blocking. See 'download_checklist_async' for the non-blocking counterpart.
     *
     * @return Result of request.
     */
    std::pair<ChecklistBase::Result, ChecklistBase::Checklist> download_checklist() const;

    /**
     * @brief Cancel an ongoing checklist download.
     *
     * This function is blocking.
     *
     * @return Result of request.
     */
    ChecklistBase::Result cancel_checklist_download() const;

    /**
     * @brief Copy constructor.
     */
    Checklist(const Checklist& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const Checklist& operator=(const Checklist&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<ChecklistImpl> _impl;
};

} // namespace mavsdk
