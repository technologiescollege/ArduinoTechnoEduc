/*
  This file is part of the Arduino_SpiNINA library.

  Copyright (C) Arduino s.r.l. and/or its affiliated companies

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef SPI_Drv_h
#define SPI_Drv_h

#include <inttypes.h>
#include "spi_types.h"

#define SPI_START_CMD_DELAY 10

#define NO_LAST_PARAM 0
#define LAST_PARAM 1

#define DUMMY_DATA 0xFF

#define WAIT_FOR_SLAVE_SELECT() \
  if (!SpiDrv::initialized) { \
    SpiDrv::begin(); \
  } \
  SpiDrv::waitForSlaveReady(); \
  SpiDrv::spiSlaveSelect();

class SpiDrv {
private:
  static void waitForSlaveSign();
  static void getParam(uint8_t* param);
public:
  static bool initialized;

  static void begin(bool force = false);
  static void end();
  static void spiDriverInit();
  static void spiSlaveSelect();
  static void spiSlaveDeselect();
  static char spiTransfer(volatile char data);
  static void waitForSlaveReady(bool const feed_watchdog = false);
  static int waitSpiChar(unsigned char waitChar);
  static int readAndCheckChar(char checkChar, char* readChar);
  static char readChar();
  static int waitResponseParams(uint8_t cmd, uint8_t numParam, tParam* params);
  static int waitResponseCmd(uint8_t cmd, uint8_t numParam, uint8_t* param, uint8_t* param_len);
  static int waitResponseData8(uint8_t cmd, uint8_t* param, uint8_t* param_len);
  static int waitResponseData16(uint8_t cmd, uint8_t* param, uint16_t* param_len);
  static int waitResponse(uint8_t cmd, uint8_t* numParamRead, uint8_t** params, uint8_t maxNumParams);
  static void sendParam(const uint8_t* param, uint8_t param_len, uint8_t lastParam = NO_LAST_PARAM);
  static void sendParamNoLen(const uint8_t* param, size_t param_len, uint8_t lastParam = NO_LAST_PARAM);
  static void sendParamLen8(const uint8_t param_len);
  static void sendParamLen16(const uint16_t param_len);
  static uint8_t readParamLen8(uint8_t* param_len = NULL);
  static uint16_t readParamLen16(uint16_t* param_len = NULL);
  static void sendBuffer(const uint8_t* param, uint16_t param_len, uint8_t lastParam = NO_LAST_PARAM);
  static void sendParam(uint16_t param, uint8_t lastParam = NO_LAST_PARAM);
  static void sendCmd(uint8_t cmd, uint8_t numParam);
  static int available();
};

extern SpiDrv spiDrv;

#endif
