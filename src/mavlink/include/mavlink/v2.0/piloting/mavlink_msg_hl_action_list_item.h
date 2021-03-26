#pragma once
// MESSAGE HL_ACTION_LIST_ITEM PACKING

#define MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM 202


typedef struct __mavlink_hl_action_list_item_t {
 uint16_t index; /*<  High-level action index. Starts at zero*/
 uint16_t command; /*<  Command ID of the command associated with the high-level action (see MAV_CMD enum)*/
 uint8_t target_system; /*<  System ID*/
 uint8_t target_component; /*<  Component ID*/
 char name[20]; /*<  High-level action name string*/
 char description[229]; /*<  High-level action description string*/
} mavlink_hl_action_list_item_t;

#define MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN 255
#define MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_MIN_LEN 255
#define MAVLINK_MSG_ID_202_LEN 255
#define MAVLINK_MSG_ID_202_MIN_LEN 255

#define MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_CRC 47
#define MAVLINK_MSG_ID_202_CRC 47

#define MAVLINK_MSG_HL_ACTION_LIST_ITEM_FIELD_NAME_LEN 20
#define MAVLINK_MSG_HL_ACTION_LIST_ITEM_FIELD_DESCRIPTION_LEN 229

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_HL_ACTION_LIST_ITEM { \
    202, \
    "HL_ACTION_LIST_ITEM", \
    6, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_hl_action_list_item_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_hl_action_list_item_t, target_component) }, \
         { "index", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_hl_action_list_item_t, index) }, \
         { "command", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_hl_action_list_item_t, command) }, \
         { "name", NULL, MAVLINK_TYPE_CHAR, 20, 6, offsetof(mavlink_hl_action_list_item_t, name) }, \
         { "description", NULL, MAVLINK_TYPE_CHAR, 229, 26, offsetof(mavlink_hl_action_list_item_t, description) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_HL_ACTION_LIST_ITEM { \
    "HL_ACTION_LIST_ITEM", \
    6, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_hl_action_list_item_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_hl_action_list_item_t, target_component) }, \
         { "index", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_hl_action_list_item_t, index) }, \
         { "command", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_hl_action_list_item_t, command) }, \
         { "name", NULL, MAVLINK_TYPE_CHAR, 20, 6, offsetof(mavlink_hl_action_list_item_t, name) }, \
         { "description", NULL, MAVLINK_TYPE_CHAR, 229, 26, offsetof(mavlink_hl_action_list_item_t, description) }, \
         } \
}
#endif

/**
 * @brief Pack a hl_action_list_item message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param index  High-level action index. Starts at zero
 * @param command  Command ID of the command associated with the high-level action (see MAV_CMD enum)
 * @param name  High-level action name string
 * @param description  High-level action description string
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hl_action_list_item_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, uint16_t index, uint16_t command, const char *name, const char *description)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN];
    _mav_put_uint16_t(buf, 0, index);
    _mav_put_uint16_t(buf, 2, command);
    _mav_put_uint8_t(buf, 4, target_system);
    _mav_put_uint8_t(buf, 5, target_component);
    _mav_put_char_array(buf, 6, name, 20);
    _mav_put_char_array(buf, 26, description, 229);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN);
#else
    mavlink_hl_action_list_item_t packet;
    packet.index = index;
    packet.command = command;
    packet.target_system = target_system;
    packet.target_component = target_component;
    mav_array_memcpy(packet.name, name, sizeof(char)*20);
    mav_array_memcpy(packet.description, description, sizeof(char)*229);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_CRC);
}

/**
 * @brief Pack a hl_action_list_item message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param index  High-level action index. Starts at zero
 * @param command  Command ID of the command associated with the high-level action (see MAV_CMD enum)
 * @param name  High-level action name string
 * @param description  High-level action description string
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hl_action_list_item_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component,uint16_t index,uint16_t command,const char *name,const char *description)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN];
    _mav_put_uint16_t(buf, 0, index);
    _mav_put_uint16_t(buf, 2, command);
    _mav_put_uint8_t(buf, 4, target_system);
    _mav_put_uint8_t(buf, 5, target_component);
    _mav_put_char_array(buf, 6, name, 20);
    _mav_put_char_array(buf, 26, description, 229);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN);
#else
    mavlink_hl_action_list_item_t packet;
    packet.index = index;
    packet.command = command;
    packet.target_system = target_system;
    packet.target_component = target_component;
    mav_array_memcpy(packet.name, name, sizeof(char)*20);
    mav_array_memcpy(packet.description, description, sizeof(char)*229);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_CRC);
}

/**
 * @brief Encode a hl_action_list_item struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param hl_action_list_item C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hl_action_list_item_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_hl_action_list_item_t* hl_action_list_item)
{
    return mavlink_msg_hl_action_list_item_pack(system_id, component_id, msg, hl_action_list_item->target_system, hl_action_list_item->target_component, hl_action_list_item->index, hl_action_list_item->command, hl_action_list_item->name, hl_action_list_item->description);
}

/**
 * @brief Encode a hl_action_list_item struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param hl_action_list_item C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hl_action_list_item_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_hl_action_list_item_t* hl_action_list_item)
{
    return mavlink_msg_hl_action_list_item_pack_chan(system_id, component_id, chan, msg, hl_action_list_item->target_system, hl_action_list_item->target_component, hl_action_list_item->index, hl_action_list_item->command, hl_action_list_item->name, hl_action_list_item->description);
}

/**
 * @brief Send a hl_action_list_item message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system  System ID
 * @param target_component  Component ID
 * @param index  High-level action index. Starts at zero
 * @param command  Command ID of the command associated with the high-level action (see MAV_CMD enum)
 * @param name  High-level action name string
 * @param description  High-level action description string
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_hl_action_list_item_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint16_t index, uint16_t command, const char *name, const char *description)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN];
    _mav_put_uint16_t(buf, 0, index);
    _mav_put_uint16_t(buf, 2, command);
    _mav_put_uint8_t(buf, 4, target_system);
    _mav_put_uint8_t(buf, 5, target_component);
    _mav_put_char_array(buf, 6, name, 20);
    _mav_put_char_array(buf, 26, description, 229);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM, buf, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_CRC);
#else
    mavlink_hl_action_list_item_t packet;
    packet.index = index;
    packet.command = command;
    packet.target_system = target_system;
    packet.target_component = target_component;
    mav_array_memcpy(packet.name, name, sizeof(char)*20);
    mav_array_memcpy(packet.description, description, sizeof(char)*229);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM, (const char *)&packet, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_CRC);
#endif
}

/**
 * @brief Send a hl_action_list_item message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_hl_action_list_item_send_struct(mavlink_channel_t chan, const mavlink_hl_action_list_item_t* hl_action_list_item)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_hl_action_list_item_send(chan, hl_action_list_item->target_system, hl_action_list_item->target_component, hl_action_list_item->index, hl_action_list_item->command, hl_action_list_item->name, hl_action_list_item->description);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM, (const char *)hl_action_list_item, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_CRC);
#endif
}

#if MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_hl_action_list_item_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component, uint16_t index, uint16_t command, const char *name, const char *description)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, index);
    _mav_put_uint16_t(buf, 2, command);
    _mav_put_uint8_t(buf, 4, target_system);
    _mav_put_uint8_t(buf, 5, target_component);
    _mav_put_char_array(buf, 6, name, 20);
    _mav_put_char_array(buf, 26, description, 229);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM, buf, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_CRC);
#else
    mavlink_hl_action_list_item_t *packet = (mavlink_hl_action_list_item_t *)msgbuf;
    packet->index = index;
    packet->command = command;
    packet->target_system = target_system;
    packet->target_component = target_component;
    mav_array_memcpy(packet->name, name, sizeof(char)*20);
    mav_array_memcpy(packet->description, description, sizeof(char)*229);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM, (const char *)packet, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_MIN_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_CRC);
#endif
}
#endif

#endif

// MESSAGE HL_ACTION_LIST_ITEM UNPACKING


/**
 * @brief Get field target_system from hl_action_list_item message
 *
 * @return  System ID
 */
static inline uint8_t mavlink_msg_hl_action_list_item_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field target_component from hl_action_list_item message
 *
 * @return  Component ID
 */
static inline uint8_t mavlink_msg_hl_action_list_item_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Get field index from hl_action_list_item message
 *
 * @return  High-level action index. Starts at zero
 */
static inline uint16_t mavlink_msg_hl_action_list_item_get_index(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field command from hl_action_list_item message
 *
 * @return  Command ID of the command associated with the high-level action (see MAV_CMD enum)
 */
static inline uint16_t mavlink_msg_hl_action_list_item_get_command(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  2);
}

/**
 * @brief Get field name from hl_action_list_item message
 *
 * @return  High-level action name string
 */
static inline uint16_t mavlink_msg_hl_action_list_item_get_name(const mavlink_message_t* msg, char *name)
{
    return _MAV_RETURN_char_array(msg, name, 20,  6);
}

/**
 * @brief Get field description from hl_action_list_item message
 *
 * @return  High-level action description string
 */
static inline uint16_t mavlink_msg_hl_action_list_item_get_description(const mavlink_message_t* msg, char *description)
{
    return _MAV_RETURN_char_array(msg, description, 229,  26);
}

/**
 * @brief Decode a hl_action_list_item message into a struct
 *
 * @param msg The message to decode
 * @param hl_action_list_item C-struct to decode the message contents into
 */
static inline void mavlink_msg_hl_action_list_item_decode(const mavlink_message_t* msg, mavlink_hl_action_list_item_t* hl_action_list_item)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    hl_action_list_item->index = mavlink_msg_hl_action_list_item_get_index(msg);
    hl_action_list_item->command = mavlink_msg_hl_action_list_item_get_command(msg);
    hl_action_list_item->target_system = mavlink_msg_hl_action_list_item_get_target_system(msg);
    hl_action_list_item->target_component = mavlink_msg_hl_action_list_item_get_target_component(msg);
    mavlink_msg_hl_action_list_item_get_name(msg, hl_action_list_item->name);
    mavlink_msg_hl_action_list_item_get_description(msg, hl_action_list_item->description);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN? msg->len : MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN;
        memset(hl_action_list_item, 0, MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_LEN);
    memcpy(hl_action_list_item, _MAV_PAYLOAD(msg), len);
#endif
}
