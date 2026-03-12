/* -------------------------------------------------------------------------- */
/* FILE NAME:   DigitalExpansionsAddresses.h
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240213
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef DIGITALEXPANSIONSADDRESSES
#define DIGITALEXPANSIONSADDRESSES

#define DIGITAL_OUT_NUM 8
#define DIGITAL_IN_NUM 16
#define ANALOG_IN_NUM 16

#define DIGITAL_EXPANSION_ADDRESS 8000

/*
 * ADDRESSES DEFINED FOR DIGITAL
 * */

#define CTRL_ADD_EXPANSION_PIN (DIGITAL_EXPANSION_ADDRESS + 0)

#define BASE_ADD_DIGITAL_OUTPUT 10
#define ADD_DIGITAL_0_OUTPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_OUTPUT + 0)
#define ADD_DIGITAL_1_OUTPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_OUTPUT + 1)
#define ADD_DIGITAL_2_OUTPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_OUTPUT + 2)
#define ADD_DIGITAL_3_OUTPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_OUTPUT + 3)
#define ADD_DIGITAL_4_OUTPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_OUTPUT + 4)
#define ADD_DIGITAL_5_OUTPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_OUTPUT + 5)
#define ADD_DIGITAL_6_OUTPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_OUTPUT + 6)
#define ADD_DIGITAL_7_OUTPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_OUTPUT + 7)
#define ADD_DIGITAL_OUTPUT                                                     \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_OUTPUT + DIGITAL_OUT_NUM)

#define BASE_ADD_DIGITAL_INPUT 20
#define ADD_DIGITAL_0_INPUT                                                    \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 0)
#define ADD_DIGITAL_1_INPUT                                                    \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 1)
#define ADD_DIGITAL_2_INPUT                                                    \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 2)
#define ADD_DIGITAL_3_INPUT                                                    \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 3)
#define ADD_DIGITAL_4_INPUT                                                    \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 4)
#define ADD_DIGITAL_5_INPUT                                                    \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 5)
#define ADD_DIGITAL_6_INPUT                                                    \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 6)
#define ADD_DIGITAL_7_INPUT                                                    \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 7)
#define ADD_DIGITAL_8_INPUT                                                    \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 8)
#define ADD_DIGITAL_9_INPUT                                                    \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 9)
#define ADD_DIGITAL_10_INPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 10)
#define ADD_DIGITAL_11_INPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 11)
#define ADD_DIGITAL_12_INPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 12)
#define ADD_DIGITAL_13_INPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 13)
#define ADD_DIGITAL_14_INPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 14)
#define ADD_DIGITAL_15_INPUT                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + 15)
#define ADD_DIGITAL_INPUT                                                      \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_DIGITAL_INPUT + DIGITAL_IN_NUM)

#define BASE_ADD_ANALOG_IN 40
#define ADD_PIN_0_ANALOG_IN (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 0)
#define ANALOG_IN_FIRST_REG ADD_PIN_0_ANALOG_IN
#define ADD_PIN_1_ANALOG_IN (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 1)
#define ADD_PIN_2_ANALOG_IN (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 2)
#define ADD_PIN_3_ANALOG_IN (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 3)
#define ADD_PIN_4_ANALOG_IN (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 4)
#define ADD_PIN_5_ANALOG_IN (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 5)
#define ADD_PIN_6_ANALOG_IN (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 6)
#define ADD_PIN_7_ANALOG_IN (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 7)
#define ADD_PIN_8_ANALOG_IN (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 8)
#define ADD_PIN_9_ANALOG_IN (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 9)
#define ADD_PIN_10_ANALOG_IN                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 10)
#define ADD_PIN_11_ANALOG_IN                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 11)
#define ADD_PIN_12_ANALOG_IN                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 12)
#define ADD_PIN_13_ANALOG_IN                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 13)
#define ADD_PIN_14_ANALOG_IN                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 14)
#define ADD_PIN_15_ANALOG_IN                                                   \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN + 15)

#define MAX_FLASH_DATA 32
#define BASE_ADD_FLASH_DATA 60
#define ADD_FLASH_0 (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_FLASH_DATA + 0)

#define ADD_FLASH_DIM                                                          \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_FLASH_DATA + MAX_FLASH_DATA)
#define ADD_FLASH_ADDRESS                                                      \
  (DIGITAL_EXPANSION_ADDRESS + BASE_ADD_FLASH_DATA + MAX_FLASH_DATA + 1)

#define PRODUCTION_OD_DATA_FLASH_ADDRESS 0x1C00
#define EXPANSION_OD_TYPE_ADDITIONA_DATA                                       \
  (PRODUCTION_DATA_FLASH_ADDRESS + MAX_FLASH_BUFFER_DIM)
#define FLASH_OD_TYPE_MECHANICAL 0
#define FLASH_OD_TYPE_STATE_SOLID 1

#endif
