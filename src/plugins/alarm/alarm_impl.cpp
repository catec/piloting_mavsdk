#include "alarm_impl.h"
#include "system.h"
#include "global_include.h"
#include "mavlink_alarm_transfer_ground_station.h"

namespace mavsdk {

using namespace std::placeholders; // for `_1`

AlarmImpl::AlarmImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

AlarmImpl::AlarmImpl(std::shared_ptr<System> system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

AlarmImpl::~AlarmImpl()
{
    _parent->unregister_plugin(this);
}

void AlarmImpl::init()
{
    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_ALARM_STATUS,
        std::bind(&AlarmImpl::process_alarm_status, this, _1),
        this);
}

void AlarmImpl::enable() {}

void AlarmImpl::disable() {}

void AlarmImpl::deinit()
{
    _parent->unregister_timeout_handler(_timeout_cookie);
    _parent->unregister_all_mavlink_message_handlers(this);
}

void AlarmImpl::process_alarm_status(const mavlink_message_t& message)
{
    mavlink_alarm_status_t alarm_status_data;
    mavlink_msg_alarm_status_decode(&message, &alarm_status_data);

    AlarmBase::AlarmStatusType type;
    switch (alarm_status_data.status) {
        case ALARM_STATUS_OK:
            type = AlarmBase::AlarmStatusType::Ok;
            break;
        case ALARM_STATUS_WARNING:
            type = AlarmBase::AlarmStatusType::Warning;
            break;
        case ALARM_STATUS_ERROR:
            type = AlarmBase::AlarmStatusType::Error;
            break;
        default:
            LogWarn() << "Unknown AlarmStatus status";
            return;
    }

    AlarmBase::AlarmStatus alarm_status;
    alarm_status.stamp_ms     = alarm_status_data.time_boot_ms;
    alarm_status.index        = alarm_status_data.index;
    alarm_status.status       = type;
    alarm_status.errors_count = alarm_status_data.errors_count;
    alarm_status.warns_count  = alarm_status_data.warns_count;

    std::lock_guard<std::mutex> lock(_alarm_status_mutex);
    if (_alarm_status_subscription) {
        auto temp_callback = _alarm_status_subscription;
        _parent->call_user_callback([temp_callback, alarm_status]() { temp_callback(alarm_status); });
    }
}

std::pair<AlarmBase::Result, AlarmBase::AlarmList> AlarmImpl::download_alarm()
{
    auto prom = std::promise<std::pair<AlarmBase::Result, AlarmBase::AlarmList>>();
    auto fut = prom.get_future();

    download_alarm_async([&prom](AlarmBase::Result result, AlarmBase::AlarmList alarm_list) {
        prom.set_value(std::make_pair<>(result, alarm_list));
    });
    return fut.get();
}

void AlarmImpl::download_alarm_async(const AlarmBase::DownloadAlarmListCallback& callback)
{
    if (_alarm_data.last_download.lock()) {
        _parent->call_user_callback([callback]() {
            if (callback) {
                AlarmBase::AlarmList alarm_list{};
                callback(AlarmBase::Result::Busy, alarm_list);
            }
        });
        return;
    }

    auto alarm_transfer =
        std::dynamic_pointer_cast<MAVLinkAlarmTransferGroundStation>(_parent->alarm_transfer());
    if (!alarm_transfer) {
        LogWarn() << "Alarm Transfer is NULL";
        return;
    }

    _alarm_data.last_download = alarm_transfer->download_items_async(
        [this, callback](MAVLinkAlarmTransfer::Result result, MAVLinkAlarmTransfer::AlarmList alarm_list) {
            auto result_and_alarm = convert_to_result_and_alarm(result, alarm_list);
            _parent->call_user_callback([callback, result_and_alarm]() {
                if (callback) {
                    callback(result_and_alarm.first, result_and_alarm.second);
                }
            });
        });
}

AlarmBase::Result AlarmImpl::cancel_alarm_download()
{
    auto ptr = _alarm_data.last_download.lock();
    if (ptr) {
        ptr->cancel();
    } else {
        LogWarn() << "No alarm download to cancel... ignoring";
    }

    return AlarmBase::Result::Success;
}

void AlarmImpl::alarm_status_async(AlarmBase::AlarmStatusCallback& callback)
{
    std::lock_guard<std::mutex> lock(_alarm_status_mutex);
    _alarm_status_subscription = callback;
}

std::pair<AlarmBase::Result, AlarmBase::AlarmList> AlarmImpl::convert_to_result_and_alarm(
    MAVLinkAlarmTransfer::Result result, MAVLinkAlarmTransfer::AlarmList& alarm_list)
{
    std::pair<AlarmBase::Result, AlarmBase::AlarmList> result_pair;

    result_pair.first = convert_result(result);
    if (result_pair.first != AlarmBase::Result::Success) {
        return result_pair;
    }

    AlarmBase::AlarmItem new_alarm_item{};
    for (const auto& int_item : alarm_list.items) {
        LogDebug() << "Assembling Message: " << int(int_item.index);

        new_alarm_item.index = int_item.index;
        new_alarm_item.name = int_item.name;
        new_alarm_item.description = int_item.description;

        result_pair.second.items.push_back(new_alarm_item);
    }

    return result_pair;
}

AlarmBase::Result AlarmImpl::convert_result(MAVLinkAlarmTransfer::Result result)
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
