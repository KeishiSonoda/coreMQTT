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
 * @file core_mqtt_v5_serializer.c
 * @brief Implementation of the MQTT v5.0 serializer.
 */

/* Standard includes. */
#include <string.h>
#include <assert.h>

/* MQTT includes. */
#include "core_mqtt_v5_serializer.h"
#include "core_mqtt_serializer.h"

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializePublishHeaderWithoutTopicV5( const MQTTPublishInfo_t * pPublishInfo,
                                                       size_t remainingLength,
                                                       uint8_t * pBuffer,
                                                       size_t * pHeaderSize,
                                                       const MqttPropBuilder_t * pPropertyBuilder )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t headerSize = 0;
    uint8_t * pIndex = pBuffer;
    uint16_t topicNameLength = 0;
    uint8_t publishFlags = 0;
    size_t propertyLength = 0;

    /* Validate arguments. */
    if( ( pPublishInfo == NULL ) || ( pBuffer == NULL ) || ( pHeaderSize == NULL ) )
    {
        LogError( ( "Invalid argument: pPublishInfo=%p, pBuffer=%p, pHeaderSize=%p.",
                    ( void * ) pPublishInfo,
                    ( void * ) pBuffer,
                    ( void * ) pHeaderSize ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->pTopicName == NULL ) || ( pPublishInfo->topicNameLength == 0 ) )
    {
        LogError( ( "Invalid topic name: pTopicName=%p, topicNameLength=%u.",
                    ( void * ) pPublishInfo->pTopicName,
                    ( unsigned int ) pPublishInfo->topicNameLength ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->qos > MQTTQoS2 ) || ( pPublishInfo->qos < MQTTQoS0 ) )
    {
        LogError( ( "Invalid QoS: %u.",
                    ( unsigned int ) pPublishInfo->qos ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder != NULL ) && !MQTT_PropBuilderIsValid( pPropertyBuilder ) )
    {
        LogError( ( "Invalid property builder." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Get the property length if a property builder is provided. */
        if( pPropertyBuilder != NULL )
        {
            MQTT_PropBuilderGetSize( pPropertyBuilder, &propertyLength );
        }

        /* The first byte of the PUBLISH packet contains the packet type and flags. */
        publishFlags = MQTT_PACKET_TYPE_PUBLISH;

        if( pPublishInfo->qos == MQTTQoS1 )
        {
            publishFlags |= 0x02U;
        }
        else if( pPublishInfo->qos == MQTTQoS2 )
        {
            publishFlags |= 0x04U;
        }
        else
        {
            /* QoS 0 doesn't add any flags. */
        }

        if( pPublishInfo->retain == true )
        {
            publishFlags |= 0x01U;
        }

        if( pPublishInfo->dup == true )
        {
            publishFlags |= 0x08U;
        }

        /* Write the packet type and flags to the buffer. */
        *pIndex = publishFlags;
        pIndex++;
        headerSize++;

        /* Write the remaining length to the buffer. */
        pIndex = MQTT_EncodeRemainingLength( pIndex, remainingLength );

        if( pIndex == NULL )
        {
            LogError( ( "Failed to encode remaining length." ) );
            status = MQTTBadParameter;
        }
        else
        {
            headerSize += ( size_t ) ( pIndex - ( pBuffer + 1 ) );

            /* Write the topic name length to the buffer. */
            topicNameLength = ( uint16_t ) pPublishInfo->topicNameLength;
            *pIndex = ( uint8_t ) ( topicNameLength >> 8 );
            pIndex++;
            *pIndex = ( uint8_t ) ( topicNameLength & 0xFF );
            pIndex++;
            headerSize += 2;

            /* The topic name is not written here, it will be sent separately. */

            /* Write the packet ID to the buffer if QoS is 1 or 2. */
            if( pPublishInfo->qos > MQTTQoS0 )
            {
                /* The packet ID is not written here, it will be sent separately. */
                headerSize += 2;
            }

            /* Write the property length to the buffer if a property builder is provided. */
            if( pPropertyBuilder != NULL )
            {
                /* Encode the property length. */
                pIndex = MQTT_EncodeRemainingLength( pIndex, propertyLength );

                if( pIndex == NULL )
                {
                    LogError( ( "Failed to encode property length." ) );
                    status = MQTTBadParameter;
                }
                else
                {
                    headerSize += ( size_t ) ( pIndex - ( pBuffer + headerSize ) );

                    /* Copy the properties from the property builder to the buffer. */
                    if( propertyLength > 0 )
                    {
                        memcpy( pIndex, pPropertyBuilder->pBuffer, propertyLength );
                        headerSize += propertyLength;
                    }
                }
            }

            *pHeaderSize = headerSize;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetPublishPacketSizeV5( const MQTTPublishInfo_t * pPublishInfo,
                                          const MqttPropBuilder_t * pPropertyBuilder,
                                          size_t * pRemainingLength,
                                          size_t * pPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t remainingLength = 0;
    size_t packetSize = 0;
    size_t propertyLength = 0;

    /* Validate arguments. */
    if( ( pPublishInfo == NULL ) || ( pRemainingLength == NULL ) || ( pPacketSize == NULL ) )
    {
        LogError( ( "Invalid argument: pPublishInfo=%p, pRemainingLength=%p, pPacketSize=%p.",
                    ( void * ) pPublishInfo,
                    ( void * ) pRemainingLength,
                    ( void * ) pPacketSize ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->pTopicName == NULL ) || ( pPublishInfo->topicNameLength == 0 ) )
    {
        LogError( ( "Invalid topic name: pTopicName=%p, topicNameLength=%u.",
                    ( void * ) pPublishInfo->pTopicName,
                    ( unsigned int ) pPublishInfo->topicNameLength ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->qos > MQTTQoS2 ) || ( pPublishInfo->qos < MQTTQoS0 ) )
    {
        LogError( ( "Invalid QoS: %u.",
                    ( unsigned int ) pPublishInfo->qos ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder != NULL ) && !MQTT_PropBuilderIsValid( pPropertyBuilder ) )
    {
        LogError( ( "Invalid property builder." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Calculate the remaining length and packet size for the PUBLISH packet. */
        
        /* Topic name length field (2 bytes) and topic name. */
        remainingLength += 2 + pPublishInfo->topicNameLength;

        /* Packet ID (2 bytes) if QoS is 1 or 2. */
        if( pPublishInfo->qos > MQTTQoS0 )
        {
            remainingLength += 2;
        }

        /* Get the property length if a property builder is provided. */
        if( pPropertyBuilder != NULL )
        {
            MQTT_PropBuilderGetSize( pPropertyBuilder, &propertyLength );

            /* Add the property length field and the properties. */
            remainingLength += MQTT_GetEncodedDataSize( propertyLength ) + propertyLength;
        }

        /* Add the payload length. */
        remainingLength += pPublishInfo->payloadLength;

        /* Calculate the full packet size. */
        packetSize = 1 + MQTT_GetEncodedDataSize( remainingLength ) + remainingLength;

        /* Check that the packet size is within the MQTT specification limits. */
        if( packetSize > MQTT_MAX_PACKET_SIZE )
        {
            LogError( ( "PUBLISH packet size exceeds the MQTT specification limit: %u > %u.",
                        ( unsigned int ) packetSize,
                        MQTT_MAX_PACKET_SIZE ) );
            status = MQTTBadParameter;
        }
        else
        {
            /* Set the output parameters. */
            *pRemainingLength = remainingLength;
            *pPacketSize = packetSize;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/