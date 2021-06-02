//
// Created by Szvetlin Tanyi on 2020. 04. 09.
//

#ifndef ARROWHEADESP_ARROWHEADHTTPSCLIENT_H
#define ARROWHEADESP_ARROWHEADHTTPSCLIENT_H

// Library includes
#include <WiFiClientSecure.h>
#include "../Util/Util.h"

class ArrowheadHTTPSClient {
private:
    /**
     * SSL capable HTTP client
     */
    WiFiClientSecure _wiFiClientSecure;
public:
    /**
     * Default constructor of the class
     */
    ArrowheadHTTPSClient();

    /**
    * Returns the instance of the HTTP client
    *
    * @return
    */
    WiFiClientSecure& getWiFiClientSecure();

    int request(const char* method, const char* host, int port, const char* path, const char* query, const char* body, String* response);
    int readResponse(String* response);

    int get(const char* address, int port, const char* path, const char* query);
    int get(const char* address, int port, const char* path, const char* query, String* response);

    int post(const char* address, int port, const char* path, const char* body);
    int post(const char* address, int port, const char* path, const char* body, String* response);

    int del(const char* address, int port, const char* path, const char* query);
    int del(const char* address, int port, const char* path, const char* query, String* response);

    // TODO patch, put
};


#endif //ARROWHEADESP_ARROWHEADHTTPSCLIENT_H
