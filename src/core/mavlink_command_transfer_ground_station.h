#pragma once

#include "mavlink_command_transfer.h"
#include "command_base.h"

namespace mavsdk {

class MAVLinkCommandTransferGroundStation : public MAVLinkCommandTransfer {
public:
    class SendWorkItem : public WorkItem {
    public:
        SendWorkItem(
            Sender& sender,
            MAVLinkMessageHandler& message_handler,
            TimeoutHandler& timeout_handler,
            const CommandBase::CommandLong& command,
            CommandBase::ResultAndAckCallback callback);
        virtual ~SendWorkItem();

        void start() override;
        void cancel() override;

        SendWorkItem(const SendWorkItem&) = delete;
        SendWorkItem(SendWorkItem&&) = delete;
        SendWorkItem& operator=(const SendWorkItem&) = delete;
        SendWorkItem& operator=(SendWorkItem&&) = delete;

    private:
        void send_command();

        void process_command_ack(const mavlink_message_t& message);
        void process_timeout();

        void callback(CommandBase::Result result);
        void callback_and_reset(CommandBase::Result result);

        CommandBase::CommandLong _command{};
        CommandBase::CommandAck _ack{};
        CommandBase::ResultAndAckCallback _callback{nullptr};
        void* _cookie{nullptr};
        uint8_t _retries_done{0};
    };

    MAVLinkCommandTransferGroundStation(
        Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler);
    ~MAVLinkCommandTransferGroundStation();

    std::weak_ptr<WorkItem> send_command_async(const CommandBase::CommandLong& command,
                                               CommandBase::ResultAndAckCallback callback);

    // Non-copyable
    MAVLinkCommandTransferGroundStation(const MAVLinkCommandTransferGroundStation&) = delete;
    const MAVLinkCommandTransferGroundStation& operator=(const MAVLinkCommandTransferGroundStation&) = delete;
};

} // namespace mavsdk
