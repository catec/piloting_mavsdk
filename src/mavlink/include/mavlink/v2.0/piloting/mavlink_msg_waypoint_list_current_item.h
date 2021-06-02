#pragma once
// MESSAGE WAYPOINT_LIST_CURRENT_ITEM PACKING

#define MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM 186


typedef struct __mavlink_waypoint_list_current_item_t {
 uint16_t seq; /*<  Waypoint ID (sequence number). Starts at zero*/
} mavlink_waypoint_list_current_item_t;

#define MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN 2
#define MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_MIN_LEN 2
#define MAVLINK_MSG_ID_186_LEN 2
#define MAVLINK_MSG_ID_186_MIN_LEN 2

#define MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_CRC 237
#define MAVLINK_MSG_ID_186_CRC 237



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_WAYPOINT_LIST_CURRENT_ITEM { \
    186, \
    "WAYPOINT_LIST_CURRENT_ITEM", \
    1, \
    {  { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_waypoint_list_current_item_t, seq) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_WAYPOINT_LIST_CURRENT_ITEM { \
    "WAYPOINT_LIST_CURRENT_ITEM", \
    1, \
    {  { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_waypoint_list_current_item_t, seq) }, \
         } \
}
#endif

/**
 * @brief Pack a waypoint_list_current_item message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param seq  Waypoint ID (sequence number). Starts at zero
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_list_current_item_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN];
    _mav_put_uint16_t(buf, 0, seq);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN);
#else
    mavlink_waypoint_list_current_item_t packet;
    packet.seq = seq;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_CRC);
}

/**
 * @brief Pack a waypoint_list_current_item message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param seq  Waypoint ID (sequence number). Starts at zero
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_list_current_item_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN];
    _mav_put_uint16_t(buf, 0, seq);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN);
#else
    mavlink_waypoint_list_current_item_t packet;
    packet.seq = seq;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_CRC);
}

/**
 * @brief Encode a waypoint_list_current_item struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param waypoint_list_current_item C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_waypoint_list_current_item_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_waypoint_list_current_item_t* waypoint_list_current_item)
{
    return mavlink_msg_waypoint_list_current_item_pack(system_id, component_id, msg, waypoint_list_current_item->seq);
}

/**
 * @brief Encode a waypoint_list_current_item struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param waypoint_list_current_item C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_waypoint_list_current_item_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_waypoint_list_current_item_t* waypoint_list_current_item)
{
    return mavlink_msg_waypoint_list_current_item_pack_chan(system_id, component_id, chan, msg, waypoint_list_current_item->seq);
}

/**
 * @brief Send a waypoint_list_current_item message
 * @param chan MAVLink channel to send the message
 *
 * @param seq  Waypoint ID (sequence number). Starts at zero
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_waypoint_list_current_item_send(mavlink_channel_t chan, uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN];
    _mav_put_uint16_t(buf, 0, seq);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM, buf, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_CRC);
#else
    mavlink_waypoint_list_current_item_t packet;
    packet.seq = seq;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM, (const char *)&packet, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_CRC);
#endif
}

/**
 * @brief Send a waypoint_list_current_item message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_waypoint_list_current_item_send_struct(mavlink_channel_t chan, const mavlink_waypoint_list_current_item_t* waypoint_list_current_item)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_waypoint_list_current_item_send(chan, waypoint_list_current_item->seq);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM, (const char *)waypoint_list_current_item, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_CRC);
#endif
}

#if MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_waypoint_list_current_item_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint16_t seq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, seq);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM, buf, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_CRC);
#else
    mavlink_waypoint_list_current_item_t *packet = (mavlink_waypoint_list_current_item_t *)msgbuf;
    packet->seq = seq;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM, (const char *)packet, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_CRC);
#endif
}
#endif

#endif

// MESSAGE WAYPOINT_LIST_CURRENT_ITEM UNPACKING


/**
 * @brief Get field seq from waypoint_list_current_item message
 *
 * @return  Waypoint ID (sequence number). Starts at zero
 */
static inline uint16_t mavlink_msg_waypoint_list_current_item_get_seq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Decode a waypoint_list_current_item message into a struct
 *
 * @param msg The message to decode
 * @param waypoint_list_current_item C-struct to decode the message contents into
 */
static inline void mavlink_msg_waypoint_list_current_item_decode(const mavlink_message_t* msg, mavlink_waypoint_list_current_item_t* waypoint_list_current_item)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    waypoint_list_current_item->seq = mavlink_msg_waypoint_list_current_item_get_seq(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN? msg->len : MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN;
        memset(waypoint_list_current_item, 0, MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_LEN);
    memcpy(waypoint_list_current_item, _MAV_PAYLOAD(msg), len);
#endif
}
