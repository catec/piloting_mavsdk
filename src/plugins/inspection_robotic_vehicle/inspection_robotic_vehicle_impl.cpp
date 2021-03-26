#include "inspection_robotic_vehicle_impl.h"
#include "system.h"
#include "global_include.h"
#include "mavlink_inspection_transfer_robotic_vehicle.h"

namespace mavsdk {

using namespace std::placeholders; // for `_1`

InspectionRoboticVehicleImpl::InspectionRoboticVehicleImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

InspectionRoboticVehicleImpl::InspectionRoboticVehicleImpl(std::shared_ptr<System> system) :
    PluginImplBase(system)
{
    _parent->register_plugin(this);
}

InspectionRoboticVehicleImpl::~InspectionRoboticVehicleImpl()
{
    _parent->unregister_plugin(this);
}

void InspectionRoboticVehicleImpl::init()
{
    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT,
        [this](const mavlink_message_t& message) {
            LogDebug() << "Inspection count message received!";
            mavlink_inspection_tasks_count_t count;
            mavlink_msg_inspection_tasks_count_decode(&message, &count);
            download_inspection(count.mission_id, count.count);
        },
        this);

    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_INSPECTION_TASKS_REQUEST,
        [this](const mavlink_message_t&) {
            LogDebug() << "Inspection request list message received!";
            upload_inspection();
        },
        this);

    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_INSPECTION_TASKS_SET_CURRENT_ITEM,
        [this](const mavlink_message_t& message) {
            LogDebug() << "Inspection set current message received!";
            process_inspection_set_current(message);
        },
        this);
}

void InspectionRoboticVehicleImpl::deinit()
{
    _parent->unregister_all_mavlink_message_handlers(this);
}

void InspectionRoboticVehicleImpl::enable() {}

void InspectionRoboticVehicleImpl::disable() {}

void InspectionRoboticVehicleImpl::set_upload_inspection(
    const InspectionBase::InspectionPlan& inspection_plan)
{
    std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
    _inspection_data.plan = inspection_plan;
}

void InspectionRoboticVehicleImpl::upload_inspection_async(
    const InspectionBase::InspectionPlan& inspection_plan,
    const InspectionBase::ResultAckCallback& callback)
{
    std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
    _inspection_data.plan = inspection_plan;
    _inspection_data.upload_callback = callback;
}

void InspectionRoboticVehicleImpl::upload_inspection()
{
    std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);

    if (!_inspection_data.upload_callback) {
        LogErr() << "Not upload callback set";
        return;
    }

    if (_inspection_data.last_upload.lock()) {
        LogWarn() << "Upload inspection is busy, skipping...";
        auto temp_callback = _inspection_data.upload_callback;
        _parent->call_user_callback([temp_callback]() {
            temp_callback(InspectionBase::Result::Busy, InspectionBase::Ack::Unknown);
        });
        return;
    }

    MAVLinkInspectionTransfer::TasksPlan plan;
    plan.mission_id = _inspection_data.plan.mission_id;
    plan.items = convert_to_int_items(_inspection_data.plan.inspection_items);

    LogDebug() << "Convert to int items done";

    auto inspection_transfer = std::dynamic_pointer_cast<MAVLinkInspectionTransferRoboticVehicle>(
        _parent->inspection_transfer());
    if (!inspection_transfer) {
        LogWarn() << "Inspection Transfer is NULL";
        return;
    }

    _inspection_data.last_upload = inspection_transfer->upload_items_async(
        plan, [this](MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::Ack ack) {
            auto result_and_ack = convert_to_result_and_ack(result, ack);
            std::lock_guard<std::recursive_mutex> lock_cb(_inspection_data.mutex);
            if (_inspection_data.upload_callback) {
                auto temp_callback = _inspection_data.upload_callback;
                _parent->call_user_callback([temp_callback, result_and_ack]() {
                    temp_callback(result_and_ack.first, result_and_ack.second);
                });
            }
        });
}

InspectionBase::Result InspectionRoboticVehicleImpl::cancel_inspection_upload()
{
    auto ptr = _inspection_data.last_upload.lock();
    if (ptr) {
        ptr->cancel();
    } else {
        LogWarn() << "No inspection upload to cancel... ignoring";
    }

    return InspectionBase::Result::Success;
}

void InspectionRoboticVehicleImpl::download_inspection_async(
    InspectionBase::DownloadInspectionCallback callback)
{
    std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
    _inspection_data.download_callback = callback;
}

void InspectionRoboticVehicleImpl::download_inspection(
    const uint16_t mission_id, const uint16_t count)
{
    std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);

    if (!_inspection_data.download_callback) {
        LogErr() << "Not download callback set";
        return;
    }

    if (_inspection_data.last_download.lock()) {
        auto temp_callback = _inspection_data.download_callback;
        _parent->call_user_callback([temp_callback]() {
            InspectionBase::InspectionPlan inspection_plan{};
            temp_callback(InspectionBase::Result::Busy, inspection_plan);
        });
        return;
    }

    auto inspection_transfer = std::dynamic_pointer_cast<MAVLinkInspectionTransferRoboticVehicle>(
        _parent->inspection_transfer());
    if (!inspection_transfer) {
        LogWarn() << "Inspection Transfer is NULL";
        return;
    }

    _inspection_data.last_download = inspection_transfer->download_items_async(
        mission_id,
        count,
        [this](
            MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::TasksPlan plan) {
            auto result_and_plan = convert_to_result_and_inspection_plan(result, plan);
            std::lock_guard<std::recursive_mutex> lock_cb(_inspection_data.mutex);
            if (_inspection_data.download_callback) {
                auto temp_callback = _inspection_data.download_callback;
                _parent->call_user_callback([temp_callback, result_and_plan]() {
                    temp_callback(result_and_plan.first, result_and_plan.second);
                });
            }
        });
}

InspectionBase::Result InspectionRoboticVehicleImpl::cancel_inspection_download()
{
    auto ptr = _inspection_data.last_download.lock();
    if (ptr) {
        ptr->cancel();
    } else {
        LogWarn() << "No inspection download to cancel... ignoring";
    }

    return InspectionBase::Result::Success;
}

void InspectionRoboticVehicleImpl::inspection_set_current_async(
    InspectionBase::InspectionSetCurrentCallback callback)
{
    std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
    _inspection_data.set_current_callback = callback;
}

void InspectionRoboticVehicleImpl::update_current_inspection_item(const uint16_t item_seq)
{
    mavlink_message_t message;
    mavlink_msg_inspection_tasks_current_item_pack(
        _parent->get_own_system_id(),
        _parent->get_own_component_id(),
        &message,
        item_seq);

    _parent->send_message(message);
}

void InspectionRoboticVehicleImpl::update_reached_inspection_item(const uint16_t item_seq)
{
    mavlink_message_t message;
    mavlink_msg_inspection_tasks_item_reached_pack(
        _parent->get_own_system_id(),
        _parent->get_own_component_id(),
        &message,
        item_seq);

    _parent->send_message(message);
}

void InspectionRoboticVehicleImpl::process_inspection_set_current(const mavlink_message_t& message)
{
    mavlink_inspection_tasks_set_current_item_t inspection_set_current;
    mavlink_msg_inspection_tasks_set_current_item_decode(&message, &inspection_set_current);

    InspectionBase::InspectionSetCurrentCallback temp_callback;

    {
        std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
        temp_callback = _inspection_data.set_current_callback;
    }

    if (temp_callback) {
        const auto seq = inspection_set_current.seq;
        _parent->call_user_callback([temp_callback, seq]() { temp_callback(seq); });
    }
}

std::vector<MAVLinkInspectionTransfer::TasksItem>
InspectionRoboticVehicleImpl::convert_to_int_items(
    const std::vector<InspectionBase::InspectionItem>& inspection_items)
{
    std::vector<MAVLinkInspectionTransfer::TasksItem> int_items;

    for (const auto& item : inspection_items) {
        MAVLinkInspectionTransfer::TasksItem next_item{static_cast<uint16_t>(int_items.size()),
                                                       item.command,
                                                       item.autocontinue,
                                                       item.param1,
                                                       item.param2,
                                                       item.param3,
                                                       item.param4,
                                                       item.x,
                                                       item.y,
                                                       item.z};

        int_items.push_back(next_item);
    }

    return int_items;
}

std::pair<InspectionBase::Result, InspectionBase::InspectionPlan>
InspectionRoboticVehicleImpl::convert_to_result_and_inspection_plan(
    MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::TasksPlan& plan)
{
    std::pair<InspectionBase::Result, InspectionBase::InspectionPlan> result_pair;

    result_pair.first = convert_result(result);
    if (result_pair.first != InspectionBase::Result::Success) {
        return result_pair;
    }

    result_pair.second.mission_id = plan.mission_id;

    InspectionBase::InspectionItem new_inspection_item{};
    for (const auto& int_item : plan.items) {
        LogDebug() << "Assembling Message: " << int(int_item.seq);

        new_inspection_item.command = int_item.command;
        new_inspection_item.autocontinue = int_item.autocontinue;
        new_inspection_item.param1 = int_item.param1;
        new_inspection_item.param2 = int_item.param2;
        new_inspection_item.param3 = int_item.param3;
        new_inspection_item.param4 = int_item.param4;
        new_inspection_item.x = int_item.x;
        new_inspection_item.y = int_item.y;
        new_inspection_item.z = int_item.z;

        result_pair.second.inspection_items.push_back(new_inspection_item);
    }

    return result_pair;
}

std::pair<InspectionBase::Result, InspectionBase::Ack>
InspectionRoboticVehicleImpl::convert_to_result_and_ack(
    MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::Ack ack)
{
    std::pair<InspectionBase::Result, InspectionBase::Ack> result_pair;
    result_pair.first = convert_result(result);
    result_pair.second = convert_ack(ack);
    return result_pair;
}

InspectionBase::Ack InspectionRoboticVehicleImpl::convert_ack(MAVLinkInspectionTransfer::Ack ack)
{
    switch (ack) {
        case MAVLinkInspectionTransfer::Ack::Accepted:
            return InspectionBase::Ack::Accepted;
        case MAVLinkInspectionTransfer::Ack::Error:
            return InspectionBase::Ack::Error;
        case MAVLinkInspectionTransfer::Ack::Unsupported:
            return InspectionBase::Ack::Unsupported;
        case MAVLinkInspectionTransfer::Ack::NoSpace:
            return InspectionBase::Ack::NoSpace;
        case MAVLinkInspectionTransfer::Ack::Invalid:
            return InspectionBase::Ack::Invalid;
        case MAVLinkInspectionTransfer::Ack::InvalidParam1:
            return InspectionBase::Ack::InvalidParam1;
        case MAVLinkInspectionTransfer::Ack::InvalidParam2:
            return InspectionBase::Ack::InvalidParam2;
        case MAVLinkInspectionTransfer::Ack::InvalidParam3:
            return InspectionBase::Ack::InvalidParam3;
        case MAVLinkInspectionTransfer::Ack::InvalidParam4:
            return InspectionBase::Ack::InvalidParam4;
        case MAVLinkInspectionTransfer::Ack::InvalidParam5:
            return InspectionBase::Ack::InvalidParam5;
        case MAVLinkInspectionTransfer::Ack::InvalidParam6:
            return InspectionBase::Ack::InvalidParam6;
        case MAVLinkInspectionTransfer::Ack::InvalidParam7:
            return InspectionBase::Ack::InvalidParam7;
        case MAVLinkInspectionTransfer::Ack::InvalidSequence:
            return InspectionBase::Ack::InvalidSequence;
        case MAVLinkInspectionTransfer::Ack::Cancelled:
            return InspectionBase::Ack::Cancelled;
        case MAVLinkInspectionTransfer::Ack::Unknown:
        default:
            return InspectionBase::Ack::Unknown;
    }
}

InspectionBase::Result
InspectionRoboticVehicleImpl::convert_result(MAVLinkInspectionTransfer::Result result)
{
    switch (result) {
        case MAVLinkInspectionTransfer::Result::Success:
            return InspectionBase::Result::Success;
        case MAVLinkInspectionTransfer::Result::ConnectionError:
            return InspectionBase::Result::ConnectionError;
        case MAVLinkInspectionTransfer::Result::Timeout:
            return InspectionBase::Result::Timeout;
        case MAVLinkInspectionTransfer::Result::Cancelled:
            return InspectionBase::Result::TransferCancelled;
        case MAVLinkInspectionTransfer::Result::InvalidSequence:
        case MAVLinkInspectionTransfer::Result::CurrentInvalid:
            return InspectionBase::Result::InvalidArgument;
        case MAVLinkInspectionTransfer::Result::ProtocolError:
            return InspectionBase::Result::ProtocolError;
        default:
            return InspectionBase::Result::Unknown;
    }
}

} // namespace mavsdk
