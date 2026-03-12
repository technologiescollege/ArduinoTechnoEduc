
/* -------------------------------------------------------------------------- */
/* FILE NAME:   OptaExpansion.h
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
#ifndef EXPANSION_H_INCLUDED
#define EXPANSION_H_INCLUDED

#include "Arduino.h"
#include "ExpansionOperations.h"
#include "OptaBluePrintCfg.h"
#include "OptaControllerCfg.h"
#include "OptaModuleProtocol.h"
#include "OptaMsgCommon.h"
#include <cstdint>
#include <map>
#include <stdint.h>
#include <functional>

#define EXECUTE_OK 0
#define EXECUTE_ERR_I2C_COMM 1
#define EXECUTE_ERR_NO_CONTROLLER 2
#define EXECUTE_ERR_UNSUPPORTED 3
#define EXECUTE_ERR_PROTOCOL 4
#define EXECUTE_ERR_SINTAX 5

#define ADD_VERSION_MAJOR 10
#define ADD_VERSION_MINOR 11
#define ADD_VERSION_RELEASE 12

#define I2C_TRANSACTION(m,p,l)    prepare_msg = [this](){return m();}; \
                                  parse_msg = [this](){return p();};   \
                                  i2c_transaction(l);

class Controller;

using FailedComm_f = void (*)(int device, int code);

namespace Opta {

class Expansion {
public:
  Expansion();
  Expansion(Controller *ptr);
  Expansion(uint8_t index, uint8_t _type, uint8_t _i2c, Controller *ptr);
  virtual ~Expansion();

  virtual void begin();

  uint8_t getType() const { return type; }
  uint8_t getI2CAddress() const { return i2c_address; }
  Controller *getCtrl() const { return ctrl; }
  uint8_t getIndex() const { return index; }

  void setIndex(uint8_t i) { index = i; }
  void setI2CAddress(uint8_t add) { i2c_address = add; }
  void setType(unsigned int t) { type = t; }
  void setCtrl(Controller *c) { ctrl = c; }
  virtual operator bool();

  virtual void write(unsigned int address, unsigned int value);
  virtual void write(unsigned int address, float value);
  virtual bool read(unsigned int address, unsigned int &value);
  virtual bool read(unsigned int address, float &value);

  bool addressExist(unsigned int address);
  bool addressFloatExist(unsigned int address);
  /* returns one of the code defined above */
  virtual unsigned int execute(uint32_t what);

  virtual void getFlashData(uint8_t *buf, uint8_t &dbuf, uint16_t &add) {
    return get_flash_data(buf, dbuf, add);
  }

  virtual bool getFwVersion(uint8_t &major, uint8_t &minor, uint8_t &release);
  virtual void setFailedCommCb(FailedComm_f f);
  auto getIregs() { return iregs; }
  auto getFregs() { return fregs; }
  void updateRegs(Expansion &exp) {
    iregs = exp.getIregs();
    fregs = exp.getFregs();
  }
  void setController(Controller *ptr) { ctrl = ptr; }

  

protected:
  FailedComm_f com_timeout;
  std::map<unsigned int, unsigned int> iregs;
  std::map<unsigned int, float> fregs;
  uint8_t index;
  uint8_t type;
  uint8_t i2c_address;
  Controller *ctrl;
  void set_flash_data(uint8_t *buf, uint8_t dbuf, uint16_t add);
  void get_flash_data(uint8_t *buf, uint8_t &dbuf, uint16_t &add);
  virtual bool verify_address(unsigned int add);

  uint8_t msg_get_fw_version();
  bool parse_ans_get_version();
  uint8_t msg_set_flash();
  uint8_t msg_get_flash();
  bool parse_ans_get_flash();

  std::function<uint8_t()> prepare_msg;
  std::function<bool()> parse_msg;
  unsigned int i2c_rv = 0;
  // in case no aswer is expected 
  virtual bool parse_dummy() {return true;}
  

  virtual unsigned int i2c_transaction(int rx_bytes);  
};

} // namespace Opta
#endif
