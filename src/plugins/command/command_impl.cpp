#include "command_impl.h"
#include "system.h"
#include "global_include.h"
#include "mavlink_command_transfer_ground_station.h"

namespace mavsdk {

CommandImpl::CommandImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

CommandImpl::CommandImpl(std::shared_ptr<System> system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

CommandImpl::~CommandImpl()
{
    _parent->unregister_plugin(this);
}

void CommandImpl::init(){}

void CommandImpl::deinit()
{
    _parent->unregister_timeout_handler(_timeout_cookie);
}

void CommandImpl::enable() {}

void CommandImpl::disable() {}

std::pair<CommandBase::Result, CommandBase::CommandAck>
CommandImpl::send_command(const CommandBase::CommandLong& command)
{
    auto prom = std::promise<std::pair<CommandBase::Result, CommandBase::CommandAck>>();
    auto res = prom.get_future();

    send_command_async(
        command, [&prom](CommandBase::Result result, CommandBase::CommandAck command_ack) {
            prom.set_value(std::make_pair<>(result, command_ack));
        });
    return res.get();
}

void CommandImpl::send_command_async(
    const CommandBase::CommandLong& command,
    const CommandBase::ResultAndAckCallback& callback)
{
    std::lock_guard<std::recursive_mutex> lock(_command_data.mutex);
    if (_command_data.last_send.lock()) {
        _parent->call_user_callback([callback]() {
            if (callback) {
                CommandBase::CommandAck ack{};
                callback(CommandBase::Result::Busy, ack);
            }
        });
        return;
    }

    auto command_transfer = std::dynamic_pointer_cast<MAVLinkCommandTransferGroundStation>(
        _parent->command_transfer());
    if (!command_transfer) {
        LogWarn() << "Command Transfer is NULL";
        return;
    }

    _command_data.last_send = command_transfer->send_command_async(
        command,
        [this, callback](
            CommandBase::Result result, CommandBase::CommandAck ack) {
            _parent->call_user_callback([callback, result, ack]() {
                if (callback) {
                    callback(result, ack);
                }
            });
        });
}

CommandBase::Result CommandImpl::cancel_send_command()
{
    std::lock_guard<std::recursive_mutex> lock(_command_data.mutex);
    auto ptr = _command_data.last_send.lock();
    if (ptr) {
        ptr->cancel();
    } else {
        LogWarn() << "No send command to cancel... ignoring";
    }

    return CommandBase::Result::Success;
}

} // namespace mavsdk
