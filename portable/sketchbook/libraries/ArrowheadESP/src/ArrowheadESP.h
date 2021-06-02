//
// Created by Szvetlin Tanyi <szvetlin@aitia.ai> on 2020. 04. 06.
//

#ifndef ARROWHEADESP_ARROWHEADESP_H
#define ARROWHEADESP_ARROWHEADESP_H

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266mDNS.h>
#endif

#ifdef ESP32
// TODO
#endif

// Library includes
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Chrono.h>

// Header Includes
#include "ArrowheadESPFS/ArrowheadESPFS.h"
#include "ArrowheadHTTPSClient/ArrowheadHTTPSClient.h"

class ArrowheadESP {
private:
    /**
     * ArrowheadESPFS instance
     */
    ArrowheadESPFS _arrowheadEspFs;

    /**
     * ArrowheadHTTPSClient instance
     */
    ArrowheadHTTPSClient _httpsClient;

    /**
     * ESP8266WebServerSecure instance
     */
    BearSSL::ESP8266WebServerSecure _secureWebServer;

    /**
     * ESP8266WebServer instance
     */
    ESP8266WebServer _webServer;

    /**
     * ServiceRegistry host and port
     */
    const char* _srHost;
    int _srPort;

    /**
     * Reconnect properties
     */
    int _maxReconnectAttempts = 3;
    int _reconnectAttempt = 0;

    /**
     * UDP capability
     */
    WiFiUDP _ntpUDP;
    // NTPClient timeClient(ntpUDP); not possible :(

    /**
     * Chronometer
     */
     Chrono _chrono;
    /**
     * Sets up the connection to the WiFi network
     *
     * @return success of the connection attempt
     */
    bool setupWiFi();
    /**
     * Sets up the certificates to the HTTP client
     *
     * @return
     */
    bool setupCertificates();
    /**
     * Sets up the secure web server with certificates
     *
     * @return
     */
    bool setupSecureWebServer();
public:
    /**
     * Default constructor of the library
     */
    ArrowheadESP();

    /**
     * Returns the instance of the ArrowheadESPFS
     *
     * @return
     */
    ArrowheadESPFS& getArrowheadESPFS();

    /**
     * Returns the instance of the ArrowheadHTTPSClient
     *
     * @return
     */
    ArrowheadHTTPSClient& getArrowheadHTTPSClient();

    /**
     * Returns the instance of the ESP8266WebServerSecure
     *
     * @return
     */
    BearSSL::ESP8266WebServerSecure& getSecureWebServer();

    /**
     * Returns the instance of the ESP8266WebServer
     *
     * @return
     */
    ESP8266WebServer& getWebServer();

    /**
     * Use secure web server
     *
     * @return
     */
    void useSecureWebServer();

    /**
     * Returns the current IP address
     * @return
     */
    String getIP();

    /**
     * Sets the host and port of the Service Registry
     *
     * @param host
     * @param port
     */
    void setServiceRegistryAddress(const char *host, int port);

    /**
     * ServiceRegistry Service implementations
     */

    /**
     * Echo
     * @return
     */
    int serviceRegistryEcho();
    /**
     * Echo
     *
     * @param response
     * @return
     */
    int serviceRegistryEcho(String* response);
    /**
     * Query
     *
     * @param body
     * @return
     */
    int serviceRegistryQuery(const char *body);
    /**
     * Query
     *
     * @param body
     * @param response
     * @return
     */
    int serviceRegistryQuery(const char *body, String* response);
    /**
     * Register
     *
     * @param body
     * @return
     */
    int serviceRegistryRegister(const char *body);
    /**
     * Register
     *
     * @param body
     * @param response
     * @return
     */
    int serviceRegistryRegister(const char *body, String* response);
    /**
     * Unregister
     *
     * @param serviceDefinition
     * @return
     */
    int serviceRegistryUnregister(const char *systemName, int port, const char *serviceDefinition);
    /**
     * Unregister
     *
     * @param serviceDefinition
     * @param response
     * @return
     */
    int serviceRegistryUnregister(const char *systemName, int port, const char *serviceDefinition, String* response);

    /**
     * Starts the operation of the library
     *
     * @return
     */
    bool begin();

    /**
     * Keeps all connection alive
     *
     * @return
     */
    int loop();
};


#endif //ARROWHEADESP_ARROWHEADESP_H
