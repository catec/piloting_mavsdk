#pragma once
// MESSAGE INSPECTION_TASKS_ITEM PACKING

#define MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM 182


typedef struct __mavlink_inspection_tasks_item_t {
 float param1; /*<  PARAM1, see MAV_CMD enum*/
 float param2; /*<  PARAM2, see MAV_CMD enum*/
 float param3; /*<  PARAM3, see MAV_CMD enum*/
 float param4; /*<  PARAM4, see MAV_CMD enum*/
 float x; /*<  PARAM5 / x position in meters*/
 float y; /*<  PARAM6 / y position in meters*/
 float z; /*<  PARAM7 / z position in meters*/
 uint16_t seq; /*<  Waypoint ID (sequence number). Starts at zero. Increases monotonically for each waypoint, no gaps in the sequence (0,1,2,3,4)*/
 uint16_t command; /*<  The scheduled action for the waypoint (see MAV_CMD enum)*/
 uint8_t target_system; /*<  System ID*/
 uint8_t target_component; /*<  Component ID*/
 uint8_t autocontinue; /*<  Autocontinue to next waypoint*/
} mavlink_inspection_tasks_item_t;

#define MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN 35
#define MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_MIN_LEN 35
#define MAVLINK_MSG_ID_182_LEN 35
#define MAVLINK_MSG_ID_182_MIN_LEN 35

#define MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_CRC 145
#define MAVLINK_MSG_ID_182_CRC 145



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_INSPECTION_TASKS_ITEM { \
    182, \
    "INSPECTION_TASKS_ITEM", \
    12, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 32, offsetof(mavlink_inspection_tasks_item_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 33, offsetof(mavlink_inspection_tasks_item_t, target_component) }, \
         { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 28, offsetof(mavlink_inspection_tasks_item_t, seq) }, \
         { "command", NULL, MAVLINK_TYPE_UINT16_T, 0, 30, offsetof(mavlink_inspection_tasks_item_t, command) }, \
         { "autocontinue", NULL, MAVLINK_TYPE_UINT8_T, 0, 34, offsetof(mavlink_inspection_tasks_item_t, autocontinue) }, \
         { "param1", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_inspection_tasks_item_t, param1) }, \
         { "param2", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_inspection_tasks_item_t, param2) }, \
         { "param3", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_inspection_tasks_item_t, param3) }, \
         { "param4", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_inspection_tasks_item_t, param4) }, \
         { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_inspection_tasks_item_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_inspection_tasks_item_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_inspection_tasks_item_t, z) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_INSPECTION_TASKS_ITEM { \
    "INSPECTION_TASKS_ITEM", \
    12, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 32, offsetof(mavlink_inspection_tasks_item_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 33, offsetof(mavlink_inspection_tasks_item_t, target_component) }, \
         { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 28, offsetof(mavlink_inspection_tasks_item_t, seq) }, \
         { "command", NULL, MAVLINK_TYPE_UINT16_T, 0, 30, offsetof(mavlink_inspection_tasks_item_t, command) }, \
         { "autocontinue", NULL, MAVLINK_TYPE_UINT8_T, 0, 34, offsetof(mavlink_inspection_tasks_item_t, autocontinue) }, \
         { "param1", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_inspection_tasks_item_t, param1) }, \
         { "param2", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_inspection_tasks_item_t, param2) }, \
         { "param3", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_inspection_tasks_item_t, param3) }, \
         { "param4", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_inspection_tasks_item_t, param4) }, \
         { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_inspection_tasks_item_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_inspection_tasks_item_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_inspection_tasks_item_t, z) }, \
         } \
}
#endif

/**
 * @brief Pack a inspection_tasks_item message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param seq  Waypoint ID (sequence number). Starts at zero. Increases monotonically for each waypoint, no gaps in the sequence (0,1,2,3,4)
 * @param command  The scheduled action for the waypoint (see MAV_CMD enum)
 * @param autocontinue  Autocontinue to next waypoint
 * @param param1  PARAM1, see MAV_CMD enum
 * @param param2  PARAM2, see MAV_CMD enum
 * @param param3  PARAM3, see MAV_CMD enum
 * @param param4  PARAM4, see MAV_CMD enum
 * @param x  PARAM5 / x position in meters
 * @param y  PARAM6 / y position in meters
 * @param z  PARAM7 / z position in meters
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_inspection_tasks_item_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, uint16_t seq, uint16_t command, uint8_t autocontinue, float param1, float param2, float param3, float param4, float x, float y, float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN];
    _mav_put_float(buf, 0, param1);
    _mav_put_float(buf, 4, param2);
    _mav_put_float(buf, 8, param3);
    _mav_put_float(buf, 12, param4);
    _mav_put_float(buf, 16, x);
    _mav_put_float(buf, 20, y);
    _mav_put_float(buf, 24, z);
    _mav_put_uint16_t(buf, 28, seq);
    _mav_put_uint16_t(buf, 30, command);
    _mav_put_uint8_t(buf, 32, target_system);
    _mav_put_uint8_t(buf, 33, target_component);
    _mav_put_uint8_t(buf, 34, autocontinue);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN);
#else
    mavlink_inspection_tasks_item_t packet;
    packet.param1 = param1;
    packet.param2 = param2;
    packet.param3 = param3;
    packet.param4 = param4;
    packet.x = x;
    packet.y = y;
    packet.z = z;
    packet.seq = seq;
    packet.command = command;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.autocontinue = autocontinue;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_CRC);
}

/**
 * @brief Pack a inspection_tasks_item message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param seq  Waypoint ID (sequence number). Starts at zero. Increases monotonically for each waypoint, no gaps in the sequence (0,1,2,3,4)
 * @param command  The scheduled action for the waypoint (see MAV_CMD enum)
 * @param autocontinue  Autocontinue to next waypoint
 * @param param1  PARAM1, see MAV_CMD enum
 * @param param2  PARAM2, see MAV_CMD enum
 * @param param3  PARAM3, see MAV_CMD enum
 * @param param4  PARAM4, see MAV_CMD enum
 * @param x  PARAM5 / x position in meters
 * @param y  PARAM6 / y position in meters
 * @param z  PARAM7 / z position in meters
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_inspection_tasks_item_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component,uint16_t seq,uint16_t command,uint8_t autocontinue,float param1,float param2,float param3,float param4,float x,float y,float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN];
    _mav_put_float(buf, 0, param1);
    _mav_put_float(buf, 4, param2);
    _mav_put_float(buf, 8, param3);
    _mav_put_float(buf, 12, param4);
    _mav_put_float(buf, 16, x);
    _mav_put_float(buf, 20, y);
    _mav_put_float(buf, 24, z);
    _mav_put_uint16_t(buf, 28, seq);
    _mav_put_uint16_t(buf, 30, command);
    _mav_put_uint8_t(buf, 32, target_system);
    _mav_put_uint8_t(buf, 33, target_component);
    _mav_put_uint8_t(buf, 34, autocontinue);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN);
#else
    mavlink_inspection_tasks_item_t packet;
    packet.param1 = param1;
    packet.param2 = param2;
    packet.param3 = param3;
    packet.param4 = param4;
    packet.x = x;
    packet.y = y;
    packet.z = z;
    packet.seq = seq;
    packet.command = command;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.autocontinue = autocontinue;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_CRC);
}

/**
 * @brief Encode a inspection_tasks_item struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param inspection_tasks_item C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_inspection_tasks_item_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_inspection_tasks_item_t* inspection_tasks_item)
{
    return mavlink_msg_inspection_tasks_item_pack(system_id, component_id, msg, inspection_tasks_item->target_system, inspection_tasks_item->target_component, inspection_tasks_item->seq, inspection_tasks_item->command, inspection_tasks_item->autocontinue, inspection_tasks_item->param1, inspection_tasks_item->param2, inspection_tasks_item->param3, inspection_tasks_item->param4, inspection_tasks_item->x, inspection_tasks_item->y, inspection_tasks_item->z);
}

/**
 * @brief Encode a inspection_tasks_item struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param inspection_tasks_item C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_inspection_tasks_item_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_inspection_tasks_item_t* inspection_tasks_item)
{
    return mavlink_msg_inspection_tasks_item_pack_chan(system_id, component_id, chan, msg, inspection_tasks_item->target_system, inspection_tasks_item->target_component, inspection_tasks_item->seq, inspection_tasks_item->command, inspection_tasks_item->autocontinue, inspection_tasks_item->param1, inspection_tasks_item->param2, inspection_tasks_item->param3, inspection_tasks_item->param4, inspection_tasks_item->x, inspection_tasks_item->y, inspection_tasks_item->z);
}

/**
 * @brief Send a inspection_tasks_item message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param seq  Waypoint ID (sequence number). Starts at zero. Increases monotonically for each waypoint, no gaps in the sequence (0,1,2,3,4)
 * @param command  The scheduled action for the waypoint (see MAV_CMD enum)
 * @param autocontinue  Autocontinue to next waypoint
 * @param param1  PARAM1, see MAV_CMD enum
 * @param param2  PARAM2, see MAV_CMD enum
 * @param param3  PARAM3, see MAV_CMD enum
 * @param param4  PARAM4, see MAV_CMD enum
 * @param x  PARAM5 / x position in meters
 * @param y  PARAM6 / y position in meters
 * @param z  PARAM7 / z position in meters
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_inspection_tasks_item_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint16_t seq, uint16_t command, uint8_t autocontinue, float param1, float param2, float param3, float param4, float x, float y, float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN];
    _mav_put_float(buf, 0, param1);
    _mav_put_float(buf, 4, param2);
    _mav_put_float(buf, 8, param3);
    _mav_put_float(buf, 12, param4);
    _mav_put_float(buf, 16, x);
    _mav_put_float(buf, 20, y);
    _mav_put_float(buf, 24, z);
    _mav_put_uint16_t(buf, 28, seq);
    _mav_put_uint16_t(buf, 30, command);
    _mav_put_uint8_t(buf, 32, target_system);
    _mav_put_uint8_t(buf, 33, target_component);
    _mav_put_uint8_t(buf, 34, autocontinue);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM, buf, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_CRC);
#else
    mavlink_inspection_tasks_item_t packet;
    packet.param1 = param1;
    packet.param2 = param2;
    packet.param3 = param3;
    packet.param4 = param4;
    packet.x = x;
    packet.y = y;
    packet.z = z;
    packet.seq = seq;
    packet.command = command;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.autocontinue = autocontinue;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM, (const char *)&packet, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_CRC);
#endif
}

/**
 * @brief Send a inspection_tasks_item message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_inspection_tasks_item_send_struct(mavlink_channel_t chan, const mavlink_inspection_tasks_item_t* inspection_tasks_item)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_inspection_tasks_item_send(chan, inspection_tasks_item->target_system, inspection_tasks_item->target_component, inspection_tasks_item->seq, inspection_tasks_item->command, inspection_tasks_item->autocontinue, inspection_tasks_item->param1, inspection_tasks_item->param2, inspection_tasks_item->param3, inspection_tasks_item->param4, inspection_tasks_item->x, inspection_tasks_item->y, inspection_tasks_item->z);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM, (const char *)inspection_tasks_item, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_CRC);
#endif
}

#if MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_inspection_tasks_item_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component, uint16_t seq, uint16_t command, uint8_t autocontinue, float param1, float param2, float param3, float param4, float x, float y, float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, param1);
    _mav_put_float(buf, 4, param2);
    _mav_put_float(buf, 8, param3);
    _mav_put_float(buf, 12, param4);
    _mav_put_float(buf, 16, x);
    _mav_put_float(buf, 20, y);
    _mav_put_float(buf, 24, z);
    _mav_put_uint16_t(buf, 28, seq);
    _mav_put_uint16_t(buf, 30, command);
    _mav_put_uint8_t(buf, 32, target_system);
    _mav_put_uint8_t(buf, 33, target_component);
    _mav_put_uint8_t(buf, 34, autocontinue);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM, buf, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_CRC);
#else
    mavlink_inspection_tasks_item_t *packet = (mavlink_inspection_tasks_item_t *)msgbuf;
    packet->param1 = param1;
    packet->param2 = param2;
    packet->param3 = param3;
    packet->param4 = param4;
    packet->x = x;
    packet->y = y;
    packet->z = z;
    packet->seq = seq;
    packet->command = command;
    packet->target_system = target_system;
    packet->target_component = target_component;
    packet->autocontinue = autocontinue;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM, (const char *)packet, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_CRC);
#endif
}
#endif

#endif

// MESSAGE INSPECTION_TASKS_ITEM UNPACKING


/**
 * @brief Get field target_system from inspection_tasks_item message
 *
 * @return  System ID
 */
static inline uint8_t mavlink_msg_inspection_tasks_item_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  32);
}

/**
 * @brief Get field target_component from inspection_tasks_item message
 *
 * @return  Component ID
 */
static inline uint8_t mavlink_msg_inspection_tasks_item_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  33);
}

/**
 * @brief Get field seq from inspection_tasks_item message
 *
 * @return  Waypoint ID (sequence number). Starts at zero. Increases monotonically for each waypoint, no gaps in the sequence (0,1,2,3,4)
 */
static inline uint16_t mavlink_msg_inspection_tasks_item_get_seq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  28);
}

/**
 * @brief Get field command from inspection_tasks_item message
 *
 * @return  The scheduled action for the waypoint (see MAV_CMD enum)
 */
static inline uint16_t mavlink_msg_inspection_tasks_item_get_command(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  30);
}

/**
 * @brief Get field autocontinue from inspection_tasks_item message
 *
 * @return  Autocontinue to next waypoint
 */
static inline uint8_t mavlink_msg_inspection_tasks_item_get_autocontinue(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  34);
}

/**
 * @brief Get field param1 from inspection_tasks_item message
 *
 * @return  PARAM1, see MAV_CMD enum
 */
static inline float mavlink_msg_inspection_tasks_item_get_param1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field param2 from inspection_tasks_item message
 *
 * @return  PARAM2, see MAV_CMD enum
 */
static inline float mavlink_msg_inspection_tasks_item_get_param2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field param3 from inspection_tasks_item message
 *
 * @return  PARAM3, see MAV_CMD enum
 */
static inline float mavlink_msg_inspection_tasks_item_get_param3(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field param4 from inspection_tasks_item message
 *
 * @return  PARAM4, see MAV_CMD enum
 */
static inline float mavlink_msg_inspection_tasks_item_get_param4(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field x from inspection_tasks_item message
 *
 * @return  PARAM5 / x position in meters
 */
static inline float mavlink_msg_inspection_tasks_item_get_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field y from inspection_tasks_item message
 *
 * @return  PARAM6 / y position in meters
 */
static inline float mavlink_msg_inspection_tasks_item_get_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field z from inspection_tasks_item message
 *
 * @return  PARAM7 / z position in meters
 */
static inline float mavlink_msg_inspection_tasks_item_get_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Decode a inspection_tasks_item message into a struct
 *
 * @param msg The message to decode
 * @param inspection_tasks_item C-struct to decode the message contents into
 */
static inline void mavlink_msg_inspection_tasks_item_decode(const mavlink_message_t* msg, mavlink_inspection_tasks_item_t* inspection_tasks_item)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    inspection_tasks_item->param1 = mavlink_msg_inspection_tasks_item_get_param1(msg);
    inspection_tasks_item->param2 = mavlink_msg_inspection_tasks_item_get_param2(msg);
    inspection_tasks_item->param3 = mavlink_msg_inspection_tasks_item_get_param3(msg);
    inspection_tasks_item->param4 = mavlink_msg_inspection_tasks_item_get_param4(msg);
    inspection_tasks_item->x = mavlink_msg_inspection_tasks_item_get_x(msg);
    inspection_tasks_item->y = mavlink_msg_inspection_tasks_item_get_y(msg);
    inspection_tasks_item->z = mavlink_msg_inspection_tasks_item_get_z(msg);
    inspection_tasks_item->seq = mavlink_msg_inspection_tasks_item_get_seq(msg);
    inspection_tasks_item->command = mavlink_msg_inspection_tasks_item_get_command(msg);
    inspection_tasks_item->target_system = mavlink_msg_inspection_tasks_item_get_target_system(msg);
    inspection_tasks_item->target_component = mavlink_msg_inspection_tasks_item_get_target_component(msg);
    inspection_tasks_item->autocontinue = mavlink_msg_inspection_tasks_item_get_autocontinue(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN? msg->len : MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN;
        memset(inspection_tasks_item, 0, MAVLINK_MSG_ID_INSPECTION_TASKS_ITEM_LEN);
    memcpy(inspection_tasks_item, _MAV_PAYLOAD(msg), len);
#endif
}
