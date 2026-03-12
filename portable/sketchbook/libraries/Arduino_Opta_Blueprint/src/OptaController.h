/* -------------------------------------------------------------------------- */
/* FILENAME:    OptaController.h
   AUTHOR:      Daniele Aimo (d.aimo@arduino.cc)
   DATE:        20230801
   REVISION:    0.0.1
   DESCRIPTION: This is the header file for the OptaController class
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef OPTA_BLUE_CONTROLLER
#define OPTA_BLUE_CONTROLLER
#include "Arduino.h"
#include "DigitalCommonCfg.h"
#include "OptaBluePrintCfg.h"
#include "OptaControllerCfg.h"
#include "OptaCrc.h"
#include "OptaExpansion.h"
#include "OptaMsgCommon.h"
#include "Wire.h"
#include "sys/_stdint.h"

#include <string>
#include <vector>

#include <cstdint>
#include <cstring>
#define SEND_RESULT_OK 0
#define SEND_RESULT_WRONG_EXPANSION_INDEX 1
#define SEND_RESULT_WRONG_EXPANSION_ATTRIBUTES 2
#define SEND_RESULT_NO_DATA_TO_TRANSMIT 3
#define SEND_RESULT_COMM_TIMEOUT 4

using namespace Opta;

class Controller;

using CommErr_f = void (*)(int device, int code);
using makeExpansion_f = Expansion *(*)();
using startUp_f = void (*)(Controller *);

class ExpType;

class Controller {
public:
  Controller();
  ~Controller();

  /* 
   Returns a positive number  corresponding to the "type" of the custom expansion that
   has been registered
   Please note that this number (identifing the type of the expansion) might 
   change depending on the application 
   In other word it is not guarantee htat a custom expansion always get the same 
   type "number"
   To get the number again use getExpansionType(std::string pr);
   This is the only function that can be called before the begin() (and actually
   it is better to call it _before_ however all works also if the custom
   expansion is registered after the begin())
   */
  int registerCustomExpansion(std::string pr, makeExpansion_f f, startUp_f su);
  void assign_custom_type_and_call_start_up();
  /* ----------------------------------------------------------- */

  /* initialize the controller it perform the assign address process */
  void begin();
  void update();
  /* performs the actual assign address process it has to be called periodically
   * in the loop to support hot-plug expansion attachment */
  void checkForExpansions();

  /* ----------------------------------------------------------- */

  /* return the number of expansion discovered */
  uint8_t getExpansionNum();
  /* return the type of expansion */
  uint8_t getExpansionType(uint8_t i);
  /* return the I2C address of the expansion */
  uint8_t getExpansionI2Caddress(uint8_t i);
  bool getFwVersion(uint8_t i, uint8_t &major, uint8_t &minor,
                    uint8_t &release);
  int getExpansionType(std::string pr);
  /* ----------------------------------------------------------- */

  /* Get the i-th expansion by copy */
  Expansion &getExpansion(int device);
  /* get the i-th expansion by pointer */
  Expansion *getExpansionPtr(int device);

  /* ----------------------------------------------------------- */

  /* send the content of the I2C tx buffer
   * send n bytes from the tx_buffer
   * wait for r bytes as answer from the device */
  uint8_t send(int add, int device, unsigned int type, int n, int r);
  uint8_t *getTxBuffer() { return tx_buffer; }
  uint8_t *getRxBuffer() { return rx_buffer; }
  void resetRxBuffer();
  void setTx(uint8_t value, uint8_t pos);
  uint8_t getRx(uint8_t pos);
  int getLastTxArgument() { return tx_buffer[BP_ARG_POS]; }

  /* ----------------------------------------------------------- */

  bool rebootExpansion(uint8_t i);
  void setFailedCommCb(CommErr_f f);

  void updateRegs(Expansion &exp);

  /* ----------------------------------------------------------------------- */
  /* DEPRECATED functions: available for DIGITAL expansions                  */
  /* ----------------------------------------------------------------------- */

  PinStatus digitalReadOpta(uint8_t device, uint8_t pin, bool update = false);
  void digitalWriteOpta(uint8_t device, uint8_t pin, PinStatus st,
                        bool update = false);
  int analogReadOpta(uint8_t device, uint8_t pin, bool update = true);
  bool updateDigitalsOut(uint8_t device);
  void updateDigitalsIn(uint8_t device);
  void updateDigitals(uint8_t device);
  void updateAnalogsIn(uint8_t device);
  void updateAnalogs(uint8_t device);
  void beginOdDefaults(uint8_t device, bool d[OPTA_DIGITAL_OUT_NUM],
                       uint16_t timeout);

  /* set at most 32 byte into ROM storage */
  void setProductionData(uint8_t device, uint8_t *data, uint8_t dlen);
  /* set opta digital mechanical */
  void setOdMechanical(uint8_t device);
  void setOdStateSolid(uint8_t device);
  void getFlashData(uint8_t device, uint8_t *buf, uint8_t &dbuf, uint16_t &add);

private:
  void init_exp_type_list();
  
  std::vector<ExpType> exp_type_list;
  /* controller receiving buffer */
  uint8_t rx_buffer[OPTA_I2C_BUFFER_DIM];
  /* controller transmission buffer */
  uint8_t tx_buffer[OPTA_I2C_BUFFER_DIM];
  /* number of bytes received */
  uint8_t rx_num;

  /* used to set temporary address during assign address process*/
  uint8_t tmp_address;
  /* used to store the number of device during temporary address assignment */
  int8_t tmp_num_of_exp;
  /* used to store the temporary address of each expansion */
  uint8_t tmp_exp_add[OPTA_CONTROLLER_MAX_EXPANSION_NUM];
  /* used to store the type of expansion during the temporary address
   * assignment */
  uint8_t tmp_exp_type[OPTA_CONTROLLER_MAX_EXPANSION_NUM];

  /* used to assign permanent address at the end of assign address process */
  int8_t address;
  /* number of expansion discovered */
  uint8_t num_of_exp;
  /* expansion addresses */
  uint8_t exp_add[OPTA_CONTROLLER_MAX_EXPANSION_NUM];
  /* expansion types */
  uint8_t exp_type[OPTA_CONTROLLER_MAX_EXPANSION_NUM];
  /* expansions arrays */
  Expansion *expansions[OPTA_CONTROLLER_MAX_EXPANSION_NUM];
  

  /* ---------------  generic message handling functions ----------------- */

  bool wait_for_device_answer(uint8_t device, uint8_t wait_for, uint16_t timeout);

  /* ---------------- message preparation functions ---------------------- */

  uint8_t msg_set_address(uint8_t add);
  uint8_t msg_get_address_and_type();
  uint8_t msg_opta_reset();
  uint8_t msg_opta_reboot();
  uint8_t msg_get_product_type();
  #ifdef USE_CONFIRM_RX_MESSAGE
  uint8_t msg_confirm_rx_address();
  #endif
  /* -------------------- parse message functions ------------------------ */

  int parse_get_product();
  bool parse_address_and_type(int slave_address);
  bool parse_opta_reboot();


  void _send(int add, int n, int r);

  bool is_detect_high();
  bool is_detect_low();

  CommErr_f failed_i2c_comm;
};

extern Controller OptaController;

//} // namespace Opta

#endif
