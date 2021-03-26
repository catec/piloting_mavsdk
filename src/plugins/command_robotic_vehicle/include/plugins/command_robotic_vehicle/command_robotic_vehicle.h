#pragma once

#include <memory>

#include "plugin_base.h"
#include "command_base.h"

namespace mavsdk {

class System;
class CommandRoboticVehicleImpl;

/**
 * @brief Enable commands.
 */
class CommandRoboticVehicle : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto command = CommandRoboticVehicle(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit CommandRoboticVehicle(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto command = CommandRoboticVehicle(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit CommandRoboticVehicle(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~CommandRoboticVehicle();

    /**
     * @brief Subscribe to commands.
     *
     * The callback must be answered immediately using the method 'send_ack'
     */
    void subscribe_command(CommandBase::CommandCallback callback);

    /**
     * @brief Send ack for a command.
     */
    void send_ack(const CommandBase::CommandAck& ack);

    /**
     * @brief Copy constructor.
     */
    CommandRoboticVehicle(const CommandRoboticVehicle& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const CommandRoboticVehicle& operator=(const CommandRoboticVehicle&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<CommandRoboticVehicleImpl> _impl;
};

} // namespace mavsdk
