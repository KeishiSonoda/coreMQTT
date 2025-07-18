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
 * @file core_mqtt_v5_connect_props.c
 * @brief Implementation of the CONNECT packet property functions for MQTT v5.0.
 */

/* Standard includes. */
#include <string.h>
#include <assert.h>

/* MQTT includes. */
#include "core_mqtt_v5_connect_props.h"
#include "core_mqtt_v5_property_ids.h"

/* Field bit definitions for CONNECT packet properties. */
#define CONNECT_FIELD_SESSION_EXPIRY        ( 1U << 0 )
#define CONNECT_FIELD_RECEIVE_MAXIMUM       ( 1U << 1 )
#define CONNECT_FIELD_MAX_PACKET_SIZE       ( 1U << 2 )
#define CONNECT_FIELD_TOPIC_ALIAS_MAX       ( 1U << 3 )
#define CONNECT_FIELD_REQUEST_RESP_INFO     ( 1U << 4 )
#define CONNECT_FIELD_REQUEST_PROBLEM_INFO  ( 1U << 5 )
#define CONNECT_FIELD_USER_PROPERTY         ( 1U << 6 )
#define CONNECT_FIELD_AUTH_METHOD           ( 1U << 7 )
#define CONNECT_FIELD_AUTH_DATA             ( 1U << 8 )

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_ConnSessionExpiry( MqttPropBuilder_t * pPropBuilder,
                                            uint32_t sessionExpiryInterval )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( pPropBuilder == NULL )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the Session Expiry Interval property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_CONNECT_SESSION_EXPIRY,
                                         &sessionExpiryInterval,
                                         sizeof( sessionExpiryInterval ),
                                         CONNECT_FIELD_SESSION_EXPIRY );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_ConnReceiveMax( MqttPropBuilder_t * pPropBuilder,
                                         uint16_t receiveMaximum )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( pPropBuilder == NULL )
    {
        status = MQTTBadParameter;
    }
    /* The Receive Maximum value must be greater than 0. */
    else if( receiveMaximum == 0 )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the Receive Maximum property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_CONNECT_RECEIVE_MAXIMUM,
                                         &receiveMaximum,
                                         sizeof( receiveMaximum ),
                                         CONNECT_FIELD_RECEIVE_MAXIMUM );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_ConnMaxPacketSize( MqttPropBuilder_t * pPropBuilder,
                                            uint32_t maxPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( pPropBuilder == NULL )
    {
        status = MQTTBadParameter;
    }
    /* The Maximum Packet Size value must be greater than 0. */
    else if( maxPacketSize == 0 )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the Maximum Packet Size property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_CONNECT_MAX_PACKET_SIZE,
                                         &maxPacketSize,
                                         sizeof( maxPacketSize ),
                                         CONNECT_FIELD_MAX_PACKET_SIZE );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_ConnTopicAliasMax( MqttPropBuilder_t * pPropBuilder,
                                            uint16_t topicAliasMaximum )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( pPropBuilder == NULL )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the Topic Alias Maximum property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_CONNECT_TOPIC_ALIAS_MAX,
                                         &topicAliasMaximum,
                                         sizeof( topicAliasMaximum ),
                                         CONNECT_FIELD_TOPIC_ALIAS_MAX );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_ConnRequestRespInfo( MqttPropBuilder_t * pPropBuilder,
                                              bool requestResponseInfo )
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
        value = requestResponseInfo ? 1 : 0;

        /* Encode the Request Response Information property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_CONNECT_REQUEST_RESP_INFO,
                                         &value,
                                         sizeof( value ),
                                         CONNECT_FIELD_REQUEST_RESP_INFO );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_ConnRequestProbInfo( MqttPropBuilder_t * pPropBuilder,
                                              bool requestProblemInfo )
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
        value = requestProblemInfo ? 1 : 0;

        /* Encode the Request Problem Information property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_CONNECT_REQUEST_PROBLEM_INFO,
                                         &value,
                                         sizeof( value ),
                                         CONNECT_FIELD_REQUEST_PROBLEM_INFO );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_ConnUserProps( MqttPropBuilder_t * pPropBuilder,
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
    else if( ( pPropBuilder->fieldsSet & CONNECT_FIELD_USER_PROPERTY ) != 0 )
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
            pPropBuilder->fieldsSet |= CONNECT_FIELD_USER_PROPERTY;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_ConnAuthMethod( MqttPropBuilder_t * pPropBuilder,
                                         const uint8_t * pAuthMethod,
                                         size_t authMethodLength )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pPropBuilder == NULL ) || ( pAuthMethod == NULL ) || ( authMethodLength == 0 ) )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the Authentication Method property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_CONNECT_AUTH_METHOD,
                                         pAuthMethod,
                                         authMethodLength,
                                         CONNECT_FIELD_AUTH_METHOD );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_ConnAuthData( MqttPropBuilder_t * pPropBuilder,
                                       const uint8_t * pAuthData,
                                       size_t authDataLength )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pPropBuilder == NULL ) || ( pAuthData == NULL ) || ( authDataLength == 0 ) )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the Authentication Data property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_CONNECT_AUTH_DATA,
                                         pAuthData,
                                         authDataLength,
                                         CONNECT_FIELD_AUTH_DATA );
    }

    return status;
}

/*-----------------------------------------------------------*/