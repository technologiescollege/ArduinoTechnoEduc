/* -------------------------------------------------------------------------- */
/* FILE NAME:   OptaDigitalCfg.h
   AUTHOR:      Daniele Aimo
   EMAIL:       maidnl74@gmail.com
   DATE:        20231116
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef OPTADIGITALCFG_H
#define OPTADIGITALCFG_H

/* VERSION of the Digital Firmware */
#define FW_VERSION_MAJOR 0
#define FW_VERSION_MINOR 1
#define FW_VERSION_RELEASE 5

/* all analog input are supposed to be grouped all together in variant pin table
 */

/* the first index of analog input in variant */
#define OPTA_DIGITAL_FIRST_ANALOG_IN 14
/* the last index of analog input in variant
   (plus one to simplify looping on all of them) */
#define OPTA_DIGITAL_LAST_ANALOG_IN (29 + 1)

/* this series of define which ANALOG CHANNEL will be published in the
   correspondent message field in the I2C message between the
   controller and the VARIOUS Opta Digital
   if the value is positive then is an analog channel
   if the value is negative then is an actual digital pin and
   the number correspond to the pin number in the variant structure*/

#define OPTA_DIGITAL_IN_INDEX_0 0
#define OPTA_DIGITAL_IN_INDEX_1 1
#define OPTA_DIGITAL_IN_INDEX_2 2
#define OPTA_DIGITAL_IN_INDEX_3 3
#define OPTA_DIGITAL_IN_INDEX_4 4
#define OPTA_DIGITAL_IN_INDEX_5 7
#define OPTA_DIGITAL_IN_INDEX_6 8
#define OPTA_DIGITAL_IN_INDEX_7 9
#define OPTA_DIGITAL_IN_INDEX_8 10
#define OPTA_DIGITAL_IN_INDEX_9 22
#define OPTA_DIGITAL_IN_INDEX_10 21
#define OPTA_DIGITAL_IN_INDEX_11 20
#define OPTA_DIGITAL_IN_INDEX_12 19
#define OPTA_DIGITAL_IN_INDEX_13 18
#define OPTA_DIGITAL_IN_INDEX_14 17
#define OPTA_DIGITAL_IN_INDEX_15 16

/* The very same happen for Digital Output ---> in this case all number are
   positive and indicate the pin index in the variant structure */
#define OPTA_DIGITAL_OUT_INDEX_0 0
#define OPTA_DIGITAL_OUT_INDEX_1 1
#define OPTA_DIGITAL_OUT_INDEX_2 2
#define OPTA_DIGITAL_OUT_INDEX_3 3
#define OPTA_DIGITAL_OUT_INDEX_4 4
#define OPTA_DIGITAL_OUT_INDEX_5 5
#define OPTA_DIGITAL_OUT_INDEX_6 6
#define OPTA_DIGITAL_OUT_INDEX_7 7

/* any analog value under this value will be considered LOW
   the converter works between 0 and 16383  */
#define OPTA_DIGITAL_TH_FOR_PIN_LOW 3000 // 5.2 Volts

#define OPTA_DIGITAL_WATCHTDOG_TIME_ms 0xFFFF

/* enable the use of Opta Digital as standalone module (Without OPTA controller
 * this feature is not fully tested) */
// #define EN_DIGITAL_STANDALONE

/* +++++++++++++++++++++++++ DEBUG EXPANSIONS DEFINES +++++++++++++++++++++++ */

/* Please note that this defines triggers the serial debug on some specific
 * functions, however the serial debug must be enabled in CommonCfg.h */

/* following depend on previous DEBUG_PARSE_MODULE_ENABLE and enable
   log messages for single parsing message on OPTA digital */
// #define DEBUG_PARSE_OPTA_ENABLE
// #define DEBUG_PARSE_OPTA_DOUT_ENABLE
// #define DEBUG_PARSE_OPTA_DIN_ENABLE
// #define DEBUG_PARSE_OPTA_AIN_ENABLE

/* print log on the setup of ADC pin and setup ADC */
// #define DEBUG_ADC_SETUP_OPTA_DIGITAL

// #define DEBUG_UPDATE_PIN_ENABLE

/* enable log of ADC register values (before the conversion in range 0-65536) */
// #define DEBUG_SERIAL_ADC_REG

// #define DEBUG_OPTA_DIGITAL_INPUT

#endif
