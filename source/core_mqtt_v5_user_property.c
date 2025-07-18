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
 * @file core_mqtt_v5_user_property.c
 * @brief Implementation of the user property functions for MQTT v5.0.
 */

/* Standard includes. */
#include <string.h>
#include <assert.h>

/* MQTT includes. */
#include "core_mqtt_v5_user_property.h"

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_UserPropertyInit( MQTTUserProperty * pUserProperty,
                                    uint8_t * pKey,
                                    size_t keyLength,
                                    uint8_t * pValue,
                                    size_t valueLength )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pUserProperty == NULL ) || ( pKey == NULL ) || ( pValue == NULL ) ||
        ( keyLength == 0 ) || ( valueLength == 0 ) )
    {
        LogError( ( "Invalid parameter: pUserProperty=%p, pKey=%p, keyLength=%lu, "
                    "pValue=%p, valueLength=%lu",
                    ( void * ) pUserProperty, ( void * ) pKey, ( unsigned long ) keyLength,
                    ( void * ) pValue, ( unsigned long ) valueLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        pUserProperty->key = pKey;
        pUserProperty->keyLength = keyLength;
        pUserProperty->value = pValue;
        pUserProperty->valueLength = valueLength;
    }

    return status;
}

/*-----------------------------------------------------------*/

bool MQTT_UserPropertyIsValid( const MQTTUserProperty * pUserProperty )
{
    bool isValid = false;

    if( ( pUserProperty != NULL ) &&
        ( pUserProperty->key != NULL ) &&
        ( pUserProperty->keyLength > 0 ) &&
        ( pUserProperty->value != NULL ) &&
        ( pUserProperty->valueLength > 0 ) )
    {
        isValid = true;
    }

    return isValid;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_UserPropertyGetSize( const MQTTUserProperty * pUserProperty,
                                       size_t * pSize )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pUserProperty == NULL ) || ( pSize == NULL ) )
    {
        LogError( ( "Invalid parameter: pUserProperty=%p, pSize=%p",
                    ( void * ) pUserProperty, ( void * ) pSize ) );
        status = MQTTBadParameter;
    }
    else if( !MQTT_UserPropertyIsValid( pUserProperty ) )
    {
        LogError( ( "User property is invalid" ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Calculate the size:
         * - 1 byte for property identifier (MQTT_PROPERTY_USER_PROPERTY)
         * - 2 bytes for key length
         * - Key bytes
         * - 2 bytes for value length
         * - Value bytes
         */
        *pSize = 1U + 2U + pUserProperty->keyLength + 2U + pUserProperty->valueLength;
    }

    return status;
}

/*-----------------------------------------------------------*/

bool MQTT_UserPropertyIsEqual( const MQTTUserProperty * pUserProperty1,
                               const MQTTUserProperty * pUserProperty2 )
{
    bool isEqual = false;

    /* Validate parameters. */
    if( ( pUserProperty1 == NULL ) || ( pUserProperty2 == NULL ) )
    {
        LogError( ( "Invalid parameter: pUserProperty1=%p, pUserProperty2=%p",
                    ( void * ) pUserProperty1, ( void * ) pUserProperty2 ) );
    }
    else if( !MQTT_UserPropertyIsValid( pUserProperty1 ) ||
             !MQTT_UserPropertyIsValid( pUserProperty2 ) )
    {
        LogError( ( "One or both user properties are invalid" ) );
    }
    else if( ( pUserProperty1->keyLength == pUserProperty2->keyLength ) &&
             ( pUserProperty1->valueLength == pUserProperty2->valueLength ) )
    {
        /* Compare key and value. */
        if( ( memcmp( pUserProperty1->key, pUserProperty2->key, pUserProperty1->keyLength ) == 0 ) &&
            ( memcmp( pUserProperty1->value, pUserProperty2->value, pUserProperty1->valueLength ) == 0 ) )
        {
            isEqual = true;
        }
    }

    return isEqual;
}

/*-----------------------------------------------------------*/