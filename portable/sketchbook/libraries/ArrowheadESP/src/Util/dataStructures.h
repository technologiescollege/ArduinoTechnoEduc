//
// Created by Szvetlin Tanyi on 2020. 04. 07.
//

#ifndef ARROWHEADESP_DATASTRUCTURES_H
#define ARROWHEADESP_DATASTRUCTURES_H

enum connStatus { NO_CONNECTION, WIFI };

/**
 * Struct for storing network related data
 */
struct netInfo {
    const char* ssid;
    const char* password;

    netInfo() {}

    netInfo(const char* _ssid,
            const char* _password) :
            ssid(_ssid),
            password(_password) {}
};

/**
 * Struct for storing provider related data
 */
struct providerInfo {
    const char* serviceRegistryAddress;
    int serviceRegistryPort;
    const char* systemName;

    providerInfo() {}

    providerInfo(const char* _serviceRegistryAddress,
            int _serviceRegistryPort,
            const char* _systemName) :
            serviceRegistryAddress(_serviceRegistryAddress),
            serviceRegistryPort(_serviceRegistryPort),
            systemName(_systemName) {}
};

/**
 * Struct for storing ssl related data
 */
struct sslInfo {
    int insecure; // used to disable CA verification
    const char* filenameCa;
    const char* filenamePk;
    const char* filenameCl;
    const char* filenameSc;
    const char* filenameSk;
    const char* publicKey;

    sslInfo() {}

    sslInfo(int _insecure,
            const char* _filenameCa,
            const char* _filenamePk,
            const char* _filenameCl,
            const char* _filenameSc,
            const char* _filenameSk,
            const char* _publicKey) :
            insecure(_insecure),
            filenameCa(_filenameCa),
            filenamePk(_filenamePk),
            filenameCl(_filenameCl),
            filenameSc(_filenameSc),
            filenameSk(_filenameSk),
            publicKey(_publicKey) {}
};



#endif //ARROWHEADESP_DATASTRUCTURES_H
