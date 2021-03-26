#pragma once

#include <memory>

#include "plugin_base.h"
#include "command_base.h"

namespace mavsdk {

class System;
class CommandImpl;

/**
 * @brief Enable commands.
 */
class Command : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto command = Command(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Command(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto command = Command(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Command(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~Command();

    /**
     * @brief Send a command from the system (asynchronous).
     *
     * This function is blocking. See 'send_command_async' for the non-blocking counterpart.
     *
     * @return Result of request.
     */
    std::pair<CommandBase::Result, CommandBase::CommandAck>
    send_command(const CommandBase::CommandLong& command);

    /**
     * @brief Send a command from the system (asynchronous).
     *
     * This function is non-blocking. See 'send_command' for the blocking counterpart.
     */
    void send_command_async(
        const CommandBase::CommandLong& command,
        const CommandBase::ResultAndAckCallback callback);

    /**
     * @brief Cancel an ongoing command send.
     *
     * This function is blocking.
     *
     * @return Result of request.
     */
    CommandBase::Result cancel_send_command() const;

    /**
     * @brief Copy constructor.
     */
    Command(const Command& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const Command& operator=(const Command&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<CommandImpl> _impl;
};

} // namespace mavsdk
