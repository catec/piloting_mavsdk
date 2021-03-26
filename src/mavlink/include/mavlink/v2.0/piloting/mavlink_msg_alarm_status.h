#pragma once
// MESSAGE ALARM_STATUS PACKING

#define MAVLINK_MSG_ID_ALARM_STATUS 204


typedef struct __mavlink_alarm_status_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp (time since system boot)*/
 uint16_t index; /*<  Alarm index*/
 uint16_t errors_count; /*<  Alarm errors count since system boot*/
 uint16_t warns_count; /*<  Alarm warnings count since system boot*/
 uint8_t status; /*<  Alarm status (see ALARM_STATUS_FLAGS enum)*/
} mavlink_alarm_status_t;

#define MAVLINK_MSG_ID_ALARM_STATUS_LEN 11
#define MAVLINK_MSG_ID_ALARM_STATUS_MIN_LEN 11
#define MAVLINK_MSG_ID_204_LEN 11
#define MAVLINK_MSG_ID_204_MIN_LEN 11

#define MAVLINK_MSG_ID_ALARM_STATUS_CRC 33
#define MAVLINK_MSG_ID_204_CRC 33



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ALARM_STATUS { \
    204, \
    "ALARM_STATUS", \
    5, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_alarm_status_t, time_boot_ms) }, \
         { "index", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_alarm_status_t, index) }, \
         { "status", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_alarm_status_t, status) }, \
         { "errors_count", NULL, MAVLINK_TYPE_UINT16_T, 0, 6, offsetof(mavlink_alarm_status_t, errors_count) }, \
         { "warns_count", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_alarm_status_t, warns_count) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ALARM_STATUS { \
    "ALARM_STATUS", \
    5, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_alarm_status_t, time_boot_ms) }, \
         { "index", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_alarm_status_t, index) }, \
         { "status", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_alarm_status_t, status) }, \
         { "errors_count", NULL, MAVLINK_TYPE_UINT16_T, 0, 6, offsetof(mavlink_alarm_status_t, errors_count) }, \
         { "warns_count", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_alarm_status_t, warns_count) }, \
         } \
}
#endif

/**
 * @brief Pack a alarm_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp (time since system boot)
 * @param index  Alarm index
 * @param status  Alarm status (see ALARM_STATUS_FLAGS enum)
 * @param errors_count  Alarm errors count since system boot
 * @param warns_count  Alarm warnings count since system boot
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_alarm_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint16_t index, uint8_t status, uint16_t errors_count, uint16_t warns_count)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ALARM_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint16_t(buf, 4, index);
    _mav_put_uint16_t(buf, 6, errors_count);
    _mav_put_uint16_t(buf, 8, warns_count);
    _mav_put_uint8_t(buf, 10, status);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ALARM_STATUS_LEN);
#else
    mavlink_alarm_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.index = index;
    packet.errors_count = errors_count;
    packet.warns_count = warns_count;
    packet.status = status;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ALARM_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ALARM_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ALARM_STATUS_MIN_LEN, MAVLINK_MSG_ID_ALARM_STATUS_LEN, MAVLINK_MSG_ID_ALARM_STATUS_CRC);
}

/**
 * @brief Pack a alarm_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp (time since system boot)
 * @param index  Alarm index
 * @param status  Alarm status (see ALARM_STATUS_FLAGS enum)
 * @param errors_count  Alarm errors count since system boot
 * @param warns_count  Alarm warnings count since system boot
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_alarm_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,uint16_t index,uint8_t status,uint16_t errors_count,uint16_t warns_count)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ALARM_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint16_t(buf, 4, index);
    _mav_put_uint16_t(buf, 6, errors_count);
    _mav_put_uint16_t(buf, 8, warns_count);
    _mav_put_uint8_t(buf, 10, status);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ALARM_STATUS_LEN);
#else
    mavlink_alarm_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.index = index;
    packet.errors_count = errors_count;
    packet.warns_count = warns_count;
    packet.status = status;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ALARM_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ALARM_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ALARM_STATUS_MIN_LEN, MAVLINK_MSG_ID_ALARM_STATUS_LEN, MAVLINK_MSG_ID_ALARM_STATUS_CRC);
}

/**
 * @brief Encode a alarm_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param alarm_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_alarm_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_alarm_status_t* alarm_status)
{
    return mavlink_msg_alarm_status_pack(system_id, component_id, msg, alarm_status->time_boot_ms, alarm_status->index, alarm_status->status, alarm_status->errors_count, alarm_status->warns_count);
}

/**
 * @brief Encode a alarm_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param alarm_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_alarm_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_alarm_status_t* alarm_status)
{
    return mavlink_msg_alarm_status_pack_chan(system_id, component_id, chan, msg, alarm_status->time_boot_ms, alarm_status->index, alarm_status->status, alarm_status->errors_count, alarm_status->warns_count);
}

/**
 * @brief Send a alarm_status message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp (time since system boot)
 * @param index  Alarm index
 * @param status  Alarm status (see ALARM_STATUS_FLAGS enum)
 * @param errors_count  Alarm errors count since system boot
 * @param warns_count  Alarm warnings count since system boot
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_alarm_status_send(mavlink_channel_t chan, uint32_t time_boot_ms, uint16_t index, uint8_t status, uint16_t errors_count, uint16_t warns_count)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ALARM_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint16_t(buf, 4, index);
    _mav_put_uint16_t(buf, 6, errors_count);
    _mav_put_uint16_t(buf, 8, warns_count);
    _mav_put_uint8_t(buf, 10, status);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ALARM_STATUS, buf, MAVLINK_MSG_ID_ALARM_STATUS_MIN_LEN, MAVLINK_MSG_ID_ALARM_STATUS_LEN, MAVLINK_MSG_ID_ALARM_STATUS_CRC);
#else
    mavlink_alarm_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.index = index;
    packet.errors_count = errors_count;
    packet.warns_count = warns_count;
    packet.status = status;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ALARM_STATUS, (const char *)&packet, MAVLINK_MSG_ID_ALARM_STATUS_MIN_LEN, MAVLINK_MSG_ID_ALARM_STATUS_LEN, MAVLINK_MSG_ID_ALARM_STATUS_CRC);
#endif
}

/**
 * @brief Send a alarm_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_alarm_status_send_struct(mavlink_channel_t chan, const mavlink_alarm_status_t* alarm_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_alarm_status_send(chan, alarm_status->time_boot_ms, alarm_status->index, alarm_status->status, alarm_status->errors_count, alarm_status->warns_count);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ALARM_STATUS, (const char *)alarm_status, MAVLINK_MSG_ID_ALARM_STATUS_MIN_LEN, MAVLINK_MSG_ID_ALARM_STATUS_LEN, MAVLINK_MSG_ID_ALARM_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_ALARM_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_alarm_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, uint16_t index, uint8_t status, uint16_t errors_count, uint16_t warns_count)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint16_t(buf, 4, index);
    _mav_put_uint16_t(buf, 6, errors_count);
    _mav_put_uint16_t(buf, 8, warns_count);
    _mav_put_uint8_t(buf, 10, status);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ALARM_STATUS, buf, MAVLINK_MSG_ID_ALARM_STATUS_MIN_LEN, MAVLINK_MSG_ID_ALARM_STATUS_LEN, MAVLINK_MSG_ID_ALARM_STATUS_CRC);
#else
    mavlink_alarm_status_t *packet = (mavlink_alarm_status_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->index = index;
    packet->errors_count = errors_count;
    packet->warns_count = warns_count;
    packet->status = status;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ALARM_STATUS, (const char *)packet, MAVLINK_MSG_ID_ALARM_STATUS_MIN_LEN, MAVLINK_MSG_ID_ALARM_STATUS_LEN, MAVLINK_MSG_ID_ALARM_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE ALARM_STATUS UNPACKING


/**
 * @brief Get field time_boot_ms from alarm_status message
 *
 * @return [ms] Timestamp (time since system boot)
 */
static inline uint32_t mavlink_msg_alarm_status_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field index from alarm_status message
 *
 * @return  Alarm index
 */
static inline uint16_t mavlink_msg_alarm_status_get_index(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  4);
}

/**
 * @brief Get field status from alarm_status message
 *
 * @return  Alarm status (see ALARM_STATUS_FLAGS enum)
 */
static inline uint8_t mavlink_msg_alarm_status_get_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Get field errors_count from alarm_status message
 *
 * @return  Alarm errors count since system boot
 */
static inline uint16_t mavlink_msg_alarm_status_get_errors_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  6);
}

/**
 * @brief Get field warns_count from alarm_status message
 *
 * @return  Alarm warnings count since system boot
 */
static inline uint16_t mavlink_msg_alarm_status_get_warns_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  8);
}

/**
 * @brief Decode a alarm_status message into a struct
 *
 * @param msg The message to decode
 * @param alarm_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_alarm_status_decode(const mavlink_message_t* msg, mavlink_alarm_status_t* alarm_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    alarm_status->time_boot_ms = mavlink_msg_alarm_status_get_time_boot_ms(msg);
    alarm_status->index = mavlink_msg_alarm_status_get_index(msg);
    alarm_status->errors_count = mavlink_msg_alarm_status_get_errors_count(msg);
    alarm_status->warns_count = mavlink_msg_alarm_status_get_warns_count(msg);
    alarm_status->status = mavlink_msg_alarm_status_get_status(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ALARM_STATUS_LEN? msg->len : MAVLINK_MSG_ID_ALARM_STATUS_LEN;
        memset(alarm_status, 0, MAVLINK_MSG_ID_ALARM_STATUS_LEN);
    memcpy(alarm_status, _MAV_PAYLOAD(msg), len);
#endif
}
