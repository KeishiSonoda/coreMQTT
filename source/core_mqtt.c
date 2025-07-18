/* Function that doesn't include property builder parameter for backward compatibility */
MQTTStatus_t MQTT_Publish( MQTTContext_t * pContext,
                           const MQTTPublishInfo_t * pPublishInfo,
                           uint16_t packetId )
{
    /* Call the implementation with NULL property builder */
    return MQTT_PublishV5(pContext, pPublishInfo, packetId, NULL);
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_PublishV5( MQTTContext_t * pContext,
                            const MQTTPublishInfo_t * pPublishInfo,
                            uint16_t packetId,
                            const MqttPropBuilder_t * pPropertyBuilder )
{
    size_t headerSize = 0UL;
    size_t remainingLength = 0UL;
    size_t packetSize = 0UL;
    MQTTPublishState_t publishStatus = MQTTStateNull;
    MQTTConnectionStatus_t connectStatus;

    /* Maximum number of bytes required by the 'fixed' part of the PUBLISH
     * packet header according to the MQTT specifications.
     * Header byte           0 + 1 = 1
     * Length (max)            + 4 = 5
     * Topic string length     + 2 = 7
     *
     * Note that since publish is one of the most common operations in MQTT
     * connection, we have moved the topic string length to the 'fixed' part of
     * the header so efficiency. Otherwise, we would need an extra vector and
     * an extra call to 'send' (in case writev is not defined) to send the
     * topic length.    */
    uint8_t mqttHeader[ 7U ];

    /* Validate arguments. */
    MQTTStatus_t status = validatePublishParams( pContext, pPublishInfo, packetId );

    if( status == MQTTSuccess )
    {
        /* Get the remaining length and packet size.*/
        if( pPropertyBuilder != NULL && MQTT_PropBuilderIsValid( pPropertyBuilder ) )
        {
            /* Use MQTT v5.0 packet size calculation when property builder is provided. */
            status = MQTT_GetPublishPacketSizeV5( pPublishInfo,
                                                 pPropertyBuilder,
                                                 &remainingLength,
                                                 &packetSize );
        }
        else
        {
            /* Use MQTT v3.1.1 packet size calculation when no property builder is provided. */
            status = MQTT_GetPublishPacketSize( pPublishInfo,
                                               &remainingLength,
                                               &packetSize );
        }
    }

    if( status == MQTTSuccess )
    {
        if( pPropertyBuilder != NULL && MQTT_PropBuilderIsValid( pPropertyBuilder ) )
        {
            /* Use MQTT v5.0 serialization when property builder is provided. */
            status = MQTT_SerializePublishHeaderWithoutTopicV5( pPublishInfo,
                                                               remainingLength,
                                                               mqttHeader,
                                                               &headerSize,
                                                               pPropertyBuilder );
        }
        else
        {
            /* Use MQTT v3.1.1 serialization when no property builder is provided. */
            status = MQTT_SerializePublishHeaderWithoutTopic( pPublishInfo,
                                                             remainingLength,
                                                             mqttHeader,
                                                             &headerSize );
        }
    }

    if( status == MQTTSuccess )
    {
        /* Take the mutex as multiple send calls are required for sending this
         * packet. */
        MQTT_PRE_STATE_UPDATE_HOOK( pContext );

        connectStatus = pContext->connectStatus;

        if( connectStatus != MQTTConnected )
        {
            status = ( connectStatus == MQTTNotConnected ) ? MQTTStatusNotConnected : MQTTStatusDisconnectPending;
        }

        if( ( status == MQTTSuccess ) && ( pPublishInfo->qos > MQTTQoS0 ) )
        {
            /* Set the flag so that the corresponding hook can be called later. */

            status = MQTT_ReserveState( pContext,
                                        packetId,
                                        pPublishInfo->qos );

            /* State already exists for a duplicate packet.
             * If a state doesn't exist, it will be handled as a new publish in
             * state engine. */
            if( ( status == MQTTStateCollision ) && ( pPublishInfo->dup == true ) )
            {
                status = MQTTSuccess;
            }
        }

        if( status == MQTTSuccess )
        {
            status = sendPublishWithoutCopy( pContext,
                                             pPublishInfo,
                                             mqttHeader,
                                             headerSize,
                                             packetId );
        }

        if( ( status == MQTTSuccess ) &&
            ( pPublishInfo->qos > MQTTQoS0 ) )
        {
            /* Update state machine after PUBLISH is sent.
             * Only to be done for QoS1 or QoS2. */
            status = MQTT_UpdateStatePublish( pContext,
                                              packetId,
                                              MQTT_SEND,
                                              pPublishInfo->qos,
                                              &publishStatus );

            if( status != MQTTSuccess )
            {
                LogError( ( "Update state for publish failed with status %s."
                            " However PUBLISH packet was sent to the broker."
                            " Any further handling of ACKs for the packet Id"
                            " will fail.",
                            MQTT_Status_strerror( status ) ) );
            }
        }

        /* mutex should be released and not before updating the state
         * because we need to make sure that the state is updated
         * after sending the publish packet, before the receive
         * loop receives ack for this and would want to update its state
         */
        MQTT_POST_STATE_UPDATE_HOOK( pContext );
    }

    if( status != MQTTSuccess )
    {
        LogError( ( "MQTT PUBLISH failed with status %s.",
                    MQTT_Status_strerror( status ) ) );
    }

    return status;
}