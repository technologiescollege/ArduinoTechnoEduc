/* -------------------------------------------------------------------------- */
/*
   FILE NAME:   OptaCrc.cpp
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

#include "OptaCrc.h"

uint8_t OptaCrc8::crc_table[CRC8_TABLE_SIZE] = {0};
bool OptaCrc8::table_inited = false;

/* -------------------------------------------------------------------------- */
void OptaCrc8::init_table(const uint8_t polynomial) {
  /* --------------------------------------------------------------------------
   */
  if (!table_inited) {
    table_inited = true;

    for (int16_t n = 0; n < CRC8_TABLE_SIZE; n++) {
      uint8_t currByte = (uint8_t)n;
      for (uint8_t bit = 0; bit < 8; bit++) {
        if ((currByte & 0x80) != 0) {
          currByte <<= 1;
          currByte ^= polynomial;
        } else {
          currByte <<= 1;
        }
      }
      crc_table[n] = currByte;
    }
  }
}

/* -------------------------------------------------------------------------- */
uint8_t OptaCrc8::calc(const uint8_t *pdata, size_t nbytes, uint8_t crc) {
  /* --------------------------------------------------------------------------
   */
  init_table(CRC_POLYNOMIAL);
  unsigned int idx;
  while (nbytes--) {
    idx = (crc ^ *pdata);
    crc = (crc_table[idx]) & 0xff;
    pdata++;
  }
  return crc;
}

/* -------------------------------------------------------------------------- */
bool OptaCrc8::verify(uint8_t rxCrc, const uint8_t *pdata, size_t nbytes) {
  /* --------------------------------------------------------------------------
   */
  uint8_t crc = calc(pdata, nbytes, 0);
  return (rxCrc == crc);
}
