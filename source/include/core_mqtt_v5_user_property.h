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
 * @file core_mqtt_v5_user_property.h
 * @brief User-facing functions and structure for MQTT v5.0 user properties.
 */
#ifndef CORE_MQTT_V5_USER_PROPERTY_H
#define CORE_MQTT_V5_USER_PROPERTY_H

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

/**
 * @ingroup mqtt_struct_types
 * @brief MQTT v5.0 User Property structure.
 */
typedef struct MQTTUserProperty
{
    /**
     * @brief Key of the user property.
     */
    uint8_t * key;

    /**
     * @brief Length of the key.
     */
    size_t keyLength;

    /**
     * @brief Value of the user property.
     */
    uint8_t * value;

    /**
     * @brief Length of the value.
     */
    size_t valueLength;
} MQTTUserProperty;

/**
 * @brief Initialize a user property with key and value.
 *
 * @param[out] pUserProperty User property to initialize.
 * @param[in] pKey Key string for the user property.
 * @param[in] keyLength Length of the key string.
 * @param[in] pValue Value string for the user property.
 * @param[in] valueLength Length of the value string.
 *
 * @return #MQTTSuccess if initialization was successful;
 * #MQTTBadParameter if invalid parameters are passed.
 */
MQTTStatus_t MQTT_UserPropertyInit( MQTTUserProperty * pUserProperty,
                                    uint8_t * pKey,
                                    size_t keyLength,
                                    uint8_t * pValue,
                                    size_t valueLength );

/**
 * @brief Check if a user property is valid.
 *
 * This function validates that the user property has non-NULL key and value
 * pointers and non-zero lengths.
 *
 * @param[in] pUserProperty User property to validate.
 *
 * @return true if the user property is valid, false otherwise.
 */
bool MQTT_UserPropertyIsValid( const MQTTUserProperty * pUserProperty );

/**
 * @brief Calculate the size needed to serialize a user property.
 *
 * @param[in] pUserProperty User property to calculate size for.
 * @param[out] pSize Pointer to store the calculated size.
 *
 * @return #MQTTSuccess if the size was calculated successfully;
 * #MQTTBadParameter if invalid parameters are passed.
 */
MQTTStatus_t MQTT_UserPropertyGetSize( const MQTTUserProperty * pUserProperty,
                                       size_t * pSize );

/**
 * @brief Compare two user properties for equality.
 *
 * This function compares the key and value of two user properties.
 *
 * @param[in] pUserProperty1 First user property to compare.
 * @param[in] pUserProperty2 Second user property to compare.
 *
 * @return true if the user properties are equal, false otherwise.
 */
bool MQTT_UserPropertyIsEqual( const MQTTUserProperty * pUserProperty1,
                               const MQTTUserProperty * pUserProperty2 );

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_V5_USER_PROPERTY_H */