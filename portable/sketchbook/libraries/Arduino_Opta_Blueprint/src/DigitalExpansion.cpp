/* -------------------------------------------------------------------------- */
/* FILE NAME:   DigitalExpansion.cpp
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

#ifdef ARDUINO_OPTA
#include "DigitalExpansion.h"
namespace Opta {

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

DigitalExpansion::DigitalExpansion() {
  iregs[CTRL_ADD_EXPANSION_PIN] = 0;
  iregs[ADD_DIGITAL_OUTPUT] = 0;
  iregs[ADD_DIGITAL_INPUT] = 0;
  iregs[ADD_PIN_0_ANALOG_IN] = 0;
  iregs[ADD_PIN_1_ANALOG_IN] = 0;
  iregs[ADD_PIN_2_ANALOG_IN] = 0;
  iregs[ADD_PIN_3_ANALOG_IN] = 0;
  iregs[ADD_PIN_4_ANALOG_IN] = 0;
  iregs[ADD_PIN_5_ANALOG_IN] = 0;
  iregs[ADD_PIN_6_ANALOG_IN] = 0;
  iregs[ADD_PIN_7_ANALOG_IN] = 0;
  iregs[ADD_PIN_8_ANALOG_IN] = 0;
  iregs[ADD_PIN_9_ANALOG_IN] = 0;
  iregs[ADD_PIN_10_ANALOG_IN] = 0;
  iregs[ADD_PIN_11_ANALOG_IN] = 0;
  iregs[ADD_PIN_12_ANALOG_IN] = 0;
  iregs[ADD_PIN_13_ANALOG_IN] = 0;
  iregs[ADD_PIN_14_ANALOG_IN] = 0;
  iregs[ADD_PIN_15_ANALOG_IN] = 0;
}

Expansion *DigitalExpansion::makeExpansion() { return new DigitalExpansion(); }

std::string DigitalExpansion::getProduct() {
  std::string rv(OPTA_DIGITAL_DESCRIPTION);
  return rv;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

DigitalExpansion::DigitalExpansion(Expansion &other) {
  DigitalExpansion &de = (DigitalExpansion &)other;

  type = EXPANSION_NOT_VALID;
  i2c_address = 0;
  ctrl = other.getCtrl();
  index = 255;

  if (other.getType() == EXPANSION_OPTA_DIGITAL_MEC ||
      other.getType() == EXPANSION_OPTA_DIGITAL_STS ||
      other.getType() == EXPANSION_DIGITAL_INVALID) {
    iregs = de.iregs;
    fregs = de.fregs;
    type = other.getType();
    i2c_address = other.getI2CAddress();
    ctrl = other.getCtrl();
    index = other.getIndex();
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* All output are off*/
uint8_t DigitalExpansion::defaults[OPTA_CONTROLLER_MAX_EXPANSION_NUM] = {
    0, 0, 0, 0, 0};
/* 0xFFFF there is no timeout */
uint16_t DigitalExpansion::timeouts[OPTA_CONTROLLER_MAX_EXPANSION_NUM] = {
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

uint8_t DigitalExpansion::last_expansion_output[OPTA_CONTROLLER_MAX_EXPANSION_NUM] = {
    0, 0, 0, 0, 0};

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function is called every time an assign address process is finished
   If the assign address process is due to a Controller reset the static 
   variable it uses are set to default value so that the expansion is 
   is actually reset to its default
   If the reset is due to the insertion of a new expansion then this function
   refresh the expansion output with the last used value */
void DigitalExpansion::startUp(Controller *ptr) {

  if (ptr == nullptr) {
    return;
  }
  for (int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
    DigitalExpansion exp = ptr->getExpansion(i);
    if (exp) {
      /* send timeout and default value */
      uint8_t tx_bytes = exp.msgDefault(ptr, i);
      if (tx_bytes) {
        ptr->send(exp.getI2CAddress(), exp.getIndex(), exp.getType(), tx_bytes,
                  getExpectedAnsLen(ANS_LEN_OD_DEFAULT_AND_TIMEOUT));
      }
      /* send intial value of outputs */
      ptr->setTx(last_expansion_output[i], BP_PAYLOAD_START_POS);
      tx_bytes = prepareSetMsg(ptr->getTxBuffer(), ARG_OD_SET_DIGITAL_OUTPUTS,
                       LEN_OD_SET_DIGITAL_OUTPUTS);
      if(tx_bytes) {
        ptr->send(exp.getI2CAddress(), exp.getIndex(), exp.getType(), tx_bytes,
                 getExpectedAnsLen(ANS_LEN_OD_SET_DIGITAL_OUTPUTS));
      }
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void DigitalExpansion::setDefault(Controller &ptr, uint8_t device,
                                  uint8_t bit_mask, uint16_t timeout) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    DigitalExpansion::defaults[device] = bit_mask;
    DigitalExpansion::timeouts[device] = timeout;
    uint8_t tx_bytes = DigitalExpansion::msgDefault(&ptr, device);
    if (tx_bytes) {
      ptr.send(ptr.getExpansionI2Caddress(device), device,
               ptr.getExpansionType(device), tx_bytes,
               getExpectedAnsLen(ANS_LEN_OD_SET_DIGITAL_OUTPUTS));
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t DigitalExpansion::msgDefault(Controller *ptr, uint8_t device) {
  if (ptr != nullptr && device < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    ptr->setTx(defaults[device], BP_PAYLOAD_START_POS);
    ptr->setTx((uint8_t)(timeouts[device] & 0xFF), BP_PAYLOAD_START_POS + 1);
    ptr->setTx((uint8_t)((timeouts[device] & 0xFF00) >> 8),
               BP_PAYLOAD_START_POS + 2);
    return prepareSetMsg(ptr->getTxBuffer(), ARG_OD_DEFAULT_AND_TIMEOUT,
                         LEN_OD_DEFAULT_AND_TIMEOUT);
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t DigitalExpansion::calcDefault(bool p0, bool p1, bool p2, bool p3,
                                      bool p4, bool p5, bool p6, bool p7) {
  uint8_t rv = 0;
  if (p0) {
    rv |= (1 << 0);
  }
  if (p1) {
    rv |= (1 << 1);
  }
  if (p2) {
    rv |= (1 << 2);
  }
  if (p3) {
    rv |= (1 << 3);
  }
  if (p4) {
    rv |= (1 << 4);
  }
  if (p5) {
    rv |= (1 << 5);
  }
  if (p6) {
    rv |= (1 << 6);
  }
  if (p7) {
    rv |= (1 << 7);
  }

  return rv;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t DigitalExpansion::msg_set_di() {
  if (ctrl != nullptr) {
    /* change note: this code has been added in order to save the last
       output values so that they can be restored when a reassign address
       process happens */
    if(getIndex() < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
      last_expansion_output[getIndex()] = iregs[ADD_DIGITAL_OUTPUT];
    }

    ctrl->setTx(iregs[ADD_DIGITAL_OUTPUT], BP_PAYLOAD_START_POS);
    return prepareSetMsg(ctrl->getTxBuffer(), ARG_OD_SET_DIGITAL_OUTPUTS,
                         LEN_OD_SET_DIGITAL_OUTPUTS);
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool DigitalExpansion::parse_ans_set_di() {
  if (ctrl != nullptr) {
    return checkAnsSetReceived(ctrl->getRxBuffer(), ANS_ARG_OD_SET_DIGITAL_OUTPUTS,
                               ANS_LEN_OD_SET_DIGITAL_OUTPUTS);
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* get digital input */
uint8_t DigitalExpansion::msg_get_di() {
  if (ctrl != nullptr) {
    return prepareGetMsg(ctrl->getTxBuffer(), ARG_OD_GET_DIGITAL_INPUTS,LEN_OD_GET_DIGITAL_INPUTS);
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
bool DigitalExpansion::parse_ans_get_di() {
  if (ctrl != nullptr) {
    if (checkAnsGetReceived(ctrl->getRxBuffer(), ANS_ARG_OD_GET_DIGITAL_INPUTS,
                            ANS_LEN_OD_GET_DIGITAL_INPUTS)) {
      iregs[ADD_DIGITAL_INPUT] = ctrl->getRx(BP_PAYLOAD_START_POS);
      iregs[ADD_DIGITAL_INPUT] += (ctrl->getRx(BP_PAYLOAD_START_POS + 1) << 8);
      return true;
    }
    return false;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* msg get analog input */
uint8_t DigitalExpansion::msg_get_ai() {
  if (ctrl != nullptr) {
    if (addressExist(CTRL_ADD_EXPANSION_PIN)) {
      ctrl->setTx(iregs[CTRL_ADD_EXPANSION_PIN], BP_PAYLOAD_START_POS);
      return prepareGetMsg(ctrl->getTxBuffer(), ARG_OD_GET_ANALOG_INPUT,
                           LEN_OD_GET_ANALOG_INPUT);
    } else {
      return 0;
    }
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool DigitalExpansion::parse_ans_get_ai() {
  if (ctrl != nullptr) {
    if (checkAnsGetReceived(ctrl->getRxBuffer(), ANS_ARG_OD_GET_ANALOG_INPUT,
                            ANS_LEN_OD_GET_ANALOG_INPUT)) {
      int offset = iregs[CTRL_ADD_EXPANSION_PIN];

      iregs[ANALOG_IN_FIRST_REG + offset] = ctrl->getRx(BP_PAYLOAD_START_POS);
      iregs[ANALOG_IN_FIRST_REG + offset] +=
          (ctrl->getRx(BP_PAYLOAD_START_POS + 1) << 8);
      return true;
    }
    return false;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* msg get all analog input */
uint8_t DigitalExpansion::msg_get_all_ai() {
  if (ctrl != nullptr) {
    return prepareGetMsg(ctrl->getTxBuffer(), ARG_OD_GET_ALL_ANALOG_INPUTS,
                         LEN_OD_GET_ALL_ANALOG_INPUTS);
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
bool DigitalExpansion::parse_ans_get_all_ai() {
  if (ctrl != nullptr) {
    if (checkAnsGetReceived(ctrl->getRxBuffer(), ANS_ARG_OD_GET_ALL_ANALOG_INPUTS,
                            ANS_LEN_OD_GET_ALL_ANALOG_INPUTS)) {
      for (int i = 0, j = 0; i < ANALOG_IN_NUM; i++, j += 2) {
        iregs[ANALOG_IN_FIRST_REG + i] = ctrl->getRx(BP_PAYLOAD_START_POS + j);
        iregs[ANALOG_IN_FIRST_REG + i] +=
            (ctrl->getRx(BP_PAYLOAD_START_POS + j + 1) << 8);
      }
      return true;
    }
    return false;
  }
  return false;
}

uint8_t DigitalExpansion::msg_set_default_values() {
  if (ctrl != nullptr) {
    ctrl->setTx(iregs[CTRL_ADD_EXPANSION_PIN], BP_PAYLOAD_START_POS);
    ctrl->setTx(iregs[CTRL_ADD_EXPANSION_PIN], BP_PAYLOAD_START_POS);
    ctrl->setTx(iregs[CTRL_ADD_EXPANSION_PIN], BP_PAYLOAD_START_POS);
    return prepareGetMsg(ctrl->getTxBuffer(), ARG_OD_GET_ALL_ANALOG_INPUTS,
                         LEN_OD_GET_ALL_ANALOG_INPUTS);
  }
  return 0;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

unsigned int DigitalExpansion::execute(uint32_t what) {
  i2c_rv = EXECUTE_OK;
  if (ctrl != nullptr) {
    switch (what) {
    /* ------------------------------------------------------------------- */
    case SET_DIGITAL_OUTPUT:
      I2C_TRANSACTION(DigitalExpansion::msg_set_di,
                      DigitalExpansion::parse_ans_set_di,
                      getExpectedAnsLen(ANS_LEN_OD_SET_DIGITAL_OUTPUTS));
      break;
    /* ------------------------------------------------------------------- */
    case GET_DIGITAL_INPUT:
      I2C_TRANSACTION(DigitalExpansion::msg_get_di,
                      DigitalExpansion::parse_ans_get_di,
                      getExpectedAnsLen(ANS_LEN_OD_GET_DIGITAL_INPUTS));
      break;
    /* ------------------------------------------------------------------- */
    case GET_SINGLE_ANALOG_INPUT:
      I2C_TRANSACTION(DigitalExpansion::msg_get_ai,
                      DigitalExpansion::parse_ans_get_ai,
                      getExpectedAnsLen(ANS_LEN_OD_GET_ANALOG_INPUT));
      break;
    /* ------------------------------------------------------------------- */
    case GET_ALL_ANALOG_INPUT:
      I2C_TRANSACTION(DigitalExpansion::msg_get_all_ai,
                      DigitalExpansion::parse_ans_get_all_ai,
                      getExpectedAnsLen(ANS_LEN_OD_GET_ALL_ANALOG_INPUTS));
      break;
    default:
      i2c_rv = Expansion::execute(what);
      break;
    }
    ctrl->updateRegs(*this);
  } else {
    i2c_rv = EXECUTE_ERR_NO_CONTROLLER;
  }
  return i2c_rv;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void DigitalExpansion::digitalWrite(int pin, PinStatus st,
                                    bool update /*= false*/) {
  if (pin >= 0 && pin <= DIGITAL_OUT_NUM) {
    if (st == HIGH) {
      iregs[ADD_DIGITAL_OUTPUT] |= (1 << pin);
    } else {
      iregs[ADD_DIGITAL_OUTPUT] &= ~(1 << pin);
    }

    if (update) {
      updateDigitalOutputs();
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
PinStatus DigitalExpansion::digitalOutRead(int pin) {
  if (pin >= 0 && pin <= DIGITAL_OUT_NUM) {
    if (iregs[ADD_DIGITAL_OUTPUT] & (1 << pin)) {
      return HIGH;
    } 
  }
  return LOW;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
PinStatus DigitalExpansion::digitalRead(int pin, bool update /*= false*/) {
  if (pin >= 0 && pin < DIGITAL_IN_NUM) {
    if (update) {
      updateDigitalInputs();
    }
  }
  if (iregs[ADD_DIGITAL_INPUT] & (1 << pin)) {
    return HIGH;
  } else {
    return LOW;
  }
  return LOW;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

int DigitalExpansion::analogRead(int pin, bool update /*= true*/) {
  //
  if (pin >= 0 && pin < DIGITAL_IN_NUM) {
    if (update) {
      write(CTRL_ADD_EXPANSION_PIN, pin);
      execute(GET_SINGLE_ANALOG_INPUT);
    }
    return iregs[ANALOG_IN_FIRST_REG + pin];
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

float DigitalExpansion::pinVoltage(int pin, bool update /* = true*/) {
  float bit = (float)analogRead(pin, update);
  return bit * 3.3 / 16383.0 / 0.1162;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void DigitalExpansion::updateDigitalInputs() { execute(GET_DIGITAL_INPUT); }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void DigitalExpansion::updateAnalogInputs() { execute(GET_ALL_ANALOG_INPUT); }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void DigitalExpansion::updateDigitalOutputs() {
  /*uint8_t err = */ execute(SET_DIGITAL_OUTPUT);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool DigitalExpansion::verify_address(unsigned int add) {
  if (add == CTRL_ADD_EXPANSION_PIN) {
    return true;
  } else if (add >= ADD_DIGITAL_0_OUTPUT && add <= ADD_DIGITAL_OUTPUT) {
    return true;
  } else if (add >= ADD_PIN_0_ANALOG_IN && add <= ADD_PIN_15_ANALOG_IN) {
    return true;
  } else if (add >= ADD_DIGITAL_0_INPUT && add <= ADD_DIGITAL_INPUT) {
    return true;
  } else if (add >= ADD_FLASH_0 && add <= ADD_FLASH_ADDRESS) {
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void DigitalExpansion::write(unsigned int address, unsigned int value) {
  if (!verify_address(address)) {
    return;
  }

  if (address >= ADD_DIGITAL_0_INPUT && address <= ADD_DIGITAL_15_INPUT) {
    /* can't write input */
    return;
  } else if (address >= ADD_PIN_0_ANALOG_IN &&
             address <= ADD_PIN_15_ANALOG_IN) {
    /* can't write input */
    return;
  } else if (address >= ADD_DIGITAL_0_OUTPUT &&
             address <= ADD_DIGITAL_7_OUTPUT) {
    int pin = address - DIGITAL_EXPANSION_ADDRESS - BASE_ADD_DIGITAL_OUTPUT;
    if (value == 0) {
      /* reset PIN */
      iregs[ADD_DIGITAL_OUTPUT] &= ~(1 << pin);
    } else {
      /* set PIN */
      iregs[ADD_DIGITAL_OUTPUT] |= (1 << pin);
    }
    return;
  }
  return Expansion::write(address, value);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool DigitalExpansion::read(unsigned int address, unsigned int &value) {

  if (!verify_address(address)) {
    return false;
  }

  if (address >= ADD_DIGITAL_0_INPUT && address <= ADD_DIGITAL_15_INPUT) {
    int pin = address - DIGITAL_EXPANSION_ADDRESS - BASE_ADD_DIGITAL_INPUT;
    if (iregs[ADD_DIGITAL_INPUT] & (1 << pin)) {
      value = 1;
    } else {
      value = 0;
    }
    return true;

  } else if (address >= ADD_DIGITAL_0_OUTPUT &&
             address <= ADD_DIGITAL_7_OUTPUT) {
    int pin = address - DIGITAL_EXPANSION_ADDRESS - BASE_ADD_DIGITAL_OUTPUT;
    if (iregs[ADD_DIGITAL_OUTPUT] & (1 << pin)) {
      value = 1;
    } else {
      value = 0;
    }
    return true;
  }
  return Expansion::read(address, value);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void DigitalExpansion::setProductData(uint8_t *data, uint8_t len) {
  Expansion::set_flash_data(data, len, PRODUCTION_OD_DATA_FLASH_ADDRESS);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void DigitalExpansion::setIsMechanical() {
  uint8_t d[2] = {FLASH_OD_TYPE_MECHANICAL, '#'};
  Expansion::set_flash_data(d, 2, EXPANSION_OD_TYPE_ADDITIONA_DATA);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void DigitalExpansion::setIsStateSolid() {
  uint8_t d[2] = {FLASH_OD_TYPE_STATE_SOLID, '#'};
  Expansion::set_flash_data(d, 2, EXPANSION_TYPE_ADDITIONA_DATA);
}



} // namespace Opta
#endif
