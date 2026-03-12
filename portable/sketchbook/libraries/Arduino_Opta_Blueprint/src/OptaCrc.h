/* -------------------------------------------------------------------------- */
/*
   FILE NAME:   OptaCrc.h
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        19102023
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:
*/
/* -------------------------------------------------------------------------- */

#ifndef OptaCrc_H
#define OptaCrc_H

#include <cstdint>
#include <cstring>

#define CRC8_TABLE_SIZE 256
#define CRC_POLYNOMIAL 0x7

class OptaCrc8 {
private:
  static uint8_t crc_table[CRC8_TABLE_SIZE];
  static bool table_inited;
  static void init_table(const uint8_t polynomial);

public:
  static uint8_t calc(const uint8_t *pdata, size_t nbytes, uint8_t crc);
  static bool verify(uint8_t rxCrc, const uint8_t *pdata, size_t nbytes);
};

#endif
