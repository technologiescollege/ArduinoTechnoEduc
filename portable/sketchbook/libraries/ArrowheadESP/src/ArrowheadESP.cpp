//
// Created by Szvetlin Tanyi <szvetlin@aitia.ai> on 2020. 04. 06.
//

#include "ArrowheadESP.h"

// #######################################
// Constructors
// #######################################

ArrowheadESP::ArrowheadESP() {
    debugPrintln("ArrowheadESP Default Constructor");
}

// #######################################
// Private functions
// #######################################

bool ArrowheadESP::setupWiFi() {
    // We have to check that everything is available before proceeding
    if (!getArrowheadESPFS().getNetInfo().ssid || !getArrowheadESPFS().getNetInfo().password) {
        return false;
    }
    while (_reconnectAttempt < _maxReconnectAttempts) {
        debugPrintln(String("Connection attempt: ") + (_reconnectAttempt + 1) + " of " + _maxReconnectAttempts);
        WiFi.disconnect();
        delay(10);
        // We start by connecting to a WiFi network
        debugPrintln(String("Connecting to ") + getArrowheadESPFS().getNetInfo().ssid);
        // WiFi in Station mode
        WiFi.mode(WIFI_STA);
        // Initiate the WiFi connection
        WiFi.begin(getArrowheadESPFS().getNetInfo().ssid, getArrowheadESPFS().getNetInfo().password);

        _chrono.restart();
        // Wait until not connected
        while (WiFi.status() != WL_CONNECTED && !_chrono.hasPassed(10000)) {
            delay(500);
            Serial.print(".");
        }

        if (WiFi.status() == WL_CONNECTED) {
            debugPrintln("");
            debugPrintln("WiFi connected");
            debugPrint("IP address: ");
            debugPrintln(WiFi.localIP());
            _reconnectAttempt = 0;
            return true;
        } else {
            debugPrintln(String("\nCould not connect to: ") + getArrowheadESPFS().getNetInfo().ssid);
            debugPrintln();
            _reconnectAttempt++;
        }
    }
    _reconnectAttempt = 0;
    return false;
}

bool ArrowheadESP::setupCertificates() {
    // Set up the NTPClient. It's constructor needs an UDP client. If somebody has a better solution then declaring it here, let me know.

    // By default 'pool.ntp.org' is used with 60 seconds update interval and
    // no offset
    NTPClient timeClient(_ntpUDP);
    timeClient.begin();
    while (!timeClient.update()) {
        timeClient.forceUpdate();
    }
    // Set the proper time for Certificate validation
    getArrowheadHTTPSClient().getWiFiClientSecure().setX509Time(timeClient.getEpochTime());
    // Setting the request timeout
    getArrowheadHTTPSClient().getWiFiClientSecure().setTimeout(5000);
    // Setting the buffer sizes
    getArrowheadHTTPSClient().getWiFiClientSecure().setBufferSizes(512, 512);

    // Disable X509 Certificate verification
    if (getArrowheadESPFS().getSSLInfo().insecure) {
        getArrowheadHTTPSClient().getWiFiClientSecure().setInsecure();
        debugPrintln("Disabled CA verification");
    }

    if(getArrowheadESPFS().loadClientCertificateFiles()) {

        // Load CA certificate
        if (getArrowheadHTTPSClient().getWiFiClientSecure().loadCACert(getArrowheadESPFS().getCA())) {
            debugPrintln("CA cert loaded");
        } else {
            debugPrintln("CA cert failed");
        }
        //delay(1000);

        // Load Client certificate
        if (getArrowheadHTTPSClient().getWiFiClientSecure().loadCertificate(getArrowheadESPFS().getCl())) {
            debugPrintln("Client cert loaded");
        } else {
            debugPrintln("Client cert failed");
        }
        //delay(1000);

        // Load Private key
        if (getArrowheadHTTPSClient().getWiFiClientSecure().loadPrivateKey(getArrowheadESPFS().getPK())) {
            debugPrintln("Private key loaded");
        } else {
            debugPrintln("Private key failed");
        }

        // close client certificate files
        getArrowheadESPFS().closeClientCertificateFiles();

    }
    else {
        Serial.println("Client certificate files could not be loaded.");
    }

    delay(1000);
}

bool ArrowheadESP::setupSecureWebServer() {
    if(getArrowheadESPFS().loadServerCertificateFiles()) { // starting in SECURE mode

        static char* serverCert;
        static char* serverKey;
        unsigned int fileSize;

        fileSize = getArrowheadESPFS().getSc().size(); 
        serverCert = (char*)malloc(fileSize + 1);
        getArrowheadESPFS().getSc().readBytes(serverCert, fileSize);
        serverCert[fileSize] = '\0';

        fileSize = getArrowheadESPFS().getSk().size(); 
        serverKey = (char*)malloc(fileSize + 1);
        getArrowheadESPFS().getSk().readBytes(serverKey, fileSize);
        serverKey[fileSize] = '\0';

        // close server certificate files
        getArrowheadESPFS().closeServerCertificateFiles();

        getSecureWebServer().getServer().setRSACert(new BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));

        free(serverCert);
        free(serverKey);

        Serial.println("SECURE web server is configured.");

    }
    else {
       Serial.println("Server certificate files could not be loaded."); 
    }
}

// #######################################
// Public functions
// #######################################

ArrowheadESPFS &ArrowheadESP::getArrowheadESPFS() {
    return _arrowheadEspFs;
}

ArrowheadHTTPSClient &ArrowheadESP::getArrowheadHTTPSClient() {
    return _httpsClient;
}

BearSSL::ESP8266WebServerSecure &ArrowheadESP::getSecureWebServer() {
    return _secureWebServer;
}

ESP8266WebServer &ArrowheadESP::getWebServer() {
    return _webServer;
}

String ArrowheadESP::getIP() {
    return WiFi.localIP().toString();
}

void ArrowheadESP::setServiceRegistryAddress(const char *host, int port) {
    this->_srHost = host;
    this->_srPort = port;
}

int ArrowheadESP::serviceRegistryEcho() {
    return getArrowheadHTTPSClient().get(_srHost, _srPort, "/serviceregistry/echo", NULL, NULL);
}

int ArrowheadESP::serviceRegistryEcho(String *response) {
    return getArrowheadHTTPSClient().get(_srHost, _srPort, "/serviceregistry/echo", NULL, response);
}

int ArrowheadESP::serviceRegistryQuery(const char *body) {
    return getArrowheadHTTPSClient().post(_srHost, _srPort, "/serviceregistry/query", body);
}

int ArrowheadESP::serviceRegistryQuery(const char *body, String *response) {
    return getArrowheadHTTPSClient().post(_srHost, _srPort, "/serviceregistry/query", body, response);
}

int ArrowheadESP::serviceRegistryRegister(const char *body) {
    return getArrowheadHTTPSClient().post(_srHost, _srPort, "/serviceregistry/register", body);
}

int ArrowheadESP::serviceRegistryRegister(const char *body, String *response) {
    return getArrowheadHTTPSClient().post(_srHost, _srPort, "/serviceregistry/register", body, response);
}

int ArrowheadESP::serviceRegistryUnregister(const char *systemName, int port, const char *serviceDefinition) {
    String query = String("?system_name=") + systemName + "&address=" + WiFi.localIP().toString() + "&port=" + port + "&service_definition=" +
                   serviceDefinition;
    return getArrowheadHTTPSClient().del(_srHost, _srPort, "/serviceregistry/unregister", query.c_str());
}

int ArrowheadESP::serviceRegistryUnregister(const char *systemName, int port, const char *serviceDefinition, String *response) {
    String query = String("?system_name=") + systemName + "&address=" + WiFi.localIP().toString() + "&port=" + port + "&service_definition=" +
                   serviceDefinition;
    return getArrowheadHTTPSClient().del(_srHost, _srPort, "/serviceregistry/unregister", query.c_str(), response);
}

bool ArrowheadESP::begin() {
    debugPrintln("ArrowheadESP - Begin");
    // Cannot proceed without WiFi connection
    if (!setupWiFi()) {
        return false;
    }
    //delay(1000);
    setupCertificates();

    if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }

    return true;
}

void ArrowheadESP::useSecureWebServer() {
    if(setupSecureWebServer()) {
        Serial.println("Using HTTPS web server.");
    }
}

int ArrowheadESP::loop() {
    // We have to check that everything is available before proceeding
    if (WiFi.status() != WL_CONNECTED) {
         return setupWiFi();
    }
    getSecureWebServer().handleClient();
    getWebServer().handleClient();
    MDNS.update();
    return -1;
}