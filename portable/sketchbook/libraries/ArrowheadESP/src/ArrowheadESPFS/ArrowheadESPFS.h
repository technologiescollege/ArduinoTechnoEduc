//
// Created by Szvetlin Tanyi on 2020. 04. 06.
//

#ifndef ARROWHEADESP_ARROWHEADESPFS_H
#define ARROWHEADESP_ARROWHEADESPFS_H

#ifdef ESP32
#include "SPIFFS.h"
#endif

// Library includes
#include "FS.h"
#include <ArduinoJson.h>

// Header includes
#include "../Util/Util.h"

// Datastructures
#include "../Util/dataStructures.h"

const uint16_t JSON_SIZE = 1024;

enum validateStates {
    NO_CONFIG, CONFIG_TOO_BIG, CANNOT_PARSE, INCOMPLETE, GOOD_CONFIG
};

class ArrowheadESPFS {
private:
    /**
     * netInfo struct storing the network configuration
     */
    netInfo _networkData;
    /**
     * sslInfo struct storing the ssl configuration
     */
    sslInfo _sslData;

    /**
     * providerlInfo struct storing the provider configuration
     */
    providerInfo _providerData;

    /**
     * Helper variables
     */
    char _ssid[32];
    char _password[32];
    char _serviceRegistryAddress[256];
    int _serviceRegistryPort = 8443;
    char _systemName[256];
    int _insecure = 0;
    char _filenameCa[32];
    char _filenamePk[32];
    char _filenameCl[32];
    char _filenameSc[32];
    char _filenameSk[32];
    char _publicKey[512];

    /**
     * CA certificate
     */
    File _ca;
    /**
     * Private Key
     */
    File _pk;
    /**
     * Client certificate
     */
    File _cl;
    /**
     * Server certificate
     */
    File _sc;
    /**
     * Server key
     */
    File _sk;

    /**
     * Loads the config file from SPIFFS
     *
     * @param fileName
     * @return success of loading the network config file
     */
    bool loadNetworkConfig(const char *fileName);

    /**
     * Loads the SSL config file from SPIFFS
     *
     * @param fileName
     * @return success of loading the ssl config file
     */
    bool loadSSLConfig(const char *fileName);

    /**
     * Loads the config file from SPIFFS
     *
     * @param fileName
     * @return success of loading the network config file
     */
    bool loadProviderConfig(const char *fileName);

    /**
     * Loads a file from SPIFFS
     * @param fileName
     * @param file
     * @return success of loading the file
     */
    bool loadFile(const char *fileName, File& file);

    /**
     * Validates the network configuration
     *
     * @param doc
     * @return validateStates enum value
     */
    int8_t validateConfig(JsonDocument *doc);
    /**
     * Validates the ssl configuration
     *
     * @param doc
     * @return validateStates enum value
     */
    int8_t validateSSLConfig(JsonDocument *doc);
    /**
     * Validates the provider configuration
     *
     * @param doc
     * @return validateStates enum value
     */
    int8_t validateProviderConfig(JsonDocument *doc);

    /**
     * Deserializes a JSON file
     *
     * @param file
     * @param doc
     * @return success of the deserialization
     */
    bool deserializeJSONFromFile(File file, JsonDocument *doc);
public:
    /**
     * Default constructor
     */
    ArrowheadESPFS();

    /**
     * Loads the network configuration
     *
     * @param configFileName
     */
    void loadConfigFile(const char *configFileName);
    /**
     * Loads the SSL configuration
     *
     * @param sslFileName
     */
    void loadSSLConfigFile(const char *sslFileName);
    /**
     * Loads the provider configuration
     *
     * @param configFileName
     */
    void loadProviderConfigFile(const char *configFileName);
    /**
     * Loads the client certificate files
     *
     * @return success of loading all the client certificate files
     */
    bool loadClientCertificateFiles();
    /**
     * Loads the server certificate files
     *
     * @return success of loading all the server certificate files
     */
    bool loadServerCertificateFiles();
    /**
     * Closes the client certificate files
     *
     */
    void closeClientCertificateFiles();
    /**
     * Closes the server certificate files
     *
     */
    void closeServerCertificateFiles();

    /**
     * Getter for the private _networkData
     *
     * @return 
     */
    netInfo getNetInfo();
    /**
     * Getter for the private _sslData
     *
     * @return
     */
    sslInfo getSSLInfo();
    /**
     * Getter for the private _providerData
     *
     * @return
     */
    providerInfo getProviderInfo();

    /**
     * Getter for the CA cert
     *
     * @return
     */
    File& getCA();
    /**
     * Getter for the PK
     *
     * @return
     */
    File& getPK();
    /**
     * Getter for the Client cert
     *
     * @return
     */
    File& getCl();
    /**
     * Getter for the Server cert
     *
     * @return
     */
    File& getSc();
    /**
     * Getter for the Server key
     *
     * @return
     */
    File& getSk();
};


#endif //ARROWHEADESP_ARROWHEADESPFS_H
