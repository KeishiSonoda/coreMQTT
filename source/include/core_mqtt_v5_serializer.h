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
 * @file core_mqtt_v5_serializer.h
 * @brief User-facing functions for serializing and deserializing MQTT 5.0
 * packets.
 */
#ifndef CORE_MQTT_V5_SERIALIZER_H
#define CORE_MQTT_V5_SERIALIZER_H

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

#include "core_mqtt_serializer.h"
#include "core_mqtt_v5_property_ids.h"
#include "core_mqtt_v5_reason_codes.h"

/**
 * @ingroup mqtt_constants
 * @brief MQTT v5.0 AUTH packet type.
 */
#define MQTT_PACKET_TYPE_AUTH           ( ( uint8_t ) 0xF0U )  /**< @brief AUTH (bidirectional). */

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
 * @ingroup mqtt_struct_types
 * @brief MQTT v5.0 Property Builder structure.
 */
typedef struct MqttPropBuilder
{
    /**
     * @brief Pointer to user-provided buffer.
     */
    uint8_t * pBuffer;

    /**
     * @brief Length of the buffer.
     */
    size_t bufferLength;

    /**
     * @brief Current write position in the buffer.
     */
    size_t currentIndex;

    /**
     * @brief Bitmask of fields that have been set.
     */
    uint32_t fieldsSet;
} MqttPropBuilder_t;

/**
 * @brief Initialize a property builder.
 *
 * @param[in] pPropBuilder Property builder to initialize.
 * @param[in] pBuffer Buffer to use for property serialization.
 * @param[in] bufferLength Length of the buffer.
 *
 * @return #MQTTSuccess if initialization was successful;
 * #MQTTBadParameter if invalid parameters are passed.
 */
MQTTStatus_t MqttPropertyBuilder_Init( MqttPropBuilder_t * pPropBuilder,
                                       uint8_t * pBuffer,
                                       size_t bufferLength );

/**
 * @brief Add a session expiry interval property to a CONNECT packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] sessionExpiry Session expiry interval in seconds.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_ConnSessionExpiry( MqttPropBuilder_t * pPropBuilder,
                                            uint32_t sessionExpiry );

/**
 * @brief Add a receive maximum property to a CONNECT packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] receiveMaximum Maximum number of QoS 1 and QoS 2 publish packets.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_ConnReceiveMax( MqttPropBuilder_t * pPropBuilder,
                                         uint16_t receiveMaximum );

/**
 * @brief Add a maximum packet size property to a CONNECT packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] maxPacketSize Maximum packet size the client will accept.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_ConnMaxPacketSize( MqttPropBuilder_t * pPropBuilder,
                                            uint32_t maxPacketSize );

/**
 * @brief Add a topic alias maximum property to a CONNECT packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] topicAliasMaximum Maximum number of topic aliases.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_ConnTopicAliasMax( MqttPropBuilder_t * pPropBuilder,
                                            uint16_t topicAliasMaximum );

/**
 * @brief Add a request response information property to a CONNECT packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] requestResponseInfo Whether to request response information.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_ConnRequestRespInfo( MqttPropBuilder_t * pPropBuilder,
                                              bool requestResponseInfo );

/**
 * @brief Add a request problem information property to a CONNECT packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] requestProblemInfo Whether to request problem information.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_ConnRequestProbInfo( MqttPropBuilder_t * pPropBuilder,
                                              bool requestProblemInfo );

/**
 * @brief Add user properties to a CONNECT packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] userPropArray Array of user properties.
 * @param[in] userPropArrayLen Number of user properties in the array.
 *
 * @return #MQTTSuccess if the properties were added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_ConnUserProps( MqttPropBuilder_t * pPropBuilder,
                                        MQTTUserProperty * userPropArray,
                                        size_t userPropArrayLen );

/**
 * @brief Add an authentication method property to a CONNECT packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] authMethod Authentication method string.
 * @param[in] authMethodLength Length of the authentication method string.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_ConnAuthMethod( MqttPropBuilder_t * pPropBuilder,
                                         uint8_t * authMethod,
                                         size_t authMethodLength );

/**
 * @brief Add authentication data property to a CONNECT packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] authData Authentication data.
 * @param[in] authDataLength Length of the authentication data.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_ConnAuthData( MqttPropBuilder_t * pPropBuilder,
                                       uint8_t * authData,
                                       size_t authDataLength );

/**
 * @brief Add a payload format indicator property to a PUBLISH packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] payloadFormat Whether the payload is UTF-8 encoded.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_PubPayloadFormat( MqttPropBuilder_t * pPropBuilder,
                                           bool payloadFormat );

/**
 * @brief Add a message expiry interval property to a PUBLISH packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] messageExpiryInterval Message expiry interval in seconds.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_PubMessageExpiry( MqttPropBuilder_t * pPropBuilder,
                                           uint32_t messageExpiryInterval );

/**
 * @brief Add a topic alias property to a PUBLISH packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] topicAlias Topic alias value.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_PubTopicAlias( MqttPropBuilder_t * pPropBuilder,
                                        uint16_t topicAlias );

/**
 * @brief Add a response topic property to a PUBLISH packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] responseTopic Response topic string.
 * @param[in] responseTopicLength Length of the response topic string.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_PubResponseTopic( MqttPropBuilder_t * pPropBuilder,
                                           char * responseTopic,
                                           size_t responseTopicLength );

/**
 * @brief Add correlation data property to a PUBLISH packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] correlationData Correlation data.
 * @param[in] correlationDataLength Length of the correlation data.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_PubCorrelationData( MqttPropBuilder_t * pPropBuilder,
                                             uint8_t * correlationData,
                                             size_t correlationDataLength );

/**
 * @brief Add user properties to a PUBLISH packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] userPropArray Array of user properties.
 * @param[in] userPropArrayLen Number of user properties in the array.
 *
 * @return #MQTTSuccess if the properties were added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_PubUserProps( MqttPropBuilder_t * pPropBuilder,
                                       MQTTUserProperty * userPropArray,
                                       size_t userPropArrayLen );

/**
 * @brief Add a content type property to a PUBLISH packet.
 *
 * @param[in] pPropBuilder Initialized property builder.
 * @param[in] contentType Content type string.
 * @param[in] contentTypeLength Length of the content type string.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_PubContentType( MqttPropBuilder_t * pPropBuilder,
                                         char * contentType,
                                         size_t contentTypeLength );

/**
 * @brief Add a reason string property to a PUBACK packet.
 *
 * @param[in] pContext Initialized MQTT context.
 * @param[in] pBuffer Buffer for the reason string.
 * @param[in] bufferLen Length of the buffer.
 *
 * @return #MQTTSuccess if the property was added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_PubAckReasonString( MQTTContext_t * pContext,
                                             uint8_t * pBuffer,
                                             size_t bufferLen );

/**
 * @brief Add user properties to a PUBACK packet.
 *
 * @param[in] pContext Initialized MQTT context.
 * @param[in] userPropArray Array of user properties.
 * @param[in] userPropArrayLen Number of user properties in the array.
 *
 * @return #MQTTSuccess if the properties were added successfully;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTNoMemory if the buffer is too small.
 */
MQTTStatus_t MQTTPropAdd_PubAckUserProperty( MQTTContext_t * pContext,
                                             MQTTUserProperty * userPropArray,
                                             size_t userPropArrayLen );

/**
 * @brief Get the size and Remaining Length of an MQTT v5.0 CONNECT packet.
 *
 * @param[in] pConnectInfo MQTT CONNECT packet parameters.
 * @param[in] pWillInfo Last Will and Testament. Pass NULL if not used.
 * @param[in] pPropertyBuilder Property builder for CONNECT packet properties.
 * @param[out] pRemainingLength The Remaining Length of the MQTT CONNECT packet.
 * @param[out] pPacketSize The total size of the MQTT CONNECT packet.
 *
 * @return #MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec; #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_GetConnectPacketSizeV5( const MQTTConnectInfo_t * pConnectInfo,
                                          const MQTTPublishInfo_t * pWillInfo,
                                          const MqttPropBuilder_t * pPropertyBuilder,
                                          size_t * pRemainingLength,
                                          size_t * pPacketSize );

/**
 * @brief Serialize an MQTT v5.0 CONNECT packet in the given fixed buffer.
 *
 * @param[in] pConnectInfo MQTT CONNECT packet parameters.
 * @param[in] pWillInfo Last Will and Testament. Pass NULL if not used.
 * @param[in] pPropertyBuilder Property builder for CONNECT packet properties.
 * @param[in] remainingLength Remaining Length provided by #MQTT_GetConnectPacketSizeV5.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_SerializeConnectV5( const MQTTConnectInfo_t * pConnectInfo,
                                      const MQTTPublishInfo_t * pWillInfo,
                                      const MqttPropBuilder_t * pPropertyBuilder,
                                      size_t remainingLength,
                                      const MQTTFixedBuffer_t * pFixedBuffer );

/**
 * @brief Get the packet size and remaining length of an MQTT v5.0 PUBLISH packet.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[in] pPropertyBuilder Property builder for PUBLISH packet properties.
 * @param[out] pRemainingLength The Remaining Length of the MQTT PUBLISH packet.
 * @param[out] pPacketSize The total size of the MQTT PUBLISH packet.
 *
 * @return #MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec or if invalid parameters are passed; #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_GetPublishPacketSizeV5( const MQTTPublishInfo_t * pPublishInfo,
                                          const MqttPropBuilder_t * pPropertyBuilder,
                                          size_t * pRemainingLength,
                                          size_t * pPacketSize );

/**
 * @brief Serialize an MQTT v5.0 PUBLISH packet in the given buffer.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[in] packetId packet ID generated by #MQTT_GetPacketId.
 * @param[in] pPropertyBuilder Property builder for PUBLISH packet properties.
 * @param[in] remainingLength Remaining Length provided by #MQTT_GetPublishPacketSizeV5.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_SerializePublishV5( const MQTTPublishInfo_t * pPublishInfo,
                                      uint16_t packetId,
                                      const MqttPropBuilder_t * pPropertyBuilder,
                                      size_t remainingLength,
                                      const MQTTFixedBuffer_t * pFixedBuffer );

/**
 * @brief Serialize an MQTT v5.0 PUBLISH packet header without the topic string.
 *
 * This function serializes the fixed header and variable header of an MQTT v5.0
 * PUBLISH packet, but does not include the topic string or payload. This is used
 * for the zero-copy publish feature.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[in] remainingLength Remaining Length provided by #MQTT_GetPublishPacketSizeV5.
 * @param[out] pBuffer Buffer for packet serialization.
 * @param[out] pHeaderSize Size of the serialized header.
 * @param[in] pPropertyBuilder Property builder for PUBLISH packet properties.
 *
 * @return #MQTTNoMemory if pBuffer is too small to hold the MQTT packet header;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_SerializePublishHeaderWithoutTopicV5( const MQTTPublishInfo_t * pPublishInfo,
                                                       size_t remainingLength,
                                                       uint8_t * pBuffer,
                                                       size_t * pHeaderSize,
                                                       const MqttPropBuilder_t * pPropertyBuilder );

/**
 * @brief Get the size and Remaining Length of an MQTT v5.0 AUTH packet.
 *
 * @param[in] reasonCode Reason code for the AUTH packet.
 * @param[in] pPropertyBuilder Property builder for AUTH packet properties.
 * @param[out] pRemainingLength The Remaining Length of the MQTT AUTH packet.
 * @param[out] pPacketSize The total size of the MQTT AUTH packet.
 *
 * @return #MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec; #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_GetAuthPacketSize( uint8_t reasonCode,
                                     const MqttPropBuilder_t * pPropertyBuilder,
                                     size_t * pRemainingLength,
                                     size_t * pPacketSize );

/**
 * @brief Serialize an MQTT v5.0 AUTH packet in the given fixed buffer.
 *
 * @param[in] reasonCode Reason code for the AUTH packet.
 * @param[in] pPropertyBuilder Property builder for AUTH packet properties.
 * @param[in] remainingLength Remaining Length provided by #MQTT_GetAuthPacketSize.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_SerializeAuth( uint8_t reasonCode,
                                 const MqttPropBuilder_t * pPropertyBuilder,
                                 size_t remainingLength,
                                 const MQTTFixedBuffer_t * pFixedBuffer );

/**
 * @brief Deserialize an MQTT v5.0 CONNACK packet.
 *
 * @param[in] pConnackPacket CONNACK packet received from the broker.
 * @param[out] pSessionPresent Whether a previous session was present.
 * @param[out] pReasonCode The reason code from the CONNACK packet.
 * @param[out] pAckProperties Pointer to store the deserialized properties.
 *
 * @return #MQTTBadParameter if invalid parameters are passed;
 * #MQTTBadResponse if the CONNACK packet is malformed;
 * #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_DeserializeConnackV5( const MQTTPacketInfo_t * pConnackPacket,
                                        bool * pSessionPresent,
                                        uint8_t * pReasonCode,
                                        MQTTAckProperties_t * pAckProperties );

/**
 * @brief Deserialize an MQTT v5.0 PUBLISH packet.
 *
 * @param[in] pPublishPacket PUBLISH packet received from the broker.
 * @param[out] pPacketId Packet ID of the PUBLISH packet.
 * @param[out] pPublishInfo Deserialized PUBLISH information.
 * @param[out] pAckProperties Pointer to store the deserialized properties.
 *
 * @return #MQTTBadParameter if invalid parameters are passed;
 * #MQTTBadResponse if the PUBLISH packet is malformed;
 * #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_DeserializePublishV5( const MQTTPacketInfo_t * pPublishPacket,
                                        uint16_t * pPacketId,
                                        MQTTPublishInfo_t * pPublishInfo,
                                        MQTTAckProperties_t * pAckProperties );

/**
 * @brief Deserialize an MQTT v5.0 PUBACK packet.
 *
 * @param[in] pPubackPacket PUBACK packet received from the broker.
 * @param[out] pPacketId Packet ID of the PUBACK packet.
 * @param[out] pReasonCode The reason code from the PUBACK packet.
 * @param[out] pAckProperties Pointer to store the deserialized properties.
 *
 * @return #MQTTBadParameter if invalid parameters are passed;
 * #MQTTBadResponse if the PUBACK packet is malformed;
 * #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_DeserializePubackV5( const MQTTPacketInfo_t * pPubackPacket,
                                       uint16_t * pPacketId,
                                       uint8_t * pReasonCode,
                                       MQTTAckProperties_t * pAckProperties );

/**
 * @brief Deserialize an MQTT v5.0 SUBACK packet.
 *
 * @param[in] pSubackPacket SUBACK packet received from the broker.
 * @param[out] pPacketId Packet ID of the SUBACK packet.
 * @param[out] pAckProperties Pointer to store the deserialized properties.
 *
 * @return #MQTTBadParameter if invalid parameters are passed;
 * #MQTTBadResponse if the SUBACK packet is malformed;
 * #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_DeserializeSubackV5( const MQTTPacketInfo_t * pSubackPacket,
                                       uint16_t * pPacketId,
                                       MQTTAckProperties_t * pAckProperties );

/**
 * @brief Deserialize an MQTT v5.0 UNSUBACK packet.
 *
 * @param[in] pUnsubackPacket UNSUBACK packet received from the broker.
 * @param[out] pPacketId Packet ID of the UNSUBACK packet.
 * @param[out] pAckProperties Pointer to store the deserialized properties.
 *
 * @return #MQTTBadParameter if invalid parameters are passed;
 * #MQTTBadResponse if the UNSUBACK packet is malformed;
 * #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_DeserializeUnsubackV5( const MQTTPacketInfo_t * pUnsubackPacket,
                                         uint16_t * pPacketId,
                                         MQTTAckProperties_t * pAckProperties );

/**
 * @brief Deserialize an MQTT v5.0 AUTH packet.
 *
 * @param[in] pAuthPacket AUTH packet received from the broker.
 * @param[out] pReasonCode The reason code from the AUTH packet.
 * @param[out] pAckProperties Pointer to store the deserialized properties.
 *
 * @return #MQTTBadParameter if invalid parameters are passed;
 * #MQTTBadResponse if the AUTH packet is malformed;
 * #MQTTSuccess otherwise.
 */
MQTTStatus_t MQTT_DeserializeAuth( const MQTTPacketInfo_t * pAuthPacket,
                                   uint8_t * pReasonCode,
                                   MQTTAckProperties_t * pAckProperties );

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_V5_SERIALIZER_H */