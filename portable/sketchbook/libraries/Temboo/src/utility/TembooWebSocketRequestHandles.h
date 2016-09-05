/*
###############################################################################
#
# Temboo Arduino library
#
# Copyright 2016, Temboo Inc.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###############################################################################
*/

#ifndef __TembooWebSocketRequestHandles__
#define __TembooWebSocketRequestHandles__

#include <Arduino.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum {
    WS_GET_REQUEST = 0,
    WS_SET_REQUEST,
    WS_DATA_REQUEST,
    WS_MESSAGE_REQUEST,
    WS_RR_REQUEST,
    WS_SESSION_ID_REQUEST,
    WS_REQUEST_ERROR,
    WS_UPDATE_CONNECTED,
    WS_UPDATE_DISCONNECTED,
    WS_NO_MESSAGE,
    WS_INTERVAL_REQUEST
} WSMessageRequest;

typedef struct TembooPinTable {
	uint8_t pin;
	uint32_t (*pinRead)(uint32_t pin);
	void  (*pinWrite)(uint32_t pin, int val);
    int currentPinValue;
    int defaultValue;
} TembooPinTable;

void addWebSocketPinData(int pin, int pinVal, bool requestResponse);
void updateIntervalTime(int intervalTime);

// Send frame
WSMessageRequest handleResponse(uint8_t* request, TembooPinTable* pinTable, int pinTableDepth, bool connectionStatus);
void logTembooDebug(const char *c);
#if defined(__cplusplus)
}
#endif



#endif /* defined(__TembooWebSocketRequestHandles__) */
