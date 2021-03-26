#pragma once

#include <unordered_map>
#include <mutex>
#include <vector>
#include <atomic>

#include "call_every_handler.h"
#include "connection.h"
#include "mavsdk.h"
#include "mavlink_include.h"
#include "mavlink_address.h"
#include "safe_queue.h"
#include "system.h"
#include "timeout_handler.h"

namespace mavsdk {

class MavsdkImpl {
public:
    MavsdkImpl();
    ~MavsdkImpl();
    MavsdkImpl(const MavsdkImpl&) = delete;
    void operator=(const MavsdkImpl&) = delete;

    std::string version() const;

    void receive_message(mavlink_message_t& message);
    bool send_message(mavlink_message_t& message);

    ConnectionResult setup_udp_connection(
        const std::string& local_ip, int local_port,
        const std::string& target_ip, int target_port);

    std::vector<std::shared_ptr<System>> systems() const;

    void set_configuration(Mavsdk::Configuration configuration);

    std::shared_ptr<System> get_system(const uint8_t system_id);

    uint8_t get_own_system_id() const;
    uint8_t get_own_component_id() const;
    Mavsdk::Configuration::UsageType get_own_usage_type() const;

    bool is_connected() const;
    bool is_connected(const uint8_t system_id) const;

    void subscribe_on_new_system(Mavsdk::NewSystemCallback callback);
    void register_on_timeout(Mavsdk::event_callback_t callback);

    void notify_on_discover(uint64_t uuid);
    void notify_on_timeout(uint64_t uuid);

    void start_sending_heartbeat();

    TimeoutHandler timeout_handler;
    CallEveryHandler call_every_handler;

    void call_user_callback_located(
        const std::string& filename, const int linenumber, const std::function<void()>& func);

    MAVLinkAddress own_address{};

private:
    void set_connection(std::shared_ptr<Connection> new_connection);
    void make_system_with_component(uint8_t system_id, uint8_t component_id);
    bool does_system_exist(uint8_t system_id);

    void work_thread();
    void process_user_callbacks_thread();

    void send_heartbeat();

    using system_entry_t = std::pair<uint8_t, std::shared_ptr<System>>;

    std::mutex _connection_mutex{};
    std::shared_ptr<Connection> _connection;

    mutable std::recursive_mutex _systems_mutex{};
    std::unordered_map<uint8_t, std::shared_ptr<System>> _systems{};

    std::mutex _new_system_callback_mutex{};
    Mavsdk::NewSystemCallback _new_system_callback{nullptr};

    Mavsdk::event_callback_t _on_discover_callback{nullptr};
    Mavsdk::event_callback_t _on_timeout_callback{nullptr};

    Time _time{};

    Mavsdk::Configuration _configuration{
        0, 0, false, Mavsdk::Configuration::UsageType::GroundStation};
    bool _is_single_system{false};

    struct UserCallback {
        UserCallback() {}
        UserCallback(const std::function<void()>& func_) : func(func_) {}
        UserCallback(
            const std::function<void()>& func_,
            const std::string& filename_,
            const int linenumber_) :
            func(func_),
            filename(filename_),
            linenumber(linenumber_)
        {}

        std::function<void()> func{};
        std::string filename{};
        int linenumber{};
    };

    std::thread* _work_thread{nullptr};
    std::thread* _process_user_callbacks_thread{nullptr};
    SafeQueue<UserCallback> _user_callback_queue{};
    bool _callback_debugging{false};

    static constexpr double _HEARTBEAT_SEND_INTERVAL_S = 1.0;
    std::atomic<bool> _sending_heartbeats{false};
    void* _heartbeat_send_cookie = nullptr;

    std::atomic<bool> _should_exit = {false};
};

} // namespace mavsdk
