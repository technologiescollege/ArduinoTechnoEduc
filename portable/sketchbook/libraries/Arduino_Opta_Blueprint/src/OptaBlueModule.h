/* -------------------------------------------------------------------------- */
/* FILENAME:    OptaBlueModule.h
   AUTHOR:      Daniele Aimo (d.aimo@arduino.cc)
   DATE:        20230801
   REVISION:    0.0.1
   DESCRIPTION: This is the header file for the Module class
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef OPTA_BLUE_MODULE
#define OPTA_BLUE_MODULE

#ifndef ARDUINO_OPTA

#include "Arduino.h"
#include "OptaBluePrintCfg.h"
#include "OptaCrc.h"
#include "OptaModuleProtocol.h"
#include "OptaMsgCommon.h"
#include "Wire.h"
#include <stdint.h>
#include <string>
// #define DEBUG_SERIAL
// #define DEBUG_RX_MODULE_ENABLE
/* when DETECT IN goes low it wait OPTA_CONTROLLER_DEBOUNCE_TIME *
   OPTA_CONTROLLER_DEBOUNCE_NUMBER ms before to consider the PIN really low */
#define OPTA_MODULE_DEBOUNCE_TIME_IN 1
#define OPTA_MODULE_DEBOUNCE_NUMBER_IN 50
#define OPTA_MODULE_DEBOUNCE_TIME_OUT 1
#define OPTA_MODULE_DEBOUNCE_NUMBER_OUT 100
/* time the DETECT IN is kept LOW after reset to signal other expansions
   that an expansion on the right it'exiting from reset and so a new
   assign addresses process will be take place soon */
#define OPTA_MODULE_DETECT_OUT_LOW_TIME 1000

#define WAIT_FOR_REBOOT 500

class Module {
public:
  Module();
  Module(TwoWire *tw, int _detect_in, int _detect_out);
  virtual void begin();
  virtual bool addressAcquired();
  virtual void update();
  virtual void end();
  /* must be public because is called in the RX callback */
  virtual int parse_rx();

  virtual uint8_t getMajorFw() = 0;
  virtual uint8_t getMinorFw() = 0;
  virtual uint8_t getReleaseFw() = 0;
  virtual std::string getProduct() = 0;
  virtual void goInBootloaderMode() = 0;
  virtual void readFromFlash(uint16_t add, uint8_t *buffer, uint8_t dim) = 0;
  virtual void writeInFlash(uint16_t add, uint8_t *buffer, uint8_t dim) = 0;

  virtual void initStatusLED() = 0;
  virtual void setStatusLedReadyForAddress() = 0;
  virtual void setStatusLedWaitingForAddress() = 0;
  virtual void setStatusLedHasAddress() = 0;

  static TwoWire *expWire;

  /* set the tx_buffer @ position pos with value v */
  void tx(uint8_t v, int pos);
  /* get value of tx_buffer @ position pos */
  uint8_t tx(int pos);
  /* get value of rx buffer @ position pos */
  uint8_t rx(int pos);
  /* set rx_buffer @ position pos with value v */
  void rx(uint8_t v, int pos);
  /* set the number of bytes received into the rx_buffer */
  void setRxNum(uint8_t n);
  /* get the number of bytes received into the rx_buffer */
  uint8_t getRxNum();
  /* set the number of bytes to be transmitted in the tx_buffer */
  void setTxNum(int8_t n);
  /* get the number of bytes to be transmitted in the tx_buffer */
  int8_t getTxNum();
  /* returns the pointer to the tx buffer */
  uint8_t *txPrt();

  void setRebootSent() { reboot_sent = millis(); }
  #ifdef USE_CONFIRM_RX_MESSAGE
  bool parse_confirm_address_rx();
  #endif
  bool parse_set_address();
  bool parse_get_address_and_type();
  bool parse_reset_controller();
  bool parse_get_version();
  bool parse_reboot();
  bool parse_set_flash();
  bool parse_get_flash();
  bool parse_get_product();
  int prepare_ans_get_product();
  int prepare_ans_get_address_and_type();
  int prepare_ans_get_version();
  int prepare_ans_reboot();
  int prepare_ans_get_flash();
  
  uint8_t getI2CAddress() { return wire_i2c_address; }

protected:
  volatile uint8_t rx_i2c_address = OPTA_DEFAULT_SLAVE_I2C_ADDRESS;
  uint8_t wire_i2c_address = OPTA_DEFAULT_SLAVE_I2C_ADDRESS;
  uint8_t rx_num;
  volatile bool reboot_required;
  volatile bool reset_required;
  #ifdef USE_CONFIRM_RX_MESSAGE
  volatile bool confirm_address_reception = false;
  #endif
  
  int expansion_type;
  unsigned long int reboot_sent;
  int detect_in;
  int detect_out;
  void updatePinStatus();

  /* handle "reset": reset here means
     1. true reset from power up
     2. reset due to reset message from controller
     3. reset due digital out of a controller on the left that goes LOW */
  virtual void reset();

  bool is_detect_in_low();

  bool is_detect_out_low();
  bool is_detect_out_high();

  void setAddress(uint8_t add);
  /* this variable need to be set in every constructor of the derived class */
  uint8_t rx_buffer[OPTA_I2C_BUFFER_DIM];
  uint8_t tx_buffer[OPTA_I2C_BUFFER_DIM];
  int8_t tx_num;
  uint16_t flash_add;
  uint8_t flash_dim;

  volatile bool set_address_msg_received;
  /* USE this in custom expansion to know when the address of the expansion
     has been set */
  bool address_set_up(bool reset = true) {
      bool rv = set_address_msg_received;
      if(reset){
        set_address_msg_received = false;
      }
      return rv;
  }
};

extern Module *OptaExpansion;

#endif
#endif
