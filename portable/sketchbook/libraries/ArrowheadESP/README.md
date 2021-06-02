# ArrowheadESP

The goal of this library is to make interfacing with the Arrowhead Framework easy with the ESP8266 module.

Please take a look at the examples included with this library to get an idea of how it works.

## Note:

The library does require the use of these libraries, so make sure they're installed as well:
* [Chrono](https://github.com/SofaPirate/Chrono)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [NTPClient](https://github.com/arduino-libraries/NTPClient)

These libraries can be installed via the Library Manager. Since Arduino IDE 1.8.10, it can automatically install them. 

In addition to those libraries, make sure that you have the ESP core files installed for your platform.

* [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)

## Generate certificate

First you should generate a `p12` certificate [based on this guide](https://github.com/arrowhead-f/core-java-spring/blob/master/documentation/certificates/create_client_certificate.pdf)
Then generate `der` certificates from it with these example commands:
```
openssl pkcs12 -in securetemperaturesensor.testcloud2.aitia.arrowhead.eu.p12 -out cacert.pem  -cacerts -nokeys
openssl pkcs12 -in securetemperaturesensor.testcloud2.aitia.arrowhead.eu.p12 -out clcert.pem  -clcerts -nokeys
openssl pkcs12 -in securetemperaturesensor.testcloud2.aitia.arrowhead.eu.p12 -out privkey.pem -nocerts
openssl pkcs12 -in securetemperaturesensor.testcloud2.aitia.arrowhead.eu.p12 -nocerts -out serverkey.pem
openssl rsa -in privkey.pem -pubout -out pubkey.pem

openssl x509 -in clcert.pem -out cert.der -outform DER 
openssl rsa -in privkey.pem -out private.der -outform DER
openssl x509 -in cacert.pem -out ca.der -outform DER
```

Copy the contents of the *pubkey.pem* file to the *sslConfig.json* file (located in the *data* directory) as it is demonstrated in the example config file (as the value of the *publicKey* field).

The library will use the following certificate files: *ca.der*, *cert.der*, *private.der*, *clcert.pem*, *serverkey.pem*. The remaining can be deleted after the generation process.

## Configure adapter

Before connecting to an Arrowhead Cloud instance, you need to specify the address of the cloud and give a name to your adapter which will identify the Arduino device in the cloud. Both can be done in the *providerConfig.json* file (located in the *data* directory).

For a use-case example please refer to the following repository: [adapter-quick-demo-dev](https://github.com/arrowhead-f/adapter-quick-demo-dev)

This library uses ideas derived from: 
* https://github.com/ItKindaWorks/ESPHelper
* https://github.com/Hal9k-dk/esp8266-restclient
