#include "alarm_robotic_vehicle_impl.h"
#include "system.h"
#include "global_include.h"
#include "mavlink_alarm_transfer_robotic_vehicle.h"

namespace mavsdk {

using namespace std::placeholders; // for `_1`

AlarmRoboticVehicleImpl::AlarmRoboticVehicleImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

AlarmRoboticVehicleImpl::AlarmRoboticVehicleImpl(std::shared_ptr<System> system) :
    PluginImplBase(system)
{
    _parent->register_plugin(this);
}

AlarmRoboticVehicleImpl::~AlarmRoboticVehicleImpl()
{
    _parent->unregister_plugin(this);
}

void AlarmRoboticVehicleImpl::init()
{
    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_ALARM_LIST_REQUEST,
        [this](const mavlink_message_t&) {
            LogDebug() << "Alarm request list message received!";
            upload_alarm();
        },
        this);
}

void AlarmRoboticVehicleImpl::deinit()
{
    _parent->unregister_all_mavlink_message_handlers(this);
}

void AlarmRoboticVehicleImpl::enable() {}

void AlarmRoboticVehicleImpl::disable() {}

void AlarmRoboticVehicleImpl::set_upload_alarm(
    const AlarmBase::AlarmList &alarm_list)
{
    std::lock_guard<std::recursive_mutex> lock(_alarm_data.mutex);
    _alarm_data.alarm_list = alarm_list;
}

void AlarmRoboticVehicleImpl::upload_alarm_async(
    const AlarmBase::AlarmList& alarm_list,
    const AlarmBase::ResultAckCallback& callback)
{
    std::lock_guard<std::recursive_mutex> lock(_alarm_data.mutex);
    _alarm_data.alarm_list = alarm_list;
    _alarm_data.upload_callback = callback;
}

void AlarmRoboticVehicleImpl::upload_alarm()
{
    std::lock_guard<std::recursive_mutex> lock(_alarm_data.mutex);

    if (!_alarm_data.upload_callback) {
        LogErr() << "Not upload callback set";
        return;
    }

    if (_alarm_data.last_upload.lock()) {
        LogWarn() << "Upload alarm is busy, skipping...";
        auto temp_callback = _alarm_data.upload_callback;
        _parent->call_user_callback([temp_callback]() {
            temp_callback(AlarmBase::Result::Busy, AlarmBase::Ack::Unknown);
        });
        return;
    }

    auto alarm_transfer =
        std::dynamic_pointer_cast<MAVLinkAlarmTransferRoboticVehicle>(_parent->alarm_transfer());
    if (!alarm_transfer) {
        LogWarn() << "Alarm Transfer is NULL";
        return;
    }

    auto alarm_list = convert_to_int_items(_alarm_data.alarm_list);

    _alarm_data.last_upload = alarm_transfer->upload_items_async(
        alarm_list, [this](MAVLinkAlarmTransfer::Result result, MAVLinkAlarmTransfer::Ack ack) {
            auto result_and_ack = convert_to_result_and_ack(result, ack);
            std::lock_guard<std::recursive_mutex> lock_cb(_alarm_data.mutex);
            if (_alarm_data.upload_callback) {
                auto temp_callback = _alarm_data.upload_callback;
                _parent->call_user_callback([temp_callback, result_and_ack]() {
                    temp_callback(result_and_ack.first, result_and_ack.second);
                });
            }
        });
}

AlarmBase::Result AlarmRoboticVehicleImpl::cancel_alarm_upload()
{
    auto ptr = _alarm_data.last_upload.lock();
    if (ptr) {
        ptr->cancel();
    } else {
        LogWarn() << "No alarm upload to cancel... ignoring";
    }

    return AlarmBase::Result::Success;
}

void AlarmRoboticVehicleImpl::send_alarm_status(
    const AlarmBase::AlarmStatus& alarm_status)
{
    uint8_t status_type;
    switch (alarm_status.status) {
        case AlarmBase::AlarmStatusType::Ok:
            status_type = ALARM_STATUS_OK;
            break;
        case AlarmBase::AlarmStatusType::Warning:
            status_type = ALARM_STATUS_WARNING;
            break;
        case AlarmBase::AlarmStatusType::Error:
            status_type = ALARM_STATUS_ERROR;
            break;
        default:
            LogWarn() << "Unknown AlarmStatus";
            return;
    }

    mavlink_message_t message;
    mavlink_msg_alarm_status_pack(
        _parent->get_own_system_id(),
        _parent->get_own_component_id(),
        &message,
        alarm_status.stamp_ms,
        alarm_status.index,
        status_type,
        alarm_status.errors_count,
        alarm_status.warns_count);

    _parent->send_message(message);
}

MAVLinkAlarmTransfer::AlarmList
AlarmRoboticVehicleImpl::convert_to_int_items(const AlarmBase::AlarmList& alarms_list_base)
{
    MAVLinkAlarmTransfer::AlarmList alarm_list;

    for (const auto& item : alarms_list_base.items) {
        MAVLinkAlarmTransfer::AlarmItem next_item{item.index, item.name, item.description};

        alarm_list.items.push_back(next_item);
    }

    return alarm_list;
}

std::pair<AlarmBase::Result, AlarmBase::Ack>
AlarmRoboticVehicleImpl::convert_to_result_and_ack(
    MAVLinkAlarmTransfer::Result result, MAVLinkAlarmTransfer::Ack ack)
{
    std::pair<AlarmBase::Result, AlarmBase::Ack> result_pair;
    result_pair.first = convert_result(result);
    result_pair.second = convert_ack(ack);
    return result_pair;
}

AlarmBase::Ack AlarmRoboticVehicleImpl::convert_ack(MAVLinkAlarmTransfer::Ack ack)
{
    switch (ack) {
        case MAVLinkAlarmTransfer::Ack::Accepted:
            return AlarmBase::Ack::Accepted;
        case MAVLinkAlarmTransfer::Ack::Error:
            return AlarmBase::Ack::Error;
        case MAVLinkAlarmTransfer::Ack::NoSpace:
            return AlarmBase::Ack::NoSpace;
        case MAVLinkAlarmTransfer::Ack::InvalidSequence:
            return AlarmBase::Ack::InvalidSequence;
        case MAVLinkAlarmTransfer::Ack::Cancelled:
            return AlarmBase::Ack::Cancelled;
        case MAVLinkAlarmTransfer::Ack::Unknown:
        default:
            return AlarmBase::Ack::Unknown;
    }
}

AlarmBase::Result
AlarmRoboticVehicleImpl::convert_result(MAVLinkAlarmTransfer::Result result)
{
    switch (result) {
        case MAVLinkAlarmTransfer::Result::Success:
            return AlarmBase::Result::Success;
        case MAVLinkAlarmTransfer::Result::ConnectionError:
            return AlarmBase::Result::ConnectionError;
        case MAVLinkAlarmTransfer::Result::Timeout:
            return AlarmBase::Result::Timeout;
        case MAVLinkAlarmTransfer::Result::Cancelled:
            return AlarmBase::Result::TransferCancelled;
        case MAVLinkAlarmTransfer::Result::InvalidSequence:
            return AlarmBase::Result::InvalidArgument;
        case MAVLinkAlarmTransfer::Result::ProtocolError:
            return AlarmBase::Result::ProtocolError;
        default:
            return AlarmBase::Result::Unknown;
    }
}

} // namespace mavsdk
