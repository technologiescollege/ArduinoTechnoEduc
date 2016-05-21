/*
###############################################################################
#
# Temboo Arduino library
#
# Copyright 2015, Temboo Inc.
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

#ifndef TEMBOOYUNSHIELD_H_
#define TEMBOOYUNSHIELD_H_

#include <Arduino.h>

///////////////////////////////////////////////////////
//  BEGIN ARDUINO YUN SHIELD SUPPORT
///////////////////////////////////////////////////////

#include <Process.h>

class TembooYunShieldChoreo : public Process {

    public:
    void begin() {Process::begin("temboo");}
    void setAccountName(const String& accountName) { addParameter("-a" + accountName);}
    void setAppKeyName(const String& appKeyName) { addParameter("-u" + appKeyName);}
    void setAppKey(const String& appKey) { addParameter("-p" + appKey);}
    void setChoreo(const String& choreo) { addParameter("-c" + choreo);}
    void setCredential(const String& credentialName) { addParameter("-e" + credentialName);}
    void setSavedInputs(const String& savedInputsName) { addParameter("-e" + savedInputsName);}
    void setProfile(const String& profileName) { addParameter("-e" + profileName);}
    void addInput(const String& inputName, const String& inputValue) { addParameter("-i" + inputName + ":" + inputValue);}
    void addOutputFilter(const String& filterName, const String& filterPath, const String& variableName) { addParameter("-o" + filterName + ":" + filterPath + ":" + variableName);}
    void setSettingsFileToWrite(const String& filePath) { addParameter("-w" + filePath);}
    void setSettingsFileToRead(const String& filePath) { addParameter("-r" + filePath);}
    void setGatewayAddress(const String& addr) { addParameter("-s" + addr);}
    
};



#endif //TEMBOOYUNSHIELD_H_
