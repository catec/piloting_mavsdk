#include "command_robotic_vehicle_impl.h"
#include "plugins/command_robotic_vehicle/command_robotic_vehicle.h"

namespace mavsdk {

CommandRoboticVehicle::CommandRoboticVehicle(System& system) :
    PluginBase(),
    _impl{new CommandRoboticVehicleImpl(system)}
{}

CommandRoboticVehicle::CommandRoboticVehicle(std::shared_ptr<System> system) :
    PluginBase(),
    _impl{new CommandRoboticVehicleImpl(system)}
{}

CommandRoboticVehicle::~CommandRoboticVehicle() {}

void CommandRoboticVehicle::subscribe_command(CommandBase::CommandCallback callback)
{
    _impl->subscribe_command(callback);
}

void CommandRoboticVehicle::send_ack(const CommandBase::CommandAck& ack)
{
    _impl->send_ack(ack);
}

} // namespace mavsdk
