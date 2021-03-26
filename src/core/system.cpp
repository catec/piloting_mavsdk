#include "system.h"
#include "global_include.h"
#include "mavsdk_impl.h"
#include "mavlink_include.h"
#include "system_impl.h"
#include "plugin_impl_base.h"
#include <functional>
#include <algorithm>

// Set to 1 to log incoming/outgoing mavlink messages.
#define MESSAGE_DEBUGGING 0

namespace mavsdk {

using namespace std::placeholders; // for `_1`

System::System(MavsdkImpl& parent, uint8_t system_id, uint8_t component_id, bool connected) :
    _system_impl(std::make_shared<SystemImpl>(parent, system_id, component_id, connected))
{}

System::~System() {}

bool System::is_connected() const
{
    return _system_impl->is_connected();
}

uint8_t System::get_system_id() const
{
    return _system_impl->get_target_system_id();
}

uint8_t System::get_component_id() const
{
    return _system_impl->get_target_component_id();
}

void System::subscribe_is_connected(IsConnectedCallback callback)
{
    return _system_impl->subscribe_is_connected(callback);
}

void System::register_component_discovered_callback(discover_callback_t callback) const
{
    return _system_impl->register_component_discovered_callback(callback);
}

void System::enable_timesync()
{
    _system_impl->enable_timesync();
}

} // namespace mavsdk
