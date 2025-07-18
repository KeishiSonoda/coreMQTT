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
 * @file core_mqtt_v5_prop_builder.h
 * @brief User-facing functions and structure for MQTT v5.0 property builder.
 */
#ifndef CORE_MQTT_V5_PROP_BUILDER_H
#define CORE_MQTT_V5_PROP_BUILDER_H

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
#include "core_mqtt_v5_user_property.h"

/**
 * @ingroup mqtt_struct_types
 * @brief MQTT v5.0 Property Builder structure.
 *
 * This structure is used to build MQTT v5.0 properties for CONNECT, PUBLISH,
 * and other packet types. It provides a type-safe and memory-efficient way
 * to construct MQTT v5.0 properties.
 */
typedef struct MqttPropBuilder
{
    /**
     * @brief Pointer to the buffer where properties will be serialized.
     */
    uint8_t * pBuffer;

    /**
     * @brief Length of the buffer.
     */
    size_t bufferLength;

    /**
     * @brief Current position in the buffer.
     */
    size_t currentIndex;

    /**
     * @brief Bitmask of fields that have been set.
     * This is used to prevent duplicate properties.
     */
    uint32_t fieldsSet;
} MqttPropBuilder_t;

/**
 * @brief Initialize a property builder with a buffer.
 *
 * This function initializes a property builder with a user-provided buffer.
 * The buffer will be used to serialize MQTT v5.0 properties.
 *
 * @param[out] pPropBuilder Property builder to initialize.
 * @param[in] pBuffer Buffer to use for serializing properties.
 * @param[in] bufferLength Length of the buffer.
 *
 * @return #MQTTSuccess if initialization was successful;
 * #MQTTBadParameter if invalid parameters are passed.
 */
MQTTStatus_t MQTT_PropBuilderInit( MqttPropBuilder_t * pPropBuilder,
                                   uint8_t * pBuffer,
                                   size_t bufferLength );

/**
 * @brief Get the size of the serialized properties.
 *
 * This function returns the size of the serialized properties in the buffer.
 *
 * @param[in] pPropBuilder Property builder to get size from.
 * @param[out] pSize Pointer to store the size.
 *
 * @return #MQTTSuccess if the size was retrieved successfully;
 * #MQTTBadParameter if invalid parameters are passed.
 */
MQTTStatus_t MQTT_PropBuilderGetSize( const MqttPropBuilder_t * pPropBuilder,
                                      size_t * pSize );

/**
 * @brief Check if the property builder is valid.
 *
 * This function validates that the property builder has a valid buffer and
 * that the current index is within the buffer bounds.
 *
 * @param[in] pPropBuilder Property builder to validate.
 *
 * @return true if the property builder is valid, false otherwise.
 */
bool MQTT_PropBuilderIsValid( const MqttPropBuilder_t * pPropBuilder );

/**
 * @brief Reset a property builder.
 *
 * This function resets a property builder to its initial state, clearing
 * all properties that have been added.
 *
 * @param[in,out] pPropBuilder Property builder to reset.
 *
 * @return #MQTTSuccess if the reset was successful;
 * #MQTTBadParameter if invalid parameters are passed.
 */
MQTTStatus_t MQTT_PropBuilderReset( MqttPropBuilder_t * pPropBuilder );

/**
 * @brief Internal function to encode a property identifier and value.
 *
 * This function is used internally by the property builder to encode
 * a property identifier and value into the buffer.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] propertyId Property identifier to encode.
 * @param[in] pValue Pointer to the value to encode.
 * @param[in] valueLength Length of the value.
 * @param[in] fieldBit Bit position in the fieldsSet bitmask.
 *
 * @return #MQTTSuccess if the property was encoded successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropBuilderEncode( MqttPropBuilder_t * pPropBuilder,
                                     uint8_t propertyId,
                                     const void * pValue,
                                     size_t valueLength,
                                     uint32_t fieldBit );

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_V5_PROP_BUILDER_H */