#include <iomanip>

#include "inspection_base.h"

namespace mavsdk {

bool operator==(
    const InspectionBase::InspectionItem& lhs, const InspectionBase::InspectionItem& rhs)
{
    return (rhs.command == lhs.command) &&
           (rhs.autocontinue == lhs.autocontinue) &&
           ((std::isnan(rhs.param1) && std::isnan(lhs.param1)) ||
            rhs.param1 == lhs.param1) &&
           ((std::isnan(rhs.param2) && std::isnan(lhs.param2)) ||
            rhs.param2 == lhs.param2) &&
           ((std::isnan(rhs.param3) && std::isnan(lhs.param3)) ||
            rhs.param3 == lhs.param3) &&
           ((std::isnan(rhs.param4) && std::isnan(lhs.param4)) ||
            rhs.param4 == lhs.param4) &&
           ((std::isnan(rhs.x) && std::isnan(lhs.x)) ||
            rhs.x == lhs.x) &&
           ((std::isnan(rhs.y) && std::isnan(lhs.y)) ||
            rhs.y == lhs.y) &&
           ((std::isnan(rhs.z) && std::isnan(lhs.z)) ||
            rhs.z == lhs.z);
}

std::ostream& operator<<(std::ostream& str, InspectionBase::InspectionItem const& inspection_item)
{
    str << std::setprecision(15);
    str << "inspection_item:\n";
    str << "{\n";
    str << "    command: " << inspection_item.command << '\n';
    str << "    autocontinue: " << unsigned(inspection_item.autocontinue) << '\n';
    str << "    param1: " << inspection_item.param1 << '\n';
    str << "    param2: " << inspection_item.param2 << '\n';
    str << "    param3: " << inspection_item.param3 << '\n';
    str << "    param4: " << inspection_item.param4 << '\n';
    str << "    x: " << inspection_item.x << '\n';
    str << "    y: " << inspection_item.y << '\n';
    str << "    z: " << inspection_item.z << '\n';
    str << '}';
    return str;
}

bool operator==(
    const InspectionBase::InspectionPlan& lhs, const InspectionBase::InspectionPlan& rhs)
{
    return (rhs.mission_id == lhs.mission_id) &&
           (rhs.inspection_items == lhs.inspection_items);
}

std::ostream& operator<<(std::ostream& str, InspectionBase::InspectionPlan const& inspection_plan)
{
    str << std::setprecision(15);
    str << "inspection_plan:\n";
    str << "{\n";
    str << "    mission_id: " << inspection_plan.mission_id << '\n';
    str << "    inspection_items: [";
    if (inspection_plan.inspection_items.size() == 0) {
        str << "]\n";
    } else {
        for (auto it = inspection_plan.inspection_items.begin();
            it != inspection_plan.inspection_items.end();
            ++it) {
            str << '\n' << *it;
            str << (it + 1 != inspection_plan.inspection_items.end() ? "," : "]\n");
        }
    }
    str << '}';
    return str;
}

bool operator==(
    const InspectionBase::InspectionProgress& lhs, const InspectionBase::InspectionProgress& rhs)
{
    return (rhs.current == lhs.current) && (rhs.reached == lhs.reached);
}

std::ostream&
operator<<(std::ostream& str, InspectionBase::InspectionProgress const& inspection_progress)
{
    str << std::setprecision(15);
    str << "inspection_progress:" << '\n' << "{\n";
    str << "    current: " << inspection_progress.current << '\n';
    str << "    reached: " << inspection_progress.reached << '\n';
    str << '}';
    return str;
}

std::ostream& operator<<(std::ostream& str, InspectionBase::Ack const& ack)
{
    switch (ack) {
        case InspectionBase::Ack::Accepted:
            return str << "Accepted";
        case InspectionBase::Ack::Error:
            return str << "Error";
        case InspectionBase::Ack::Unsupported:
            return str << "Unsupported";
        case InspectionBase::Ack::NoSpace:
            return str << "NoSpace";
        case InspectionBase::Ack::Invalid:
            return str << "Invalid";
        case InspectionBase::Ack::InvalidParam1:
            return str << "InvalidParam1";
        case InspectionBase::Ack::InvalidParam2:
            return str << "InvalidParam2";
        case InspectionBase::Ack::InvalidParam3:
            return str << "InvalidParam3";
        case InspectionBase::Ack::InvalidParam4:
            return str << "InvalidParam4";
        case InspectionBase::Ack::InvalidParam5:
            return str << "InvalidParam5";
        case InspectionBase::Ack::InvalidParam6:
            return str << "InvalidParam6";
        case InspectionBase::Ack::InvalidParam7:
            return str << "InvalidParam7";
        case InspectionBase::Ack::InvalidSequence:
            return str << "InvalidSequence";
        case InspectionBase::Ack::Cancelled:
            return str << "Cancelled";
        case InspectionBase::Ack::Unknown:
        default:
            return str << "Unknown";
    }
}

std::ostream& operator<<(std::ostream& str, InspectionBase::Result const& result)
{
    switch (result) {
        case InspectionBase::Result::Success:
            return str << "Success";
        case InspectionBase::Result::ConnectionError:
            return str << "Connection Error";
        case InspectionBase::Result::ProtocolError:
            return str << "Protocol Error";
        case InspectionBase::Result::Busy:
            return str << "Busy";
        case InspectionBase::Result::Timeout:
            return str << "Timeout";
        case InspectionBase::Result::InvalidArgument:
            return str << "Invalid Argument";
        case InspectionBase::Result::TransferCancelled:
            return str << "Transfer Cancelled";
        case InspectionBase::Result::Unknown:
        default:
            return str << "Unknown";
    }
}

} // namespace mavsdk
