/*
 * coreMQTT <DEVELOPMENT BRANCH>
 * Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file core_mqtt_v5_property_ids.h
 * @brief Property identifier constants for MQTT v5.0.
 */
#ifndef CORE_MQTT_V5_PROPERTY_IDS_H
#define CORE_MQTT_V5_PROPERTY_IDS_H

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

/* Standard includes. */
#include <stdint.h>

/**
 * @ingroup mqtt_constants
 * @brief MQTT v5.0 property identifiers.
 *
 * These constants define the property identifiers for MQTT v5.0 properties.
 * The properties are grouped by the packet types they can be used with.
 */

/**
 * @brief Property identifiers that can be used in multiple packet types.
 */
/* User Property can be used in all packet types that support properties */
#define MQTT_PROPERTY_USER_PROPERTY                 ( ( uint8_t ) 0x26U )
/* Reason String can be used in CONNACK, PUBACK, PUBREC, PUBREL, PUBCOMP, SUBACK, UNSUBACK, DISCONNECT, AUTH */
#define MQTT_PROPERTY_REASON_STRING                 ( ( uint8_t ) 0x1FU )

/**
 * @brief Property identifiers for CONNECT packet.
 */
#define MQTT_PROPERTY_CONNECT_SESSION_EXPIRY        ( ( uint8_t ) 0x11U )
#define MQTT_PROPERTY_CONNECT_RECEIVE_MAXIMUM       ( ( uint8_t ) 0x21U )
#define MQTT_PROPERTY_CONNECT_MAX_PACKET_SIZE       ( ( uint8_t ) 0x27U )
#define MQTT_PROPERTY_CONNECT_TOPIC_ALIAS_MAX       ( ( uint8_t ) 0x22U )
#define MQTT_PROPERTY_CONNECT_REQUEST_RESP_INFO     ( ( uint8_t ) 0x19U )
#define MQTT_PROPERTY_CONNECT_REQUEST_PROBLEM_INFO  ( ( uint8_t ) 0x17U )
#define MQTT_PROPERTY_CONNECT_AUTH_METHOD           ( ( uint8_t ) 0x15U )
#define MQTT_PROPERTY_CONNECT_AUTH_DATA             ( ( uint8_t ) 0x16U )

/**
 * @brief Property identifiers for CONNACK packet.
 */
#define MQTT_PROPERTY_CONNACK_SESSION_EXPIRY        ( ( uint8_t ) 0x11U )
#define MQTT_PROPERTY_CONNACK_RECEIVE_MAXIMUM       ( ( uint8_t ) 0x21U )
#define MQTT_PROPERTY_CONNACK_MAX_QOS               ( ( uint8_t ) 0x24U )
#define MQTT_PROPERTY_CONNACK_RETAIN_AVAILABLE      ( ( uint8_t ) 0x25U )
#define MQTT_PROPERTY_CONNACK_MAX_PACKET_SIZE       ( ( uint8_t ) 0x27U )
#define MQTT_PROPERTY_CONNACK_ASSIGNED_CLIENT_ID    ( ( uint8_t ) 0x12U )
#define MQTT_PROPERTY_CONNACK_TOPIC_ALIAS_MAX       ( ( uint8_t ) 0x22U )
#define MQTT_PROPERTY_CONNACK_RESPONSE_INFO         ( ( uint8_t ) 0x1AU )
#define MQTT_PROPERTY_CONNACK_SERVER_REFERENCE      ( ( uint8_t ) 0x1CU )
#define MQTT_PROPERTY_CONNACK_AUTH_METHOD           ( ( uint8_t ) 0x15U )
#define MQTT_PROPERTY_CONNACK_AUTH_DATA             ( ( uint8_t ) 0x16U )

/**
 * @brief Property identifiers for PUBLISH packet.
 */
#define MQTT_PROPERTY_PUBLISH_PAYLOAD_FORMAT        ( ( uint8_t ) 0x01U )
#define MQTT_PROPERTY_PUBLISH_MESSAGE_EXPIRY        ( ( uint8_t ) 0x02U )
#define MQTT_PROPERTY_PUBLISH_CONTENT_TYPE          ( ( uint8_t ) 0x03U )
#define MQTT_PROPERTY_PUBLISH_RESPONSE_TOPIC        ( ( uint8_t ) 0x08U )
#define MQTT_PROPERTY_PUBLISH_CORRELATION_DATA      ( ( uint8_t ) 0x09U )
#define MQTT_PROPERTY_PUBLISH_SUBSCRIPTION_ID       ( ( uint8_t ) 0x0BU )
#define MQTT_PROPERTY_PUBLISH_TOPIC_ALIAS           ( ( uint8_t ) 0x23U )

/**
 * @brief Property identifiers for PUBACK, PUBREC, PUBREL, PUBCOMP packets.
 */
/* These packets use the same property identifiers */
#define MQTT_PROPERTY_PUBACK_REASON_STRING          MQTT_PROPERTY_REASON_STRING
#define MQTT_PROPERTY_PUBACK_USER_PROPERTY          MQTT_PROPERTY_USER_PROPERTY

#define MQTT_PROPERTY_PUBREC_REASON_STRING          MQTT_PROPERTY_REASON_STRING
#define MQTT_PROPERTY_PUBREC_USER_PROPERTY          MQTT_PROPERTY_USER_PROPERTY

#define MQTT_PROPERTY_PUBREL_REASON_STRING          MQTT_PROPERTY_REASON_STRING
#define MQTT_PROPERTY_PUBREL_USER_PROPERTY          MQTT_PROPERTY_USER_PROPERTY

#define MQTT_PROPERTY_PUBCOMP_REASON_STRING         MQTT_PROPERTY_REASON_STRING
#define MQTT_PROPERTY_PUBCOMP_USER_PROPERTY         MQTT_PROPERTY_USER_PROPERTY

/**
 * @brief Property identifiers for SUBSCRIBE packet.
 */
#define MQTT_PROPERTY_SUBSCRIBE_SUBSCRIPTION_ID     ( ( uint8_t ) 0x0BU )
#define MQTT_PROPERTY_SUBSCRIBE_USER_PROPERTY       MQTT_PROPERTY_USER_PROPERTY

/**
 * @brief Property identifiers for SUBACK packet.
 */
#define MQTT_PROPERTY_SUBACK_REASON_STRING          MQTT_PROPERTY_REASON_STRING
#define MQTT_PROPERTY_SUBACK_USER_PROPERTY          MQTT_PROPERTY_USER_PROPERTY

/**
 * @brief Property identifiers for UNSUBSCRIBE packet.
 */
#define MQTT_PROPERTY_UNSUBSCRIBE_USER_PROPERTY     MQTT_PROPERTY_USER_PROPERTY

/**
 * @brief Property identifiers for UNSUBACK packet.
 */
#define MQTT_PROPERTY_UNSUBACK_REASON_STRING        MQTT_PROPERTY_REASON_STRING
#define MQTT_PROPERTY_UNSUBACK_USER_PROPERTY        MQTT_PROPERTY_USER_PROPERTY

/**
 * @brief Property identifiers for DISCONNECT packet.
 */
#define MQTT_PROPERTY_DISCONNECT_SESSION_EXPIRY     ( ( uint8_t ) 0x11U )
#define MQTT_PROPERTY_DISCONNECT_REASON_STRING      MQTT_PROPERTY_REASON_STRING
#define MQTT_PROPERTY_DISCONNECT_SERVER_REFERENCE   ( ( uint8_t ) 0x1CU )
#define MQTT_PROPERTY_DISCONNECT_USER_PROPERTY      MQTT_PROPERTY_USER_PROPERTY

/**
 * @brief Property identifiers for AUTH packet.
 */
#define MQTT_PROPERTY_AUTH_AUTHENTICATION_METHOD    ( ( uint8_t ) 0x15U )
#define MQTT_PROPERTY_AUTH_AUTHENTICATION_DATA      ( ( uint8_t ) 0x16U )
#define MQTT_PROPERTY_AUTH_REASON_STRING            MQTT_PROPERTY_REASON_STRING
#define MQTT_PROPERTY_AUTH_USER_PROPERTY            MQTT_PROPERTY_USER_PROPERTY

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_V5_PROPERTY_IDS_H */