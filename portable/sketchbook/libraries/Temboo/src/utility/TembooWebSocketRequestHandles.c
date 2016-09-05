/*
###############################################################################
#
# Temboo library
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

#include <ctype.h>

#include "TembooWebSocketRequestHandles.h"
#include "TembooGlobal.h"

static WSMessageRequest handleMessageRequest(char** saveptr);
static WSMessageRequest handleIntervalRequest(char** saveptr);
static WSMessageRequest handleSetRequest(char** saveptr, TembooPinTable* pinTable, int pinTableDepth);
static WSMessageRequest handleGetRequest(char** saveptr, TembooPinTable* pinTable, int pinTableDepth);
static WSMessageRequest handleMonitoringUpdateRequest(char**saveptr);

static const uint16_t WS_MAX_PAYLOAD_SIZE = 126;



WSMessageRequest handleGetRequest(char** saveptr, TembooPinTable* pinTable, int pinTableDepth) {
    char* textField = strtok_r(NULL, "|", &(*saveptr));
    int pinNum = -1;
    int pinVal = -1;
    WSMessageRequest rc = WS_REQUEST_ERROR;
    // strlen should catch the + sign
    while (textField != NULL && strlen(textField) >= 2) {
        if (*textField == 'P') {
            if (isdigit(textField[1]) && strlen(&textField[1]) < 4) {
               pinNum = atoi(&textField[1]);
            }
        } else if (*textField == '+') {
            break;
        }
        textField = strtok_r(NULL, "|", &(*saveptr));
    }
    
    if (pinNum >= 0) {
        //obtain the data and send to Temboo
        int i = 0;
        for (i = 0; i < pinTableDepth; i++) {
            if (pinTable[i].pin == pinNum) {
                if (pinTable[i].pinRead != NULL) {
                    // input pin
                    pinVal = pinTable[i].pinRead(pinNum);
                } else {
                    // output pin/actuator, use saved value
                    pinVal = pinTable[i].currentPinValue;
                }
                rc = WS_GET_REQUEST;
                break;
            }
        }
    }
    if (rc != WS_REQUEST_ERROR) {
        // Send pinVal
        addWebSocketPinData(pinNum, pinVal, true);
    }
    return rc;
}

WSMessageRequest handleSetRequest(char** saveptr, TembooPinTable* pinTable, int pinTableDepth) {
    int pinNum = -1;
    int pinVal = -1;
    bool pinValSet = false;
    WSMessageRequest rc = WS_REQUEST_ERROR;
    char* textField = strtok_r(NULL, "|", &(*saveptr));
    while (textField != NULL && strlen(textField) >= 2) {
        if (*textField == 'P') {
            if (isdigit(textField[1]) && strlen(&textField[1]) < 4) {
               pinNum = atoi(&textField[1]);
            }
        } else if (*textField == 'V') {
            if (isdigit(textField[1]) && strlen(&textField[1]) < 7) {
                pinVal = atoi(&textField[1]);
                pinValSet = true;
            }
            
        } else if (*textField == '+') {
            break;
        }
        textField = strtok_r(NULL, "|", &(*saveptr));
    }
    
    if (pinNum >= 0 &&  pinValSet) {
         int i = 0;
         for (; i < pinTableDepth; i++) {
            if (pinTable[i].pin == pinNum) {
                if (pinTable[i].pinWrite != NULL) {
                    pinTable[i].currentPinValue = pinVal;
                    pinTable[i].pinWrite(pinNum, pinVal);
                    rc = WS_SET_REQUEST;
                }
                break;
            }
        }
    }
    if (rc != WS_REQUEST_ERROR) {
        // with pinval found, send the data to Temboo
        addWebSocketPinData(pinNum, pinVal, true);
    }
    return rc;
}

WSMessageRequest handleMessageRequest(char** saveptr) {
    char* textField = strtok_r(NULL, "|", &(*saveptr));
    WSMessageRequest rc = WS_REQUEST_ERROR;
    if (textField != NULL && strlen(textField) >= 2) {
        if (*textField == 'T') {
            logTembooDebug(&textField[1]);
            rc = WS_MESSAGE_REQUEST;
        }
    }
    return rc;
}

WSMessageRequest handleMonitoringUpdateRequest(char** saveptr) {
    char* textField = strtok_r(NULL, "|", &(*saveptr));
    WSMessageRequest rc = WS_REQUEST_ERROR;
    if (textField != NULL && strlen(textField) >= 2) {
        if (*textField == 'T') {
            if (!strcmp(&textField[1], "true")) {
                rc = WS_UPDATE_CONNECTED;
            } else if (!strcmp(&textField[1], "false")) {
                rc = WS_UPDATE_DISCONNECTED;
            } else {
                rc = WS_REQUEST_ERROR;
            }
        }
    }
    return rc;
}

WSMessageRequest handleIntervalRequest(char** saveptr) {
    char* textField = strtok_r(NULL, "|", &(*saveptr));
    WSMessageRequest rc = WS_REQUEST_ERROR;
    if (textField != NULL && strlen(textField) >= 2) {
        if (*textField == 'V') {
            if (isdigit(textField[1]) && strlen(&textField[1]) < 6) {
               updateIntervalTime(atoi(&textField[1]));
               rc = WS_INTERVAL_REQUEST;
            }
        }
    }
    return rc;
}

WSMessageRequest handleResponse(uint8_t* request, TembooPinTable* pinTable, int pinTableDepth, bool connectionStatus) {
    // parse response to find request type
    char* saveptr = NULL;
    char* requestField = strtok_r((char*)request, "|", &saveptr);
    WSMessageRequest rc = WS_REQUEST_ERROR;
    while (requestField != NULL) {
        if (*requestField == 'M') {
            switch (requestField[1]) {
                // only read web socket messages if device is connected
                // to the web socket
                case 'G':
                    // send pin data to Temboo
                    if (connectionStatus) {
                        rc = handleGetRequest(&saveptr, pinTable, pinTableDepth);
                    }
                    break;
                case 'S':
                    // change state of a pin
                    if (connectionStatus) {
                        rc = handleSetRequest(&saveptr, pinTable, pinTableDepth);
                    }
                    break;
                case 'E':
                    // print message
                    rc = handleMessageRequest(&saveptr);
                    break;
                case 'U':
                    rc = handleMonitoringUpdateRequest(&saveptr);
                    break;
                case 'I':
                    rc = handleIntervalRequest(&saveptr);
                    break;
                default:
                    // return error
                    rc = WS_REQUEST_ERROR;
                    break;
            }
        }
        if (rc == WS_REQUEST_ERROR) {
            break;
        }
        requestField = strtok_r(NULL, "|", &saveptr);
    }
    return rc;
}
