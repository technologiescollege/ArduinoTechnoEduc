/* -------------------------------------------------------------------------- */
/* FILE NAME:   OptaDigitalProtocol.h
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240508
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef OPTADIGITALPROTOCOL
#define OPTADIGITALPROTOCOL

#include "OptaBlueProtocol.h"
/* msg get opta-digital digital input */
#define ARG_OD_GET_DIGITAL_INPUTS 0x04
#define LEN_OD_GET_DIGITAL_INPUTS 0x00

/* msg get opta-digital analog input */
#define ARG_OD_GET_ANALOG_INPUT 0x05
#define LEN_OD_GET_ANALOG_INPUT 0x01

/* define set opta-digital digital output*/
#define ARG_OD_SET_DIGITAL_OUTPUTS 0x06
#define LEN_OD_SET_DIGITAL_OUTPUTS 0x01

/* define get opta-digital ALL analog input */
#define ARG_OD_GET_ALL_ANALOG_INPUTS 0x07
#define LEN_OD_GET_ALL_ANALOG_INPUTS 0x00

/* define set opta-digital default output and timeout */
#define ARG_OD_DEFAULT_AND_TIMEOUT 0x08
#define LEN_OD_DEFAULT_AND_TIMEOUT 0x03

/* answer get opta-digital digital input */
#define ANS_ARG_OD_GET_DIGITAL_INPUTS ARG_OD_GET_DIGITAL_INPUTS
#define ANS_LEN_OD_GET_DIGITAL_INPUTS 0x02

/* answer get opta-digital analog input */
#define ANS_ARG_OD_GET_ANALOG_INPUT ARG_OD_GET_ANALOG_INPUT
#define ANS_LEN_OD_GET_ANALOG_INPUT 0x02

/* answer get opta-digital ALL analog input */
#define ANS_ARG_OD_GET_ALL_ANALOG_INPUTS ARG_OD_GET_ALL_ANALOG_INPUTS
#define ANS_LEN_OD_GET_ALL_ANALOG_INPUTS (16 * 2)

#define ANS_ARG_OD_DEFAULT_AND_TIMEOUT ARG_OD_DEFAULT_AND_TIMEOUT
#define ANS_LEN_OD_DEFAULT_AND_TIMEOUT 0

/* answer get opta-digital set default and timeout */
#define ANS_ARG_OD_SET_DIGITAL_OUTPUTS ARG_OD_SET_DIGITAL_OUTPUTS
#define ANS_LEN_OD_SET_DIGITAL_OUTPUTS 0

#define OPTA_DIGITAL_GET_DIN_BUFFER_DIM (ANS_LEN_OD_GET_DIGITAL_INPUTS + BP_HEADER_DIM + 1)
#define OPTA_DIGITAL_GET_ALL_AIN_BUFFER_DIM (ANS_LEN_OD_GET_ALL_ANALOG_INPUTS + BP_HEADER_DIM  + 1)

#endif
