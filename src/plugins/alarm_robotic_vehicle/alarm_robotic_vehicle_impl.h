#pragma once

#include "plugins/alarm_robotic_vehicle/alarm_robotic_vehicle.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"
#include "mavlink_alarm_transfer.h"

namespace mavsdk {

class AlarmRoboticVehicleImpl : public PluginImplBase {
public:
    explicit AlarmRoboticVehicleImpl(System& system);
    explicit AlarmRoboticVehicleImpl(std::shared_ptr<System> system);
    ~AlarmRoboticVehicleImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    void set_upload_alarm(const AlarmBase::AlarmList& alarm_list);
    void upload_alarm_async(
        const AlarmBase::AlarmList& alarm_list,
        const AlarmBase::ResultAckCallback& callback);
    AlarmBase::Result cancel_alarm_upload();

    void send_alarm_status(const AlarmBase::AlarmStatus&);

    // Non-copyable
    AlarmRoboticVehicleImpl(const AlarmRoboticVehicleImpl&) = delete;
    const AlarmRoboticVehicleImpl& operator=(const AlarmRoboticVehicleImpl&) = delete;

private:
    void upload_alarm();

    MAVLinkAlarmTransfer::AlarmList convert_to_int_items(const AlarmBase::AlarmList& alarm_list_base);

    // FIXME: make static
    std::pair<AlarmBase::Result, AlarmBase::Ack> convert_to_result_and_ack(
        MAVLinkAlarmTransfer::Result result, MAVLinkAlarmTransfer::Ack ack);

    static AlarmBase::Ack convert_ack(MAVLinkAlarmTransfer::Ack ack);

    static AlarmBase::Result convert_result(MAVLinkAlarmTransfer::Result result);

    struct {
        mutable std::recursive_mutex mutex{};
        AlarmBase::ResultAckCallback upload_callback{nullptr};
        AlarmBase::AlarmList alarm_list{};
        std::weak_ptr<MAVLinkAlarmTransfer::WorkItem> last_upload{};
    } _alarm_data{};

    void* _timeout_cookie{nullptr};
};

} // namespace mavsdk
