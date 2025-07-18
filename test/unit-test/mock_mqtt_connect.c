/**
 * @brief Mock implementation of MQTT_Connect for unit tests.
 * 
 * This mock implementation allows the tests to continue working without
 * having to update all the test calls.
 */
#include "core_mqtt.h"

/* Original function declaration */
MQTTStatus_t MQTT_Connect_Original( MQTTContext_t * pContext,
                                   const MQTTConnectInfo_t * pConnectInfo,
                                   const MQTTPublishInfo_t * pWillInfo,
                                   uint32_t timeoutMs,
                                   bool * pSessionPresent );

/* Mock implementation that forwards to the original function */
MQTTStatus_t MQTT_Connect( MQTTContext_t * pContext,
                          const MQTTConnectInfo_t * pConnectInfo,
                          const MQTTPublishInfo_t * pWillInfo,
                          uint32_t timeoutMs,
                          bool * pSessionPresent,
                          const MqttPropBuilder_t * pPropertyBuilder )
{
    /* Ignore the property builder parameter in tests */
    return MQTT_Connect_Original(pContext, pConnectInfo, pWillInfo, timeoutMs, pSessionPresent);
}