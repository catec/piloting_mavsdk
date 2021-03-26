#include "mavlink_command_transfer_robotic_vehicle.h"

namespace mavsdk {

MAVLinkCommandTransferRoboticVehicle::MAVLinkCommandTransferRoboticVehicle(
    Sender& sender, MAVLinkMessageHandler& message_handler, TimeoutHandler& timeout_handler) :
    MAVLinkCommandTransfer(sender, message_handler, timeout_handler)
{}

MAVLinkCommandTransferRoboticVehicle::~MAVLinkCommandTransferRoboticVehicle() {}

} // namespace mavsdk
