#include <iomanip>

#include "checklist_base.h"

namespace mavsdk {

bool operator==(const ChecklistBase::ChecklistItem& lhs, const ChecklistBase::ChecklistItem& rhs)
{
    return (
        ((rhs.index == lhs.index)) &&
        (rhs.name == lhs.name) && (rhs.description == lhs.description));
}

std::ostream& operator<<(std::ostream& str, ChecklistBase::ChecklistItem const& checklist_item)
{
    str << std::setprecision(15);
    str << "checklist_item:\n";
    str << "{\n";
    str << "    index: " << checklist_item.index << '\n';
    str << "    name: " << checklist_item.name << '\n';
    str << "    description: " << checklist_item.description << '\n';
    str << '}';
    return str;
}

bool operator==(const ChecklistBase::Checklist& lhs, const ChecklistBase::Checklist& rhs)
{
    return (rhs.items == lhs.items);
}

std::ostream& operator<<(std::ostream& str, ChecklistBase::Checklist const& checklist)
{
    str << std::setprecision(15);
    str << "checklist:\n";
    str << "{\n";
    str << "    items: [";
    if (checklist.items.size() == 0) {
        str << "]\n";
    } else {
        for (auto it = checklist.items.begin(); it != checklist.items.end(); ++it) {
            str << '\n' << *it;
            str << (it + 1 != checklist.items.end() ? "," : "]\n");
        }
    }
    str << '}';
    return str;
}

std::ostream& operator<<(std::ostream& str, ChecklistBase::Ack const& ack)
{
    switch (ack) {
        case ChecklistBase::Ack::Accepted:
            return str << "Accepted";
        case ChecklistBase::Ack::Error:
            return str << "Error";
        case ChecklistBase::Ack::NoSpace:
            return str << "NoSpace";
        case ChecklistBase::Ack::InvalidSequence:
            return str << "InvalidSequence";
        case ChecklistBase::Ack::Cancelled:
            return str << "Cancelled";
        case ChecklistBase::Ack::Unknown:
        default:
            return str << "Unknown";
    }
}

std::ostream& operator<<(std::ostream& str, ChecklistBase::Result const& result)
{
    switch (result) {
        case ChecklistBase::Result::Success:
            return str << "Success";
        case ChecklistBase::Result::ConnectionError:
            return str << "Connection Error";
        case ChecklistBase::Result::ProtocolError:
            return str << "Protocol Error";
        case ChecklistBase::Result::Busy:
            return str << "Busy";
        case ChecklistBase::Result::Timeout:
            return str << "Timeout";
        case ChecklistBase::Result::InvalidArgument:
            return str << "Invalid Argument";
        case ChecklistBase::Result::TransferCancelled:
            return str << "Transfer Cancelled";
        case ChecklistBase::Result::Unknown:
        default:
            return str << "Unknown";
    }
}

} // namespace mavsdk
