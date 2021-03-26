#include "command_robotic_vehicle_impl.h"
#include "system.h"
#include "global_include.h"

namespace mavsdk {

CommandRoboticVehicleImpl::CommandRoboticVehicleImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

CommandRoboticVehicleImpl::CommandRoboticVehicleImpl(std::shared_ptr<System> system) :
    PluginImplBase(system)
{
    _parent->register_plugin(this);
}

CommandRoboticVehicleImpl::~CommandRoboticVehicleImpl()
{
    _parent->unregister_plugin(this);
}

void CommandRoboticVehicleImpl::init()
{
    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_COMMAND_LONG,
        [this](const mavlink_message_t& message) {
            LogDebug() << "Command message received!";
            process_command(message);
        },
        this);
}

void CommandRoboticVehicleImpl::deinit()
{
    _parent->unregister_all_mavlink_message_handlers(this);
}

void CommandRoboticVehicleImpl::enable() {}

void CommandRoboticVehicleImpl::disable() {}

void CommandRoboticVehicleImpl::subscribe_command(CommandBase::CommandCallback callback)
{
    std::lock_guard<std::recursive_mutex> lock(_command_data.mutex);
    _command_data.command_callback = callback;
}

void CommandRoboticVehicleImpl::send_ack(const CommandBase::CommandAck& ack)
{
    mavlink_message_t message;
    mavlink_msg_command_ack_pack(
        _parent->get_own_system_id(),
        _parent->get_own_component_id(),
        &message,
        ack.command,
        ack.result,
        ack.progress,
        ack.result_param2,
        _parent->get_target_system_id(),
        _parent->get_target_component_id());

    if (!_parent->send_message(message))
        LogErr() << "Connection send error with ack.command (" << ack.command << ")";
}

void CommandRoboticVehicleImpl::process_command(const mavlink_message_t& message)
{
    mavlink_command_long_t command_long;
    mavlink_msg_command_long_decode(&message, &command_long);

    CommandBase::CommandCallback temp_callback;

    {
        std::lock_guard<std::recursive_mutex> lock(_command_data.mutex);
        temp_callback = _command_data.command_callback;
    }

    if (temp_callback) {
        CommandBase::CommandLong cmd(command_long);
        _parent->call_user_callback([temp_callback, cmd]() { temp_callback(cmd); });
    }
}

} // namespace mavsdk
