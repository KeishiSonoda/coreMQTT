import re
import sys

def update_mqtt_calls(file_path):
    with open(file_path, 'r') as file:
        content = file.read()
    
    # Pattern to match MQTT_Connect calls with 6 parameters (with NULL as the last parameter)
    pattern = r'(status = MQTT_Connect\(\s*&mqttContext,\s*&connectInfo,\s*NULL,\s*timeout,\s*&sessionPresent,\s*NULL\s*\);)'
    replacement = r'status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, content)
    
    # Pattern for calls with willInfo
    pattern = r'(status = MQTT_Connect\(\s*&mqttContext,\s*&connectInfo,\s*&willInfo,\s*timeout,\s*&sessionPresent,\s*NULL\s*\);)'
    replacement = r'status = MQTT_Connect( &mqttContext, &connectInfo, &willInfo, timeout, &sessionPresent );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, updated_content)
    
    # Pattern for calls with sessionPresentResult
    pattern = r'(status = MQTT_Connect\(\s*&mqttContext,\s*&connectInfo,\s*NULL,\s*timeout,\s*&sessionPresentResult,\s*NULL\s*\);)'
    replacement = r'status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresentResult );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, updated_content)
    
    # Pattern for calls with 0U timeout
    pattern = r'(status = MQTT_Connect\(\s*&mqttContext,\s*&connectInfo,\s*NULL,\s*0U,\s*&sessionPresent,\s*NULL\s*\);)'
    replacement = r'status = MQTT_Connect( &mqttContext, &connectInfo, NULL, 0U, &sessionPresent );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, updated_content)
    
    # Pattern for calls with 0 timeout
    pattern = r'(status = MQTT_Connect\(\s*&mqttContext,\s*&connectInfo,\s*NULL,\s*0,\s*&sessionPresent,\s*NULL\s*\);)'
    replacement = r'status = MQTT_Connect( &mqttContext, &connectInfo, NULL, 0, &sessionPresent );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, updated_content)
    
    # Pattern for calls with MQTT_NO_TIMEOUT_MS
    pattern = r'(status = MQTT_Connect\(\s*&mqttContext,\s*&connectInfo,\s*NULL,\s*MQTT_NO_TIMEOUT_MS,\s*&sessionPresent,\s*NULL\s*\);)'
    replacement = r'status = MQTT_Connect( &mqttContext, &connectInfo, NULL, MQTT_NO_TIMEOUT_MS, &sessionPresent );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, updated_content)
    
    # Pattern for calls with NULL as first parameter
    pattern = r'(status = MQTT_Connect\(\s*NULL,\s*&connectInfo,\s*NULL,\s*timeout,\s*&sessionPresent,\s*NULL\s*\);)'
    replacement = r'status = MQTT_Connect( NULL, &connectInfo, NULL, timeout, &sessionPresent );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, updated_content)
    
    # Pattern for calls with NULL as second parameter
    pattern = r'(status = MQTT_Connect\(\s*&mqttContext,\s*NULL,\s*NULL,\s*timeout,\s*&sessionPresent,\s*NULL\s*\);)'
    replacement = r'status = MQTT_Connect( &mqttContext, NULL, NULL, timeout, &sessionPresent );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, updated_content)
    
    # Pattern for calls with NULL as fifth parameter
    pattern = r'(status = MQTT_Connect\(\s*&mqttContext,\s*&connectInfo,\s*NULL,\s*timeout,\s*NULL,\s*NULL\s*\);)'
    replacement = r'status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, NULL );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, updated_content)
    
    # Pattern for multi-line calls
    pattern = r'(status = MQTT_Connect\(\s*&mqttContext,\s*&connectInfo,\s*NULL,\s*timeout,\s*&sessionPresent,\s*NULL\s*\);)'
    replacement = r'status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, updated_content)
    
    # Add patterns for MQTT_ConnectV5 calls with property builder
    pattern = r'(status = MQTT_Connect\(\s*&mqttContext,\s*&connectInfo,\s*NULL,\s*timeout,\s*&sessionPresent,\s*pPropertyBuilder\s*\);)'
    replacement = r'status = MQTT_ConnectV5( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent, pPropertyBuilder );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, updated_content)
    
    # Add patterns for MQTT_PublishV5 calls with property builder
    pattern = r'(status = MQTT_Publish\(\s*&mqttContext,\s*&publishInfo,\s*packetId,\s*pPropertyBuilder\s*\);)'
    replacement = r'status = MQTT_PublishV5( &mqttContext, &publishInfo, packetId, pPropertyBuilder );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, updated_content)
    
    # Add patterns for MQTT_PublishV5 calls with PACKET_ID and property builder
    pattern = r'(status = MQTT_Publish\(\s*&mqttContext,\s*&publishInfo,\s*PACKET_ID,\s*pPropertyBuilder\s*\);)'
    replacement = r'status = MQTT_PublishV5( &mqttContext, &publishInfo, PACKET_ID, pPropertyBuilder );'
    
    # Replace all occurrences
    updated_content = re.sub(pattern, replacement, updated_content)
    
    with open(file_path, 'w') as file:
        file.write(updated_content)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python update_mqtt_connect_calls.py <file_path>")
        sys.exit(1)
    
    update_mqtt_calls(sys.argv[1])
    print(f"Updated MQTT calls in {sys.argv[1]}")