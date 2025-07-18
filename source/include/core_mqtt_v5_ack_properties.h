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
 * @file core_mqtt_v5_ack_properties.h
 * @brief User-facing functions and structure for MQTT v5.0 ACK properties.
 */
#ifndef CORE_MQTT_V5_ACK_PROPERTIES_H
#define CORE_MQTT_V5_ACK_PROPERTIES_H

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
#include "core_mqtt_v5_prop_builder.h"

/**
 * @ingroup mqtt_struct_types
 * @brief MQTT v5.0 ACK Properties structure.
 */
typedef struct MQTTAckProperties
{
    /**
     * @brief Reason string for the ACK packet.
     */
    uint8_t * pReasonString;

    /**
     * @brief Length of the reason string.
     */
    size_t reasonStringLength;

    /**
     * @brief Array of user properties.
     */
    MQTTUserProperty * pUserProperties;

    /**
     * @brief Number of user properties in the array.
     */
    size_t userPropertiesCount;

    /**
     * @brief Array of reason codes for SUBACK and UNSUBACK packets.
     */
    uint8_t * pReasonCodes;

    /**
     * @brief Number of reason codes in the array.
     */
    size_t reasonCodesCount;
} MQTTAckProperties_t;

/**
 * @brief Initialize an ACK properties structure.
 *
 * @param[out] pAckProperties ACK properties structure to initialize.
 *
 * @return #MQTTSuccess if initialization was successful;
 * #MQTTBadParameter if invalid parameters are passed.
 */
MQTTStatus_t MQTT_AckPropertiesInit( MQTTAckProperties_t * pAckProperties );

/**
 * @brief Set the reason string in an ACK properties structure.
 *
 * @param[out] pAckProperties ACK properties structure to update.
 * @param[in] pReasonString Reason string to set.
 * @param[in] reasonStringLength Length of the reason string.
 *
 * @return #MQTTSuccess if the reason string was set successfully;
 * #MQTTBadParameter if invalid parameters are passed.
 */
MQTTStatus_t MQTT_AckPropertiesSetReasonString( MQTTAckProperties_t * pAckProperties,
                                               uint8_t * pReasonString,
                                               size_t reasonStringLength );

/**
 * @brief Set the user properties in an ACK properties structure.
 *
 * @param[out] pAckProperties ACK properties structure to update.
 * @param[in] pUserProperties Array of user properties to set.
 * @param[in] userPropertiesCount Number of user properties in the array.
 *
 * @return #MQTTSuccess if the user properties were set successfully;
 * #MQTTBadParameter if invalid parameters are passed.
 */
MQTTStatus_t MQTT_AckPropertiesSetUserProperties( MQTTAckProperties_t * pAckProperties,
                                                 MQTTUserProperty * pUserProperties,
                                                 size_t userPropertiesCount );

/**
 * @brief Set the reason codes in an ACK properties structure (for SUBACK and UNSUBACK packets).
 *
 * @param[out] pAckProperties ACK properties structure to update.
 * @param[in] pReasonCodes Array of reason codes to set.
 * @param[in] reasonCodesCount Number of reason codes in the array.
 *
 * @return #MQTTSuccess if the reason codes were set successfully;
 * #MQTTBadParameter if invalid parameters are passed.
 */
MQTTStatus_t MQTT_AckPropertiesSetReasonCodes( MQTTAckProperties_t * pAckProperties,
                                              uint8_t * pReasonCodes,
                                              size_t reasonCodesCount );

/**
 * @brief Check if an ACK properties structure is valid.
 *
 * This function validates that the ACK properties structure has valid fields.
 *
 * @param[in] pAckProperties ACK properties structure to validate.
 *
 * @return true if the ACK properties structure is valid, false otherwise.
 */
bool MQTT_AckPropertiesIsValid( const MQTTAckProperties_t * pAckProperties );

/**
 * @brief Calculate the size needed to serialize the ACK properties.
 *
 * @param[in] pAckProperties ACK properties structure to calculate size for.
 * @param[out] pSize Pointer to store the calculated size.
 *
 * @return #MQTTSuccess if the size was calculated successfully;
 * #MQTTBadParameter if invalid parameters are passed.
 */
MQTTStatus_t MQTT_AckPropertiesGetSize( const MQTTAckProperties_t * pAckProperties,
                                       size_t * pSize );

/**
 * @brief Add Reason String property to ACK packet.
 *
 * This function adds the Reason String property to an ACK packet.
 * The Reason String is a UTF-8 encoded string that provides additional
 * information about the result of the request.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] pReasonString Reason string.
 * @param[in] reasonStringLength Length of the reason string.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTT_PropAdd_PubAckReasonString( MqttPropBuilder_t * pPropBuilder,
                                             const uint8_t * pReasonString,
                                             size_t reasonStringLength );

/**
 * @brief Add User Properties to ACK packet.
 *
 * This function adds User Properties to an ACK packet.
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
MQTTStatus_t MQTT_PropAdd_PubAckUserProperty( MqttPropBuilder_t * pPropBuilder,
                                             const MQTTUserProperty * pUserProperties,
                                             size_t userPropertiesCount );

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_V5_ACK_PROPERTIES_H */