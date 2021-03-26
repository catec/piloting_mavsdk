#pragma once
// MESSAGE INSPECTION_TASKS_COUNT PACKING

#define MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT 180


typedef struct __mavlink_inspection_tasks_count_t {
 uint16_t mission_id; /*<  Mission identifier*/
 uint16_t count; /*<  Number of inspection tasks items in the sequence*/
 uint8_t target_system; /*<  System ID*/
 uint8_t target_component; /*<  Component ID*/
} mavlink_inspection_tasks_count_t;

#define MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN 6
#define MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_MIN_LEN 6
#define MAVLINK_MSG_ID_180_LEN 6
#define MAVLINK_MSG_ID_180_MIN_LEN 6

#define MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_CRC 238
#define MAVLINK_MSG_ID_180_CRC 238



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_INSPECTION_TASKS_COUNT { \
    180, \
    "INSPECTION_TASKS_COUNT", \
    4, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_inspection_tasks_count_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_inspection_tasks_count_t, target_component) }, \
         { "mission_id", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_inspection_tasks_count_t, mission_id) }, \
         { "count", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_inspection_tasks_count_t, count) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_INSPECTION_TASKS_COUNT { \
    "INSPECTION_TASKS_COUNT", \
    4, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_inspection_tasks_count_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_inspection_tasks_count_t, target_component) }, \
         { "mission_id", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_inspection_tasks_count_t, mission_id) }, \
         { "count", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_inspection_tasks_count_t, count) }, \
         } \
}
#endif

/**
 * @brief Pack a inspection_tasks_count message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param mission_id  Mission identifier
 * @param count  Number of inspection tasks items in the sequence
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_inspection_tasks_count_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, uint16_t mission_id, uint16_t count)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN];
    _mav_put_uint16_t(buf, 0, mission_id);
    _mav_put_uint16_t(buf, 2, count);
    _mav_put_uint8_t(buf, 4, target_system);
    _mav_put_uint8_t(buf, 5, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN);
#else
    mavlink_inspection_tasks_count_t packet;
    packet.mission_id = mission_id;
    packet.count = count;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_CRC);
}

/**
 * @brief Pack a inspection_tasks_count message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param mission_id  Mission identifier
 * @param count  Number of inspection tasks items in the sequence
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_inspection_tasks_count_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component,uint16_t mission_id,uint16_t count)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN];
    _mav_put_uint16_t(buf, 0, mission_id);
    _mav_put_uint16_t(buf, 2, count);
    _mav_put_uint8_t(buf, 4, target_system);
    _mav_put_uint8_t(buf, 5, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN);
#else
    mavlink_inspection_tasks_count_t packet;
    packet.mission_id = mission_id;
    packet.count = count;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_CRC);
}

/**
 * @brief Encode a inspection_tasks_count struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param inspection_tasks_count C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_inspection_tasks_count_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_inspection_tasks_count_t* inspection_tasks_count)
{
    return mavlink_msg_inspection_tasks_count_pack(system_id, component_id, msg, inspection_tasks_count->target_system, inspection_tasks_count->target_component, inspection_tasks_count->mission_id, inspection_tasks_count->count);
}

/**
 * @brief Encode a inspection_tasks_count struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param inspection_tasks_count C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_inspection_tasks_count_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_inspection_tasks_count_t* inspection_tasks_count)
{
    return mavlink_msg_inspection_tasks_count_pack_chan(system_id, component_id, chan, msg, inspection_tasks_count->target_system, inspection_tasks_count->target_component, inspection_tasks_count->mission_id, inspection_tasks_count->count);
}

/**
 * @brief Send a inspection_tasks_count message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param mission_id  Mission identifier
 * @param count  Number of inspection tasks items in the sequence
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_inspection_tasks_count_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint16_t mission_id, uint16_t count)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN];
    _mav_put_uint16_t(buf, 0, mission_id);
    _mav_put_uint16_t(buf, 2, count);
    _mav_put_uint8_t(buf, 4, target_system);
    _mav_put_uint8_t(buf, 5, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT, buf, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_CRC);
#else
    mavlink_inspection_tasks_count_t packet;
    packet.mission_id = mission_id;
    packet.count = count;
    packet.target_system = target_system;
    packet.target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT, (const char *)&packet, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_CRC);
#endif
}

/**
 * @brief Send a inspection_tasks_count message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_inspection_tasks_count_send_struct(mavlink_channel_t chan, const mavlink_inspection_tasks_count_t* inspection_tasks_count)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_inspection_tasks_count_send(chan, inspection_tasks_count->target_system, inspection_tasks_count->target_component, inspection_tasks_count->mission_id, inspection_tasks_count->count);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT, (const char *)inspection_tasks_count, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_CRC);
#endif
}

#if MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_inspection_tasks_count_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component, uint16_t mission_id, uint16_t count)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, mission_id);
    _mav_put_uint16_t(buf, 2, count);
    _mav_put_uint8_t(buf, 4, target_system);
    _mav_put_uint8_t(buf, 5, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT, buf, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_CRC);
#else
    mavlink_inspection_tasks_count_t *packet = (mavlink_inspection_tasks_count_t *)msgbuf;
    packet->mission_id = mission_id;
    packet->count = count;
    packet->target_system = target_system;
    packet->target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT, (const char *)packet, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_MIN_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_CRC);
#endif
}
#endif

#endif

// MESSAGE INSPECTION_TASKS_COUNT UNPACKING


/**
 * @brief Get field target_system from inspection_tasks_count message
 *
 * @return  System ID
 */
static inline uint8_t mavlink_msg_inspection_tasks_count_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field target_component from inspection_tasks_count message
 *
 * @return  Component ID
 */
static inline uint8_t mavlink_msg_inspection_tasks_count_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Get field mission_id from inspection_tasks_count message
 *
 * @return  Mission identifier
 */
static inline uint16_t mavlink_msg_inspection_tasks_count_get_mission_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field count from inspection_tasks_count message
 *
 * @return  Number of inspection tasks items in the sequence
 */
static inline uint16_t mavlink_msg_inspection_tasks_count_get_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  2);
}

/**
 * @brief Decode a inspection_tasks_count message into a struct
 *
 * @param msg The message to decode
 * @param inspection_tasks_count C-struct to decode the message contents into
 */
static inline void mavlink_msg_inspection_tasks_count_decode(const mavlink_message_t* msg, mavlink_inspection_tasks_count_t* inspection_tasks_count)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    inspection_tasks_count->mission_id = mavlink_msg_inspection_tasks_count_get_mission_id(msg);
    inspection_tasks_count->count = mavlink_msg_inspection_tasks_count_get_count(msg);
    inspection_tasks_count->target_system = mavlink_msg_inspection_tasks_count_get_target_system(msg);
    inspection_tasks_count->target_component = mavlink_msg_inspection_tasks_count_get_target_component(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN? msg->len : MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN;
        memset(inspection_tasks_count, 0, MAVLINK_MSG_ID_INSPECTION_TASKS_COUNT_LEN);
    memcpy(inspection_tasks_count, _MAV_PAYLOAD(msg), len);
#endif
}
