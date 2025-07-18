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
 * @file core_mqtt_v5_property_builder.h
 * @brief Internal functions for building MQTT v5.0 properties.
 */
#ifndef CORE_MQTT_V5_PROPERTY_BUILDER_H
#define CORE_MQTT_V5_PROPERTY_BUILDER_H

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

#include "core_mqtt_v5_serializer.h"

/**
 * @brief Internal function to append a byte property to a property builder.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] propertyId Property identifier.
 * @param[in] value Byte value to append.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MqttPropBuilder_AppendByte( MqttPropBuilder_t * pPropBuilder,
                                         uint8_t propertyId,
                                         uint8_t value );

/**
 * @brief Internal function to append a two-byte integer property to a property builder.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] propertyId Property identifier.
 * @param[in] value Two-byte integer value to append.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MqttPropBuilder_AppendTwoByteInt( MqttPropBuilder_t * pPropBuilder,
                                               uint8_t propertyId,
                                               uint16_t value );

/**
 * @brief Internal function to append a four-byte integer property to a property builder.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] propertyId Property identifier.
 * @param[in] value Four-byte integer value to append.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MqttPropBuilder_AppendFourByteInt( MqttPropBuilder_t * pPropBuilder,
                                                uint8_t propertyId,
                                                uint32_t value );

/**
 * @brief Internal function to append a UTF-8 string property to a property builder.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] propertyId Property identifier.
 * @param[in] pString UTF-8 string to append.
 * @param[in] stringLength Length of the UTF-8 string.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MqttPropBuilder_AppendUtf8String( MqttPropBuilder_t * pPropBuilder,
                                               uint8_t propertyId,
                                               const char * pString,
                                               size_t stringLength );

/**
 * @brief Internal function to append a binary data property to a property builder.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] propertyId Property identifier.
 * @param[in] pData Binary data to append.
 * @param[in] dataLength Length of the binary data.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MqttPropBuilder_AppendBinaryData( MqttPropBuilder_t * pPropBuilder,
                                               uint8_t propertyId,
                                               const uint8_t * pData,
                                               size_t dataLength );

/**
 * @brief Internal function to append a user property to a property builder.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] pUserProperty User property to append.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MqttPropBuilder_AppendUserProperty( MqttPropBuilder_t * pPropBuilder,
                                                 const MQTTUserProperty * pUserProperty );

/**
 * @brief Internal function to get the size of the property section.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[out] pSize Size of the property section.
 *
 * @return #MQTTSuccess if the size was calculated successfully;
 * #MQTTBadParameter if invalid parameters are passed.
 */
MQTTStatus_t MqttPropBuilder_GetSize( const MqttPropBuilder_t * pPropBuilder,
                                      size_t * pSize );

/**
 * @brief Internal function to encode the property length.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[out] pBuffer Buffer to encode the property length into.
 * @param[in,out] pIndex Current index in the buffer.
 *
 * @return #MQTTSuccess if the length was encoded successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MqttPropBuilder_EncodeLength( const MqttPropBuilder_t * pPropBuilder,
                                           uint8_t * pBuffer,
                                           size_t * pIndex );

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_V5_PROPERTY_BUILDER_H */