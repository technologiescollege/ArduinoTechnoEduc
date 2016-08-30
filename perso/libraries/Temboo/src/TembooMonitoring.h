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

#ifndef TEMBOOMESSAGING_H_
#define TEMBOOMESSAGING_H_

#include <Arduino.h>
#include <Process.h>

#include "utility/TembooWebSocketRequestHandles.h"

class TembooMessaging : public Process {

public:
    TembooMessaging(TembooPinTable* pinTable, int pinTableSize);
    void begin();

    void setAccountName(const String& accountName);
    void setAccountName(const char* accountName);

    void setAppKeyName(const String& appKeyName);
    void setAppKeyName(const char* appKeyName);

    void setAppKey(const String& appKey);
    void setAppKey(const char* appKey);
    
    void setDeviceID(const String& appKey);
    void setDeviceID(const char* appKey);

    void addPinConfig(int pin, const char* type, int mode, int defaultValue);

    void initiateConnection();
    
    WSMessageRequest poll();
    
    static void sendData(int pin, int pinVal, bool requestResponse=false);
    void sendData(int pin, float pinVal);
    void updatePinValue(int pinNum, int pinVal);
    int retrievePinValue(int pinNum);
    void setPinsToDefaultState();

    bool isConnected();

private:
    const char* m_accountName;
    const char* m_appKey;
    const char* m_appKeyName;
    const char* m_deviceID;
    bool m_connectionStatus;
    int m_pinTableSize;
    int m_pinTableDepth;
    long int m_connectionAttemptTime;
    TembooPinTable* m_pinTable;
    unsigned long m_lastPingTime;

    void startMessaging();
    void disconnectWSConnection(int closeCode, const char* closeReason);
    void sendError(const char* errorText);
    void sendPing();
    
};

#endif