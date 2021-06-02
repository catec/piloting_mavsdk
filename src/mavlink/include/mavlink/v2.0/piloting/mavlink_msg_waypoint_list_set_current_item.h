#pragma once
// MESSAGE WAYPOINT_LIST_SET_CURRENT_ITEM PACKING

#define MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM 185


typedef struct __mavlink_waypoint_list_set_current_item_t {
 uint16_t seq; /*<  Waypoint ID (sequence number). Starts at zero*/
 uint8_t target_system; /*<  System ID*/
 uint8_t target_component; /*<  Component ID*/
} mavlink_waypoint_list_set_current_item_t;

#define MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN 4
#define MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_MIN_LEN 4
#define MAVLINK_MSG_ID_185_LEN 4
#define MAVLINK_MSG_ID_185_MIN_LEN 4

#define MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_CRC 6
#define MAVLINK_MSG_ID_185_CRC 6



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_WAYPOINT_LIST_SET_CURRENT_ITEM { \
    185, \
    "WAYPOINT_LIST_SET_CURRENT_ITEM", \
    3, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_waypoint_list_set_current_item_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_waypoint_list_set_current_item_t, target_component) }, \
         { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_waypoint_list_set_current_item_t, seq) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_WAYPOINT_LIST_SET_CURRENT_ITEM { \
    "WAYPOINT_LIST_SET_CURRENT_ITEM", \
    3, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_waypoint_list_set_current_item_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_waypoint_list_set_current_item_t, target_component) }, \
         { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_waypoint_list_set_current_item_t, seq) }, \
         } \
}
#endif

/**
 * @brief Pack a waypoint_list_set_current_item message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param seq  Waypoint ID (sequence number). Starts at zero
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_list_set_current_item_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN];
    _mav_put_uint16_t(buf, 0, seq);
    _mav_put_uint8_t(buf, 2, target_system);
    _mav_put_uint8_t(buf, 3, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN);
#else
    mavlink_waypoint_list_set_current_item_t packet;
    packet.seq = seq;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_CRC);
}

/**
 * @brief Pack a waypoint_list_set_current_item message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param seq  Waypoint ID (sequence number). Starts at zero
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_list_set_current_item_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component,uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN];
    _mav_put_uint16_t(buf, 0, seq);
    _mav_put_uint8_t(buf, 2, target_system);
    _mav_put_uint8_t(buf, 3, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN);
#else
    mavlink_waypoint_list_set_current_item_t packet;
    packet.seq = seq;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_CRC);
}

/**
 * @brief Encode a waypoint_list_set_current_item struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param waypoint_list_set_current_item C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_waypoint_list_set_current_item_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_waypoint_list_set_current_item_t* waypoint_list_set_current_item)
{
    return mavlink_msg_waypoint_list_set_current_item_pack(system_id, component_id, msg, waypoint_list_set_current_item->target_system, waypoint_list_set_current_item->target_component, waypoint_list_set_current_item->seq);
}

/**
 * @brief Encode a waypoint_list_set_current_item struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param waypoint_list_set_current_item C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_waypoint_list_set_current_item_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_waypoint_list_set_current_item_t* waypoint_list_set_current_item)
{
    return mavlink_msg_waypoint_list_set_current_item_pack_chan(system_id, component_id, chan, msg, waypoint_list_set_current_item->target_system, waypoint_list_set_current_item->target_component, waypoint_list_set_current_item->seq);
}

/**
 * @brief Send a waypoint_list_set_current_item message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param seq  Waypoint ID (sequence number). Starts at zero
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_waypoint_list_set_current_item_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN];
    _mav_put_uint16_t(buf, 0, seq);
    _mav_put_uint8_t(buf, 2, target_system);
    _mav_put_uint8_t(buf, 3, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM, buf, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_CRC);
#else
    mavlink_waypoint_list_set_current_item_t packet;
    packet.seq = seq;
    packet.target_system = target_system;
    packet.target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM, (const char *)&packet, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_CRC);
#endif
}

/**
 * @brief Send a waypoint_list_set_current_item message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_waypoint_list_set_current_item_send_struct(mavlink_channel_t chan, const mavlink_waypoint_list_set_current_item_t* waypoint_list_set_current_item)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_waypoint_list_set_current_item_send(chan, waypoint_list_set_current_item->target_system, waypoint_list_set_current_item->target_component, waypoint_list_set_current_item->seq);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM, (const char *)waypoint_list_set_current_item, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_CRC);
#endif
}

#if MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_waypoint_list_set_current_item_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component, uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, seq);
    _mav_put_uint8_t(buf, 2, target_system);
    _mav_put_uint8_t(buf, 3, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM, buf, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_CRC);
#else
    mavlink_waypoint_list_set_current_item_t *packet = (mavlink_waypoint_list_set_current_item_t *)msgbuf;
    packet->seq = seq;
    packet->target_system = target_system;
    packet->target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM, (const char *)packet, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_CRC);
#endif
}
#endif

#endif

// MESSAGE WAYPOINT_LIST_SET_CURRENT_ITEM UNPACKING


/**
 * @brief Get field target_system from waypoint_list_set_current_item message
 *
 * @return  System ID
 */
static inline uint8_t mavlink_msg_waypoint_list_set_current_item_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Get field target_component from waypoint_list_set_current_item message
 *
 * @return  Component ID
 */
static inline uint8_t mavlink_msg_waypoint_list_set_current_item_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  3);
}

/**
 * @brief Get field seq from waypoint_list_set_current_item message
 *
 * @return  Waypoint ID (sequence number). Starts at zero
 */
static inline uint16_t mavlink_msg_waypoint_list_set_current_item_get_seq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Decode a waypoint_list_set_current_item message into a struct
 *
 * @param msg The message to decode
 * @param waypoint_list_set_current_item C-struct to decode the message contents into
 */
static inline void mavlink_msg_waypoint_list_set_current_item_decode(const mavlink_message_t* msg, mavlink_waypoint_list_set_current_item_t* waypoint_list_set_current_item)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    waypoint_list_set_current_item->seq = mavlink_msg_waypoint_list_set_current_item_get_seq(msg);
    waypoint_list_set_current_item->target_system = mavlink_msg_waypoint_list_set_current_item_get_target_system(msg);
    waypoint_list_set_current_item->target_component = mavlink_msg_waypoint_list_set_current_item_get_target_component(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN? msg->len : MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN;
        memset(waypoint_list_set_current_item, 0, MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_LEN);
    memcpy(waypoint_list_set_current_item, _MAV_PAYLOAD(msg), len);
#endif
}
