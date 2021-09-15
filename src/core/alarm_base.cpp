#include <iomanip>

#include "alarm_base.h"

namespace mavsdk {

bool operator==(const AlarmBase::AlarmItem& lhs, const AlarmBase::AlarmItem& rhs)
{
    return (
        ((rhs.index == lhs.index)) &&
        (rhs.name == lhs.name) && (rhs.description == lhs.description));
}

std::ostream& operator<<(std::ostream& str, AlarmBase::AlarmItem const& value)
{
    str << std::setprecision(15);
    str << "alarm_item:\n";
    str << "{\n";
    str << "    index: " << value.index << '\n';
    str << "    name: " << value.name << '\n';
    str << "    description: " << value.description << '\n';
    str << '}';
    return str;
}

bool operator==(const AlarmBase::AlarmList& lhs, const AlarmBase::AlarmList& rhs)
{
    return (rhs.items == lhs.items);
}

std::ostream& operator<<(std::ostream& str, AlarmBase::AlarmList const& value)
{
    str << std::setprecision(15);
    str << "alarm_list:\n";
    str << "{\n";
    str << "    alarm_items: [";
    if (value.items.size() == 0) {
        str << "]\n";
    } else {
        for (auto it = value.items.begin(); it != value.items.end(); ++it) {
            str << '\n' << *it;
            str << (it + 1 != value.items.end() ? "," : "]\n");
        }
    }
    str << '}';
    return str;
}

std::ostream& operator<<(std::ostream& str, AlarmBase::Ack const& value)
{
    switch (value) {
        case AlarmBase::Ack::Accepted:
            return str << "Accepted";
        case AlarmBase::Ack::Error:
            return str << "Error";
        case AlarmBase::Ack::NoSpace:
            return str << "NoSpace";
        case AlarmBase::Ack::InvalidSequence:
            return str << "InvalidSequence";
        case AlarmBase::Ack::Cancelled:
            return str << "Cancelled";
        case AlarmBase::Ack::Unknown:
        default:
            return str << "Unknown";
    }
}

std::ostream& operator<<(std::ostream& str, AlarmBase::Result const& value)
{
    switch (value) {
        case AlarmBase::Result::Success:
            return str << "Success";
        case AlarmBase::Result::ConnectionError:
            return str << "Connection Error";
        case AlarmBase::Result::ProtocolError:
            return str << "Protocol Error";
        case AlarmBase::Result::Busy:
            return str << "Busy";
        case AlarmBase::Result::Timeout:
            return str << "Timeout";
        case AlarmBase::Result::InvalidArgument:
            return str << "Invalid Argument";
        case AlarmBase::Result::TransferCancelled:
            return str << "Transfer Cancelled";
        case AlarmBase::Result::Unknown:
        default:
            return str << "Unknown";
    }
}

std::ostream& operator<<(std::ostream& str, AlarmBase::AlarmStatusType const& value)
{
    switch (value) {
        case AlarmBase::AlarmStatusType::Ok:
            return str << "Ok";
        case AlarmBase::AlarmStatusType::Error:
            return str << "Error";
        case AlarmBase::AlarmStatusType::Warning:
            return str << "Warning";
        default:
            return str << "Unknown";
    }
}

bool operator==(const AlarmBase::AlarmStatus& lhs, const AlarmBase::AlarmStatus& rhs)
{
    return (
        (lhs.stamp_ms == rhs.stamp_ms) && (lhs.index == rhs.index) && (lhs.status == rhs.status) &&
        (lhs.errors_count == rhs.warns_count) && (lhs.errors_count == rhs.errors_count));
}

std::ostream& operator<<(std::ostream& str, AlarmBase::AlarmStatus const& value)
{
    str << std::setprecision(15);
    str << "alarm_status:" << '\n' << "{\n";
    str << "    stamp: " << value.stamp_ms << '\n';
    str << "    index: " << value.index << '\n';
    str << "    status: " << value.status << '\n';
    str << "    errors_count: " << value.errors_count << '\n';
    str << "    warns_count: " << value.warns_count << '\n';
    return str;
}

} // namespace mavsdk
