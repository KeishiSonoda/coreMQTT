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
 * @file core_mqtt_v5_ack_properties.c
 * @brief Implementation of the ACK properties functions for MQTT v5.0.
 */

/* Standard includes. */
#include <string.h>
#include <assert.h>

/* MQTT includes. */
#include "core_mqtt_v5_ack_properties.h"
#include "core_mqtt_v5_property_ids.h"

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_AckPropertiesInit( MQTTAckProperties_t * pAckProperties )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( pAckProperties == NULL )
    {
        LogError( ( "Invalid parameter: pAckProperties=%p",
                    ( void * ) pAckProperties ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Initialize all fields to zero/NULL. */
        memset( pAckProperties, 0, sizeof( MQTTAckProperties_t ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_AckPropertiesSetReasonString( MQTTAckProperties_t * pAckProperties,
                                               uint8_t * pReasonString,
                                               size_t reasonStringLength )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pAckProperties == NULL ) || ( pReasonString == NULL ) || ( reasonStringLength == 0 ) )
    {
        LogError( ( "Invalid parameter: pAckProperties=%p, pReasonString=%p, reasonStringLength=%lu",
                    ( void * ) pAckProperties, ( void * ) pReasonString, ( unsigned long ) reasonStringLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        pAckProperties->pReasonString = pReasonString;
        pAckProperties->reasonStringLength = reasonStringLength;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_AckPropertiesSetUserProperties( MQTTAckProperties_t * pAckProperties,
                                                 MQTTUserProperty * pUserProperties,
                                                 size_t userPropertiesCount )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t i;

    /* Validate parameters. */
    if( ( pAckProperties == NULL ) || ( pUserProperties == NULL ) || ( userPropertiesCount == 0 ) )
    {
        LogError( ( "Invalid parameter: pAckProperties=%p, pUserProperties=%p, userPropertiesCount=%lu",
                    ( void * ) pAckProperties, ( void * ) pUserProperties, ( unsigned long ) userPropertiesCount ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Validate each user property. */
        for( i = 0; i < userPropertiesCount; i++ )
        {
            if( !MQTT_UserPropertyIsValid( &pUserProperties[ i ] ) )
            {
                LogError( ( "User property at index %lu is invalid", ( unsigned long ) i ) );
                status = MQTTBadParameter;
                break;
            }
        }

        if( status == MQTTSuccess )
        {
            pAckProperties->pUserProperties = pUserProperties;
            pAckProperties->userPropertiesCount = userPropertiesCount;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_AckPropertiesSetReasonCodes( MQTTAckProperties_t * pAckProperties,
                                              uint8_t * pReasonCodes,
                                              size_t reasonCodesCount )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pAckProperties == NULL ) || ( pReasonCodes == NULL ) || ( reasonCodesCount == 0 ) )
    {
        LogError( ( "Invalid parameter: pAckProperties=%p, pReasonCodes=%p, reasonCodesCount=%lu",
                    ( void * ) pAckProperties, ( void * ) pReasonCodes, ( unsigned long ) reasonCodesCount ) );
        status = MQTTBadParameter;
    }
    else
    {
        pAckProperties->pReasonCodes = pReasonCodes;
        pAckProperties->reasonCodesCount = reasonCodesCount;
    }

    return status;
}

/*-----------------------------------------------------------*/

bool MQTT_AckPropertiesIsValid( const MQTTAckProperties_t * pAckProperties )
{
    bool isValid = false;
    size_t i;

    if( pAckProperties != NULL )
    {
        /* The structure is valid if it has at least one of the following:
         * - A reason string
         * - User properties
         * - Reason codes
         */
        if( ( pAckProperties->pReasonString != NULL && pAckProperties->reasonStringLength > 0 ) ||
            ( pAckProperties->pUserProperties != NULL && pAckProperties->userPropertiesCount > 0 ) ||
            ( pAckProperties->pReasonCodes != NULL && pAckProperties->reasonCodesCount > 0 ) )
        {
            isValid = true;

            /* If there are user properties, validate each one. */
            if( pAckProperties->pUserProperties != NULL && pAckProperties->userPropertiesCount > 0 )
            {
                for( i = 0; i < pAckProperties->userPropertiesCount; i++ )
                {
                    if( !MQTT_UserPropertyIsValid( &pAckProperties->pUserProperties[ i ] ) )
                    {
                        isValid = false;
                        break;
                    }
                }
            }
        }
    }

    return isValid;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_AckPropertiesGetSize( const MQTTAckProperties_t * pAckProperties,
                                       size_t * pSize )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t size = 0;
    size_t i;
    size_t userPropertySize;

    /* Validate parameters. */
    if( ( pAckProperties == NULL ) || ( pSize == NULL ) )
    {
        LogError( ( "Invalid parameter: pAckProperties=%p, pSize=%p",
                    ( void * ) pAckProperties, ( void * ) pSize ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Calculate size for reason string if present. */
        if( pAckProperties->pReasonString != NULL && pAckProperties->reasonStringLength > 0 )
        {
            /* 1 byte for property identifier + 2 bytes for length + string bytes */
            size += 1U + 2U + pAckProperties->reasonStringLength;
        }

        /* Calculate size for user properties if present. */
        if( pAckProperties->pUserProperties != NULL && pAckProperties->userPropertiesCount > 0 )
        {
            for( i = 0; i < pAckProperties->userPropertiesCount; i++ )
            {
                status = MQTT_UserPropertyGetSize( &pAckProperties->pUserProperties[ i ], &userPropertySize );

                if( status != MQTTSuccess )
                {
                    LogError( ( "Failed to get size of user property at index %lu", ( unsigned long ) i ) );
                    break;
                }

                size += userPropertySize;
            }
        }

        /* Calculate size for reason codes if present. */
        if( ( status == MQTTSuccess ) && 
            ( pAckProperties->pReasonCodes != NULL ) && 
            ( pAckProperties->reasonCodesCount > 0 ) )
        {
            /* Each reason code is 1 byte. */
            size += pAckProperties->reasonCodesCount;
        }

        if( status == MQTTSuccess )
        {
            *pSize = size;
        }
    }

    return status;
}

/*-----------------------------------------------------------*//
* Field bit definitions for ACK packet properties. */
#define ACK_FIELD_REASON_STRING        ( 1U << 0 )
#define ACK_FIELD_USER_PROPERTY        ( 1U << 1 )

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_PubAckReasonString( MqttPropBuilder_t * pPropBuilder,
                                             const uint8_t * pReasonString,
                                             size_t reasonStringLength )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pPropBuilder == NULL ) || ( pReasonString == NULL ) || ( reasonStringLength == 0 ) )
    {
        LogError( ( "Invalid parameter: pPropBuilder=%p, pReasonString=%p, reasonStringLength=%lu",
                    ( void * ) pPropBuilder, ( void * ) pReasonString, ( unsigned long ) reasonStringLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the Reason String property. */
        status = MQTT_PropBuilderEncode( pPropBuilder,
                                         MQTT_PROPERTY_REASON_STRING,
                                         pReasonString,
                                         reasonStringLength,
                                         ACK_FIELD_REASON_STRING );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropAdd_PubAckUserProperty( MqttPropBuilder_t * pPropBuilder,
                                             const MQTTUserProperty * pUserProperties,
                                             size_t userPropertiesCount )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t i;

    /* Validate parameters. */
    if( ( pPropBuilder == NULL ) || ( pUserProperties == NULL ) || ( userPropertiesCount == 0 ) )
    {
        LogError( ( "Invalid parameter: pPropBuilder=%p, pUserProperties=%p, userPropertiesCount=%lu",
                    ( void * ) pPropBuilder, ( void * ) pUserProperties, ( unsigned long ) userPropertiesCount ) );
        status = MQTTBadParameter;
    }
    /* Check if user properties have already been added. */
    else if( ( pPropBuilder->fieldsSet & ACK_FIELD_USER_PROPERTY ) != 0 )
    {
        LogError( ( "User properties have already been added to this ACK packet" ) );
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
                LogError( ( "User property at index %lu is invalid", ( unsigned long ) i ) );
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
            pPropBuilder->fieldsSet |= ACK_FIELD_USER_PROPERTY;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/