#include "mavlink_command_transfer_ground_station.h"
#include "log.h"

namespace mavsdk {

MAVLinkCommandTransferGroundStation::MAVLinkCommandTransferGroundStation(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    MAVLinkCommandTransfer(sender, message_handler, timeout_handler)
{}

MAVLinkCommandTransferGroundStation::~MAVLinkCommandTransferGroundStation() {}

std::weak_ptr<MAVLinkCommandTransfer::WorkItem>
MAVLinkCommandTransferGroundStation::send_command_async(
    const CommandBase::CommandLong &command,
    CommandBase::ResultAndAckCallback callback)
{
    auto ptr = std::make_shared<SendWorkItem>(
        _sender, _message_handler, _timeout_handler, command, callback);

    _work_queue.push_back(ptr);

    return std::weak_ptr<WorkItem>(ptr);
}


MAVLinkCommandTransferGroundStation::SendWorkItem::SendWorkItem(Sender& sender,
    MAVLinkMessageHandler& message_handler,
    TimeoutHandler& timeout_handler,
    const CommandBase::CommandLong& command,
    CommandBase::ResultAndAckCallback callback) :
    WorkItem(sender, message_handler, timeout_handler),
    _command(command),
    _callback(callback)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _message_handler.register_one(
        MAVLINK_MSG_ID_COMMAND_ACK,
        [this](const mavlink_message_t& message) { process_command_ack(message); },
        this);
}

MAVLinkCommandTransferGroundStation::SendWorkItem::~SendWorkItem()
{
    std::lock_guard<std::mutex> lock(_mutex);
    _message_handler.unregister_all(this);
    _timeout_handler.remove(_cookie);
}

void MAVLinkCommandTransferGroundStation::SendWorkItem::start()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _started = true;
    _retries_done = 0;
    _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);

    send_command();
}

void MAVLinkCommandTransferGroundStation::SendWorkItem::cancel()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _timeout_handler.remove(_cookie);
    callback_and_reset(CommandBase::Result::Cancelled);
}

void MAVLinkCommandTransferGroundStation::SendWorkItem::send_command()
{
    mavlink_message_t message;
    mavlink_msg_command_long_pack(
        _sender.own_address.system_id,
        _sender.own_address.component_id,
        &message,
        _sender.target_address.system_id,
        _sender.target_address.component_id,
        _command.command,
        _retries_done,
        _command.params.param1,
        _command.params.param2,
        _command.params.param3,
        _command.params.param4,
        _command.params.param5,
        _command.params.param6,
        _command.params.param7);

    if (!_sender.send_message(message)) {
        _timeout_handler.remove(_cookie);
        callback_and_reset(CommandBase::Result::ConnectionError);
        return;
    }

    LogDebug() << "CommandLong sent: " << _command.command;

    ++_retries_done;
}

void MAVLinkCommandTransferGroundStation::SendWorkItem::process_command_ack(
    const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    mavlink_command_ack_t command_ack;
    mavlink_msg_command_ack_decode(&message, &command_ack);

    if (_command.command != command_ack.command) {
        // If the command does not match with our current command, ignore it.
        LogWarn() << "Command ack " << int(command_ack.command)
                  << " not matching our current command: " << _command.command;
        return;
    }

    _timeout_handler.remove(_cookie);

    _ack = CommandBase::CommandAck{command_ack};

    if (command_ack.result == MAV_RESULT_IN_PROGRESS) {
        _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);
        callback(CommandBase::Result::InProgress);
    } else {
        callback_and_reset(CommandBase::Result::Success);
    }
}

void MAVLinkCommandTransferGroundStation::SendWorkItem::process_timeout()
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_retries_done >= retries) {
        callback_and_reset(CommandBase::Result::Timeout);
        return;
    }

    _timeout_handler.add([this]() { process_timeout(); }, timeout_s, &_cookie);
    send_command();
}

void MAVLinkCommandTransferGroundStation::SendWorkItem::callback(CommandBase::Result result)
{
    if (_callback) {
        _callback(result, _ack);
    }
}

void MAVLinkCommandTransferGroundStation::SendWorkItem::callback_and_reset(CommandBase::Result result)
{
    callback(result);
    _callback = nullptr;
    _done = true;
}

} // namespace mavsdk
