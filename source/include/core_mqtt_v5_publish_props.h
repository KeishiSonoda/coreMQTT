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
 * @file core_mqtt_v5_publish_props.h
 * @brief User-facing functions for MQTT v5.0 PUBLISH packet properties.
 */
#ifndef CORE_MQTT_V5_PUBLISH_PROPS_H
#define CORE_MQTT_V5_PUBLISH_PROPS_H

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
 * @brief Add Payload Format Indicator property to PUBLISH packet.
 *
 * This function adds the Payload Format Indicator property to a PUBLISH packet.
 * The Payload Format Indicator is a boolean value that indicates whether the
 * payload is formatted as UTF-8 encoded character data.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] payloadFormatIndicator Payload format indicator flag.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_PubPayloadFormat( MqttPropBuilder_t * pPropBuilder,
                                           bool payloadFormatIndicator );

/**
 * @brief Add Message Expiry Interval property to PUBLISH packet.
 *
 * This function adds the Message Expiry Interval property to a PUBLISH packet.
 * The Message Expiry Interval is the time in seconds after which the server
 * should delete the message if it has not been delivered to a matching subscriber.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] messageExpiryInterval Message expiry interval in seconds.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_PubMessageExpiry( MqttPropBuilder_t * pPropBuilder,
                                           uint32_t messageExpiryInterval );

/**
 * @brief Add Topic Alias property to PUBLISH packet.
 *
 * This function adds the Topic Alias property to a PUBLISH packet.
 * The Topic Alias is an integer value that is used to identify the topic
 * instead of using the topic name.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] topicAlias Topic alias value.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_PubTopicAlias( MqttPropBuilder_t * pPropBuilder,
                                        uint16_t topicAlias );

/**
 * @brief Add Response Topic property to PUBLISH packet.
 *
 * This function adds the Response Topic property to a PUBLISH packet.
 * The Response Topic is a UTF-8 encoded string that is used as the topic name
 * for a response message.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] pResponseTopic Response topic string.
 * @param[in] responseTopicLength Length of the response topic string.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_PubResponseTopic( MqttPropBuilder_t * pPropBuilder,
                                           const uint8_t * pResponseTopic,
                                           size_t responseTopicLength );

/**
 * @brief Add Correlation Data property to PUBLISH packet.
 *
 * This function adds the Correlation Data property to a PUBLISH packet.
 * The Correlation Data is binary data that is used by the sender of the
 * request message to identify which request the response message is for.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] pCorrelationData Correlation data.
 * @param[in] correlationDataLength Length of the correlation data.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_PubCorrelationData( MqttPropBuilder_t * pPropBuilder,
                                             const uint8_t * pCorrelationData,
                                             size_t correlationDataLength );

/**
 * @brief Add User Properties to PUBLISH packet.
 *
 * This function adds User Properties to a PUBLISH packet.
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
MQTTStatus_t MQTT_PropAdd_PubUserProps( MqttPropBuilder_t * pPropBuilder,
                                       const MQTTUserProperty * pUserProperties,
                                       size_t userPropertiesCount );

/**
 * @brief Add Content Type property to PUBLISH packet.
 *
 * This function adds the Content Type property to a PUBLISH packet.
 * The Content Type is a UTF-8 encoded string that describes the content
 * of the payload.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] pContentType Content type string.
 * @param[in] contentTypeLength Length of the content type string.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_PubContentType( MqttPropBuilder_t * pPropBuilder,
                                         const uint8_t * pContentType,
                                         size_t contentTypeLength );

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_V5_PUBLISH_PROPS_H */