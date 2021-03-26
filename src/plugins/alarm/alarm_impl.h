#pragma once

#include "plugins/alarm/alarm.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"
#include "mavlink_alarm_transfer.h"

namespace mavsdk {

class AlarmImpl : public PluginImplBase {
public:
    explicit AlarmImpl(System& system);
    explicit AlarmImpl(std::shared_ptr<System> system);
    ~AlarmImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    std::pair<AlarmBase::Result, AlarmBase::AlarmList> download_alarm();
    void download_alarm_async(const AlarmBase::DownloadAlarmListCallback& callback);
    AlarmBase::Result cancel_alarm_download();

    void alarm_status_async(AlarmBase::AlarmStatusCallback&);

    // Non-copyable
    AlarmImpl(const AlarmImpl&) = delete;
    const AlarmImpl& operator=(const AlarmImpl&) = delete;

private:
    void process_alarm_status(const mavlink_message_t&);

    // FIXME: make static
    std::pair<AlarmBase::Result, AlarmBase::AlarmList> convert_to_result_and_alarm(
        MAVLinkAlarmTransfer::Result result, MAVLinkAlarmTransfer::AlarmList& alarm_list);

    static AlarmBase::Result convert_result(MAVLinkAlarmTransfer::Result result);

    struct AlarmData {
        mutable std::recursive_mutex mutex{};
        std::weak_ptr<MAVLinkAlarmTransfer::WorkItem> last_download{};
    } _alarm_data{};

    mutable std::mutex _alarm_status_mutex{};
    AlarmBase::AlarmStatusCallback _alarm_status_subscription{nullptr};

    void* _timeout_cookie{nullptr};
};

} // namespace mavsdk
