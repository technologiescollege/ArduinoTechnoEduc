//
// Created by Szvetlin Tanyi on 2020. 04. 09.
//

#include "ArrowheadHTTPSClient.h"

// #######################################
// Constructors
// #######################################

ArrowheadHTTPSClient::ArrowheadHTTPSClient() {
    debugPrintln("ArrowheadHTTPSClient Default Constructor");
}

// #######################################
// Public functions
// #######################################

WiFiClientSecure &ArrowheadHTTPSClient::getWiFiClientSecure() {
    return _wiFiClientSecure;
}

int ArrowheadHTTPSClient::get(const char *address, int port, const char *path, const char *query) {
    return request("GET", address, port, path, query, NULL, NULL);
}

int ArrowheadHTTPSClient::get(const char *address, int port, const char *path, const char *query, String *response) {
    return request("GET", address, port, path, query, NULL, response);
}

int ArrowheadHTTPSClient::post(const char *address, int port, const char *path, const char *body) {
    return request("POST", address, port, path, NULL, body, NULL);
}

int ArrowheadHTTPSClient::post(const char *address, int port, const char *path, const char *body, String *response) {
    return request("POST", address, port, path, NULL, body, response);
}

int ArrowheadHTTPSClient::del(const char *address, int port, const char *path, const char *query) {
    return request("DELETE", address, port, path, query, NULL, NULL);
}

int ArrowheadHTTPSClient::del(const char *address, int port, const char *path, const char *query, String *response) {
    return request("DELETE", address, port, path, query, NULL, response);
}

int ArrowheadHTTPSClient::request(const char *method, const char *host, int port, const char *path,
                                  const char *query, const char *body, String *response) {
    if (!_wiFiClientSecure.connect(host, port)) {
        debugPrintln(String("Connection failed: ") + host + ":" + String(port));
        return 0;
    }

    debugPrintln(String("Connection successful: ") + host + ":" + String(port));

    String pathWithQueryParams = path;
    if(query) {
       pathWithQueryParams.concat(query);
    }

    String request = String(method) + " " + pathWithQueryParams + " HTTP/1.1\r\n";
/*  TODO header support
    for(int i=0; i<num_headers; i++){
        request += String(headers[i]) + "\r\n";
    }*/
    request += "Host: " + String(host) + ":" + String(port) + "\r\n";
    request += "Connection: close\r\n";
    if (body != NULL) {
        char contentLength[30];
        sprintf(contentLength, "Content-Length: %d\r\n", strlen(body));
        request += String(contentLength);

        request += "Content-Type: application/json\r\n";
    }
    request += "\r\n";

    if (body != NULL) {
        request += String(body);
        request += "\r\n\r\n";
    }

    _wiFiClientSecure.print(request);

    //make sure you write all those bytes.
    //delay(1000);

    int statusCode = readResponse(response);


    //cleanup
    // TODO num_headers = 0;
    _wiFiClientSecure.stop();
    // delay(50);

    return statusCode;
}

int ArrowheadHTTPSClient::readResponse(String *response) {
    // an http request ends with a blank line
    debugPrintln("READING response");
    boolean currentLineIsBlank = true;
    boolean httpBody = false;
    boolean inStatus = false;

    char statusCode[4];
    int i = 0;
    int code = 0;

    while (_wiFiClientSecure.connected()) {
        if (_wiFiClientSecure.available()) {
            char c = _wiFiClientSecure.read();

            if (c == ' ' && !inStatus) {
                inStatus = true;
            }

            if (inStatus && i < 3 && c != ' ') {
                statusCode[i] = c;
                i++;
            }
            if (i == 3) {
                statusCode[i] = '\0';
                code = atoi(statusCode);
            }

            if (httpBody) {
                //only write response if its not null
                if (response != NULL) response->concat(c);
            } else {
                if (c == '\n' && currentLineIsBlank) {
                    httpBody = true;
                }

                if (c == '\n') {
                    // your starting a new line
                    currentLineIsBlank = true;
                } else if (c != '\r') {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
    }
    return code;
}