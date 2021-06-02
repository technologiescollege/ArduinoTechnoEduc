//
// Created by Szvetlin Tanyi on 2020. 04. 06.
//

#include "ArrowheadESPFS.h"

// #######################################
// Constructors
// #######################################

ArrowheadESPFS::ArrowheadESPFS() {
    debugPrintln("ArrowheadESPFS Default Constructor");
    // Start the filesystem
    if (!SPIFFS.begin()) {
        debugPrintln("Failed to mount file system");
        return;
    } else {
        debugPrintln("File system mounted");
    }
}

// #######################################
// Private functions
// #######################################

bool ArrowheadESPFS::loadNetworkConfig(const char *fileName) {
    File networkConfig;
    if(!loadFile(fileName, networkConfig)){
        return false;
    }

    StaticJsonDocument <JSON_SIZE> doc;
    if (!deserializeJSONFromFile(networkConfig, &doc)) {
        return false;
    }

    if (validateConfig(&doc) != GOOD_CONFIG) {
        return false;
    }

    // copy the values into variables, so their value won't change if the doc is changed
    strlcpy(_ssid, doc["ssid"], sizeof(_ssid));
    strlcpy(_password, doc["password"], sizeof(_password));

    // assign value to the struct
    _networkData = {
            ssid: _ssid,
            password: _password
    };

    debugPrintln("Reading config file with values: ");
    debugPrintln(String("SSID: ") + _networkData.ssid);
    debugPrintln(String("Password: ") + _networkData.password);

    networkConfig.close();
    return true;
}

bool ArrowheadESPFS::loadProviderConfig(const char *fileName) {
    File providerConfig;
    if(!loadFile(fileName, providerConfig)){
        return false;
    }

    StaticJsonDocument <JSON_SIZE> doc;
    if (!deserializeJSONFromFile(providerConfig, &doc)) {
        return false;
    }

    if (validateProviderConfig(&doc) != GOOD_CONFIG) {
        return false;
    }

    // copy the values into variables, so their value won't change if the doc is changed
    strlcpy(_serviceRegistryAddress, doc["serviceRegistryAddress"], sizeof(_serviceRegistryAddress));
    strlcpy(_systemName, doc["systemName"], sizeof(_systemName));

    if(doc.containsKey("serviceRegistryPort")){
        _serviceRegistryPort = atoi(doc["serviceRegistryPort"]);
    }

    // assign value to the struct
    _providerData = {
            serviceRegistryAddress: _serviceRegistryAddress,
            serviceRegistryPort: _serviceRegistryPort,
            systemName: _systemName,
    };

    debugPrintln("Reading provider config file with values: ");
    debugPrintln(String("Service registry address: ") + _providerData.serviceRegistryAddress);
    debugPrintln(String("Service registry port: ") + _providerData.serviceRegistryPort);
    debugPrintln(String("System name: ") + _providerData.systemName);

    providerConfig.close();
    return true;
}

bool ArrowheadESPFS::loadSSLConfig(const char *fileName) {
    File sslConfig;
    if(!loadFile(fileName, sslConfig)){
        return false;
    }

    StaticJsonDocument<JSON_SIZE> doc;
    if(!deserializeJSONFromFile(sslConfig, &doc)) {
        return false;
    }

    if(validateSSLConfig(&doc) != GOOD_CONFIG) {
        return false;
    }

    if(doc.containsKey("insecure")){
        _insecure = atoi(doc["insecure"]);
    }

    // copy the values into variables, so their value won't change if the doc is changed
    strlcpy(_filenameCa, doc["filenameCa"], sizeof(_filenameCa));
    strlcpy(_filenamePk, doc["filenamePk"], sizeof(_filenamePk));
    strlcpy(_filenameCl, doc["filenameCl"], sizeof(_filenameCl));
    strlcpy(_filenameSc, doc["filenameSc"], sizeof(_filenameSc));
    strlcpy(_filenameSk, doc["filenameSk"], sizeof(_filenameSk));
    strlcpy(_publicKey, doc["publicKey"], sizeof(_publicKey));

    // assign value to the struct
    _sslData = {
            insecure : _insecure,
            filenameCa : _filenameCa,
            filenamePk : _filenamePk,
            filenameCl : _filenameCl,
            filenameSc : _filenameSc,
            filenameSk : _filenameSk,
            publicKey : _publicKey
    };

    debugPrintln("Reading config file with values: ");
    debugPrintln(String("CA filename: ") + _sslData.filenameCa);
    debugPrintln(String("PK filename: ") + _sslData.filenamePk);
    debugPrintln(String("CL filename: ") + _sslData.filenameCl);
    debugPrintln(String("Server cert filename: ") + _sslData.filenameSc);
    debugPrintln(String("Server key filename: ") + _sslData.filenameSk);
    debugPrintln(String("Insecure: ") + _sslData.insecure);
    debugPrintln(String("Public key: ") + _sslData.publicKey);

    sslConfig.close();
    return true;
}

bool ArrowheadESPFS::loadFile(const char *fileName, File &file) {
    if(!SPIFFS.exists(String("/") + fileName)){
        debugPrintln(String("File ") + fileName + " does not exist!");
        return false;
    }
    debugPrintln(String("Opening file: ") + fileName);
    //open the file as read only
    file = SPIFFS.open(String("/") + fileName, "r");

    //check to make sure, opening was possible
    if (!file) {
        debugPrintln(String("Failed to open ") + fileName);
        file.close();
        return false;
    } else {
        debugPrintln(String("Successfully opened ") + fileName);
        return true;
    }
}

int8_t ArrowheadESPFS::validateConfig(JsonDocument *doc) {
    if (doc == NULL || doc->size() == 0) {
        return NO_CONFIG;
    }

    if (!doc->containsKey("ssid") || !doc->containsKey("password")) {
        if (!doc->containsKey("ssid")) {
            debugPrintln("JSON - Missing ssid!");
        }
        if (!doc->containsKey("password")) {
            debugPrintln("JSON - Missing password!");
        }

        debugPrintln("Config is incomplete");
        return INCOMPLETE;
    }

    debugPrintln("Config is good");
    return GOOD_CONFIG;
}

int8_t ArrowheadESPFS::validateProviderConfig(JsonDocument *doc) {
    if (doc == NULL || doc->size() == 0) {
        return NO_CONFIG;
    }

    if (!doc->containsKey("serviceRegistryAddress") || !doc->containsKey("systemName")) {
        if (!doc->containsKey("serviceRegistryAddress")) {
            debugPrintln("JSON - Missing serviceRegistryAddress!");
        }
        if (!doc->containsKey("systemName")) {
            debugPrintln("JSON - Missing systemName!");
        }

        debugPrintln("Provider config is incomplete");
        return INCOMPLETE;
    }

    debugPrintln("Provider config is good");
    return GOOD_CONFIG;
}

int8_t ArrowheadESPFS::validateSSLConfig(JsonDocument *doc) {
    if(doc == NULL || doc->size() == 0) {
        return NO_CONFIG;
    }

    if(!doc->containsKey("filenameCa") ||
       !doc->containsKey("filenamePk") ||
       !doc->containsKey("filenameCl")) {

        if(!doc->containsKey("filenameCa")) {
            debugPrintln("JSON - Missing filenameCa");
        }

        if(!doc->containsKey("filenamePk")) {
            debugPrintln("JSON - Missing filenamePk");
        }

        if(!doc->containsKey("filenameCl")) {
            debugPrintln("JSON - Missing filenameCl");
        }

        debugPrintln("SSL Config is incomplete");
        return INCOMPLETE;
    }

    debugPrintln("Config is good");
    return GOOD_CONFIG;
}

bool ArrowheadESPFS::deserializeJSONFromFile(File file, JsonDocument *doc) {
    if(doc == NULL){
        return false;
    }

    size_t size = file.size();
    debugPrintln(String("JSON File Size: ") + size);
    if (size > JSON_SIZE) {
        debugPrintln("JSON File too large - returning false");
        return false;
    }

    DeserializationError error = deserializeJson(*doc, file);
    if (error && size != 0) {
        debugPrintln("JSON File corrupt/could not be deserialized - returning false");
        return false;
    }

    debugPrintln("JSON File successfully parsed");
    return true;
}

// #######################################
// Public functions
// #######################################

void ArrowheadESPFS::loadConfigFile(const char *configFileName) {
    bool configLoaded = false;
    debugPrintln(String("Trying to load ") + configFileName);

    configLoaded = loadNetworkConfig(configFileName);

    if (!configLoaded) {
        debugPrintln("Could not load config...");
    } else {
        debugPrintln("Config loaded");
    }
}

void ArrowheadESPFS::loadProviderConfigFile(const char *configFileName) {
    bool configLoaded = false;
    debugPrintln(String("Trying to load ") + configFileName);

    configLoaded = loadProviderConfig(configFileName);

    if (!configLoaded) {
        debugPrintln("Could not load config...");
    } else {
        debugPrintln("Config loaded");
    }
}

void ArrowheadESPFS::loadSSLConfigFile(const char *sslFileName) {
    bool sslConfigLoaded = false;
    debugPrintln(String("Trying to load ") + sslFileName);

    sslConfigLoaded = loadSSLConfig(sslFileName);

    if(!sslConfigLoaded) {
        debugPrintln("Could not load SSL config...");
    } else {
        debugPrintln("SSL Config loaded");
    }
}

bool ArrowheadESPFS::loadClientCertificateFiles() {
    int certLoadSuccess = 0;
    if(loadFile(_filenameCa, _ca)){
        debugPrintln("CA cert loaded from SPIFFS!");
        certLoadSuccess++;
    } else {
        debugPrintln("CA cert couldn't be loaded from SPIFFS!");
    }
    if(loadFile(_filenamePk, _pk)){
        debugPrintln("PK loaded from SPIFFS!");
        certLoadSuccess++;
    } else {
        debugPrintln("PK couldn't be loaded from SPIFFS!");
    }
    if(loadFile(_filenameCl, _cl)){
        debugPrintln("Client cert loaded from SPIFFS!");
        certLoadSuccess++;
    } else {
        debugPrintln("Client cert couldn't be loaded from SPIFFS!");
    }
    return certLoadSuccess == 3;
}

bool ArrowheadESPFS::loadServerCertificateFiles() {
    int certLoadSuccess = 0;
    if(loadFile(_filenameSc, _sc)){
        debugPrintln("Server cert loaded from SPIFFS!");
        certLoadSuccess++;
    } else {
        debugPrintln("Server cert couldn't be loaded from SPIFFS!");
    }
    if(loadFile(_filenameSk, _sk)){
        debugPrintln("Server key loaded from SPIFFS!");
        certLoadSuccess++;
    } else {
        debugPrintln("Server key couldn't be loaded from SPIFFS!");
    }
    return certLoadSuccess == 2;
}

void ArrowheadESPFS::closeClientCertificateFiles() {
    _ca.close();
    _pk.close();
    _cl.close();
}

void ArrowheadESPFS::closeServerCertificateFiles() {
    _sc.close();
    _sk.close();
}

netInfo ArrowheadESPFS::getNetInfo() {
    return _networkData;
}

sslInfo ArrowheadESPFS::getSSLInfo() {
    return _sslData;
}

providerInfo ArrowheadESPFS::getProviderInfo() {
    return _providerData;
}

File& ArrowheadESPFS::getCA() {
    return _ca;
}

File& ArrowheadESPFS::getPK() {
    return _pk;
}

File& ArrowheadESPFS::getCl() {
    return _cl;
}

File& ArrowheadESPFS::getSc() {
    return _sc;
}

File& ArrowheadESPFS::getSk() {
    return _sk;
}