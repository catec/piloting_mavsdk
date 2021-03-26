#pragma once

#include "mavlink_address.h"
#include "mavlink_include.h"
#include "mavlink_message_handler.h"

namespace mavsdk {
class Sender {
public:
    Sender(MAVLinkAddress& new_own_address, MAVLinkAddress& new_target_address) :
        own_address(new_own_address),
        target_address(new_target_address)
    {}
    virtual ~Sender() = default;
    virtual bool send_message(mavlink_message_t& message) = 0;
    MAVLinkAddress& own_address;
    MAVLinkAddress& target_address;
};
}