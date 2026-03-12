/* -------------------------------------------------------------------------- */
/* FILE NAME:   Expansion.cpp
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240208
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include <stdint.h>
#ifdef ARDUINO_OPTA
#include "DigitalExpansion.h"
#include "DigitalMechExpansion.h"
#include "DigitalStSolidExpansion.h"
#include "OptaController.h"
#include "OptaExpansion.h"
namespace Opta {

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

Expansion::Expansion() {
  type = EXPANSION_NOT_VALID;
  i2c_address = 0;
  ctrl = nullptr;
  com_timeout = nullptr;
  index = 255;
}

Expansion::Expansion(Controller *ptr) {
  //
  type = EXPANSION_NOT_VALID;
  i2c_address = 0;
  ctrl = ptr;
  com_timeout = nullptr;
  index = 255;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

Expansion::Expansion(uint8_t device, uint8_t _type, uint8_t _i2c,
                     Controller *ptr) {
  index = device;
  type = _type;
  i2c_address = _i2c;
  ctrl = ptr;
  com_timeout = nullptr;
}

void Expansion::setFailedCommCb(FailedComm_f f) { com_timeout = f; }
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

Expansion::operator bool() {
  if (type != EXPANSION_NOT_VALID && i2c_address != 0 && ctrl != nullptr &&
      index < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

Expansion::~Expansion() {}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Expansion::write(unsigned int address, unsigned int value) {
  iregs[address] = value;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Expansion::write(unsigned int address, float value) {
  fregs[address] = value;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Expansion::begin() {}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool Expansion::read(unsigned int address, unsigned int &value) {
  value = 0;
  if (iregs.find(address) != iregs.end()) {
    value = iregs[address];
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool Expansion::read(unsigned int address, float &value) {
  value = 0;
  if (fregs.find(address) != fregs.end()) {
    value = fregs[address];
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool Expansion::addressExist(unsigned int address) {
  if (iregs.find(address) == iregs.end()) {
    return false;
  } else {
    return true;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool Expansion::addressFloatExist(unsigned int address) {
  if (fregs.find(address) == fregs.end()) {
    return false;
  } else {
    return true;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

unsigned int Expansion::execute(uint32_t what) {
  i2c_rv = EXECUTE_OK;
  if (ctrl != nullptr) {
    switch (what) {
    case WRITE_FLASH:
      I2C_TRANSACTION(msg_set_flash, parse_dummy, 0);
      break;
    case READ_FLASH: 
      I2C_TRANSACTION(msg_get_flash,
                      parse_ans_get_flash,
                      getExpectedAnsLen(ANS_LEN_GET_DATA_FROM_FLASH));
      
      break;
    case GET_VERSION:
      I2C_TRANSACTION(msg_get_fw_version,
                      parse_ans_get_version,
                      getExpectedAnsLen(ANS_LEN_GET_VERSION));

      break;

    default:
      i2c_rv = EXECUTE_ERR_UNSUPPORTED;
      break;
    }
  } else {
    i2c_rv = EXECUTE_ERR_NO_CONTROLLER;
  }
  return i2c_rv;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t Expansion::msg_set_flash() {
  if (addressExist(ADD_FLASH_DIM) && addressExist(ADD_FLASH_ADDRESS) &&
      addressExist(ADD_FLASH_0)) {
    ctrl->setTx((uint8_t)(iregs[ADD_FLASH_ADDRESS] & 0xFF), SAVE_ADDRESS_1_POS);
    ctrl->setTx((uint8_t)((iregs[ADD_FLASH_ADDRESS] & 0xFF00) >> 8),
                SAVE_ADDRESS_2_POS);
    ctrl->setTx((uint8_t)iregs[ADD_FLASH_DIM], SAVE_DIMENSION_POS);
    for (uint8_t i = 0; i < iregs[ADD_FLASH_DIM]; i++) {
      ctrl->setTx((uint8_t)iregs[ADD_FLASH_0 + i], SAVE_DATA_INIT_POS + i);
    }
    return prepareSetMsg(ctrl->getTxBuffer(), ARG_SAVE_IN_DATA_FLASH,
                         LEN_SAVE_IN_DATA_FLASH);

  } else {
    return 0;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t Expansion::msg_get_flash() {
  if (addressExist(ADD_FLASH_DIM) && addressExist(ADD_FLASH_ADDRESS)) {
    ctrl->setTx((uint8_t)(iregs[ADD_FLASH_ADDRESS] & 0xFF), READ_ADDRESS_1_POS);
    ctrl->setTx((uint8_t)((iregs[ADD_FLASH_ADDRESS] & 0xFF00) >> 8),
                READ_ADDRESS_2_POS);
    ctrl->setTx((uint8_t)iregs[ADD_FLASH_DIM], READ_DATA_DIM_POS);
    return prepareGetMsg(ctrl->getTxBuffer(), ARG_GET_DATA_FROM_FLASH,
                         LEN_GET_DATA_FROM_FLASH);

  } else {
    return 0;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool Expansion::parse_ans_get_flash() {
  if (checkAnsGetReceived(ctrl->getRxBuffer(), ANS_ARG_GET_DATA_FROM_FLASH,
                          ANS_LEN_GET_DATA_FROM_FLASH)) {
    iregs[ADD_FLASH_DIM] = ctrl->getRx(ANS_GET_DATA_DIMENSION_POS);
    iregs[ADD_FLASH_ADDRESS] = ctrl->getRx(ANS_GET_DATA_ADDRESS_1_POS);
    iregs[ADD_FLASH_ADDRESS] +=
        ((uint16_t)ctrl->getRx(ANS_GET_DATA_ADDRESS_2_POS) << 8);
    int len = (iregs[ADD_FLASH_DIM] > MAX_FLASH_DATA) ? MAX_FLASH_DATA
                                                      : iregs[ADD_FLASH_DIM];
    for (int i = 0; i < len; i++) {
      iregs[ADD_FLASH_0 + i] =
          (uint16_t)ctrl->getRx(ANS_GET_DATA_DATA_INIT_POS + i);
    }
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t Expansion::msg_get_fw_version() {
  return prepareGetMsg(ctrl->getTxBuffer(), ARG_GET_VERSION, LEN_GET_VERSION);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool Expansion::parse_ans_get_version() {
  if (checkAnsGetReceived(ctrl->getRxBuffer(), ANS_ARG_GET_VERSION,
                          ANS_LEN_GET_VERSION)) {
    iregs[ADD_VERSION_MAJOR] = ctrl->getRx(GET_VERSION_MAJOR_POS);
    iregs[ADD_VERSION_MINOR] = ctrl->getRx(GET_VERSION_MINOR_POS);
    iregs[ADD_VERSION_RELEASE] = ctrl->getRx(GET_VERSION_RELEASE_POS);
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Expansion::set_flash_data(uint8_t *buf, uint8_t dbuf, uint16_t add) {
  for (int i = 0; i < dbuf && i < MAX_FLASH_DATA; i++) {
    iregs[ADD_FLASH_0 + i] = (int)*(buf + i);
  }
  iregs[ADD_FLASH_DIM] = (int)((dbuf > MAX_FLASH_DATA) ? MAX_FLASH_DATA : dbuf);
  iregs[ADD_FLASH_ADDRESS] = (int)add;
  execute(WRITE_FLASH);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Expansion::get_flash_data(uint8_t *buf, uint8_t &dbuf, uint16_t &add) {
  iregs[ADD_FLASH_DIM] = (int)((dbuf > MAX_FLASH_DATA) ? MAX_FLASH_DATA : dbuf);
  iregs[ADD_FLASH_ADDRESS] = (int)add;
  execute(READ_FLASH);
  dbuf = (int)((iregs[ADD_FLASH_DIM] > MAX_FLASH_DATA) ? MAX_FLASH_DATA
                                                       : iregs[ADD_FLASH_DIM]);
  add = iregs[ADD_FLASH_ADDRESS];
  for (int i = 0; i < dbuf && i < MAX_FLASH_DATA; i++) {
    *(buf + i) = (uint8_t)iregs[ADD_FLASH_0 + i];
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool Expansion::verify_address(unsigned int add) { return false; }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
bool Expansion::getFwVersion(uint8_t &major, uint8_t &minor, uint8_t &release) {

  uint8_t err = execute(GET_VERSION);
  if (err == EXECUTE_OK) {
    major = iregs[ADD_VERSION_MAJOR];
    minor = iregs[ADD_VERSION_MINOR];
    release = iregs[ADD_VERSION_RELEASE];
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
unsigned int Expansion::i2c_transaction(int rx_bytes) {
  
  i2c_rv = EXECUTE_ERR_SINTAX;
  if (prepare_msg && ctrl != nullptr) {
      uint8_t err = ctrl->send(i2c_address, index, type, prepare_msg(), rx_bytes);
      i2c_rv = EXECUTE_ERR_I2C_COMM;
      if (err == SEND_RESULT_OK) {
        if (parse_msg) {
          if (!parse_msg()) {
            i2c_rv = EXECUTE_ERR_PROTOCOL;
          }
          i2c_rv = EXECUTE_OK;
        }
      } else if (err == SEND_RESULT_COMM_TIMEOUT) {
        if (com_timeout != nullptr) {
          com_timeout(index, ctrl->getLastTxArgument());
        }
      }
  }
  return i2c_rv;
}

} // namespace Opta
#endif
