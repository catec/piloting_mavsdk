#include "mavsdk_impl.h"

#include <algorithm>
#include <mutex>
#include <utility>

#include "connection.h"
#include "global_include.h"
#include "udp_connection.h"
#include "system.h"
#include "system_impl.h"
#include "cli_arg.h"
#include "version.h"
#include "log.h"

namespace mavsdk {

MavsdkImpl::MavsdkImpl() : timeout_handler(_time), call_every_handler(_time)
{
    LogInfo() << "MAVSDK version: " << mavsdk_version;

    if (const char* env_p = std::getenv("MAVSDK_CALLBACK_DEBUGGING")) {
        if (env_p && std::string("1").compare(env_p) == 0) {
            LogDebug() << "Callback debugging is on.";
            _callback_debugging = true;
        }
    }

    _work_thread = new std::thread(&MavsdkImpl::work_thread, this);

    _process_user_callbacks_thread =
        new std::thread(&MavsdkImpl::process_user_callbacks_thread, this);
}

MavsdkImpl::~MavsdkImpl()
{
    call_every_handler.remove(_heartbeat_send_cookie);

    _should_exit = true;

    if (_process_user_callbacks_thread != nullptr) {
        _user_callback_queue.stop();
        _process_user_callbacks_thread->join();
        delete _process_user_callbacks_thread;
        _process_user_callbacks_thread = nullptr;
    }

    if (_work_thread != nullptr) {
        _work_thread->join();
        delete _work_thread;
        _work_thread = nullptr;
    }

    {
        std::lock_guard<std::recursive_mutex> lock(_systems_mutex);
        _systems.clear();
    }

    {
        std::lock_guard<std::mutex> lock(_connection_mutex);
        _connection = nullptr;
    }
}

std::string MavsdkImpl::version() const
{
    static unsigned version_counter = 0;

    ++version_counter;

    switch (version_counter) {
        case 10:
            return "You were wondering about the name of this library?";
        case 11:
            return "Let's look at the history:";
        case 12:
            return "DroneLink";
        case 13:
            return "DroneCore";
        case 14:
            return "DronecodeSDK";
        case 15:
            return "MAVSDK";
        case 16:
            return "And that's it...";
        case 17:
            return "At least for now ¯\\_(ツ)_/¯.";
        default:
            return mavsdk_version;
    }
}

std::vector<std::shared_ptr<System>> MavsdkImpl::systems() const
{
    std::vector<std::shared_ptr<System>> systems_result{};

    {
        std::lock_guard<std::recursive_mutex> lock(_systems_mutex);
        for (auto& system : _systems) {
            // We ignore the 0 entry because it's just a null system.
            // It's only created because the older, deprecated API needs a
            // reference.
            if (system.first == 0) {
                continue;
            }
            systems_result.push_back(std::shared_ptr<System>(system.second));
        }
    }

    return systems_result;
}

void MavsdkImpl::receive_message(mavlink_message_t& message)
{
    // Don't ever create a system with sysid 0.
    if (message.sysid == 0) {
        return;
    }

    std::lock_guard<std::recursive_mutex> lock(_systems_mutex);

    if (!does_system_exist(message.sysid)) {
        make_system_with_component(message.sysid, message.compid);
    } else {
        _systems.at(message.sysid)->system_impl()->add_new_component(message.compid);
    }

    if (does_system_exist(message.sysid)) {
        _systems.at(message.sysid)->system_impl()->process_mavlink_message(message);
    }
}

bool MavsdkImpl::send_message(mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_connection_mutex);

    if (!_connection) {
        LogWarn() << "no available connecting";
        return false;
    }

    if (!_connection->send_message(message)) {
        LogErr() << "send fail";
        return false;
    }

    return true;
}

ConnectionResult MavsdkImpl::setup_udp_connection(
    const std::string& local_ip, int local_port,
    const std::string& target_ip, int target_port)
{
    auto new_conn = std::make_shared<UdpConnection>(
        std::bind(&MavsdkImpl::receive_message, this, std::placeholders::_1),
        local_ip,
        local_port);
    if (!new_conn) {
        return ConnectionResult::ConnectionError;
    }

    ConnectionResult ret = new_conn->start();
    _is_single_system = false;
    if (ret == ConnectionResult::Success) {
        new_conn->add_remote(target_ip, target_port);
        set_connection(new_conn);
    }
    return ret;
}

void MavsdkImpl::set_connection(std::shared_ptr<Connection> new_connection)
{
    {
        std::lock_guard<std::mutex> lock(_connection_mutex);
        _connection = new_connection;
    }

    if (_configuration.get_always_send_heartbeats()) {
        start_sending_heartbeat();
    }
}

void MavsdkImpl::set_configuration(Mavsdk::Configuration configuration)
{
    _configuration = configuration;

    own_address.system_id = configuration.get_system_id();
    own_address.component_id = configuration.get_component_id();
}

std::shared_ptr<System> MavsdkImpl::get_system(const uint8_t system_id)
{
    std::shared_ptr<System> res = nullptr;

    {
        std::lock_guard<std::recursive_mutex> lock(_systems_mutex);
        auto it = _systems.find(system_id);
        if (it != _systems.end()) {
            res = it->second;
        }
    }

    return res;
}

uint8_t MavsdkImpl::get_own_system_id() const
{
    return own_address.system_id;
}

uint8_t MavsdkImpl::get_own_component_id() const
{
    return own_address.component_id;
}

Mavsdk::Configuration::UsageType MavsdkImpl::get_own_usage_type() const
{
    return _configuration.get_usage_type();
}

bool MavsdkImpl::is_connected() const
{
    std::lock_guard<std::recursive_mutex> lock(_systems_mutex);

    if (_systems.empty()) {
        return false;
    }

    return _systems.begin()->second->is_connected();
}

bool MavsdkImpl::is_connected(const uint8_t system_id) const
{
    std::lock_guard<std::recursive_mutex> lock(_systems_mutex);

    auto system = _systems.find(system_id);
    if (system == _systems.end()) {
        LogErr() << "System ID: " << int(system_id) << " does not exist";
        return false;
    }

    return system->second->is_connected();
}

void MavsdkImpl::make_system_with_component(uint8_t system_id, uint8_t comp_id)
{
    std::lock_guard<std::recursive_mutex> lock(_systems_mutex);

    if (_should_exit) {
        // When the system got destroyed in the destructor, we have to give up.
        return;
    }

    LogInfo() << "New: System ID: " << int(system_id) << " Comp ID: " << int(comp_id);
    // Make a system with its first component
    auto new_system = std::make_shared<System>(*this, system_id, comp_id, _is_single_system);

    _systems.insert(std::pair<uint8_t, std::shared_ptr<System>>(system_id, new_system));
}

bool MavsdkImpl::does_system_exist(uint8_t system_id)
{
    std::lock_guard<std::recursive_mutex> lock(_systems_mutex);

    if (_should_exit) {
        // When the system got destroyed in the destructor, we have to give up.
        return false;
    }

    return (_systems.find(system_id) != _systems.end());
}

void MavsdkImpl::notify_on_discover(const uint64_t system_id)
{
    if (_on_discover_callback) {
        _on_discover_callback(system_id);
    }

    std::lock_guard<std::mutex> lock(_new_system_callback_mutex);
    if (_new_system_callback) {
        auto temp_callback = _new_system_callback;
        call_user_callback([temp_callback]() { temp_callback(); });
    }
}

void MavsdkImpl::notify_on_timeout(const uint64_t system_id)
{
    LogDebug() << "Lost " << system_id;
    if (_on_timeout_callback) {
        _on_timeout_callback(system_id);
    }

    std::lock_guard<std::mutex> lock(_new_system_callback_mutex);
    if (_new_system_callback) {
        auto temp_callback = _new_system_callback;
        call_user_callback([temp_callback]() { temp_callback(); });
    }
}

void MavsdkImpl::subscribe_on_new_system(Mavsdk::NewSystemCallback callback)
{
    std::lock_guard<std::mutex> lock(_new_system_callback_mutex);
    _new_system_callback = callback;

    const auto is_any_system_connected = [this]() {
        auto all_systems = systems();
        return std::any_of(all_systems.cbegin(), all_systems.cend(), [](auto& system) {
            return system->is_connected();
        });
    };

    if (_new_system_callback != nullptr && is_any_system_connected()) {
        _new_system_callback();
    }
}

void MavsdkImpl::register_on_timeout(const Mavsdk::event_callback_t callback)
{
    _on_timeout_callback = callback;
}

void MavsdkImpl::work_thread()
{
    while (!_should_exit) {
        timeout_handler.run_once();
        call_every_handler.run_once();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void MavsdkImpl::call_user_callback_located(
    const std::string& filename, const int linenumber, const std::function<void()>& func)
{
    auto callback_size = _user_callback_queue.size();
    if (callback_size == 10) {
        LogWarn()
            << "User callback queue too slow.\n"
               "See: https://mavsdk.mavlink.io/develop/en/cpp/troubleshooting.html#user_callbacks";

    } else if (callback_size == 99) {
        LogErr()
            << "User callback queue overflown\n"
               "See: https://mavsdk.mavlink.io/develop/en/cpp/troubleshooting.html#user_callbacks";

    } else if (callback_size == 100) {
        return;
    }

    // We only need to keep track of filename and linenumber if we're actually debugging this.
    UserCallback user_callback =
        _callback_debugging ? UserCallback{func, filename, linenumber} : UserCallback{func};

    _user_callback_queue.enqueue(user_callback);
}

void MavsdkImpl::process_user_callbacks_thread()
{
    while (!_should_exit) {
        auto callback = _user_callback_queue.dequeue();
        if (!callback) {
            continue;
        }

        void* cookie{nullptr};

        const double timeout_s = 1.0;
        timeout_handler.add(
            [&]() {
                if (_callback_debugging) {
                    LogWarn() << "Callback called from " << callback.value().filename << ":"
                              << callback.value().linenumber << " took more than " << timeout_s
                              << " second to run.";
                    fflush(stdout);
                    fflush(stderr);
                    abort();
                } else {
                    LogWarn()
                        << "Callback took more than " << timeout_s << " second to run.\n"
                        << "See: https://mavsdk.mavlink.io/develop/en/cpp/troubleshooting.html#user_callbacks";
                }
            },
            timeout_s,
            &cookie);
        callback.value().func();
        timeout_handler.remove(cookie);
    }
}

void MavsdkImpl::start_sending_heartbeat()
{
    call_every_handler.add(
        [this]() { send_heartbeat(); }, _HEARTBEAT_SEND_INTERVAL_S, &_heartbeat_send_cookie);
}

void MavsdkImpl::send_heartbeat()
{
    mavlink_message_t message;
    mavlink_msg_heartbeat_pack(
        own_address.system_id,
        own_address.component_id,
        &message,
        0, 0, 0, 0, 0);
    send_message(message);
}

} // namespace mavsdk
