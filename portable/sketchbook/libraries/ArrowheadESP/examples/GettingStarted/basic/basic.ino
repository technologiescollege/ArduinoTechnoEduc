/*
 * ArrowheadESP Basic example by Szvetlin Tanyi <szvetlin@aitia.ai>
 * Connects to local network. 
 * Loads SSL certificates, then registers a temperature service in Service Registry.
 *
 */

 #include <ArrowheadESP.h>

ArrowheadESP Arrowhead;

const char* systemName = "securetemperaturesensor"; // name of the system, must match the common name of the certificate
int port = 8080; // doesn't really matter what number we type in, since it won't listen on it anyway
const char* publicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAubjry6ja1CZUGtRV/0WX" \
"C37s+mWgWJPtqJxMzaQPP+geZVlnm9QlDBAM+UW5Kjc1BWUPfCrVK6hAmHJQj7T2" \
"v76/+uV+E8vkg4bFjmwutUENItaZGivxf8Fy1lEEGs0168w1YEvyvrljtaK6Vu3O" \
"/Yum/KWHf2sGOEJv5xRTZy0HcfIDxXlXGEK43klrcrMAgp/AT59QosBh5zpyvK2K" \
"hxeO44pFt+EwYtgFYQ2t0gZWnWzHt1e6Hj5/80SAJvWL8IUHcWNzc3BFXfjn503Q" \
"kbLsSWnFC2uwi5tMdgiZd1zTSYrmkDfr4AMNZO8RxC0bCaM3MxUl+SnSLiPyO6yU" \
"PwIDAQAB";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Arrowhead.getArrowheadESPFS().loadConfigFile("netConfig.json"); // loads network config from file system
  Arrowhead.getArrowheadESPFS().loadSSLConfigFile("sslConfig.json"); // loads ssl config from file system
  Arrowhead.getArrowheadESPFS().loadProviderConfigFile("providerConfig.json"); // loads provider config from file system
  //Arrowhead.useSecureWebServer(); // call secure configuration if you plan to use secure web server

  // Set the Address and port of the Service Registry.
  Arrowhead.setServiceRegistryAddress(
    Arrowhead.getArrowheadESPFS().getProviderInfo().serviceRegistryAddress,
    Arrowhead.getArrowheadESPFS().getProviderInfo().serviceRegistryPort
  );

  bool startupSuccess = Arrowhead.begin(); // true of connection to WiFi and loading Certificates is successful
  if(startupSuccess){
    String response = "";
    int statusCode = Arrowhead.serviceRegistryEcho(&response);
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
    Serial.print("Response body from server: ");
    Serial.println(response);
    Serial.println("############################");
    Serial.println();

    String serviceRegistryEntry = "{\"endOfValidity\":\"2020-12-05 12:00:00\",\"interfaces\":[\"HTTP-SECURE-SenML\"],\"providerSystem\":{\"address\":\" "+ Arrowhead.getIP() +"\",\"authenticationInfo\":\""+ publicKey +"\",\"port\":"+ port +",\"systemName\":\""+ systemName +"\"},\"secure\":\"CERTIFICATE\",\"serviceDefinition\":\"temperature\",\"serviceUri\":\"/\",\"version\":1}";  

    response = "";
    statusCode = Arrowhead.serviceRegistryRegister(serviceRegistryEntry.c_str(), &response);
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
    Serial.print("Response body from server: ");
    Serial.println(response);
    Serial.println("############################");
    Serial.println();
    }
} 


void loop() {
  Arrowhead.loop(); // keep network connection up
  // put your main code here, to run repeatedly:

  yield();
}
