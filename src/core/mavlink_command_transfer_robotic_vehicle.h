#pragma once

#include "mavlink_command_transfer.h"

namespace mavsdk {

class MAVLinkCommandTransferRoboticVehicle : public MAVLinkCommandTransfer {
public:
    MAVLinkCommandTransferRoboticVehicle(
        Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler);
    ~MAVLinkCommandTransferRoboticVehicle();

    // Non-copyable
    MAVLinkCommandTransferRoboticVehicle(const MAVLinkCommandTransferRoboticVehicle&) = delete;
    const MAVLinkCommandTransferRoboticVehicle& operator=(const MAVLinkCommandTransferRoboticVehicle&) = delete;
};

} // namespace mavsdk
