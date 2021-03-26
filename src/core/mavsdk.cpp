#include "mavsdk.h"

#include "mavsdk_impl.h"
#include "global_include.h"

namespace mavsdk {

Mavsdk::Mavsdk()
{
    _impl = std::make_shared<MavsdkImpl>();
}

Mavsdk::~Mavsdk() {}

std::string Mavsdk::version() const
{
    return _impl->version();
}

ConnectionResult Mavsdk::setup_udp_connection(
    const std::string& local_ip, int local_port,
    const std::string& target_ip, int target_port)
{
    return _impl->setup_udp_connection(local_ip, local_port, target_ip, target_port);
}

std::vector<std::shared_ptr<System>> Mavsdk::systems() const
{
    return _impl->systems();
}

void Mavsdk::set_configuration(Configuration configuration)
{
    _impl->set_configuration(configuration);
}

std::shared_ptr<System> Mavsdk::system(uint8_t system_id) const
{
    return _impl->get_system(system_id);
}

bool Mavsdk::is_connected() const
{
    return _impl->is_connected();
}

bool Mavsdk::is_connected(const uint64_t system_id) const
{
    return _impl->is_connected(system_id);
}

void Mavsdk::subscribe_on_new_system(const NewSystemCallback callback)
{
    _impl->subscribe_on_new_system(callback);
}

void Mavsdk::register_on_timeout(const event_callback_t callback)
{
    _impl->register_on_timeout(callback);
}


Mavsdk::Configuration::Configuration(
    uint8_t system_id,
    uint8_t component_id,
    bool always_send_heartbeats,
    Mavsdk::Configuration::UsageType usage_type) :
    _system_id(system_id),
    _component_id(component_id),
    _always_send_heartbeats(always_send_heartbeats),
    _usage_type(usage_type)
{}

uint8_t Mavsdk::Configuration::get_system_id() const
{
    return _system_id;
}

void Mavsdk::Configuration::set_system_id(uint8_t system_id)
{
    _system_id = system_id;
}

uint8_t Mavsdk::Configuration::get_component_id() const
{
    return _component_id;
}

void Mavsdk::Configuration::set_component_id(uint8_t component_id)
{
    _component_id = component_id;
}

bool Mavsdk::Configuration::get_always_send_heartbeats() const
{
    return _always_send_heartbeats;
}

void Mavsdk::Configuration::set_always_send_heartbeats(bool always_send_heartbeats)
{
    _always_send_heartbeats = always_send_heartbeats;
}

Mavsdk::Configuration::UsageType Mavsdk::Configuration::get_usage_type() const
{
    return _usage_type;
}

void Mavsdk::Configuration::set_usage_type(Mavsdk::Configuration::UsageType usage_type)
{
    _usage_type = usage_type;
}

} // namespace mavsdk
