#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>

#include "deprecated.h"
#include "system.h"
#include "connection_result.h"

namespace mavsdk {

class MavsdkImpl;
class System;

/**
 * @brief This is the main class of MAVSDK (a MAVLink API Library).

 * It is used to manage the connection.
 *
 * An instance of this class must be created (first) in order to use the library.
 * The instance must be destroyed after use in order to break connection and release all resources.
 */
class Mavsdk {
public:
    /** @brief Default UDP bind IP (accepts any incoming connections). */
    static constexpr auto DEFAULT_UDP_BIND_IP = "0.0.0.0";
    /** @brief Default UDP bind port (same port as used by MAVROS). */
    static constexpr int DEFAULT_UDP_PORT = 14540;

    /**
     * @brief Constructor.
     */
    Mavsdk();

    /**
     * @brief Destructor.
     *
     * Disconnects and releases all resources.
     */
    ~Mavsdk();

    /**
     * @brief Returns the version of MAVSDK.
     *
     * Note, you're not supposed to request the version too many times.
     *
     * @return A string containing the version.
     */
    std::string version() const;

    /**
     * @brief Sets an udp connection to send/receive mavlink messages
     * to other mavsdk system.
     *
     * @param local_ip Local IP direction to open and receive udp messages.
     * @param local_port Local port to open and receive udp messages.
     * @param target_ip IP direction to use to send udp messages (other mavsdk system machine ip).
     * @param target_port Remote port to use to send udp messages.
     *
     * @return The connection result
     */
    ConnectionResult setup_udp_connection(
        const std::string& local_ip, int local_port,
        const std::string& target_ip, int target_port);

    /**
     * @brief Get a vector of systems which have been discovered or set-up.
     *
     * @note If one remote system has been added using setup_udp_connection(), it won't appear here
     * until it sends a heartbeat msg.
     *
     * @return The vector of systems which are available.
     */
    std::vector<std::shared_ptr<System>> systems() const;

    /**
     * @brief Possible configurations.
     */
    class Configuration {
    public:
        /**
         * @brief UsageTypes of configurations, used for automatic ID setting
         */
        enum class UsageType { GroundStation, RoboticVehicle };

        /**
         * @brief Create new Configuration via manually configured
         * system and component ID.
         * @param system_id the system id to store in this configuration
         * @param component_id the component id to store in this configuration
         * @param always_send_heartbeats send heartbeats by default even without a system connected
         */
        explicit Configuration(
            uint8_t system_id,
            uint8_t component_id,
            bool always_send_heartbeats,
            Mavsdk::Configuration::UsageType usage_type);

        Configuration() = delete;
        ~Configuration() = default;

        /**
         * @brief Get the system id of this configuration
         * @return `uint8_t` the system id stored in this configuration, from 1-255
         */
        uint8_t get_system_id() const;

        /**
         * @brief Set the system id of this configuration.
         */
        void set_system_id(uint8_t system_id);

        /**
         * @brief Get the component id of this configuration
         * @return `uint8_t` the component id stored in this configuration,from 1-255
         */
        uint8_t get_component_id() const;

        /**
         * @brief Set the component id of this configuration.
         */
        void set_component_id(uint8_t component_id);

        /**
         * @brief Get whether to send heartbeats by default.
         * @return whether to always send heartbeats
         */
        bool get_always_send_heartbeats() const;

        /**
         * @brief Set whether to send heartbeats by default.
         */
        void set_always_send_heartbeats(bool always_send_heartbeats);

        /** @brief Usage type of this configuration. */
        UsageType get_usage_type() const;

        /** @brief Set the usage type of this configuration. */
        void set_usage_type(UsageType usage_type);

    private:
        uint8_t _system_id;
        uint8_t _component_id;
        bool _always_send_heartbeats;
        UsageType _usage_type;
    };

    /**
     * @brief Set `Configuration` of SDK.
     *
     * The default configuration is `Configuration::GroundStation`
     * The configuration is used in order to set the MAVLink system ID, the
     * component ID, as well as the MAV_TYPE accordingly.
     *
     * @param configuration Configuration chosen.
     */
    void set_configuration(Configuration configuration);

    /**
     * @brief Get the system with the specified system_id.
     *
     * This returns a system for a given system_id if such a system has been discovered and a null
     * system otherwise.

     * @param system_id system id to get.
     * @return A reference to the specified system.
     */
    std::shared_ptr<System> system(uint8_t system_id) const;

    /**
     * @brief Callback type discover and timeout notifications.
     */
    using NewSystemCallback = std::function<void()>;

    /**
     * @brief Get notification about a change in systems.
     *
     * This gets called whenever a system is added.
     *
     * @note Only one subscriber is possible at any time. On a second
     * subscription, the previous one is overwritten. To unsubscribe, pass nullptr;
     *
     * @param callback Callback to subscribe.
     */
    void subscribe_on_new_system(NewSystemCallback callback);

    /**
     * @brief Callback type for discover and timeout notifications.
     *
     * @note This typedef is deprecated because the UUID is replaced by uid with 18 bytes.
     *
     * @param system_id system_id.
     */
    typedef std::function<void(uint64_t system_id)> event_callback_t;

    /**
     * @brief Returns `true` if exactly one system is currently connected.
     *
     * Connected means we are receiving heartbeats from this system.
     * It means the same as "discovered" and "not timed out".
     *
     * If multiple systems have connected, this will return `false`.
     *
     * @return `true` if exactly one system is connected.
     */
    bool is_connected() const;

    /**
     * @brief Returns `true` if a system is currently connected.
     *
     * Connected means we are receiving heartbeats from this system.
     * It means the same as "discovered" and "not timed out".
     *
     * @param system_id system_id of system to check.
     * @return `true` if system is connected.
     */
    bool is_connected(uint64_t system_id) const;

    /**
     * @brief Register callback for system timeout.
     *
     * This sets a callback that will be notified if no heartbeat of the system has been received
     * in 3 seconds.
     *
     * @note Only one callback can be registered at a time. If this function is called several
     * times, previous callbacks will be overwritten.
     *
     * @param callback Callback to register.
     */
    void register_on_timeout(event_callback_t callback);

private:
    /* @private. */
    std::shared_ptr<MavsdkImpl> _impl{};

    // Non-copyable
    Mavsdk(const Mavsdk&) = delete;
    const Mavsdk& operator=(const Mavsdk&) = delete;
};

} // namespace mavsdk
