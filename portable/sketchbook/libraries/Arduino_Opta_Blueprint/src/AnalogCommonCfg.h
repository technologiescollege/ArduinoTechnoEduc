/* -------------------------------------------------------------------------- */
/* FILE NAME:   CommonAnalog.h
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240415
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

/* This files contains definitions and types that are used both at the level
 * of Analog Fw and of the Controller */

#ifndef COMMONANALOG
#define COMMONANALOG

typedef enum { OA_VOLTAGE_ADC, OA_CURRENT_ADC } OaAdcType_t;
typedef enum { OA_VOLTAGE_DAC, OA_CURRENT_DAC } OaDacType_t;

/* ###################### Channel functions ################################# */

typedef enum {
  CH_FUNC_HIGH_IMPEDENCE,
  CH_FUNC_VOLTAGE_OUTPUT,
  CH_FUNC_CURRENT_OUTPUT,
  CH_FUNC_VOLTAGE_INPUT,
  CH_FUNC_CURRENT_INPUT_EXT_POWER,    //DAC CURRENT
  CH_FUNC_CURRENT_INPUT_LOOP_POWER,   //
  CH_FUNC_RESISTANCE_MEASUREMENT,
  CH_FUNC_DIGITAL_INPUT,
  CH_FUNC_DIGITAL_INPUT_LOOP_POWER,
  CH_FUNC_UNDEFINED,
  CH_FUNC_RESISTANCE_MEASUREMENT_3_WIRES
} CfgFun_t;

#if defined ARDUINO_UNO_TESTALOG_SHIELD
#define OA_AN_CHANNELS_NUM 4
#define OA_GPO_NUM 4
#define OA_AN_DEVICES_NUM 1
#define OA_PWM_CHANNELS_NUM 4
#else
#define OA_AN_CHANNELS_NUM 8
#define OA_PWM_CHANNELS_NUM 4
#define OA_GPO_NUM 8
#define OA_AN_DEVICES_NUM 2

#endif

#define OA_AN_DEVICE_0 0
#define OA_AN_DEVICE_1 1
#define OA_CH_0 0
#define OA_CH_1 1
#define OA_CH_2 2
#define OA_CH_3 3
#define OA_CH_4 4
#define OA_CH_5 5
#ifdef ARDUINO_UNO_TESTALOG_SHIELD
#define OA_CH_6 2
#define OA_CH_7 3
#else
#define OA_CH_6 6
#define OA_CH_7 7
#endif
#define OA_CH_8 8
#define OA_CH_9 9
#define OA_CH_10 10
#define OA_CH_11 11
#define OA_CH_NUM 12

#define OA_PWM_CH_FIRST OA_CH_8
#define OA_PWM_CH_0 OA_CH_8
#define OA_PWM_CH_1 OA_CH_9
#define OA_PWM_CH_2 OA_CH_10
#define OA_PWM_CH_3 OA_CH_11
#define OA_PWM_CH_LAST OA_CH_11

#define OA_LED_1 0
#define OA_LED_2 1
#define OA_LED_3 2
#define OA_LED_4 3
#define OA_LED_5 4
#define OA_LED_6 5
#define OA_LED_7 6
#define OA_LED_8 7

#define OA_LED_NUM 8

#define OA_FIRST_PWM_CH 8
#define OA_LAST_PWM_CH 11

#define OA_MAX_DAC_VALUE 8191

#define MAX_FLASH_BUFFER_DIM 32
#define PRODUCTION_DATA_FLASH_ADDRESS 0x1C00

#define OPTA_ANALOG_DESCRIPTION "ARDUINO OPTA ANALOG"
#endif
