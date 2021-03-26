#pragma once

#include "plugins/command_robotic_vehicle/command_robotic_vehicle.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"

namespace mavsdk {

class System;

class CommandRoboticVehicleImpl : public PluginImplBase {
public:
    explicit CommandRoboticVehicleImpl(System& system);
    explicit CommandRoboticVehicleImpl(std::shared_ptr<System> system);
    ~CommandRoboticVehicleImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    void subscribe_command(CommandBase::CommandCallback callback);

    void send_ack(const CommandBase::CommandAck& ack);

    // Non-copyable
    CommandRoboticVehicleImpl(const CommandRoboticVehicleImpl&) = delete;
    const CommandRoboticVehicleImpl& operator=(const CommandRoboticVehicleImpl&) = delete;

private:
   void process_command(const mavlink_message_t& message);

   struct {
       mutable std::recursive_mutex mutex{};
       CommandBase::CommandCallback command_callback{nullptr};
   } _command_data{};

   void* _timeout_cookie{nullptr};
};

} // namespace mavsdk
