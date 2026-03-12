/* -------------------------------------------------------------------------- */
/* FILE NAME:   ExpansionOperations.h
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240216
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef EXPANSIONOPERATIONS
#define EXPANSIONOPERATIONS

/*
 * OPERATIONS SUPPORTED by DIGITAL
 * */
#define SET_DIGITAL_OUTPUT 1       // Digital
#define GET_DIGITAL_INPUT 2        // Digital, Analog
#define GET_SINGLE_ANALOG_INPUT 3  // Digital, Analog
#define GET_ALL_ANALOG_INPUT 4     // Digital, Analog
#define SET_DEFAULT_OUTPUT_VALUE 5 // Digital

/*
 * SUPPORTED BY OA and OD
 * */
#define WRITE_FLASH 7
#define READ_FLASH 8
#define GET_VERSION 9

#define BEGIN_CHANNEL_AS_ADC 10
#define BEGIN_CHANNEL_AS_DI 11
#define BEGIN_CHANNEL_AS_RTD 12
#define BEGIN_CHANNEL_AS_DAC 13

#define SET_PWM 14
#define SET_SINGLE_ANALOG_OUTPUT 15
#define GET_RTD 16
#define SET_LED 17
#define SEND_TIMING 18
#define SET_ALL_ANALOG_OUTPUTS 19
#define BEGIN_CHANNEL_AS_HIGH_IMP 20
#define GET_CHANNEL_FUNCTION 21


#endif
