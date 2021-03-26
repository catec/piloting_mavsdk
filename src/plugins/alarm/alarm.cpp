#include "alarm_impl.h"
#include "plugins/alarm/alarm.h"

namespace mavsdk {

Alarm::Alarm(System& system) : PluginBase(), _impl{new AlarmImpl(system)} {}

Alarm::Alarm(std::shared_ptr<System> system) : PluginBase(), _impl{new AlarmImpl(system)} {}

Alarm::~Alarm() {}

void Alarm::download_alarm_async(const AlarmBase::DownloadAlarmListCallback callback)
{
    _impl->download_alarm_async(callback);
}

std::pair<AlarmBase::Result, AlarmBase::AlarmList> Alarm::download_alarm() const
{
    return _impl->download_alarm();
}

AlarmBase::Result Alarm::cancel_alarm_download() const
{
    return _impl->cancel_alarm_download();
}

void Alarm::subscribe_alarm_status(AlarmBase::AlarmStatusCallback callback)
{
    _impl->alarm_status_async(callback);
}

} // namespace mavsdk
