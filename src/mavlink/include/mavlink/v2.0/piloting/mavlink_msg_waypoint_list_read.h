#pragma once
// MESSAGE WAYPOINT_LIST_READ PACKING

#define MAVLINK_MSG_ID_WAYPOINT_LIST_READ 181


typedef struct __mavlink_waypoint_list_read_t {
 uint16_t seq; /*<  Waypoint ID (sequence number). Starts at zero*/
 uint8_t target_system; /*<  System ID*/
 uint8_t target_component; /*<  Component ID*/
} mavlink_waypoint_list_read_t;

#define MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN 4
#define MAVLINK_MSG_ID_WAYPOINT_LIST_READ_MIN_LEN 4
#define MAVLINK_MSG_ID_181_LEN 4
#define MAVLINK_MSG_ID_181_MIN_LEN 4

#define MAVLINK_MSG_ID_WAYPOINT_LIST_READ_CRC 71
#define MAVLINK_MSG_ID_181_CRC 71



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_WAYPOINT_LIST_READ { \
    181, \
    "WAYPOINT_LIST_READ", \
    3, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_waypoint_list_read_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_waypoint_list_read_t, target_component) }, \
         { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_waypoint_list_read_t, seq) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_WAYPOINT_LIST_READ { \
    "WAYPOINT_LIST_READ", \
    3, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_waypoint_list_read_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_waypoint_list_read_t, target_component) }, \
         { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_waypoint_list_read_t, seq) }, \
         } \
}
#endif

/**
 * @brief Pack a waypoint_list_read message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param seq  Waypoint ID (sequence number). Starts at zero
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_list_read_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN];
    _mav_put_uint16_t(buf, 0, seq);
    _mav_put_uint8_t(buf, 2, target_system);
    _mav_put_uint8_t(buf, 3, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN);
#else
    mavlink_waypoint_list_read_t packet;
    packet.seq = seq;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WAYPOINT_LIST_READ;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_CRC);
}

/**
 * @brief Pack a waypoint_list_read message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param seq  Waypoint ID (sequence number). Starts at zero
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_list_read_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component,uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN];
    _mav_put_uint16_t(buf, 0, seq);
    _mav_put_uint8_t(buf, 2, target_system);
    _mav_put_uint8_t(buf, 3, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN);
#else
    mavlink_waypoint_list_read_t packet;
    packet.seq = seq;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WAYPOINT_LIST_READ;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_CRC);
}

/**
 * @brief Encode a waypoint_list_read struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param waypoint_list_read C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_waypoint_list_read_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_waypoint_list_read_t* waypoint_list_read)
{
    return mavlink_msg_waypoint_list_read_pack(system_id, component_id, msg, waypoint_list_read->target_system, waypoint_list_read->target_component, waypoint_list_read->seq);
}

/**
 * @brief Encode a waypoint_list_read struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param waypoint_list_read C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_waypoint_list_read_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_waypoint_list_read_t* waypoint_list_read)
{
    return mavlink_msg_waypoint_list_read_pack_chan(system_id, component_id, chan, msg, waypoint_list_read->target_system, waypoint_list_read->target_component, waypoint_list_read->seq);
}

/**
 * @brief Send a waypoint_list_read message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param seq  Waypoint ID (sequence number). Starts at zero
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_waypoint_list_read_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN];
    _mav_put_uint16_t(buf, 0, seq);
    _mav_put_uint8_t(buf, 2, target_system);
    _mav_put_uint8_t(buf, 3, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_READ, buf, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_CRC);
#else
    mavlink_waypoint_list_read_t packet;
    packet.seq = seq;
    packet.target_system = target_system;
    packet.target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_READ, (const char *)&packet, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_CRC);
#endif
}

/**
 * @brief Send a waypoint_list_read message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_waypoint_list_read_send_struct(mavlink_channel_t chan, const mavlink_waypoint_list_read_t* waypoint_list_read)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_waypoint_list_read_send(chan, waypoint_list_read->target_system, waypoint_list_read->target_component, waypoint_list_read->seq);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_READ, (const char *)waypoint_list_read, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_CRC);
#endif
}

#if MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_waypoint_list_read_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component, uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, seq);
    _mav_put_uint8_t(buf, 2, target_system);
    _mav_put_uint8_t(buf, 3, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_READ, buf, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_CRC);
#else
    mavlink_waypoint_list_read_t *packet = (mavlink_waypoint_list_read_t *)msgbuf;
    packet->seq = seq;
    packet->target_system = target_system;
    packet->target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_READ, (const char *)packet, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_CRC);
#endif
}
#endif

#endif

// MESSAGE WAYPOINT_LIST_READ UNPACKING


/**
 * @brief Get field target_system from waypoint_list_read message
 *
 * @return  System ID
 */
static inline uint8_t mavlink_msg_waypoint_list_read_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Get field target_component from waypoint_list_read message
 *
 * @return  Component ID
 */
static inline uint8_t mavlink_msg_waypoint_list_read_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  3);
}

/**
 * @brief Get field seq from waypoint_list_read message
 *
 * @return  Waypoint ID (sequence number). Starts at zero
 */
static inline uint16_t mavlink_msg_waypoint_list_read_get_seq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Decode a waypoint_list_read message into a struct
 *
 * @param msg The message to decode
 * @param waypoint_list_read C-struct to decode the message contents into
 */
static inline void mavlink_msg_waypoint_list_read_decode(const mavlink_message_t* msg, mavlink_waypoint_list_read_t* waypoint_list_read)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    waypoint_list_read->seq = mavlink_msg_waypoint_list_read_get_seq(msg);
    waypoint_list_read->target_system = mavlink_msg_waypoint_list_read_get_target_system(msg);
    waypoint_list_read->target_component = mavlink_msg_waypoint_list_read_get_target_component(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN? msg->len : MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN;
        memset(waypoint_list_read, 0, MAVLINK_MSG_ID_WAYPOINT_LIST_READ_LEN);
    memcpy(waypoint_list_read, _MAV_PAYLOAD(msg), len);
#endif
}
