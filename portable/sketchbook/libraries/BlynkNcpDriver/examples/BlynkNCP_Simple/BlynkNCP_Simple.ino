/* === CONFIGURATION ============= */

/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_FIRMWARE_BUILD_TIME     __DATE__ " " __TIME__

#define SerialDbg     Serial
//#define SerialNCP   Serial1

/* =============================== */

#include "NCP_Helpers.h"

void setup()
{
  SerialDbg.begin(115200);
  waitSerialConsole(SerialDbg);

  // Power-up NCP (if needed)
#if defined(ARDUINO_NANO_RP2040_CONNECT)
  pinMode(NINA_RESETN, OUTPUT);
  digitalWrite(NINA_RESETN, HIGH);
#endif

  if (!ncpSetupSerial()) {
    return;
  }

  const char* ncpFwVer = "unknown";
  if (rpc_blynk_getNcpVersion(&ncpFwVer)) {
    SerialDbg.print(F("NCP firmware: "));
    SerialDbg.println(ncpFwVer);
  }

  // Setup the indicator LED, user button (if needed)
#if defined(ARDUINO_NANO_RP2040_CONNECT)
  rpc_hw_initRGB(27, 25, 26, true);
  rpc_hw_setLedBrightness(128);
#elif defined(BLYNK_NCP_TYPE_WITTY_CLOUD)
  rpc_hw_initUserButton(4, true);
  rpc_hw_initRGB(15, 12, 13, false);
#else
  //rpc_hw_initUserButton(0, true);
  //rpc_hw_initLED(19, false);
  //rpc_hw_setLedBrightness(160);
#endif

  // Set config mode timeout to 30 minutes, for testing purposes
  rpc_blynk_setConfigTimeout(30*60);

  // Provide MCU firmware info.
  // This info is mainly used for the Primary MCU OTA updates
  rpc_blynk_setFirmwareInfo(BLYNK_FIRMWARE_TYPE,
                            BLYNK_FIRMWARE_VERSION,
                            BLYNK_FIRMWARE_BUILD_TIME,
                            BLYNK_RPC_LIB_VERSION);

  // White labeling (use this ONLY if you have a branded Blynk App)
  //rpc_blynk_setVendorPrefix("MyCompany");
  //rpc_blynk_setVendorServer("dashboard.mycompany.com");

  // Product setup
  if (!rpc_blynk_initialize(BLYNK_TEMPLATE_ID, BLYNK_TEMPLATE_NAME)) {
    SerialDbg.println(F("rpc_blynk_initialize failed"));
  }
}

void sendPeriodicMessage()
{
  static uint32_t last_change = millis();
  if (millis() - last_change > 10000) {
    last_change += 10000;

    // Send a value to Blynk Virtual Pin 1
    virtualWrite(1, millis());
  }
}

void loop()
{
  rpc_run();

  sendPeriodicMessage();
}

// Handle Blynk Virtual Pin value updates
void rpc_client_blynkVPinChange_impl(uint16_t vpin, buffer_t param)
{
    // NOTE: we could copy the buffer, but we use 0-copy instead
    // But we need to 0-terminate it, overwriting the CRC8
    param.data[param.length] = '\0';

    SerialDbg.print(F("Got data on Virtual Pin "));
    SerialDbg.println(vpin);

    // Param format. Most values will be plain strings: "Hello world", "1234", "123.456", etc.
    // However, sometimes the value contains multiple items (an array). In this case, the values are separated using a 0x00 byte, i.e:
    // "First\0Second\0Third"
}

// Define the callback for the NCP state change event
void rpc_client_blynkStateChange_impl(uint8_t state)
{
  SerialDbg.print(F("NCP state: "));
  SerialDbg.println(ncpGetStateString(state));
  if ((RpcBlynkState)state == BLYNK_STATE_CONNECTED) {
    // Send a value to Virtual Pin 1
    virtualWrite(1, "hello world!");
  }
}

// Handle various NCP events
void rpc_client_processEvent_impl(uint8_t event)
{
    switch ((RpcEvent)event) {
    case RPC_EVENT_NCP_REBOOTING:
      SerialDbg.println(F("NCP is rebooting. TODO: reinitialize NCP"));
      break;
    case RPC_EVENT_HW_USER_CLICK:      break;
    case RPC_EVENT_HW_USER_DBLCLICK:   break;
    case RPC_EVENT_HW_USER_LONGPRESS:  break;
    case RPC_EVENT_HW_USER_CONFIGRESET: break;
    case RPC_EVENT_BLYNK_PROVISIONED:  break;
    case RPC_EVENT_BLYNK_TIME_SYNC:    break;
    case RPC_EVENT_BLYNK_TIME_CHANGED: break;
    default: break;
    }
}

