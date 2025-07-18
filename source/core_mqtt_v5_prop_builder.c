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
 * @file core_mqtt_v5_prop_builder.c
 * @brief Implementation of the property builder functions for MQTT v5.0.
 */

/* Standard includes. */
#include <string.h>
#include <assert.h>

/* MQTT includes. */
#include "core_mqtt_v5_prop_builder.h"
#include "core_mqtt_v5_property_ids.h"

/*-----------------------------------------------------------*/

/**
 * @brief Encode a byte value into the buffer.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] value Byte value to encode.
 *
 * @return #MQTTSuccess if the value was encoded successfully;
 * #MQTTNoMemory if the buffer is too small.
 */
static MQTTStatus_t encodeByte( MqttPropBuilder_t * pPropBuilder,
                                uint8_t value );

/**
 * @brief Encode a two-byte value into the buffer.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] value Two-byte value to encode.
 *
 * @return #MQTTSuccess if the value was encoded successfully;
 * #MQTTNoMemory if the buffer is too small.
 */
static MQTTStatus_t encodeTwoByteInt( MqttPropBuilder_t * pPropBuilder,
                                      uint16_t value );

/**
 * @brief Encode a four-byte value into the buffer.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] value Four-byte value to encode.
 *
 * @return #MQTTSuccess if the value was encoded successfully;
 * #MQTTNoMemory if the buffer is too small.
 */
static MQTTStatus_t encodeFourByteInt( MqttPropBuilder_t * pPropBuilder,
                                       uint32_t value );

/**
 * @brief Encode a UTF-8 string into the buffer.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] pString String to encode.
 * @param[in] stringLength Length of the string.
 *
 * @return #MQTTSuccess if the string was encoded successfully;
 * #MQTTNoMemory if the buffer is too small.
 */
static MQTTStatus_t encodeUTF8String( MqttPropBuilder_t * pPropBuilder,
                                      const uint8_t * pString,
                                      size_t stringLength );

/**
 * @brief Encode a binary data into the buffer.
 *
 * @param[in,out] pPropBuilder Property builder to use.
 * @param[in] pData Data to encode.
 * @param[in] dataLength Length of the data.
 *
 * @return #MQTTSuccess if the data was encoded successfully;
 * #MQTTNoMemory if the buffer is too small.
 */
static MQTTStatus_t encodeBinaryData( MqttPropBuilder_t * pPropBuilder,
                                      const uint8_t * pData,
                                      size_t dataLength );

/*-----------------------------------------------------------*/

static MQTTStatus_t encodeByte( MqttPropBuilder_t * pPropBuilder,
                                uint8_t value )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Check if there is enough space in the buffer. */
    if( pPropBuilder->currentIndex + 1 > pPropBuilder->bufferLength )
    {
        status = MQTTNoMemory;
    }
    else
    {
        /* Write the byte value to the buffer. */
        pPropBuilder->pBuffer[ pPropBuilder->currentIndex ] = value;
        pPropBuilder->currentIndex++;
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t encodeTwoByteInt( MqttPropBuilder_t * pPropBuilder,
                                      uint16_t value )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Check if there is enough space in the buffer. */
    if( pPropBuilder->currentIndex + 2 > pPropBuilder->bufferLength )
    {
        status = MQTTNoMemory;
    }
    else
    {
        /* Write the two-byte value to the buffer in big-endian format. */
        pPropBuilder->pBuffer[ pPropBuilder->currentIndex ] = ( uint8_t ) ( value >> 8 );
        pPropBuilder->pBuffer[ pPropBuilder->currentIndex + 1 ] = ( uint8_t ) ( value & 0xFF );
        pPropBuilder->currentIndex += 2;
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t encodeFourByteInt( MqttPropBuilder_t * pPropBuilder,
                                       uint32_t value )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Check if there is enough space in the buffer. */
    if( pPropBuilder->currentIndex + 4 > pPropBuilder->bufferLength )
    {
        status = MQTTNoMemory;
    }
    else
    {
        /* Write the four-byte value to the buffer in big-endian format. */
        pPropBuilder->pBuffer[ pPropBuilder->currentIndex ] = ( uint8_t ) ( value >> 24 );
        pPropBuilder->pBuffer[ pPropBuilder->currentIndex + 1 ] = ( uint8_t ) ( ( value >> 16 ) & 0xFF );
        pPropBuilder->pBuffer[ pPropBuilder->currentIndex + 2 ] = ( uint8_t ) ( ( value >> 8 ) & 0xFF );
        pPropBuilder->pBuffer[ pPropBuilder->currentIndex + 3 ] = ( uint8_t ) ( value & 0xFF );
        pPropBuilder->currentIndex += 4;
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t encodeUTF8String( MqttPropBuilder_t * pPropBuilder,
                                      const uint8_t * pString,
                                      size_t stringLength )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Check if there is enough space in the buffer. */
    if( pPropBuilder->currentIndex + 2 + stringLength > pPropBuilder->bufferLength )
    {
        status = MQTTNoMemory;
    }
    else
    {
        /* Write the string length as a two-byte integer. */
        status = encodeTwoByteInt( pPropBuilder, ( uint16_t ) stringLength );

        if( status == MQTTSuccess )
        {
            /* Write the string data to the buffer. */
            memcpy( &pPropBuilder->pBuffer[ pPropBuilder->currentIndex ], pString, stringLength );
            pPropBuilder->currentIndex += stringLength;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t encodeBinaryData( MqttPropBuilder_t * pPropBuilder,
                                      const uint8_t * pData,
                                      size_t dataLength )
{
    /* Binary data is encoded the same way as UTF-8 strings in MQTT v5.0. */
    return encodeUTF8String( pPropBuilder, pData, dataLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropBuilderInit( MqttPropBuilder_t * pPropBuilder,
                                   uint8_t * pBuffer,
                                   size_t bufferLength )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pPropBuilder == NULL ) || ( pBuffer == NULL ) || ( bufferLength == 0 ) )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Initialize the property builder. */
        pPropBuilder->pBuffer = pBuffer;
        pPropBuilder->bufferLength = bufferLength;
        pPropBuilder->currentIndex = 0;
        pPropBuilder->fieldsSet = 0;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropBuilderGetSize( const MqttPropBuilder_t * pPropBuilder,
                                      size_t * pSize )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pPropBuilder == NULL ) || ( pSize == NULL ) )
    {
        status = MQTTBadParameter;
    }
    else
    {
        *pSize = pPropBuilder->currentIndex;
    }

    return status;
}

/*-----------------------------------------------------------*/

bool MQTT_PropBuilderIsValid( const MqttPropBuilder_t * pPropBuilder )
{
    bool isValid = false;

    if( ( pPropBuilder != NULL ) &&
        ( pPropBuilder->pBuffer != NULL ) &&
        ( pPropBuilder->bufferLength > 0 ) &&
        ( pPropBuilder->currentIndex <= pPropBuilder->bufferLength ) )
    {
        isValid = true;
    }

    return isValid;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropBuilderReset( MqttPropBuilder_t * pPropBuilder )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( pPropBuilder == NULL )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Reset the property builder. */
        pPropBuilder->currentIndex = 0;
        pPropBuilder->fieldsSet = 0;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PropBuilderEncode( MqttPropBuilder_t * pPropBuilder,
                                     uint8_t propertyId,
                                     const void * pValue,
                                     size_t valueLength,
                                     uint32_t fieldBit )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Validate parameters. */
    if( ( pPropBuilder == NULL ) || ( pValue == NULL ) )
    {
        status = MQTTBadParameter;
    }
    /* Check if the property has already been set. */
    else if( ( pPropBuilder->fieldsSet & fieldBit ) != 0 )
    {
        status = MQTTBadParameter;
    }
    else
    {
        /* Encode the property identifier. */
        status = encodeByte( pPropBuilder, propertyId );

        if( status == MQTTSuccess )
        {
            /* Encode the property value based on the property identifier. */
            switch( propertyId )
            {
                /* Byte properties */
                case MQTT_PROPERTY_PUBLISH_PAYLOAD_FORMAT:
                case MQTT_PROPERTY_CONNECT_REQUEST_RESP_INFO:
                case MQTT_PROPERTY_CONNECT_REQUEST_PROBLEM_INFO:
                case MQTT_PROPERTY_CONNACK_MAX_QOS:
                case MQTT_PROPERTY_CONNACK_RETAIN_AVAILABLE:
                    status = encodeByte( pPropBuilder, *( ( const uint8_t * ) pValue ) );
                    break;

                /* Two-byte integer properties */
                case MQTT_PROPERTY_CONNECT_RECEIVE_MAXIMUM:
                case MQTT_PROPERTY_CONNECT_TOPIC_ALIAS_MAX:
                case MQTT_PROPERTY_CONNACK_RECEIVE_MAXIMUM:
                case MQTT_PROPERTY_CONNACK_TOPIC_ALIAS_MAX:
                case MQTT_PROPERTY_PUBLISH_TOPIC_ALIAS:
                    status = encodeTwoByteInt( pPropBuilder, *( ( const uint16_t * ) pValue ) );
                    break;

                /* Four-byte integer properties */
                case MQTT_PROPERTY_CONNECT_SESSION_EXPIRY:
                case MQTT_PROPERTY_CONNECT_MAX_PACKET_SIZE:
                case MQTT_PROPERTY_CONNACK_SESSION_EXPIRY:
                case MQTT_PROPERTY_CONNACK_MAX_PACKET_SIZE:
                case MQTT_PROPERTY_PUBLISH_MESSAGE_EXPIRY:
                case MQTT_PROPERTY_DISCONNECT_SESSION_EXPIRY:
                    status = encodeFourByteInt( pPropBuilder, *( ( const uint32_t * ) pValue ) );
                    break;

                /* UTF-8 string properties */
                case MQTT_PROPERTY_CONNECT_AUTH_METHOD:
                case MQTT_PROPERTY_CONNACK_ASSIGNED_CLIENT_ID:
                case MQTT_PROPERTY_CONNACK_RESPONSE_INFO:
                case MQTT_PROPERTY_CONNACK_SERVER_REFERENCE:
                case MQTT_PROPERTY_CONNACK_AUTH_METHOD:
                case MQTT_PROPERTY_PUBLISH_CONTENT_TYPE:
                case MQTT_PROPERTY_PUBLISH_RESPONSE_TOPIC:
                case MQTT_PROPERTY_REASON_STRING:
                case MQTT_PROPERTY_DISCONNECT_SERVER_REFERENCE:
                case MQTT_PROPERTY_AUTH_AUTHENTICATION_METHOD:
                    status = encodeUTF8String( pPropBuilder, ( const uint8_t * ) pValue, valueLength );
                    break;

                /* Binary data properties */
                case MQTT_PROPERTY_CONNECT_AUTH_DATA:
                case MQTT_PROPERTY_CONNACK_AUTH_DATA:
                case MQTT_PROPERTY_PUBLISH_CORRELATION_DATA:
                case MQTT_PROPERTY_AUTH_AUTHENTICATION_DATA:
                    status = encodeBinaryData( pPropBuilder, ( const uint8_t * ) pValue, valueLength );
                    break;

                /* User property is handled separately */
                case MQTT_PROPERTY_USER_PROPERTY:
                    /* This should not be reached as user properties are handled separately. */
                    status = MQTTBadParameter;
                    break;

                default:
                    /* Unknown property identifier. */
                    status = MQTTBadParameter;
                    break;
            }

            /* If the property was encoded successfully, mark it as set. */
            if( status == MQTTSuccess )
            {
                pPropBuilder->fieldsSet |= fieldBit;
            }
        }
    }

    return status;
}

/*-----------------------------------------------------------*/