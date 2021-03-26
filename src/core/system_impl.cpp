#include "system.h"
#include "global_include.h"
#include "mavsdk_impl.h"
#include "mavlink_include.h"
#include "mavlink_inspection_transfer_ground_station.h"
#include "mavlink_inspection_transfer_robotic_vehicle.h"
#include "mavlink_checklist_transfer_robotic_vehicle.h"
#include "mavlink_checklist_transfer_ground_station.h"
#include "mavlink_alarm_transfer_robotic_vehicle.h"
#include "mavlink_alarm_transfer_ground_station.h"
#include "mavlink_hl_action_transfer_robotic_vehicle.h"
#include "mavlink_hl_action_transfer_ground_station.h"
#include "mavlink_command_transfer_robotic_vehicle.h"
#include "mavlink_command_transfer_ground_station.h"
#include "system_impl.h"
#include "plugin_impl_base.h"
#include "log.h"
#include <cstdlib>
#include <functional>
#include <algorithm>
#include <future>
#include <cassert>

// Set to 1 to log incoming/outgoing mavlink messages.
#define MESSAGE_DEBUGGING 0

namespace mavsdk {

using namespace std::placeholders; // for `_1`

SystemImpl::SystemImpl(MavsdkImpl& parent, uint8_t system_id, uint8_t comp_id, bool connected) :
    Sender(parent.own_address, _target_address),
    _parent(parent),
    _timesync(*this),
    _ping(*this)
{
    switch (_parent.get_own_usage_type()) {
        case Mavsdk::Configuration::UsageType::GroundStation:
            _command_transfer = std::make_shared<MAVLinkCommandTransferGroundStation>(
                *this, _message_handler, _parent.timeout_handler);
            _inspection_transfer = std::make_shared<MAVLinkInspectionTransferGroundStation>(
                *this, _message_handler, _parent.timeout_handler);
            _checklist_transfer = std::make_shared<MAVLinkChecklistTransferGroundStation>(
                *this, _message_handler, _parent.timeout_handler);
            _alarm_transfer = std::make_shared<MAVLinkAlarmTransferGroundStation>(
                *this, _message_handler, _parent.timeout_handler);
            _hl_action_transfer = std::make_shared<MAVLinkHLActionTransferGroundStation>(
                *this, _message_handler, _parent.timeout_handler);
            break;
        default:
            _command_transfer = std::make_shared<MAVLinkCommandTransferRoboticVehicle>(
                *this, _message_handler, _parent.timeout_handler);
            _inspection_transfer = std::make_shared<MAVLinkInspectionTransferRoboticVehicle>(
                *this, _message_handler, _parent.timeout_handler);
            _checklist_transfer = std::make_shared<MAVLinkChecklistTransferRoboticVehicle>(
                *this, _message_handler, _parent.timeout_handler);
            _alarm_transfer = std::make_shared<MAVLinkAlarmTransferRoboticVehicle>(
                *this, _message_handler, _parent.timeout_handler);
            _hl_action_transfer = std::make_shared<MAVLinkHLActionTransferRoboticVehicle>(
                *this, _message_handler, _parent.timeout_handler);
            break;
    }

    _target_address.system_id = system_id;
    _target_address.component_id = comp_id;

    if (connected) {
        _always_connected = true;
        set_connected(system_id);
    }
    _system_thread = new std::thread(&SystemImpl::system_thread, this);

    _message_handler.register_one(
        MAVLINK_MSG_ID_HEARTBEAT, std::bind(&SystemImpl::process_heartbeat, this, _1), this);

    add_new_component(comp_id);
}

SystemImpl::~SystemImpl()
{
    _should_exit = true;
    _message_handler.unregister_all(this);

    if (!_always_connected) {
        unregister_timeout_handler(_heartbeat_timeout_cookie);
    }

    if (_system_thread != nullptr) {
        _system_thread->join();
        delete _system_thread;
        _system_thread = nullptr;
    }
}

bool SystemImpl::is_connected() const
{
    return _connected;
}

void SystemImpl::register_mavlink_message_handler(
    uint16_t msg_id, mavlink_message_handler_t callback, const void* cookie)
{
    _message_handler.register_one(msg_id, callback, cookie);
}

void SystemImpl::unregister_mavlink_message_handler(uint16_t msg_id, const void* cookie)
{
    _message_handler.unregister_one(msg_id, cookie);
}

void SystemImpl::unregister_all_mavlink_message_handlers(const void* cookie)
{
    _message_handler.unregister_all(cookie);
}

void SystemImpl::register_timeout_handler(
    const std::function<void()>& callback, double duration_s, void** cookie)
{
    _parent.timeout_handler.add(callback, duration_s, cookie);
}

void SystemImpl::refresh_timeout_handler(const void* cookie)
{
    _parent.timeout_handler.refresh(cookie);
}

void SystemImpl::unregister_timeout_handler(const void* cookie)
{
    _parent.timeout_handler.remove(cookie);
}

void SystemImpl::enable_timesync()
{
    _timesync.enable();
}

void SystemImpl::subscribe_is_connected(System::IsConnectedCallback callback)
{
    std::lock_guard<std::mutex> lock(_connection_mutex);
    _is_connected_callback = callback;
}

void SystemImpl::process_mavlink_message(mavlink_message_t& message)
{
    // This is a low level interface where incoming messages can be tampered
    // with or even dropped.
    if (_incoming_messages_intercept_callback) {
        const bool keep = _incoming_messages_intercept_callback(message);
        if (!keep) {
            LogDebug() << "Dropped incoming message: " << int(message.msgid);
            return;
        }
    }

    _message_handler.process_message(message);
}

void SystemImpl::add_call_every(std::function<void()> callback, float interval_s, void** cookie)
{
    _parent.call_every_handler.add(callback, interval_s, cookie);
}

void SystemImpl::change_call_every(float interval_s, const void* cookie)
{
    _parent.call_every_handler.change(interval_s, cookie);
}

void SystemImpl::reset_call_every(const void* cookie)
{
    _parent.call_every_handler.reset(cookie);
}

void SystemImpl::remove_call_every(const void* cookie)
{
    _parent.call_every_handler.remove(cookie);
}

void SystemImpl::process_heartbeat(const mavlink_message_t& message)
{
    mavlink_heartbeat_t heartbeat;
    mavlink_msg_heartbeat_decode(&message, &heartbeat);

    set_connected(message.sysid);
}

void SystemImpl::heartbeats_timed_out()
{
    LogInfo() << "heartbeats timed out";
    set_disconnected(1);
}

void SystemImpl::system_thread()
{
    dl_time_t last_ping_time{};

    while (!_should_exit) {
        _timesync.do_work();
        if (_command_transfer)
            _command_transfer->do_work();
        if (_inspection_transfer)
            _inspection_transfer->do_work();
        if (_checklist_transfer)
            _checklist_transfer->do_work();
        if (_alarm_transfer)
            _alarm_transfer->do_work();
        if (_hl_action_transfer)
            _hl_action_transfer->do_work();

        if (_time.elapsed_since_s(last_ping_time) >= SystemImpl::_ping_interval_s) {
            if (_connected) {
                _ping.run_once();
            }
            last_ping_time = _time.steady_time();
        }

        if (_connected) {
            // Work fairly fast if we're connected.
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else {
            // Be less aggressive when unconnected.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

std::string SystemImpl::component_name(uint8_t component_id)
{
    switch (component_id) {
        case MAV_COMP_ID_AUTOPILOT1:
            return "Autopilot";
        case MAV_COMP_ID_CAMERA:
            return "Camera 1";
        case MAV_COMP_ID_CAMERA2:
            return "Camera 2";
        case MAV_COMP_ID_CAMERA3:
            return "Camera 3";
        case MAV_COMP_ID_CAMERA4:
            return "Camera 4";
        case MAV_COMP_ID_CAMERA5:
            return "Camera 5";
        case MAV_COMP_ID_CAMERA6:
            return "Camera 6";
        case MAV_COMP_ID_GIMBAL:
            return "Gimbal";
        default:
            return "Unknown component";
    }
}

ComponentType SystemImpl::component_type(uint8_t component_id)
{
    switch (component_id) {
        case MAV_COMP_ID_AUTOPILOT1:
            return ComponentType::AUTOPILOT;
        case MAV_COMP_ID_CAMERA:
        case MAV_COMP_ID_CAMERA2:
        case MAV_COMP_ID_CAMERA3:
        case MAV_COMP_ID_CAMERA4:
        case MAV_COMP_ID_CAMERA5:
        case MAV_COMP_ID_CAMERA6:
            return ComponentType::CAMERA;
        case MAV_COMP_ID_GIMBAL:
            return ComponentType::GIMBAL;
        default:
            return ComponentType::UNKNOWN;
    }
}

void SystemImpl::add_new_component(uint8_t component_id)
{
    if (component_id == 0) {
        return;
    }

    auto res_pair = _components.insert(component_id);
    if (res_pair.second) {
        std::lock_guard<std::mutex> lock(_component_discovered_callback_mutex);
        if (_component_discovered_callback != nullptr) {
            const ComponentType type = component_type(component_id);
            auto temp_callback = _component_discovered_callback;
            call_user_callback([temp_callback, type]() { temp_callback(type); });
        }
        LogDebug() << "Component " << component_name(component_id) << " (" << int(component_id)
                   << ") added.";
    }
}

size_t SystemImpl::total_components() const
{
    return _components.size();
}

void SystemImpl::register_component_discovered_callback(discover_callback_t callback)
{
    std::lock_guard<std::mutex> lock(_component_discovered_callback_mutex);
    _component_discovered_callback = callback;

    if (total_components() > 0) {
        for (const auto& elem : _components) {
            const ComponentType type = component_type(elem);
            if (_component_discovered_callback) {
                auto temp_callback = _component_discovered_callback;
                call_user_callback([temp_callback, type]() { temp_callback(type); });
            }
        }
    }
}

bool SystemImpl::send_message(mavlink_message_t& message)
{
    // This is a low level interface where incoming messages can be tampered
    // with or even dropped.
    if (_outgoing_messages_intercept_callback) {
        const bool keep = _outgoing_messages_intercept_callback(message);
        if (!keep) {
            // We fake that everything was sent as instructed because
            // a potential loss would happen later and we would not be informed
            // about it.
            LogDebug() << "Dropped outgoing message: " << int(message.msgid);
            return true;
        }
    }

#if MESSAGE_DEBUGGING == 1
    LogDebug() << "Sending msg " << size_t(message.msgid);
#endif
    return _parent.send_message(message);
}

void SystemImpl::set_connected(uint8_t sysid)
{
    bool enable_needed = false;
    {
        std::lock_guard<std::mutex> lock(_connection_mutex);

        if (!_connected) {
            LogDebug() << "Discovered " << _components.size() << " component(s)";

            _connected = true;
            _parent.notify_on_discover(sysid); //_uuid

            // Send a heartbeat back immediately.
            _parent.start_sending_heartbeat();

            if (!_always_connected) {
                register_timeout_handler(
                    std::bind(&SystemImpl::heartbeats_timed_out, this),
                    _HEARTBEAT_TIMEOUT_S,
                    &_heartbeat_timeout_cookie);
            }
            enable_needed = true;

            if (_is_connected_callback) {
                const auto temp_callback = _is_connected_callback;
                _parent.call_user_callback([temp_callback]() { temp_callback(true); });
            }

        } else if (_connected && !_always_connected) {
            refresh_timeout_handler(_heartbeat_timeout_cookie);
        }
        // If not yet connected there is nothing to do/
    }
    if (enable_needed) {
        std::lock_guard<std::mutex> lock(_plugin_impls_mutex);
        for (auto plugin_impl : _plugin_impls) {
            plugin_impl->enable();
        }
    }
}

void SystemImpl::set_disconnected(uint8_t sysid)
{
    {
        std::lock_guard<std::mutex> lock(_connection_mutex);

        // This might not be needed because this is probably called from the triggered
        // timeout anyway but it should also do no harm.
        // unregister_timeout_handler(_heartbeat_timeout_cookie);
        //_heartbeat_timeout_cookie = nullptr;

        _connected = false;
        _parent.notify_on_timeout(sysid);
        if (_is_connected_callback) {
            const auto temp_callback = _is_connected_callback;
            _parent.call_user_callback([temp_callback]() { temp_callback(false); });
        }
    }

    {
        std::lock_guard<std::mutex> lock(_plugin_impls_mutex);
        for (auto plugin_impl : _plugin_impls) {
            plugin_impl->disable();
        }
    }
}

uint8_t SystemImpl::get_target_system_id() const
{
    return _target_address.system_id;
}

uint8_t SystemImpl::get_target_component_id() const
{
    return _target_address.component_id;
}

uint8_t SystemImpl::get_own_system_id() const
{
    return _parent.get_own_system_id();
}

uint8_t SystemImpl::get_own_component_id() const
{
    return _parent.get_own_component_id();
}

void SystemImpl::register_plugin(PluginImplBase* plugin_impl)
{
    assert(plugin_impl);

    plugin_impl->init();

    {
        std::lock_guard<std::mutex> lock(_plugin_impls_mutex);
        _plugin_impls.push_back(plugin_impl);
    }

    // If we're connected already, let's enable it straightaway.
    if (_connected) {
        plugin_impl->enable();
    }
}

void SystemImpl::unregister_plugin(PluginImplBase* plugin_impl)
{
    assert(plugin_impl);

    plugin_impl->disable();
    plugin_impl->deinit();

    // Remove first, so it won't get enabled/disabled anymore.
    {
        std::lock_guard<std::mutex> lock(_plugin_impls_mutex);
        auto found = std::find(_plugin_impls.begin(), _plugin_impls.end(), plugin_impl);
        if (found != _plugin_impls.end()) {
            _plugin_impls.erase(found);
        }
    }
}

void SystemImpl::call_user_callback_located(
    const std::string& filename, const int linenumber, const std::function<void()>& func)
{
    _parent.call_user_callback_located(filename, linenumber, func);
}

void SystemImpl::intercept_incoming_messages(std::function<bool(mavlink_message_t&)> callback)
{
    _incoming_messages_intercept_callback = callback;
}

void SystemImpl::intercept_outgoing_messages(std::function<bool(mavlink_message_t&)> callback)
{
    _outgoing_messages_intercept_callback = callback;
}

} // namespace mavsdk
