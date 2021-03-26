#include "command_impl.h"
#include "plugins/command/command.h"

namespace mavsdk {

Command::Command(System& system) : PluginBase(), _impl{new CommandImpl(system)} {}

Command::Command(std::shared_ptr<System> system) : PluginBase(), _impl{new CommandImpl(system)} {}

Command::~Command() {}

std::pair<CommandBase::Result, CommandBase::CommandAck>
Command::send_command(const CommandBase::CommandLong& command)
{
    return _impl->send_command(command);
}

void Command::send_command_async(
    const CommandBase::CommandLong& command,
    const CommandBase::ResultAndAckCallback callback)
{
    _impl->send_command_async(command, callback);
}

CommandBase::Result Command::cancel_send_command() const
{
    return _impl->cancel_send_command();
}

} // namespace mavsdk
