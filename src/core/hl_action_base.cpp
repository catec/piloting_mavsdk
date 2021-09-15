#include <iomanip>

#include "hl_action_base.h"

namespace mavsdk {

bool operator==(const HLActionBase::HLActionItem& lhs, const HLActionBase::HLActionItem& rhs)
{
    return (
        ((rhs.index == lhs.index)) &&
        (rhs.name == lhs.name) && (rhs.description == lhs.description));
}

std::ostream& operator<<(std::ostream& str, HLActionBase::HLActionItem const& hl_action_item)
{
    str << std::setprecision(15);
    str << "hl_action_item:\n";
    str << "{\n";
    str << "    index: " << hl_action_item.index << '\n';
    str << "    command: " << hl_action_item.command << '\n';
    str << "    name: " << hl_action_item.name << '\n';
    str << "    description: " << hl_action_item.description << '\n';
    str << '}';
    return str;
}

bool operator==(const HLActionBase::HLActionList& lhs, const HLActionBase::HLActionList& rhs)
{
    return (rhs.items == lhs.items);
}

std::ostream& operator<<(std::ostream& str, HLActionBase::HLActionList const& hl_action_list)
{
    str << std::setprecision(15);
    str << "hl_action_list:\n";
    str << "{\n";
    str << "    hl_action_items: [";
    if (hl_action_list.items.size() == 0) {
        str << "]\n";
    } else {
        for (auto it = hl_action_list.items.begin(); it != hl_action_list.items.end(); ++it) {
            str << '\n' << *it;
            str << (it + 1 != hl_action_list.items.end() ? "," : "]\n");
        }
    }
    str << '}';
    return str;
}

std::ostream& operator<<(std::ostream& str, HLActionBase::Ack const& ack)
{
    switch (ack) {
        case HLActionBase::Ack::Accepted:
            return str << "Accepted";
        case HLActionBase::Ack::Error:
            return str << "Error";
        case HLActionBase::Ack::NoSpace:
            return str << "NoSpace";
        case HLActionBase::Ack::InvalidSequence:
            return str << "InvalidSequence";
        case HLActionBase::Ack::Cancelled:
            return str << "Cancelled";
        case HLActionBase::Ack::Unknown:
        default:
            return str << "Unknown";
    }
}

std::ostream& operator<<(std::ostream& str, HLActionBase::Result const& result)
{
    switch (result) {
        case HLActionBase::Result::Success:
            return str << "Success";
        case HLActionBase::Result::ConnectionError:
            return str << "Connection Error";
        case HLActionBase::Result::ProtocolError:
            return str << "Protocol Error";
        case HLActionBase::Result::Busy:
            return str << "Busy";
        case HLActionBase::Result::Timeout:
            return str << "Timeout";
        case HLActionBase::Result::InvalidArgument:
            return str << "Invalid Argument";
        case HLActionBase::Result::TransferCancelled:
            return str << "Transfer Cancelled";
        case HLActionBase::Result::Unknown:
        default:
            return str << "Unknown";
    }
}

} // namespace mavsdk
