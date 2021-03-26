#pragma once

#include "global_include.h"
#include "mavlink_address.h"
#include "mavlink_include.h"
#include "mavlink_message_handler.h"
#include "mavlink_checklist_transfer.h"
#include "mavlink_alarm_transfer.h"
#include "mavlink_hl_action_transfer.h"
#include "mavlink_inspection_transfer.h"
#include "mavlink_command_transfer.h"
#include "ping.h"
#include "timeout_handler.h"
#include "safe_queue.h"
#include "timesync.h"
#include "system.h"
#include <cstdint>
#include <functional>
#include <atomic>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <future>

namespace mavsdk {

class MavsdkImpl;
class PluginImplBase;

// This class is the impl of System. This is to hide the private methods
// and functionality from the public library API.
class SystemImpl : public Sender {
public:
    explicit SystemImpl(
        MavsdkImpl& parent, uint8_t system_id, uint8_t component_id, bool connected);
    ~SystemImpl();

    void enable_timesync();

    void subscribe_is_connected(System::IsConnectedCallback callback);

    void process_mavlink_message(mavlink_message_t& message);

    typedef std::function<void(const mavlink_message_t&)> mavlink_message_handler_t;

    void register_mavlink_message_handler(
        uint16_t msg_id, mavlink_message_handler_t callback, const void* cookie);

    void unregister_mavlink_message_handler(uint16_t msg_id, const void* cookie);
    void unregister_all_mavlink_message_handlers(const void* cookie);

    void register_timeout_handler(
        const std::function<void()>& callback, double duration_s, void** cookie);
    void refresh_timeout_handler(const void* cookie);
    void unregister_timeout_handler(const void* cookie);

    void add_call_every(std::function<void()> callback, float interval_s, void** cookie);
    void change_call_every(float interval_s, const void* cookie);
    void reset_call_every(const void* cookie);
    void remove_call_every(const void* cookie);

    bool send_message(mavlink_message_t& message) override;

    // Adds unique component ids
    void add_new_component(uint8_t component_id);
    size_t total_components() const;

    void register_component_discovered_callback(discover_callback_t callback);

    uint8_t get_target_system_id() const;
    uint8_t get_target_component_id() const;

    uint8_t get_own_system_id() const;
    uint8_t get_own_component_id() const;

    bool is_connected() const;

    Time& get_time() { return _time; }
    AutopilotTime& get_autopilot_time() { return _autopilot_time; }

    double get_ping_time_s() const { return _ping.last_ping_time_s(); }

    void register_plugin(PluginImplBase* plugin_impl);
    void unregister_plugin(PluginImplBase* plugin_impl);

    void call_user_callback_located(
        const std::string& filename, const int linenumber, const std::function<void()>& func);

    std::shared_ptr<MAVLinkInspectionTransfer> inspection_transfer() { return _inspection_transfer; }
    std::shared_ptr<MAVLinkChecklistTransfer> checklist_transfer() { return _checklist_transfer; }
    std::shared_ptr<MAVLinkAlarmTransfer> alarm_transfer() { return _alarm_transfer; }
    std::shared_ptr<MAVLinkHLActionTransfer> hl_action_transfer() { return _hl_action_transfer; }
    std::shared_ptr<MAVLinkCommandTransfer> command_transfer() { return _command_transfer; }

    void intercept_incoming_messages(std::function<bool(mavlink_message_t&)> callback);
    void intercept_outgoing_messages(std::function<bool(mavlink_message_t&)> callback);

    // Non-copyable
    SystemImpl(const SystemImpl&) = delete;
    const SystemImpl& operator=(const SystemImpl&) = delete;

private:
    void process_heartbeat(const mavlink_message_t& message);
    void heartbeats_timed_out();
    void set_connected(uint8_t sysid);
    void set_disconnected(uint8_t sysid);

    static std::string component_name(uint8_t component_id);
    static ComponentType component_type(uint8_t component_id);

    void system_thread();

    std::mutex _component_discovered_callback_mutex{};
    discover_callback_t _component_discovered_callback{nullptr};

    MAVLinkAddress _target_address{};

    Time _time{};
    AutopilotTime _autopilot_time{};

    // Needs to be before anything else because they can depend on it.
    MAVLinkMessageHandler _message_handler{};

    uint64_t _uuid{0};

    int _uuid_retries = 0;
    std::atomic<bool> _uuid_initialized{false};

    bool _always_connected{false};

    MavsdkImpl& _parent;

    std::thread* _system_thread{nullptr};
    std::atomic<bool> _should_exit{false};

    static constexpr double _HEARTBEAT_TIMEOUT_S = 3.0;

    std::mutex _connection_mutex{};
    std::atomic<bool> _connected{false};
    System::IsConnectedCallback _is_connected_callback{nullptr};
    void* _heartbeat_timeout_cookie = nullptr;

    static constexpr double _ping_interval_s = 5.0;

    Timesync _timesync;
    Ping _ping;

    std::shared_ptr<MAVLinkInspectionTransfer> _inspection_transfer{};
    std::shared_ptr<MAVLinkChecklistTransfer> _checklist_transfer{};
    std::shared_ptr<MAVLinkAlarmTransfer> _alarm_transfer{};
    std::shared_ptr<MAVLinkHLActionTransfer> _hl_action_transfer{};
    std::shared_ptr<MAVLinkCommandTransfer> _command_transfer{};

    std::mutex _plugin_impls_mutex{};
    std::vector<PluginImplBase*> _plugin_impls{};

    // We used set to maintain unique component ids
    std::unordered_set<uint8_t> _components{};

    std::function<bool(mavlink_message_t&)> _incoming_messages_intercept_callback{nullptr};
    std::function<bool(mavlink_message_t&)> _outgoing_messages_intercept_callback{nullptr};
};

} // namespace mavsdk
