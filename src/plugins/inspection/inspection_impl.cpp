#include "inspection_impl.h"
#include "system.h"
#include "global_include.h"
#include "mavlink_inspection_transfer_ground_station.h"

namespace mavsdk {

using namespace std::placeholders; // for `_1`

InspectionImpl::InspectionImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

InspectionImpl::InspectionImpl(std::shared_ptr<System> system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

InspectionImpl::~InspectionImpl()
{
    _parent->unregister_plugin(this);
}

void InspectionImpl::init()
{
    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM,
        std::bind(&InspectionImpl::process_inspection_current, this, _1),
        this);

    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_REACHED,
        std::bind(&InspectionImpl::process_inspection_item_reached, this, _1),
        this);
}

void InspectionImpl::enable() {}

void InspectionImpl::disable()
{
    reset_inspection_progress();
}

void InspectionImpl::deinit()
{
    _parent->unregister_timeout_handler(_timeout_cookie);
    _parent->unregister_all_mavlink_message_handlers(this);
}

void InspectionImpl::reset_inspection_progress()
{
    std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
    _inspection_data.last_current_mavlink_inspection_item = -1;
    _inspection_data.last_reached_mavlink_inspection_item = -1;
    _inspection_data.last_current_reported_inspection_item = -1;
    _inspection_data.last_reached_reported_inspection_item = -1;
}

void InspectionImpl::process_inspection_current(const mavlink_message_t& message)
{
    mavlink_waypoint_list_current_item_t inspection_current;
    mavlink_msg_waypoint_list_current_item_decode(&message, &inspection_current);

    bool should_report = false;
    {
        std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
        _inspection_data.last_current_mavlink_inspection_item = inspection_current.seq;
        should_report = _inspection_data.last_current_reported_inspection_item !=
                        _inspection_data.last_current_mavlink_inspection_item;
    }

    if (should_report) {
        report_progress();
    }
}

void InspectionImpl::process_inspection_item_reached(const mavlink_message_t& message)
{
    mavlink_waypoint_list_item_reached_t inspection_item_reached;
    mavlink_msg_waypoint_list_item_reached_decode(&message, &inspection_item_reached);

    bool should_report = false;
    {
        std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
        _inspection_data.last_reached_mavlink_inspection_item = inspection_item_reached.seq;
        should_report = _inspection_data.last_reached_reported_inspection_item !=
                        _inspection_data.last_reached_mavlink_inspection_item;
    }

    if (should_report) {
        report_progress();
    }
}

std::pair<InspectionBase::Result, InspectionBase::Ack>
InspectionImpl::upload_inspection(const InspectionBase::WaypointList& list)
{
    auto prom = std::promise<std::pair<InspectionBase::Result, InspectionBase::Ack>>();
    auto fut = prom.get_future();

    upload_inspection_async(
        list, [&prom](InspectionBase::Result result, InspectionBase::Ack ack) {
            prom.set_value(std::make_pair<>(result, ack));
        });
    return fut.get();
}

void InspectionImpl::upload_inspection_async(
    const InspectionBase::WaypointList& list,
    const InspectionBase::ResultAckCallback& callback)
{
    if (_inspection_data.last_upload.lock()) {
        _parent->call_user_callback([callback]() {
            if (callback) {
                callback(InspectionBase::Result::Busy, InspectionBase::Ack::Unknown);
            }
        });
        return;
    }

    reset_inspection_progress();

    MAVLinkInspectionTransfer::WaypointList wp_list;
    wp_list.plan_id = list.plan_id;
    wp_list.sync_id = list.sync_id;
    wp_list.items = convert_to_int_items(list.items);

    auto inspection_transfer = std::dynamic_pointer_cast<MAVLinkInspectionTransferGroundStation>(
        _parent->inspection_transfer());
    if (!inspection_transfer) {
        LogWarn() << "Inspection Transfer is NULL";
        return;
    }

    _inspection_data.last_upload = inspection_transfer->upload_items_async(
        wp_list,
        [this, callback](
            MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::Ack ack) {
            auto result_and_ack = convert_to_result_and_ack(result, ack);
            _parent->call_user_callback([callback, result_and_ack]() {
                if (callback) {
                    callback(result_and_ack.first, result_and_ack.second);
                }
            });
        });
}

InspectionBase::Result InspectionImpl::cancel_inspection_upload()
{
    auto ptr = _inspection_data.last_upload.lock();
    if (ptr) {
        ptr->cancel();
    } else {
        LogWarn() << "No inspection upload to cancel... ignoring";
    }

    return InspectionBase::Result::Success;
}

std::pair<InspectionBase::Result, InspectionBase::WaypointList>
InspectionImpl::download_inspection()
{
    auto prom = std::promise<std::pair<InspectionBase::Result, InspectionBase::WaypointList>>();
    auto fut = prom.get_future();

    download_inspection_async(
        [&prom](InspectionBase::Result result, InspectionBase::WaypointList list) {
            prom.set_value(std::make_pair<>(result, list));
        });
    return fut.get();
}

void InspectionImpl::download_inspection_async(
    const InspectionBase::DownloadInspectionCallback& callback)
{
    if (_inspection_data.last_download.lock()) {
        _parent->call_user_callback([callback]() {
            if (callback) {
                InspectionBase::WaypointList list{};
                callback(InspectionBase::Result::Busy, list);
            }
        });
        return;
    }

    auto inspection_transfer = std::dynamic_pointer_cast<MAVLinkInspectionTransferGroundStation>(
        _parent->inspection_transfer());
    if (!inspection_transfer) {
        LogWarn() << "Inspection Transfer is NULL";
        return;
    }

    _inspection_data.last_download = inspection_transfer->download_items_async(
        [this, callback](
            MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::WaypointList list) {
            auto result_and_list = convert_to_result_and_waypoint_list(result, list);
            _parent->call_user_callback([callback, result_and_list]() {
                if (callback) {
                    callback(result_and_list.first, result_and_list.second);
                }
            });
        });
}

InspectionBase::Result InspectionImpl::cancel_inspection_download()
{
    auto ptr = _inspection_data.last_download.lock();
    if (ptr) {
        ptr->cancel();
    } else {
        LogWarn() << "No inspection download to cancel... ignoring";
    }

    return InspectionBase::Result::Success;
}

std::vector<MAVLinkInspectionTransfer::WaypointItem>
InspectionImpl::convert_to_int_items(const std::vector<InspectionBase::WaypointItem>& items)
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
InspectionImpl::convert_to_result_and_waypoint_list(
    MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::WaypointList& list)
{
    std::pair<InspectionBase::Result, InspectionBase::WaypointList> result_pair;

    result_pair.first = convert_result(result);
    if (result_pair.first != InspectionBase::Result::Success) {
        return result_pair;
    }

    //    if (int_items.empty()) {
    //        LogDebug() << "Returning NoInspectionAvailable since count 0 received";

    //        result_pair.first = InspectionBase::Result::NoInspectionAvailable;
    //        return result_pair;
    //    }

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

void InspectionImpl::set_current_inspection_item(uint16_t current)
{
    mavlink_message_t message;
    mavlink_msg_waypoint_list_set_current_item_pack(
        _parent->get_own_system_id(),
        _parent->get_own_component_id(),
        &message,
        _parent->get_target_system_id(),
        _parent->get_target_component_id(),
        current);

    _parent->send_message(message);
}

void InspectionImpl::report_progress()
{
    int current, reached;
    InspectionBase::InspectionProgressCallback temp_callback;

    {
        std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
        temp_callback = _inspection_data.progress_callback;
        current = _inspection_data.last_current_mavlink_inspection_item;
        reached = _inspection_data.last_reached_mavlink_inspection_item;
        _inspection_data.last_current_reported_inspection_item = current;
        _inspection_data.last_reached_reported_inspection_item = reached;
    }

    if (temp_callback) {
        LogDebug() << "current: " << current << ", reached: " << reached;
        InspectionBase::InspectionProgress progress;
        progress.current = current;
        progress.reached = reached;
        _parent->call_user_callback([temp_callback, progress]() { temp_callback(progress); });
    }
}

InspectionBase::InspectionProgress InspectionImpl::inspection_progress()
{
    std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
    InspectionBase::InspectionProgress progress;
    progress.current = _inspection_data.last_current_mavlink_inspection_item;
    progress.reached = _inspection_data.last_reached_mavlink_inspection_item;
    return progress;
}

void InspectionImpl::inspection_progress_async(InspectionBase::InspectionProgressCallback callback)
{
    std::lock_guard<std::recursive_mutex> lock(_inspection_data.mutex);
    _inspection_data.progress_callback = callback;
}

std::pair<InspectionBase::Result, InspectionBase::Ack> InspectionImpl::convert_to_result_and_ack(
    MAVLinkInspectionTransfer::Result result, MAVLinkInspectionTransfer::Ack ack)
{
    std::pair<InspectionBase::Result, InspectionBase::Ack> result_pair;
    result_pair.first = convert_result(result);
    result_pair.second = convert_ack(ack);
    return result_pair;
}

InspectionBase::Ack InspectionImpl::convert_ack(MAVLinkInspectionTransfer::Ack ack)
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

InspectionBase::Result InspectionImpl::convert_result(MAVLinkInspectionTransfer::Result result)
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
