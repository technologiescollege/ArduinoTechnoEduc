/* -------------------------------------------------------------------------- */
/* FILENAME:
   AUTHOR:      Daniele Aimo (d.aimo@arduino.cc)
   DATE:        20230801
   REVISION:    0.0.1
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */
#include "OptaModuleProtocol.h"
#include <cstring>
#ifndef ARDUINO_OPTA

#include "OptaBlueModule.h"

/* This definition are used to avoid compilation error when using this file in a
 * Custom expansion library. PLEASE NOTE that the value have NO SENSE since the
 * Custom expansion library must always use the Module constructor with
 * arguments that specify the correct DETECT_IN and DETECT_OUT pin, so that the
 * following defines are completely irrelevant */

#ifndef DETECT_IN
#define DETECT_IN 0
#endif

#ifndef DETECT_OUT
#define DETECT_OUT 0
#endif

Module *OptaExpansion = nullptr;

TwoWire *Module::expWire = nullptr;

/* 20240515_moved_I2C_reset moved reset I2C to main */
volatile bool reset_I2C_bus = false;

#define NACK_ANSWER_LEN (2)
static uint8_t nack_answer[NACK_ANSWER_LEN] = {0xFA, 0xFE};

/* -------------------------------------------------------------------------- */
/* Module RX event: callback called when Module receive from Controller on I2C*/
/* -------------------------------------------------------------------------- */
void receive_event(int n) {

#if defined DEBUG_SERIAL && defined DEBUG_RX_MODULE_ENABLE
  Serial.println();
  Serial.print("- start RX: ");
#endif

  if (OptaExpansion != nullptr && Module::expWire != nullptr) {
    OptaExpansion->setRxNum(0);
    for (int i = 0; i < n && i < OPTA_I2C_BUFFER_DIM; i++) {
      int r = Module::expWire->read();

#if defined DEBUG_SERIAL && defined DEBUG_RX_MODULE_ENABLE
      Serial.print(r, HEX);
      Serial.print(" ");
#endif

      OptaExpansion->rx((uint8_t)r, i);
      OptaExpansion->setRxNum(i + 1);
    }
#if defined DEBUG_SERIAL && defined DEBUG_RX_MODULE_ENABLE
    Serial.println("[end RX]");
#endif
    /* parse received message */
    OptaExpansion->setTxNum(OptaExpansion->parse_rx());
  }
}

/* -------------------------------------------------------------------------- */
/* Module Request event: callback called when Module receive a request from I2C
   Controller                                                                 */
/* -------------------------------------------------------------------------- */
void request_event() {
#if defined DEBUG_SERIAL && defined DEBUG_TX_MODULE_ENABLE
  Serial.println();
  Serial.print("- start TX: ");
  for (int i = 0; i < OptaExpansion->getTxNum(); i++) {
    Serial.print(*(OptaExpansion->txPrt() + i), HEX);
    Serial.print(" ");
  }
#endif
  if (OptaExpansion != nullptr && 
      Module::expWire != nullptr && 
      OptaExpansion->getTxNum() > 0) {
    Module::expWire->write(OptaExpansion->txPrt(), OptaExpansion->getTxNum());
  }
  else {
    Module::expWire->write(nack_answer,NACK_ANSWER_LEN);
  }

  uint8_t *ck = OptaExpansion->txPrt();
  if (*(ck + BP_ARG_POS) == ARG_REBOOT) {
    OptaExpansion->setRebootSent();
  }

#if defined DEBUG_SERIAL && defined DEBUG_TX_MODULE_ENABLE
  Serial.println("[end TX]");
#endif
}

/* -------------------------------------------------------------------------- */
/* SETTER / GETTER functions                                                  */
/* -------------------------------------------------------------------------- */

/* set the tx_buffer @ position pos with value v */
void Module::tx(uint8_t v, int pos) {
  if (pos >= 0 && pos < OPTA_I2C_BUFFER_DIM) {
    tx_buffer[pos] = v;
  }
}

/* get value of tx_buffer @ position pos */
uint8_t Module::tx(int pos) {
  if (pos >= 0 && pos < OPTA_I2C_BUFFER_DIM) {
    return tx_buffer[pos];
  }
  return 0;
}

/* get value of rx buffer @ position pos */
uint8_t Module::rx(int pos) {
  if (pos >= 0 && pos < OPTA_I2C_BUFFER_DIM) {
    return rx_buffer[pos];
  }
  return 0;
}

/* set rx_buffer @ position pos with value v */
void Module::rx(uint8_t v, int pos) {
  if (pos >= 0 && pos < OPTA_I2C_BUFFER_DIM) {
    rx_buffer[pos] = v;
  }
}

/* set the number of bytes received into the rx_buffer */
void Module::setRxNum(uint8_t n) { rx_num = n; }
/* get the number of bytes received into the rx_buffer */
uint8_t Module::getRxNum() { return rx_num; }
/* set the number of bytes to be transmitted in the tx_buffer */
void Module::setTxNum(int8_t n) { tx_num = n; }
/* get the number of bytes to be transmitted in the tx_buffer */
int8_t Module::getTxNum() { return tx_num; }
/* returns the pointer to the tx buffer */
uint8_t *Module::txPrt() { return tx_buffer; }

/* -------------------------------------------------------------------------- */
/* Handle reset                                                               */
/* -------------------------------------------------------------------------- */
void Module::reset() {

  if (Module::expWire != nullptr) {
    Module::expWire->end();
  }
  #ifdef USE_CONFIRM_RX_MESSAGE
  confirm_address_reception = false;
  #endif

  setStatusLedWaitingForAddress();
  /* put address to invalid */
  wire_i2c_address = OPTA_DEFAULT_SLAVE_I2C_ADDRESS;
  rx_i2c_address = OPTA_DEFAULT_SLAVE_I2C_ADDRESS; 
  
  /* detect_in (toward Controller) as Output */
  pinMode(detect_in, OUTPUT);
  /* put detect in pin to LOW -> signal the MODULE wants an address */
  digitalWrite(detect_in, LOW);

  /* detect_out (toward other Module) as output*/
  pinMode(detect_out, OUTPUT);
  digitalWrite(detect_out, LOW);
  delay(OPTA_MODULE_DETECT_OUT_LOW_TIME);

  pinMode(detect_out, INPUT_PULLUP);

  while (is_detect_out_low()) {
    
  }
  
  /* put I2C address to the default one */
  if (Module::expWire != nullptr) {
    Module::expWire->begin(OPTA_DEFAULT_SLAVE_I2C_ADDRESS);
  }
  
}

/* --------------------------------------------------------------------------
 */
/* Module constructor (does nothing) */
/* --------------------------------------------------------------------------
 */
Module::Module()
    : rx_num(0), reboot_required(false),
      reset_required(false), 
      expansion_type(OPTA_CONTROLLER_CUSTOM_MIN_TYPE), reboot_sent(0),
      detect_in(DETECT_IN), detect_out(DETECT_OUT) {
  Module::expWire = &Wire;
}

Module::Module(TwoWire *tw, int _detect_in, int _detect_out)
    : rx_num(0),
      reboot_required(false), reset_required(false), 
      expansion_type(OPTA_CONTROLLER_CUSTOM_MIN_TYPE), reboot_sent(0),
      detect_in(_detect_in), detect_out(_detect_out) {
  Module::expWire = tw;
}
/* --------------------------------------------------------------------------
 */
/* Return true if the module has obtained an address from the controller */
/* --------------------------------------------------------------------------
 */
bool Module::addressAcquired() {
  return ((wire_i2c_address > OPTA_DEFAULT_SLAVE_I2C_ADDRESS));
}

/* --------------------------------------------------------------------------
 */
/* Module begin */
/* --------------------------------------------------------------------------
 */
void Module::begin() {
  
  initStatusLED();
#ifdef DEBUG_SERIAL
  Serial.begin(115200);
  delay(5000);
  // while(!Serial) { }
  Serial.println("[Log]: START");
#endif

  
  reset();

  /* Set up callbacks */
  if (Module::expWire != nullptr) {
    Module::expWire->onReceive(receive_event);
    Module::expWire->onRequest(request_event);
  }
}

/* --------------------------------------------------------------------------
 */
/* Module begin */
/* --------------------------------------------------------------------------
 */
void Module::update() {
  if(reset_I2C_bus) {
    /* 20240515_moved_I2C_reset moved reset I2C to main */
    pinMode(detect_out, INPUT_PULLUP);
    delay(1);
    /* accept the address only if detect out is HIGH */
    if (digitalRead(detect_out) == HIGH) {
      wire_i2c_address = rx_i2c_address;
      setAddress(wire_i2c_address);
    }
    reset_I2C_bus = false;
  }

  updatePinStatus();

  if (reset_required) {
    reset();
    reset_required = false;
  }

  if (reboot_required && reboot_sent > 0) {
    while ((millis() - reboot_sent) < WAIT_FOR_REBOOT) {
    }
    reboot_required = false;
    reboot_sent = 0;
    goInBootloaderMode();
  }

#if defined DEBUG_SERIAL && defined DEBUG_EXPANSION_PRINT_ADDRESS
  static unsigned long int start = millis();

  if (millis() - start > 1000) {
    start = millis();
    Serial.print("ADDRESS 0x");
    Serial.println(wire_i2c_address, HEX);
  }
#endif
}

#ifdef USE_CONFIRM_RX_MESSAGE
/* -------------------------------------------------------------------------- */
bool Module::parse_confirm_address_rx() {
  /* ------------------------------------------------------------------------ */
  if (checkSetMsgReceived(rx_buffer, ARG_CONFIRM_ADDRESS_RX, LEN_CONFIRM_ADDRESS_RX)) {
    if(rx_buffer[CONFIRM_ADDRESS_FIRST_POS] == CONFIRM_ADDRESS_FIRST_VALUE && 
      rx_buffer[CONFIRM_ADDRESS_SECOND_POS] == CONFIRM_ADDRESS_SECOND_VALUE) {
      confirm_address_reception = true;
    }
    return true;
  }
  return false;
}
#endif


/* -------------------------------------------------------------------------- */
bool Module::parse_set_address() {
  /* ------------------------------------------------------------------------ */
  if (checkSetMsgReceived(rx_buffer, ARG_ADDRESS, LEN_ADDRESS)) {
    rx_i2c_address = rx_buffer[BP_PAYLOAD_START_POS];
    set_address_msg_received = true;
    return true;
  }
  return false;
}

/* ------------------------------------------------------------------------ */
bool Module::parse_get_address_and_type() {
  /* ---------------------------------------------------------------------- */
  if (checkGetMsgReceived(rx_buffer, ARG_ADDRESS_AND_TYPE, LEN_ADDRESS_AND_TYPE)) {
    return true;
  }
  return false;
}

/* ------------------------------------------------------------------------ */
bool Module::parse_get_product() {
  /* ---------------------------------------------------------------------- */
  if (checkGetMsgReceived(rx_buffer, ARG_GET_PRODUCT_TYPE, LEN_GET_PRODUCT_TYPE)) {
    return true;
  }
  return false;
}
/* ------------------------------------------------------------------------ */
bool Module::parse_reset_controller() {
  /* ---------------------------------------------------------------------- */
  if (checkSetMsgReceived(rx_buffer, ARG_CONTROLLER_RESET, LEN_CONTROLLER_RESET)) {
    if (rx_buffer[BP_PAYLOAD_START_POS] == CONTROLLER_RESET_CODE) {
      return true;
    }
  }
  return false;
}

/* ------------------------------------------------------------------------ */
int Module::prepare_ans_get_product() {
/* ------------------------------------------------------------------------ */
  std::string pr(getProduct());
  memset(tx_buffer, 0x0, OPTA_I2C_BUFFER_DIM);
  tx_buffer[ANS_GET_PRODUCT_SIZE_POS] = pr.size();
  for (unsigned int i = 0; i < pr.size() && i < 32; i++) {
    tx_buffer[ANS_GET_PRODUCT_SIZE_POS + 1 + i] = pr[i];
  }
  int rv = prepareGetAns(tx_buffer, ANS_ARG_GET_PRODUCT_TYPE,ANS_LEN_GET_PRODUCT_TYPE);
  return rv;
}
/* ------------------------------------------------------------------------ */
int Module::prepare_ans_get_version() {
  /* ---------------------------------------------------------------------- */
  tx_buffer[GET_VERSION_MAJOR_POS] = getMajorFw();
  tx_buffer[GET_VERSION_MINOR_POS] = getMinorFw();
  tx_buffer[GET_VERSION_RELEASE_POS] = getReleaseFw();
  return prepareGetAns(tx_buffer, ANS_ARG_GET_VERSION, ANS_LEN_GET_VERSION);
}

/* ------------------------------------------------------------------------ */
bool Module::parse_get_version() {
  /* ----------------------------------------------------------------------
   */
  if (checkGetMsgReceived(rx_buffer, ARG_GET_VERSION, LEN_GET_VERSION)) {
    return true;
  }
  return false;
}

/* ------------------------------------------------------------------------ */
int Module::prepare_ans_reboot() {
  /* ----------------------------------------------------------------------
   */
  tx_buffer[ANS_REBOOT_CODE_POS] = ANS_REBOOT_CODE;
  return prepareSetAns(tx_buffer, ANS_ARG_REBOOT, ANS_LEN_REBOOT);
}

/* ------------------------------------------------------------------------ */
bool Module::parse_reboot() {
  /* ----------------------------------------------------------------------
   */
  if (checkSetMsgReceived(rx_buffer, ARG_REBOOT, LEN_REBOOT)) {
    if (rx_buffer[REBOOT_1_POS] == REBOOT_1_VALUE &&
        rx_buffer[REBOOT_2_POS] == REBOOT_2_VALUE) {
      return true;
    }
  }
  return false;
}

/* ------------------------------------------------------------------------ */
int Module::prepare_ans_get_address_and_type() {
  /* ----------------------------------------------------------------------
   */
  tx_buffer[BP_PAYLOAD_START_POS] = wire_i2c_address;
  tx_buffer[BP_PAYLOAD_START_POS + 1] = expansion_type;
  return prepareGetAns(tx_buffer, ANS_ARG_ADDRESS_AND_TYPE,ANS_LEN_ADDRESS_AND_TYPE);
}

__WEAK void new_i2c_address_obtained(void *ptr) { (void)ptr; }

/* ------------------------------------------------------------------------ */
/* Parse the content of the rx_buffer
   this function only consider the messages related to addressing process
   so that it return the size of answer to be transmitted or -1 if it cannot
   handle the message
   Please note that this function MUST be called in all derived classes in
   order to manage correctly the addressing process */
/* ------------------------------------------------------------------------ */
int Module::parse_rx() {
  /* ----------------------------------------------------------------------
   */
  /* set address message */
  if (parse_set_address()) {
    /* 20240515_moved_I2C_reset moved reset I2C to main */
    reset_I2C_bus = true;
    return 0;
  }
  /* get address and type message */
  else if (parse_get_address_and_type()) {
    int rv = prepare_ans_get_address_and_type();
    new_i2c_address_obtained(this);
    return rv;
  }
  #ifdef USE_CONFIRM_RX_MESSAGE
  else if(parse_confirm_address_rx()) {
    return 0;
  }
  #endif
  /* reset control message */
  else if (parse_reset_controller()) {
    reset_required = true;
    return 0;
  }
  /* get fw version */
  else if (parse_get_version()) {
    int rv = prepare_ans_get_version();
    return rv;
  }
  /* REBOOT --> update FW */
  else if (parse_reboot()) {
    int rv = prepare_ans_reboot();
    reboot_required = true;
    return rv;
  }
  /* FLASH WRITE */
  else if (parse_set_flash()) {
    return 0;
  }
  /* FLASH READ */
  else if (parse_get_flash()) {
    int rv = prepare_ans_get_flash();
    return rv;
  } else if (parse_get_product()) {
    int rv = prepare_ans_get_product();
    return rv;
  }

  return -1;
}

/* ------------------------------------------------------------------------ */
bool Module::is_detect_in_low() {
  /* ----------------------------------------------------------------------
   */
  pinMode(detect_in, INPUT_PULLUP);
  delay(1);
  PinStatus dtcin = digitalRead(detect_in);
  if (dtcin == LOW) {
    int num = 0;
    while (dtcin == LOW && num < OPTA_MODULE_DEBOUNCE_NUMBER_IN) {
      dtcin = digitalRead(detect_in);
      if (dtcin == HIGH) {
        break;
      }
      delay(OPTA_MODULE_DEBOUNCE_TIME_IN);
      num++;
    }
  }
  return (dtcin == LOW);
}

/* ------------------------------------------------------------------------ */
bool Module::is_detect_out_low() {
  /* ----------------------------------------------------------------------
   */

  pinMode(detect_out, INPUT_PULLUP);
  delay(1);
  PinStatus dtcout = digitalRead(detect_out);

  if (dtcout == LOW) {
    int num = 0;
    while ( dtcout == LOW && num < OPTA_MODULE_DEBOUNCE_NUMBER_OUT) {                   
      dtcout = digitalRead(detect_out);
      if (dtcout == HIGH) {
        break;
      }
      delay(OPTA_MODULE_DEBOUNCE_TIME_OUT);
      num++;
    }
  }
  return (dtcout == LOW);
}

/* ------------------------------------------------------------------------ */
bool Module::is_detect_out_high() {
  /* ----------------------------------------------------------------------
   */

  pinMode(detect_out, INPUT_PULLUP);
  delay(1);
  PinStatus dtcout = digitalRead(detect_out);

  int num = 0;
  while (dtcout == HIGH && num < OPTA_MODULE_DEBOUNCE_NUMBER_OUT) {
    dtcout = digitalRead(detect_out);
    if (dtcout == LOW) {
      break;
    }
    delay(OPTA_MODULE_DEBOUNCE_TIME_OUT);
    num++;
  }
  return (dtcout == HIGH);
}
/* ------------------------------------------------------------------------ */
void Module::updatePinStatus() {
/* ------------------------------------------------------------------------ */

#if defined DEBUG_SERIAL && defined DEBUG_UPDATE_PIN_ENABLE
  Serial.print("- UPDATE PIN ");
#endif

  if (!addressAcquired()) {
#if defined DEBUG_SERIAL && defined DEBUG_UPDATE_PIN_ENABLE
    Serial.println("ADDRESS not ACQUIRED");
#endif

    pinMode(detect_out, INPUT_PULLUP);
    if(digitalRead(detect_out) == LOW) {
      reset();
    }

    setStatusLedReadyForAddress();
    /* detect_in (toward Controller) as Output */
    pinMode(detect_in, OUTPUT);
    /* put detect in pin to LOW -> signal the MODULE wants an address */
    digitalWrite(detect_in, LOW);
  } else {
#if defined DEBUG_SERIAL && defined DEBUG_UPDATE_PIN_ENABLE
    Serial.print("ADDRESS ACQUIRED ");
    Serial.println(wire_i2c_address, HEX);
#endif
    setStatusLedHasAddress();
    #ifdef USE_CONFIRM_RX_MESSAGE
    if(confirm_address_reception) {
    #endif  
      if (is_detect_in_low()) {
        reset_required = true;
      } else if (is_detect_out_low()) {
        reset_required = true;
      } else {
        pinMode(detect_out, INPUT_PULLUP);
        digitalWrite(detect_in, HIGH);
      }
    #ifdef USE_CONFIRM_RX_MESSAGE  
    }
    #endif
  }
}

/* ------------------------------------------------------------------------ */
bool Module::parse_set_flash() {
  /* ---------------------------------------------------------------------- */
  if (checkSetMsgReceived(rx_buffer, ARG_SAVE_IN_DATA_FLASH,
                          LEN_SAVE_IN_DATA_FLASH)) {

    uint16_t add = rx_buffer[SAVE_ADDRESS_1_POS];
    add += (rx_buffer[SAVE_ADDRESS_2_POS] << 8);
    uint8_t d = rx_buffer[SAVE_DIMENSION_POS];

    writeInFlash(add, rx_buffer + SAVE_DATA_INIT_POS, 32);
    return true;
  } else {
    return false;
  }
}

/* ------------------------------------------------------------------------ */
bool Module::parse_get_flash() {
  /* ---------------------------------------------------------------------- */
  if (checkGetMsgReceived(rx_buffer, ARG_GET_DATA_FROM_FLASH,
                          LEN_GET_DATA_FROM_FLASH)) {
    flash_add = rx_buffer[READ_ADDRESS_1_POS];
    flash_add += (rx_buffer[READ_ADDRESS_2_POS] << 8);
    flash_dim = rx_buffer[READ_DATA_DIM_POS];
    return true;
  }
  return false;
}





/* ------------------------------------------------------------------------ */
int Module::prepare_ans_get_flash() {
  /* -------------------------------------------------------------------- */
  tx_buffer[ANS_GET_DATA_ADDRESS_1_POS] = (uint8_t)(flash_add & 0xFF);
  tx_buffer[ANS_GET_DATA_ADDRESS_2_POS] = (uint8_t)((flash_add & 0xFF00) >> 8);
  tx_buffer[ANS_GET_DATA_DIMENSION_POS] = flash_dim;

  readFromFlash(flash_add, tx_buffer + ANS_GET_DATA_DATA_INIT_POS, flash_dim);

  return prepareGetAns(tx_buffer, ANS_ARG_GET_DATA_FROM_FLASH,
                       ANS_LEN_GET_DATA_FROM_FLASH);
}
/* ------------------------------------------------------------------------ */
void Module::end() {}
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* Give the device a new slave address (add) */
/* ------------------------------------------------------------------------ */
void Module::setAddress(uint8_t add) {
  if (Module::expWire != nullptr) {
    Module::expWire->end();
    Module::expWire->begin(add);
  }
}
#endif
