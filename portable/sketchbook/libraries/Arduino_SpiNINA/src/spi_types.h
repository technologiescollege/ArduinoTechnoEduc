/*
  This file is part of the Arduino_SpiNINA library.

  Copyright (C) Arduino s.r.l. and/or its affiliated companies

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <inttypes.h>

#define CMD_FLAG 0
#define REPLY_FLAG 1 << 7
#define DATA_FLAG 0x40

#define WIFI_SPI_ACK 1
#define WIFI_SPI_ERR 0xFF

#define TIMEOUT_CHAR 1000

#define START_CMD 0xE0
#define END_CMD 0xEE
#define ERR_CMD 0xEF
#define CMD_POS 1        // Position of Command OpCode on SPI stream
#define PARAM_LEN_POS 2  // Position of Param len on SPI stream


enum numParams {
  PARAM_NUMS_0,
  PARAM_NUMS_1,
  PARAM_NUMS_2,
  PARAM_NUMS_3,
  PARAM_NUMS_4,
  PARAM_NUMS_5,
  PARAM_NUMS_6,
  MAX_PARAM_NUMS
};

#define MAX_PARAMS MAX_PARAM_NUMS - 1
#define PARAM_LEN_SIZE 1

typedef struct __attribute__((__packed__)) {
  uint8_t paramLen;
  char* param;
} tParam;

typedef struct __attribute__((__packed__)) {
  uint16_t dataLen;
  char* data;
} tDataParam;

typedef struct __attribute__((__packed__)) {
  unsigned char cmd;
  unsigned char tcmd;
  unsigned char nParam;
  tParam params[MAX_PARAMS];
} tSpiMsg;

typedef struct __attribute__((__packed__)) {
  unsigned char cmd;
  unsigned char tcmd;
  unsigned char nParam;
  tDataParam params[MAX_PARAMS];
} tSpiMsgData;

typedef struct __attribute__((__packed__)) {
  unsigned char cmd;
  unsigned char tcmd;
  unsigned char nParam;
} tSpiHdr;

typedef struct __attribute__((__packed__)) {
  uint8_t paramLen;
  uint32_t param;
} tLongParam;

typedef struct __attribute__((__packed__)) {
  uint8_t paramLen;
  uint16_t param;
} tIntParam;

typedef struct __attribute__((__packed__)) {
  uint8_t paramLen;
  uint8_t param;
} tByteParam;
