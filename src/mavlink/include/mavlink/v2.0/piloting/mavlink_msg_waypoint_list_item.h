#pragma once
// MESSAGE WAYPOINT_LIST_ITEM PACKING

#define MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM 182


typedef struct __mavlink_waypoint_list_item_t {
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
 char task_uuid[37]; /*<  Identifier of the associated inspection task*/
 char task_type_uuid[37]; /*<  Identifier of the associated inspection task type*/
 uint8_t autocontinue; /*<  Autocontinue to next waypoint*/
} mavlink_waypoint_list_item_t;

#define MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN 109
#define MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_MIN_LEN 109
#define MAVLINK_MSG_ID_182_LEN 109
#define MAVLINK_MSG_ID_182_MIN_LEN 109

#define MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_CRC 214
#define MAVLINK_MSG_ID_182_CRC 214

#define MAVLINK_MSG_WAYPOINT_LIST_ITEM_FIELD_TASK_UUID_LEN 37
#define MAVLINK_MSG_WAYPOINT_LIST_ITEM_FIELD_TASK_TYPE_UUID_LEN 37

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_WAYPOINT_LIST_ITEM { \
    182, \
    "WAYPOINT_LIST_ITEM", \
    14, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 32, offsetof(mavlink_waypoint_list_item_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 33, offsetof(mavlink_waypoint_list_item_t, target_component) }, \
         { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 28, offsetof(mavlink_waypoint_list_item_t, seq) }, \
         { "task_uuid", NULL, MAVLINK_TYPE_CHAR, 37, 34, offsetof(mavlink_waypoint_list_item_t, task_uuid) }, \
         { "task_type_uuid", NULL, MAVLINK_TYPE_CHAR, 37, 71, offsetof(mavlink_waypoint_list_item_t, task_type_uuid) }, \
         { "command", NULL, MAVLINK_TYPE_UINT16_T, 0, 30, offsetof(mavlink_waypoint_list_item_t, command) }, \
         { "autocontinue", NULL, MAVLINK_TYPE_UINT8_T, 0, 108, offsetof(mavlink_waypoint_list_item_t, autocontinue) }, \
         { "param1", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_waypoint_list_item_t, param1) }, \
         { "param2", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_waypoint_list_item_t, param2) }, \
         { "param3", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_waypoint_list_item_t, param3) }, \
         { "param4", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_waypoint_list_item_t, param4) }, \
         { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_waypoint_list_item_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_waypoint_list_item_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_waypoint_list_item_t, z) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_WAYPOINT_LIST_ITEM { \
    "WAYPOINT_LIST_ITEM", \
    14, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 32, offsetof(mavlink_waypoint_list_item_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 33, offsetof(mavlink_waypoint_list_item_t, target_component) }, \
         { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 28, offsetof(mavlink_waypoint_list_item_t, seq) }, \
         { "task_uuid", NULL, MAVLINK_TYPE_CHAR, 37, 34, offsetof(mavlink_waypoint_list_item_t, task_uuid) }, \
         { "task_type_uuid", NULL, MAVLINK_TYPE_CHAR, 37, 71, offsetof(mavlink_waypoint_list_item_t, task_type_uuid) }, \
         { "command", NULL, MAVLINK_TYPE_UINT16_T, 0, 30, offsetof(mavlink_waypoint_list_item_t, command) }, \
         { "autocontinue", NULL, MAVLINK_TYPE_UINT8_T, 0, 108, offsetof(mavlink_waypoint_list_item_t, autocontinue) }, \
         { "param1", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_waypoint_list_item_t, param1) }, \
         { "param2", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_waypoint_list_item_t, param2) }, \
         { "param3", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_waypoint_list_item_t, param3) }, \
         { "param4", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_waypoint_list_item_t, param4) }, \
         { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_waypoint_list_item_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_waypoint_list_item_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_waypoint_list_item_t, z) }, \
         } \
}
#endif

/**
 * @brief Pack a waypoint_list_item message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param seq  Waypoint ID (sequence number). Starts at zero. Increases monotonically for each waypoint, no gaps in the sequence (0,1,2,3,4)
 * @param task_uuid  Identifier of the associated inspection task
 * @param task_type_uuid  Identifier of the associated inspection task type
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
static inline uint16_t mavlink_msg_waypoint_list_item_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, uint16_t seq, const char *task_uuid, const char *task_type_uuid, uint16_t command, uint8_t autocontinue, float param1, float param2, float param3, float param4, float x, float y, float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN];
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
    _mav_put_uint8_t(buf, 108, autocontinue);
    _mav_put_char_array(buf, 34, task_uuid, 37);
    _mav_put_char_array(buf, 71, task_type_uuid, 37);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN);
#else
    mavlink_waypoint_list_item_t packet;
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
    mav_array_memcpy(packet.task_uuid, task_uuid, sizeof(char)*37);
    mav_array_memcpy(packet.task_type_uuid, task_type_uuid, sizeof(char)*37);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_CRC);
}

/**
 * @brief Pack a waypoint_list_item message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param seq  Waypoint ID (sequence number). Starts at zero. Increases monotonically for each waypoint, no gaps in the sequence (0,1,2,3,4)
 * @param task_uuid  Identifier of the associated inspection task
 * @param task_type_uuid  Identifier of the associated inspection task type
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
static inline uint16_t mavlink_msg_waypoint_list_item_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component,uint16_t seq,const char *task_uuid,const char *task_type_uuid,uint16_t command,uint8_t autocontinue,float param1,float param2,float param3,float param4,float x,float y,float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN];
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
    _mav_put_uint8_t(buf, 108, autocontinue);
    _mav_put_char_array(buf, 34, task_uuid, 37);
    _mav_put_char_array(buf, 71, task_type_uuid, 37);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN);
#else
    mavlink_waypoint_list_item_t packet;
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
    mav_array_memcpy(packet.task_uuid, task_uuid, sizeof(char)*37);
    mav_array_memcpy(packet.task_type_uuid, task_type_uuid, sizeof(char)*37);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_CRC);
}

/**
 * @brief Encode a waypoint_list_item struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param waypoint_list_item C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_waypoint_list_item_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_waypoint_list_item_t* waypoint_list_item)
{
    return mavlink_msg_waypoint_list_item_pack(system_id, component_id, msg, waypoint_list_item->target_system, waypoint_list_item->target_component, waypoint_list_item->seq, waypoint_list_item->task_uuid, waypoint_list_item->task_type_uuid, waypoint_list_item->command, waypoint_list_item->autocontinue, waypoint_list_item->param1, waypoint_list_item->param2, waypoint_list_item->param3, waypoint_list_item->param4, waypoint_list_item->x, waypoint_list_item->y, waypoint_list_item->z);
}

/**
 * @brief Encode a waypoint_list_item struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param waypoint_list_item C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_waypoint_list_item_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_waypoint_list_item_t* waypoint_list_item)
{
    return mavlink_msg_waypoint_list_item_pack_chan(system_id, component_id, chan, msg, waypoint_list_item->target_system, waypoint_list_item->target_component, waypoint_list_item->seq, waypoint_list_item->task_uuid, waypoint_list_item->task_type_uuid, waypoint_list_item->command, waypoint_list_item->autocontinue, waypoint_list_item->param1, waypoint_list_item->param2, waypoint_list_item->param3, waypoint_list_item->param4, waypoint_list_item->x, waypoint_list_item->y, waypoint_list_item->z);
}

/**
 * @brief Send a waypoint_list_item message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param seq  Waypoint ID (sequence number). Starts at zero. Increases monotonically for each waypoint, no gaps in the sequence (0,1,2,3,4)
 * @param task_uuid  Identifier of the associated inspection task
 * @param task_type_uuid  Identifier of the associated inspection task type
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

static inline void mavlink_msg_waypoint_list_item_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint16_t seq, const char *task_uuid, const char *task_type_uuid, uint16_t command, uint8_t autocontinue, float param1, float param2, float param3, float param4, float x, float y, float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN];
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
    _mav_put_uint8_t(buf, 108, autocontinue);
    _mav_put_char_array(buf, 34, task_uuid, 37);
    _mav_put_char_array(buf, 71, task_type_uuid, 37);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM, buf, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_CRC);
#else
    mavlink_waypoint_list_item_t packet;
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
    mav_array_memcpy(packet.task_uuid, task_uuid, sizeof(char)*37);
    mav_array_memcpy(packet.task_type_uuid, task_type_uuid, sizeof(char)*37);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM, (const char *)&packet, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_CRC);
#endif
}

/**
 * @brief Send a waypoint_list_item message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_waypoint_list_item_send_struct(mavlink_channel_t chan, const mavlink_waypoint_list_item_t* waypoint_list_item)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_waypoint_list_item_send(chan, waypoint_list_item->target_system, waypoint_list_item->target_component, waypoint_list_item->seq, waypoint_list_item->task_uuid, waypoint_list_item->task_type_uuid, waypoint_list_item->command, waypoint_list_item->autocontinue, waypoint_list_item->param1, waypoint_list_item->param2, waypoint_list_item->param3, waypoint_list_item->param4, waypoint_list_item->x, waypoint_list_item->y, waypoint_list_item->z);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM, (const char *)waypoint_list_item, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_CRC);
#endif
}

#if MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_waypoint_list_item_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component, uint16_t seq, const char *task_uuid, const char *task_type_uuid, uint16_t command, uint8_t autocontinue, float param1, float param2, float param3, float param4, float x, float y, float z)
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
    _mav_put_uint8_t(buf, 108, autocontinue);
    _mav_put_char_array(buf, 34, task_uuid, 37);
    _mav_put_char_array(buf, 71, task_type_uuid, 37);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM, buf, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_CRC);
#else
    mavlink_waypoint_list_item_t *packet = (mavlink_waypoint_list_item_t *)msgbuf;
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
    mav_array_memcpy(packet->task_uuid, task_uuid, sizeof(char)*37);
    mav_array_memcpy(packet->task_type_uuid, task_type_uuid, sizeof(char)*37);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM, (const char *)packet, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_CRC);
#endif
}
#endif

#endif

// MESSAGE WAYPOINT_LIST_ITEM UNPACKING


/**
 * @brief Get field target_system from waypoint_list_item message
 *
 * @return  System ID
 */
static inline uint8_t mavlink_msg_waypoint_list_item_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  32);
}

/**
 * @brief Get field target_component from waypoint_list_item message
 *
 * @return  Component ID
 */
static inline uint8_t mavlink_msg_waypoint_list_item_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  33);
}

/**
 * @brief Get field seq from waypoint_list_item message
 *
 * @return  Waypoint ID (sequence number). Starts at zero. Increases monotonically for each waypoint, no gaps in the sequence (0,1,2,3,4)
 */
static inline uint16_t mavlink_msg_waypoint_list_item_get_seq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  28);
}

/**
 * @brief Get field task_uuid from waypoint_list_item message
 *
 * @return  Identifier of the associated inspection task
 */
static inline uint16_t mavlink_msg_waypoint_list_item_get_task_uuid(const mavlink_message_t* msg, char *task_uuid)
{
    return _MAV_RETURN_char_array(msg, task_uuid, 37,  34);
}

/**
 * @brief Get field task_type_uuid from waypoint_list_item message
 *
 * @return  Identifier of the associated inspection task type
 */
static inline uint16_t mavlink_msg_waypoint_list_item_get_task_type_uuid(const mavlink_message_t* msg, char *task_type_uuid)
{
    return _MAV_RETURN_char_array(msg, task_type_uuid, 37,  71);
}

/**
 * @brief Get field command from waypoint_list_item message
 *
 * @return  The scheduled action for the waypoint (see MAV_CMD enum)
 */
static inline uint16_t mavlink_msg_waypoint_list_item_get_command(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  30);
}

/**
 * @brief Get field autocontinue from waypoint_list_item message
 *
 * @return  Autocontinue to next waypoint
 */
static inline uint8_t mavlink_msg_waypoint_list_item_get_autocontinue(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  108);
}

/**
 * @brief Get field param1 from waypoint_list_item message
 *
 * @return  PARAM1, see MAV_CMD enum
 */
static inline float mavlink_msg_waypoint_list_item_get_param1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field param2 from waypoint_list_item message
 *
 * @return  PARAM2, see MAV_CMD enum
 */
static inline float mavlink_msg_waypoint_list_item_get_param2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field param3 from waypoint_list_item message
 *
 * @return  PARAM3, see MAV_CMD enum
 */
static inline float mavlink_msg_waypoint_list_item_get_param3(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field param4 from waypoint_list_item message
 *
 * @return  PARAM4, see MAV_CMD enum
 */
static inline float mavlink_msg_waypoint_list_item_get_param4(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field x from waypoint_list_item message
 *
 * @return  PARAM5 / x position in meters
 */
static inline float mavlink_msg_waypoint_list_item_get_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field y from waypoint_list_item message
 *
 * @return  PARAM6 / y position in meters
 */
static inline float mavlink_msg_waypoint_list_item_get_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field z from waypoint_list_item message
 *
 * @return  PARAM7 / z position in meters
 */
static inline float mavlink_msg_waypoint_list_item_get_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Decode a waypoint_list_item message into a struct
 *
 * @param msg The message to decode
 * @param waypoint_list_item C-struct to decode the message contents into
 */
static inline void mavlink_msg_waypoint_list_item_decode(const mavlink_message_t* msg, mavlink_waypoint_list_item_t* waypoint_list_item)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    waypoint_list_item->param1 = mavlink_msg_waypoint_list_item_get_param1(msg);
    waypoint_list_item->param2 = mavlink_msg_waypoint_list_item_get_param2(msg);
    waypoint_list_item->param3 = mavlink_msg_waypoint_list_item_get_param3(msg);
    waypoint_list_item->param4 = mavlink_msg_waypoint_list_item_get_param4(msg);
    waypoint_list_item->x = mavlink_msg_waypoint_list_item_get_x(msg);
    waypoint_list_item->y = mavlink_msg_waypoint_list_item_get_y(msg);
    waypoint_list_item->z = mavlink_msg_waypoint_list_item_get_z(msg);
    waypoint_list_item->seq = mavlink_msg_waypoint_list_item_get_seq(msg);
    waypoint_list_item->command = mavlink_msg_waypoint_list_item_get_command(msg);
    waypoint_list_item->target_system = mavlink_msg_waypoint_list_item_get_target_system(msg);
    waypoint_list_item->target_component = mavlink_msg_waypoint_list_item_get_target_component(msg);
    mavlink_msg_waypoint_list_item_get_task_uuid(msg, waypoint_list_item->task_uuid);
    mavlink_msg_waypoint_list_item_get_task_type_uuid(msg, waypoint_list_item->task_type_uuid);
    waypoint_list_item->autocontinue = mavlink_msg_waypoint_list_item_get_autocontinue(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN? msg->len : MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN;
        memset(waypoint_list_item, 0, MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_LEN);
    memcpy(waypoint_list_item, _MAV_PAYLOAD(msg), len);
#endif
}
