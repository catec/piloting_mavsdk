#pragma once

#include <memory>
#include <functional>

#include "deprecated.h"

namespace mavsdk {

/**
 * @brief Component Types
 */
enum class ComponentType { UNKNOWN, AUTOPILOT, CAMERA, GIMBAL };

/**
 * @brief type for component discovery callback
 */
typedef std::function<void(ComponentType)> discover_callback_t;

class SystemImpl;
class MavsdkImpl;
class PluginImplBase;

/**
 * @brief This class represents a system, made up of one or more components
 * (e.g. autopilot, cameras, servos, gimbals, etc).
 *
 * System objects are used to interact with UAVs (including their components) and standalone
 * cameras. They are not created directly by application code, but are returned by the Mavsdk
 * class.
 */
class System {
public:
    /** @private Constructor, used internally
     *
     * This constructor is not (and should not be) directly called by application code.
     *
     * @param parent `MavsdkImpl` dependency.
     * @param system_id System id.
     * @param comp_id Component id.
     * @param connected If true then the system doesn't wait for heartbeat to go into connected
     * state
     */
    explicit System(MavsdkImpl& parent, uint8_t system_id, uint8_t comp_id, bool connected);
    /**
     * @brief Destructor.
     */
    ~System();

    /**
     * @brief Checks if the system is connected.
     *
     * A system is connected when heartbeats are arriving (discovered and not timed out).
     * @return `true` if the system is connected.
     */
    bool is_connected() const;

    /**
     * @brief MAVLink System ID of connected system.
     *
     * @note: this is 0 if nothing is connected yet.
     *
     * @return the system ID.
     */
    uint8_t get_system_id() const;

    /**
     * @brief MAVLink Component ID of connected system.
     *
     * @note: this is 0 if nothing is connected yet.
     *
     * @return the component ID.
     */
    uint8_t get_component_id() const;

    /**
     * @brief type for is connected callback.
     */
    using IsConnectedCallback = std::function<void(bool)>;

    /**
     * @brief Subscribe to callback to be called when system connection state changes.
     *
     * @param callback Callback which will be called.
     */
    void subscribe_is_connected(IsConnectedCallback callback);

    /**
     * @brief Register a callback to be called when a component is discovered.
     *
     * @param callback a function of type void(ComponentType) which will be called with the
     * component type of the new component.
     */
    void register_component_discovered_callback(discover_callback_t callback) const;

    /**
     * @brief Enable time synchronization using the TIMESYNC messages.
     */
    void enable_timesync();

    /**
     * @brief Copy constructor (object is not copyable).
     */
    System(const System&) = delete;

    /**
     * @brief Equality operator (object is not copyable).
     */
    const System& operator=(const System&) = delete;

private:
    std::shared_ptr<SystemImpl> system_impl() { return _system_impl; };

    /*
     * MavsdkImpl and PluginImplBase need access to SystemImpl class.
     * This is not pretty but it's not easy to hide the methods from library
     * users if not like that (or with an ugly reinterpret_cast).
     */
    friend MavsdkImpl;
    friend PluginImplBase;

    std::shared_ptr<SystemImpl> _system_impl;
};

} // namespace mavsdk
