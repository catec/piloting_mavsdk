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
        MAVLINK_MSG_ID_WAYPOINT_LIST_COUNT,
        [this](const mavlink_message_t& message) {
            LogDebug() << "Waypoint list count message received!";
            mavlink_waypoint_list_count_t count;
            mavlink_msg_waypoint_list_count_decode(&message, &count);
            download_inspection(count.plan_id, count.sync_id, count.count);
        },
        this);

    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_WAYPOINT_LIST_REQUEST,
        [this](const mavlink_message_t&) {
            LogDebug() << "Waypoint list request message received!";
            upload_inspection();
        },
        this);

    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM,
        [this](const mavlink_message_t& message) {
            LogDebug() << "Waypoint list set current message received!";
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
    const InspectionBase::WaypointList& list)
{
    std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
    _inspection_data.list = list;
}

void InspectionRoboticVehicleImpl::upload_inspection_async(
    const InspectionBase::WaypointList& list,
    const InspectionBase::ResultAckCallback& callback)
{
    std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
    _inspection_data.list = list;
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

    MAVLinkInspectionTransfer::WaypointList list;
    list.plan_id = _inspection_data.list.plan_id;
    list.sync_id = _inspection_data.list.sync_id;
    list.items = convert_to_int_items(_inspection_data.list.items);

    LogDebug() << "Convert to int items done";

    auto inspection_transfer = std::dynamic_pointer_cast<MAVLinkInspectionTransferRoboticVehicle>(
        _parent->inspection_transfer());
    if (!inspection_transfer) {
        LogWarn() << "Inspection Transfer is NULL";
        return;
    }

    _inspection_data.last_upload = inspection_transfer->upload_items_async(
        list, [this](MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::Ack ack) {
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
   const uint32_t plan_id, const uint32_t sync_id, const uint16_t count)
{
    std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);

    if (!_inspection_data.download_callback) {
        LogErr() << "Not download callback set";
        return;
    }

    if (_inspection_data.last_download.lock()) {
        auto temp_callback = _inspection_data.download_callback;
        _parent->call_user_callback([temp_callback]() {
            InspectionBase::WaypointList list{};
            temp_callback(InspectionBase::Result::Busy, list);
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
        count,
        [this, plan_id, sync_id](
            MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::WaypointList list) {
            list.plan_id = plan_id;
            list.sync_id = sync_id;
            auto result_and_list = convert_to_result_and_waypoint_list(result, list);
            std::lock_guard<std::recursive_mutex> lock_cb(_inspection_data.mutex);
            if (_inspection_data.download_callback) {
                auto temp_callback = _inspection_data.download_callback;
                _parent->call_user_callback([temp_callback, result_and_list]() {
                    temp_callback(result_and_list.first, result_and_list.second);
                });
            }
        });
}

InspectionBase::Result InspectionRoboticVehicleImpl::send_download_ack(
    const InspectionBase::Ack& ack)
{
    mavlink_message_t message;
    mavlink_msg_waypoint_list_ack_pack(
       _parent->get_own_system_id(),
       _parent->get_own_component_id(),
       &message,
       _parent->get_target_system_id(),
       _parent->get_target_component_id(),
       static_cast<uint8_t>(ack));

    if (!_parent->send_message(message))
    {
        LogErr() << "Connection send error with ack (" << ack << ")";
        return InspectionBase::Result::ConnectionError;
    }

    LogDebug() << "Waypoint list ask sent: " << ack;

    return InspectionBase::Result::Success;
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
    mavlink_msg_waypoint_list_current_item_pack(
        _parent->get_own_system_id(),
        _parent->get_own_component_id(),
        &message,
        item_seq);

    _parent->send_message(message);
}

void InspectionRoboticVehicleImpl::update_reached_inspection_item(const uint16_t item_seq)
{
    mavlink_message_t message;
    mavlink_msg_waypoint_list_item_reached_pack(
        _parent->get_own_system_id(),
        _parent->get_own_component_id(),
        &message,
        item_seq);

    _parent->send_message(message);
}

void InspectionRoboticVehicleImpl::process_inspection_set_current(const mavlink_message_t& message)
{
    mavlink_waypoint_list_set_current_item_t set_current;
    mavlink_msg_waypoint_list_set_current_item_decode(&message, &set_current);

    InspectionBase::InspectionSetCurrentCallback temp_callback;

    {
        std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
        temp_callback = _inspection_data.set_current_callback;
    }

    if (temp_callback) {
        const auto seq = set_current.seq;
        _parent->call_user_callback([temp_callback, seq]() { temp_callback(seq); });
    }
}

std::vector<MAVLinkInspectionTransfer::WaypointItem>
InspectionRoboticVehicleImpl::convert_to_int_items(
    const std::vector<InspectionBase::WaypointItem>& items)
{
    std::vector<MAVLinkInspectionTransfer::WaypointItem> int_items;

    for (const auto& item : items) {
        MAVLinkInspectionTransfer::WaypointItem next_item{
            static_cast<uint16_t>(int_items.size()),
            item.task_id,
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

std::pair<InspectionBase::Result, InspectionBase::WaypointList>
InspectionRoboticVehicleImpl::convert_to_result_and_waypoint_list(
    MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::WaypointList& list)
{
    std::pair<InspectionBase::Result, InspectionBase::WaypointList> result_pair;

    result_pair.first = convert_result(result);
    if (result_pair.first != InspectionBase::Result::Success) {
        return result_pair;
    }

    result_pair.second.plan_id = list.plan_id;
    result_pair.second.sync_id = list.sync_id;

    InspectionBase::WaypointItem new_item{};
    for (const auto& int_item : list.items) {
        LogDebug() << "Assembling Message: " << int(int_item.seq);

        new_item.task_id      = int_item.task_id;
        new_item.command      = int_item.command;
        new_item.autocontinue = int_item.autocontinue;
        new_item.param1       = int_item.param1;
        new_item.param2       = int_item.param2;
        new_item.param3       = int_item.param3;
        new_item.param4       = int_item.param4;
        new_item.x            = int_item.x;
        new_item.y            = int_item.y;
        new_item.z            = int_item.z;

        result_pair.second.items.push_back(new_item);
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
