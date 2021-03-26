#include <iomanip>

#include "command_base.h"

namespace mavsdk {

std::ostream& operator<<(std::ostream& str, CommandBase::Result const& value)
{
    switch (value) {
        case CommandBase::Result::Success:
            str << "Success";
            break;
        case CommandBase::Result::ConnectionError:
            str << "ConnectionError";
            break;
        case CommandBase::Result::Busy:
            str << "Busy";
            break;
        case CommandBase::Result::Timeout:
            str << "Timeout";
            break;
        case CommandBase::Result::Cancelled:
            str << "Cancelled";
            break;
        case CommandBase::Result::InProgress:
            str << "InProgress";
            break;
        default:
            str << "UnknownError";
            break;
    }

    return str;
}

bool operator==(
    const CommandBase::CommandAck& lhs, const CommandBase::CommandAck& rhs)
{
    return (
        (lhs.command == rhs.command) &&
        (lhs.result == rhs.result) && (lhs.progress == rhs.progress) &&
        (lhs.result_param2 == rhs.result_param2));
}

std::ostream& operator<<(std::ostream& str, CommandBase::CommandAck const& value)
{
    str << std::setprecision(15);
    str << "command_ack:" << '\n' << "{\n";
    str << "    command: " << value.command << '\n';
    str << "    result: " << (unsigned)(value.result) << '\n';
    str << "    progress: " << (unsigned)(value.progress) << '\n';
    str << "    result_param2: " << value.result_param2 << '\n';
    str << "}";
    return str;
}

bool operator==(
    const CommandBase::CommandLong& lhs, const CommandBase::CommandLong& rhs)
{
    return (
        (lhs.command == rhs.command) &&
        (lhs.confirmation == rhs.confirmation) && (lhs.params.param1 == rhs.params.param1) &&
        (lhs.params.param2 == rhs.params.param2) && (lhs.params.param3 == rhs.params.param3) &&
        (lhs.params.param4 == rhs.params.param4) && (lhs.params.param5 == rhs.params.param5) &&
        (lhs.params.param6 == rhs.params.param6) && (lhs.params.param7 == rhs.params.param7));
}

std::ostream& operator<<(std::ostream& str, CommandBase::CommandLong const& command_long)
{
    str << std::setprecision(15);
    str << "command_long:" << '\n' << "{\n";
    str << "    command: " << command_long.command << '\n';
    str << "    confirmation: " << (unsigned)(command_long.confirmation) << '\n';
    str << "    params: " << '\n';
    str << "        param1: " << command_long.params.param1 << '\n';
    str << "        param2: " << command_long.params.param2 << '\n';
    str << "        param3: " << command_long.params.param3 << '\n';
    str << "        param4: " << command_long.params.param4 << '\n';
    str << "        param5: " << command_long.params.param5 << '\n';
    str << "        param6: " << command_long.params.param6 << '\n';
    str << "        param7: " << command_long.params.param7 << '\n';
    str << "}";
    return str;
}

} // namespace mavsdk
