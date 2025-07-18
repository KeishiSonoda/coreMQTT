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
 * @file core_mqtt_v5_connect_props.h
 * @brief User-facing functions for MQTT v5.0 CONNECT packet properties.
 */
#ifndef CORE_MQTT_V5_CONNECT_PROPS_H
#define CORE_MQTT_V5_CONNECT_PROPS_H

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

/* Standard includes. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* MQTT includes. */
#include "core_mqtt_serializer.h"
#include "core_mqtt_v5_prop_builder.h"
#include "core_mqtt_v5_user_property.h"

/**
 * @brief Add Session Expiry Interval property to CONNECT packet.
 *
 * This function adds the Session Expiry Interval property to a CONNECT packet.
 * The Session Expiry Interval is the time in seconds after which the server
 * will expire the session if the client disconnects.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] sessionExpiryInterval Session expiry interval in seconds.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_ConnSessionExpiry( MqttPropBuilder_t * pPropBuilder,
                                            uint32_t sessionExpiryInterval );

/**
 * @brief Add Receive Maximum property to CONNECT packet.
 *
 * This function adds the Receive Maximum property to a CONNECT packet.
 * The Receive Maximum is the maximum number of QoS 1 and QoS 2 publications
 * that the client is willing to process concurrently.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] receiveMaximum Receive maximum value.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_ConnReceiveMax( MqttPropBuilder_t * pPropBuilder,
                                         uint16_t receiveMaximum );

/**
 * @brief Add Maximum Packet Size property to CONNECT packet.
 *
 * This function adds the Maximum Packet Size property to a CONNECT packet.
 * The Maximum Packet Size is the maximum packet size the client is willing
 * to accept from the server.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] maxPacketSize Maximum packet size in bytes.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_ConnMaxPacketSize( MqttPropBuilder_t * pPropBuilder,
                                            uint32_t maxPacketSize );

/**
 * @brief Add Topic Alias Maximum property to CONNECT packet.
 *
 * This function adds the Topic Alias Maximum property to a CONNECT packet.
 * The Topic Alias Maximum is the highest value that the client will accept
 * as a Topic Alias sent by the server.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] topicAliasMaximum Topic alias maximum value.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_ConnTopicAliasMax( MqttPropBuilder_t * pPropBuilder,
                                            uint16_t topicAliasMaximum );

/**
 * @brief Add Request Response Information property to CONNECT packet.
 *
 * This function adds the Request Response Information property to a CONNECT packet.
 * The Request Response Information is a boolean value that indicates whether the
 * client wants the server to return response information in the CONNACK packet.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] requestResponseInfo Request response information flag.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_ConnRequestRespInfo( MqttPropBuilder_t * pPropBuilder,
                                              bool requestResponseInfo );

/**
 * @brief Add Request Problem Information property to CONNECT packet.
 *
 * This function adds the Request Problem Information property to a CONNECT packet.
 * The Request Problem Information is a boolean value that indicates whether the
 * client wants the server to return additional diagnostic information in case
 * of an error.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] requestProblemInfo Request problem information flag.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_ConnRequestProbInfo( MqttPropBuilder_t * pPropBuilder,
                                              bool requestProblemInfo );

/**
 * @brief Add User Properties to CONNECT packet.
 *
 * This function adds User Properties to a CONNECT packet.
 * User Properties are user-defined key-value pairs that can be used to
 * send application-specific information.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] pUserProperties Array of user properties.
 * @param[in] userPropertiesCount Number of user properties in the array.
 *
 * @return #MQTTSuccess if the properties were added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_ConnUserProps( MqttPropBuilder_t * pPropBuilder,
                                        const MQTTUserProperty * pUserProperties,
                                        size_t userPropertiesCount );

/**
 * @brief Add Authentication Method property to CONNECT packet.
 *
 * This function adds the Authentication Method property to a CONNECT packet.
 * The Authentication Method is a string that specifies the authentication
 * method to use.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] pAuthMethod Authentication method string.
 * @param[in] authMethodLength Length of the authentication method string.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_ConnAuthMethod( MqttPropBuilder_t * pPropBuilder,
                                         const uint8_t * pAuthMethod,
                                         size_t authMethodLength );

/**
 * @brief Add Authentication Data property to CONNECT packet.
 *
 * This function adds the Authentication Data property to a CONNECT packet.
 * The Authentication Data is binary data that contains authentication data.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] pAuthData Authentication data.
 * @param[in] authDataLength Length of the authentication data.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_ConnAuthData( MqttPropBuilder_t * pPropBuilder,
                                       const uint8_t * pAuthData,
                                       size_t authDataLength );

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_V5_CONNECT_PROPS_H */