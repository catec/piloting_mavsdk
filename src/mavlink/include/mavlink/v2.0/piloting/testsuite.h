/** @file
 *    @brief MAVLink comm protocol testsuite generated from piloting.xml
 *    @see http://qgroundcontrol.org/mavlink/
 */
#pragma once
#ifndef PILOTING_TESTSUITE_H
#define PILOTING_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_piloting(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_common(system_id, component_id, last_msg);
    mavlink_test_piloting(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"


static void mavlink_test_waypoint_list_count(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_WAYPOINT_LIST_COUNT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_waypoint_list_count_t packet_in = {
        963497464,17443,151,218
    };
    mavlink_waypoint_list_count_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.plan_id = packet_in.plan_id;
        packet1.count = packet_in.count;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_WAYPOINT_LIST_COUNT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_WAYPOINT_LIST_COUNT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_count_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_waypoint_list_count_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_count_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.plan_id , packet1.count );
    mavlink_msg_waypoint_list_count_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_count_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.plan_id , packet1.count );
    mavlink_msg_waypoint_list_count_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_waypoint_list_count_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_count_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.plan_id , packet1.count );
    mavlink_msg_waypoint_list_count_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_waypoint_list_read(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_WAYPOINT_LIST_READ >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_waypoint_list_read_t packet_in = {
        17235,139,206
    };
    mavlink_waypoint_list_read_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.seq = packet_in.seq;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_WAYPOINT_LIST_READ_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_WAYPOINT_LIST_READ_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_read_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_waypoint_list_read_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_read_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.seq );
    mavlink_msg_waypoint_list_read_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_read_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.seq );
    mavlink_msg_waypoint_list_read_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_waypoint_list_read_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_read_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.seq );
    mavlink_msg_waypoint_list_read_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_waypoint_list_item(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_waypoint_list_item_t packet_in = {
        963497464,45.0,73.0,101.0,129.0,157.0,185.0,213.0,18899,19003,113,180,247
    };
    mavlink_waypoint_list_item_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.task_id = packet_in.task_id;
        packet1.param1 = packet_in.param1;
        packet1.param2 = packet_in.param2;
        packet1.param3 = packet_in.param3;
        packet1.param4 = packet_in.param4;
        packet1.x = packet_in.x;
        packet1.y = packet_in.y;
        packet1.z = packet_in.z;
        packet1.seq = packet_in.seq;
        packet1.command = packet_in.command;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        packet1.autocontinue = packet_in.autocontinue;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_item_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_waypoint_list_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_item_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.seq , packet1.task_id , packet1.command , packet1.autocontinue , packet1.param1 , packet1.param2 , packet1.param3 , packet1.param4 , packet1.x , packet1.y , packet1.z );
    mavlink_msg_waypoint_list_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_item_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.seq , packet1.task_id , packet1.command , packet1.autocontinue , packet1.param1 , packet1.param2 , packet1.param3 , packet1.param4 , packet1.x , packet1.y , packet1.z );
    mavlink_msg_waypoint_list_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_waypoint_list_item_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_item_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.seq , packet1.task_id , packet1.command , packet1.autocontinue , packet1.param1 , packet1.param2 , packet1.param3 , packet1.param4 , packet1.x , packet1.y , packet1.z );
    mavlink_msg_waypoint_list_item_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_waypoint_list_ack(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_WAYPOINT_LIST_ACK >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_waypoint_list_ack_t packet_in = {
        5,72,139
    };
    mavlink_waypoint_list_ack_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        packet1.type = packet_in.type;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_WAYPOINT_LIST_ACK_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_WAYPOINT_LIST_ACK_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_ack_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_waypoint_list_ack_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_ack_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.type );
    mavlink_msg_waypoint_list_ack_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_ack_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.type );
    mavlink_msg_waypoint_list_ack_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_waypoint_list_ack_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_ack_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.type );
    mavlink_msg_waypoint_list_ack_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_waypoint_list_request(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_WAYPOINT_LIST_REQUEST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_waypoint_list_request_t packet_in = {
        5,72
    };
    mavlink_waypoint_list_request_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_WAYPOINT_LIST_REQUEST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_WAYPOINT_LIST_REQUEST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_request_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_waypoint_list_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_request_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component );
    mavlink_msg_waypoint_list_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_request_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component );
    mavlink_msg_waypoint_list_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_waypoint_list_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_request_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component );
    mavlink_msg_waypoint_list_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_waypoint_list_set_current_item(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_waypoint_list_set_current_item_t packet_in = {
        17235,139,206
    };
    mavlink_waypoint_list_set_current_item_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.seq = packet_in.seq;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_WAYPOINT_LIST_SET_CURRENT_ITEM_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_set_current_item_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_waypoint_list_set_current_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_set_current_item_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.seq );
    mavlink_msg_waypoint_list_set_current_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_set_current_item_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.seq );
    mavlink_msg_waypoint_list_set_current_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_waypoint_list_set_current_item_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_set_current_item_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.seq );
    mavlink_msg_waypoint_list_set_current_item_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_waypoint_list_current_item(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_waypoint_list_current_item_t packet_in = {
        17235
    };
    mavlink_waypoint_list_current_item_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.seq = packet_in.seq;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_WAYPOINT_LIST_CURRENT_ITEM_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_current_item_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_waypoint_list_current_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_current_item_pack(system_id, component_id, &msg , packet1.seq );
    mavlink_msg_waypoint_list_current_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_current_item_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.seq );
    mavlink_msg_waypoint_list_current_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_waypoint_list_current_item_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_current_item_send(MAVLINK_COMM_1 , packet1.seq );
    mavlink_msg_waypoint_list_current_item_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_waypoint_list_item_reached(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_REACHED >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_waypoint_list_item_reached_t packet_in = {
        17235
    };
    mavlink_waypoint_list_item_reached_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.seq = packet_in.seq;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_REACHED_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_WAYPOINT_LIST_ITEM_REACHED_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_item_reached_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_waypoint_list_item_reached_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_item_reached_pack(system_id, component_id, &msg , packet1.seq );
    mavlink_msg_waypoint_list_item_reached_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_item_reached_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.seq );
    mavlink_msg_waypoint_list_item_reached_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_waypoint_list_item_reached_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_waypoint_list_item_reached_send(MAVLINK_COMM_1 , packet1.seq );
    mavlink_msg_waypoint_list_item_reached_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_check_list_request(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CHECK_LIST_REQUEST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_check_list_request_t packet_in = {
        5,72
    };
    mavlink_check_list_request_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CHECK_LIST_REQUEST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CHECK_LIST_REQUEST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_request_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_check_list_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_request_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component );
    mavlink_msg_check_list_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_request_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component );
    mavlink_msg_check_list_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_check_list_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_request_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component );
    mavlink_msg_check_list_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_check_list_count(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CHECK_LIST_COUNT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_check_list_count_t packet_in = {
        17235,139,206
    };
    mavlink_check_list_count_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.count = packet_in.count;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CHECK_LIST_COUNT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CHECK_LIST_COUNT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_count_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_check_list_count_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_count_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.count );
    mavlink_msg_check_list_count_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_count_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.count );
    mavlink_msg_check_list_count_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_check_list_count_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_count_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.count );
    mavlink_msg_check_list_count_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_check_list_read(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CHECK_LIST_READ >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_check_list_read_t packet_in = {
        17235,139,206
    };
    mavlink_check_list_read_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.index = packet_in.index;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CHECK_LIST_READ_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CHECK_LIST_READ_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_read_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_check_list_read_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_read_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.index );
    mavlink_msg_check_list_read_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_read_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.index );
    mavlink_msg_check_list_read_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_check_list_read_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_read_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.index );
    mavlink_msg_check_list_read_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_check_list_item(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CHECK_LIST_ITEM >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_check_list_item_t packet_in = {
        17235,139,206,"EFGHIJKLMNOPQRSTUVW","YZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRST"
    };
    mavlink_check_list_item_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.index = packet_in.index;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        mav_array_memcpy(packet1.name, packet_in.name, sizeof(char)*20);
        mav_array_memcpy(packet1.description, packet_in.description, sizeof(char)*231);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CHECK_LIST_ITEM_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CHECK_LIST_ITEM_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_item_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_check_list_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_item_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.index , packet1.name , packet1.description );
    mavlink_msg_check_list_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_item_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.index , packet1.name , packet1.description );
    mavlink_msg_check_list_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_check_list_item_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_item_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.index , packet1.name , packet1.description );
    mavlink_msg_check_list_item_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_check_list_ack(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CHECK_LIST_ACK >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_check_list_ack_t packet_in = {
        5,72,139
    };
    mavlink_check_list_ack_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        packet1.type = packet_in.type;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CHECK_LIST_ACK_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CHECK_LIST_ACK_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_ack_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_check_list_ack_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_ack_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.type );
    mavlink_msg_check_list_ack_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_ack_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.type );
    mavlink_msg_check_list_ack_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_check_list_ack_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_check_list_ack_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.type );
    mavlink_msg_check_list_ack_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_alarm_list_request(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ALARM_LIST_REQUEST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_alarm_list_request_t packet_in = {
        5,72
    };
    mavlink_alarm_list_request_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ALARM_LIST_REQUEST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ALARM_LIST_REQUEST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_request_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_alarm_list_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_request_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component );
    mavlink_msg_alarm_list_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_request_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component );
    mavlink_msg_alarm_list_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_alarm_list_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_request_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component );
    mavlink_msg_alarm_list_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_alarm_list_count(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ALARM_LIST_COUNT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_alarm_list_count_t packet_in = {
        17235,139,206
    };
    mavlink_alarm_list_count_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.count = packet_in.count;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ALARM_LIST_COUNT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ALARM_LIST_COUNT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_count_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_alarm_list_count_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_count_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.count );
    mavlink_msg_alarm_list_count_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_count_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.count );
    mavlink_msg_alarm_list_count_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_alarm_list_count_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_count_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.count );
    mavlink_msg_alarm_list_count_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_alarm_list_read(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ALARM_LIST_READ >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_alarm_list_read_t packet_in = {
        17235,139,206
    };
    mavlink_alarm_list_read_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.index = packet_in.index;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ALARM_LIST_READ_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ALARM_LIST_READ_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_read_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_alarm_list_read_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_read_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.index );
    mavlink_msg_alarm_list_read_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_read_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.index );
    mavlink_msg_alarm_list_read_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_alarm_list_read_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_read_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.index );
    mavlink_msg_alarm_list_read_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_alarm_list_item(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ALARM_LIST_ITEM >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_alarm_list_item_t packet_in = {
        17235,139,206,"EFGHIJKLMNOPQRSTUVW","YZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRST"
    };
    mavlink_alarm_list_item_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.index = packet_in.index;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        mav_array_memcpy(packet1.name, packet_in.name, sizeof(char)*20);
        mav_array_memcpy(packet1.description, packet_in.description, sizeof(char)*231);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ALARM_LIST_ITEM_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ALARM_LIST_ITEM_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_item_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_alarm_list_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_item_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.index , packet1.name , packet1.description );
    mavlink_msg_alarm_list_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_item_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.index , packet1.name , packet1.description );
    mavlink_msg_alarm_list_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_alarm_list_item_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_item_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.index , packet1.name , packet1.description );
    mavlink_msg_alarm_list_item_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_alarm_list_ack(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ALARM_LIST_ACK >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_alarm_list_ack_t packet_in = {
        5,72,139
    };
    mavlink_alarm_list_ack_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        packet1.type = packet_in.type;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ALARM_LIST_ACK_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ALARM_LIST_ACK_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_ack_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_alarm_list_ack_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_ack_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.type );
    mavlink_msg_alarm_list_ack_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_ack_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.type );
    mavlink_msg_alarm_list_ack_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_alarm_list_ack_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_list_ack_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.type );
    mavlink_msg_alarm_list_ack_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_hl_action_list_request(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_HL_ACTION_LIST_REQUEST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_hl_action_list_request_t packet_in = {
        5,72
    };
    mavlink_hl_action_list_request_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_HL_ACTION_LIST_REQUEST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_HL_ACTION_LIST_REQUEST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_request_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_hl_action_list_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_request_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component );
    mavlink_msg_hl_action_list_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_request_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component );
    mavlink_msg_hl_action_list_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_hl_action_list_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_request_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component );
    mavlink_msg_hl_action_list_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_hl_action_list_count(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_HL_ACTION_LIST_COUNT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_hl_action_list_count_t packet_in = {
        17235,139,206
    };
    mavlink_hl_action_list_count_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.count = packet_in.count;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_HL_ACTION_LIST_COUNT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_HL_ACTION_LIST_COUNT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_count_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_hl_action_list_count_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_count_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.count );
    mavlink_msg_hl_action_list_count_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_count_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.count );
    mavlink_msg_hl_action_list_count_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_hl_action_list_count_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_count_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.count );
    mavlink_msg_hl_action_list_count_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_hl_action_list_read(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_HL_ACTION_LIST_READ >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_hl_action_list_read_t packet_in = {
        17235,139,206
    };
    mavlink_hl_action_list_read_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.index = packet_in.index;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_HL_ACTION_LIST_READ_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_HL_ACTION_LIST_READ_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_read_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_hl_action_list_read_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_read_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.index );
    mavlink_msg_hl_action_list_read_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_read_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.index );
    mavlink_msg_hl_action_list_read_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_hl_action_list_read_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_read_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.index );
    mavlink_msg_hl_action_list_read_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_hl_action_list_item(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_hl_action_list_item_t packet_in = {
        17235,17339,17,84,"GHIJKLMNOPQRSTUVWXY","ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRST"
    };
    mavlink_hl_action_list_item_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.index = packet_in.index;
        packet1.command = packet_in.command;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        mav_array_memcpy(packet1.name, packet_in.name, sizeof(char)*20);
        mav_array_memcpy(packet1.description, packet_in.description, sizeof(char)*229);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_HL_ACTION_LIST_ITEM_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_item_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_hl_action_list_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_item_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.index , packet1.command , packet1.name , packet1.description );
    mavlink_msg_hl_action_list_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_item_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.index , packet1.command , packet1.name , packet1.description );
    mavlink_msg_hl_action_list_item_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_hl_action_list_item_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_item_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.index , packet1.command , packet1.name , packet1.description );
    mavlink_msg_hl_action_list_item_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_hl_action_list_ack(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_HL_ACTION_LIST_ACK >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_hl_action_list_ack_t packet_in = {
        5,72,139
    };
    mavlink_hl_action_list_ack_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        packet1.type = packet_in.type;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_HL_ACTION_LIST_ACK_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_HL_ACTION_LIST_ACK_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_ack_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_hl_action_list_ack_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_ack_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.type );
    mavlink_msg_hl_action_list_ack_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_ack_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.type );
    mavlink_msg_hl_action_list_ack_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_hl_action_list_ack_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hl_action_list_ack_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.type );
    mavlink_msg_hl_action_list_ack_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_alarm_status(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ALARM_STATUS >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_alarm_status_t packet_in = {
        963497464,17443,17547,17651,163
    };
    mavlink_alarm_status_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.index = packet_in.index;
        packet1.errors_count = packet_in.errors_count;
        packet1.warns_count = packet_in.warns_count;
        packet1.status = packet_in.status;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ALARM_STATUS_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ALARM_STATUS_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_status_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_alarm_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_status_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.index , packet1.status , packet1.errors_count , packet1.warns_count );
    mavlink_msg_alarm_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_status_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.index , packet1.status , packet1.errors_count , packet1.warns_count );
    mavlink_msg_alarm_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_alarm_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_alarm_status_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.index , packet1.status , packet1.errors_count , packet1.warns_count );
    mavlink_msg_alarm_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_text_status(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_TEXT_STATUS >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_text_status_t packet_in = {
        5,"BCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRST"
    };
    mavlink_text_status_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.severity = packet_in.severity;
        
        mav_array_memcpy(packet1.text, packet_in.text, sizeof(char)*254);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_TEXT_STATUS_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_TEXT_STATUS_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_text_status_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_text_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_text_status_pack(system_id, component_id, &msg , packet1.severity , packet1.text );
    mavlink_msg_text_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_text_status_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.severity , packet1.text );
    mavlink_msg_text_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_text_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_text_status_send(MAVLINK_COMM_1 , packet1.severity , packet1.text );
    mavlink_msg_text_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_piloting(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_waypoint_list_count(system_id, component_id, last_msg);
    mavlink_test_waypoint_list_read(system_id, component_id, last_msg);
    mavlink_test_waypoint_list_item(system_id, component_id, last_msg);
    mavlink_test_waypoint_list_ack(system_id, component_id, last_msg);
    mavlink_test_waypoint_list_request(system_id, component_id, last_msg);
    mavlink_test_waypoint_list_set_current_item(system_id, component_id, last_msg);
    mavlink_test_waypoint_list_current_item(system_id, component_id, last_msg);
    mavlink_test_waypoint_list_item_reached(system_id, component_id, last_msg);
    mavlink_test_check_list_request(system_id, component_id, last_msg);
    mavlink_test_check_list_count(system_id, component_id, last_msg);
    mavlink_test_check_list_read(system_id, component_id, last_msg);
    mavlink_test_check_list_item(system_id, component_id, last_msg);
    mavlink_test_check_list_ack(system_id, component_id, last_msg);
    mavlink_test_alarm_list_request(system_id, component_id, last_msg);
    mavlink_test_alarm_list_count(system_id, component_id, last_msg);
    mavlink_test_alarm_list_read(system_id, component_id, last_msg);
    mavlink_test_alarm_list_item(system_id, component_id, last_msg);
    mavlink_test_alarm_list_ack(system_id, component_id, last_msg);
    mavlink_test_hl_action_list_request(system_id, component_id, last_msg);
    mavlink_test_hl_action_list_count(system_id, component_id, last_msg);
    mavlink_test_hl_action_list_read(system_id, component_id, last_msg);
    mavlink_test_hl_action_list_item(system_id, component_id, last_msg);
    mavlink_test_hl_action_list_ack(system_id, component_id, last_msg);
    mavlink_test_alarm_status(system_id, component_id, last_msg);
    mavlink_test_text_status(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // PILOTING_TESTSUITE_H
