#pragma once

#include "plugins/command/command.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"
#include "mavlink_command_transfer.h"

namespace mavsdk {

class System;

class CommandImpl : public PluginImplBase {
public:
    explicit CommandImpl(System& system);
    explicit CommandImpl(std::shared_ptr<System> system);
    ~CommandImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    std::pair<CommandBase::Result, CommandBase::CommandAck>
    send_command(const CommandBase::CommandLong& command);
    void send_command_async(
        const CommandBase::CommandLong& command,
        const CommandBase::ResultAndAckCallback& callback);
    CommandBase::Result cancel_send_command();

    // Non-copyable
    CommandImpl(const CommandImpl&) = delete;
    const CommandImpl& operator=(const CommandImpl&) = delete;

private:
    struct {
        mutable std::recursive_mutex mutex{};
        std::weak_ptr<MAVLinkCommandTransfer::WorkItem> last_send{};
    } _command_data{};

    void* _timeout_cookie{nullptr};
};

} // namespace mavsdk
