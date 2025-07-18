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
 * @file core_mqtt_v5_publish_props.c
 * @brief Implementation of the PUBLISH packet property functions for MQTT v5.0.
 */

/* Standard includes. */
#include <string.h>
#include <assert.h>

/* MQTT includes. */
#include "core_mqtt_v5_publish_props.h"
#include "core_mqtt_v5_property_ids.h"

/* Field bit definitions for PUBLISH packet properties. */
#define PUBLISH_FIELD_PAYLOAD_FORMAT       ( 1U << 0 )
#define PUBLISH_FIELD_MESSAGE_EXPIRY       ( 1U << 1 )
#define PUBLISH_FIELD_TOPIC_ALIAS          ( 1U << 2 )
#define PUBLISH_FIELD_RESPONSE_TOPIC       ( 1U << 3 )
#define PUBLISH_FIELD_CORRELATION_DATA     ( 1U << 4 )
#define PUBLISH_FIELD_USER_PROPERTY        ( 1U << 5 )
#define PUBLISH_FIELD_CONTENT_TYPE         ( 1U << 6 )

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_PubPayloadFormat( MqttPropBuilder_t * pPropBuilder,
                                           bool payloadFormatIndicator )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t value = 0;

    /* Validate parameters. */
    if( pPropBuilder == NULL )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Convert boolean to byte value. */
        value = payloadFormatIndicator ? 1 : 0;

        /* Encode the Payload Format Indicator property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_PUBLISH_PAYLOAD_FORMAT,
                                         &value,
                                         sizeof( value ),
                                         PUBLISH_FIELD_PAYLOAD_FORMAT );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_PubMessageExpiry( MqttPropBuilder_t * pPropBuilder,
                                           uint32_t messageExpiryInterval )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( pPropBuilder == NULL )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the Message Expiry Interval property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_PUBLISH_MESSAGE_EXPIRY,
                                         &messageExpiryInterval,
                                         sizeof( messageExpiryInterval ),
                                         PUBLISH_FIELD_MESSAGE_EXPIRY );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_PubTopicAlias( MqttPropBuilder_t * pPropBuilder,
                                        uint16_t topicAlias )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( pPropBuilder == NULL )
    {
        status = MQTTBadParameter;
    }
    /* The Topic Alias value must be greater than 0. */
    else if( topicAlias == 0 )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the Topic Alias property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_PUBLISH_TOPIC_ALIAS,
                                         &topicAlias,
                                         sizeof( topicAlias ),
                                         PUBLISH_FIELD_TOPIC_ALIAS );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_PubResponseTopic( MqttPropBuilder_t * pPropBuilder,
                                           const uint8_t * pResponseTopic,
                                           size_t responseTopicLength )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pPropBuilder == NULL ) || ( pResponseTopic == NULL ) || ( responseTopicLength == 0 ) )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the Response Topic property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_PUBLISH_RESPONSE_TOPIC,
                                         pResponseTopic,
                                         responseTopicLength,
                                         PUBLISH_FIELD_RESPONSE_TOPIC );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_PubCorrelationData( MqttPropBuilder_t * pPropBuilder,
                                             const uint8_t * pCorrelationData,
                                             size_t correlationDataLength )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pPropBuilder == NULL ) || ( pCorrelationData == NULL ) || ( correlationDataLength == 0 ) )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the Correlation Data property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_PUBLISH_CORRELATION_DATA,
                                         pCorrelationData,
                                         correlationDataLength,
                                         PUBLISH_FIELD_CORRELATION_DATA );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_PubUserProps( MqttPropBuilder_t * pPropBuilder,
                                       const MQTTUserProperty * pUserProperties,
                                       size_t userPropertiesCount )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t i;

    /* Validate parameters. */
    if( ( pPropBuilder == NULL ) || ( pUserProperties == NULL ) || ( userPropertiesCount == 0 ) )
    {
        status = MQTTBadParameter;
    }
    /* Check if user properties have already been added. */
    else if( ( pPropBuilder->fieldsSet & PUBLISH_FIELD_USER_PROPERTY ) != 0 )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode each user property. */
        for( i = 0; i < userPropertiesCount; i++ )
        {
            /* Validate the user property. */
            if( !MQTT_UserPropertyIsValid( &pUserProperties[ i ] ) )
            {
                status = MQTTBadParameter;
                break;
            }

            /* Encode the User Property identifier. */
            status = MQTT_PropBuilderEncode( pPropBuilder,
                                             MQTT_PROPERTY_USER_PROPERTY,
                                             NULL,
                                             0,
                                             0 );

            if( status != MQTTSuccess )
            {
                break;
            }

            /* Encode the key. */
            status = MQTT_PropBuilderEncode( pPropBuilder,
                                             0,
                                             pUserProperties[ i ].key,
                                             pUserProperties[ i ].keyLength,
                                             0 );

            if( status != MQTTSuccess )
            {
                break;
            }

            /* Encode the value. */
            status = MQTT_PropBuilderEncode( pPropBuilder,
                                             0,
                                             pUserProperties[ i ].value,
                                             pUserProperties[ i ].valueLength,
                                             0 );

            if( status != MQTTSuccess )
            {
                break;
            }
        }

        /* If all user properties were encoded successfully, mark the field as set. */
        if( status == MQTTSuccess )
        {
            pPropBuilder->fieldsSet |= PUBLISH_FIELD_USER_PROPERTY;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_PubContentType( MqttPropBuilder_t * pPropBuilder,
                                         const uint8_t * pContentType,
                                         size_t contentTypeLength )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pPropBuilder == NULL ) || ( pContentType == NULL ) || ( contentTypeLength == 0 ) )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the Content Type property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_PUBLISH_CONTENT_TYPE,
                                         pContentType,
                                         contentTypeLength,
                                         PUBLISH_FIELD_CONTENT_TYPE );
    }

    return status;
}

/*-----------------------------------------------------------*/