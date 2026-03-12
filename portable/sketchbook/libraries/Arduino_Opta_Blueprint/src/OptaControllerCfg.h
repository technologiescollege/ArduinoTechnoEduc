/* -------------------------------------------------------------------------- */
/* FILE NAME:   OptaControllerCfg.h
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20231116
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef CONTROLLERCFG_H
#define CONTROLLERCFG_H
/* the pin used by CONTROLLER to identify if MODULES are connected */
#define OPTA_CONTROLLER_DETECT_PIN PG_8
/* the maximum number of opta expansion that can be attached */
/* initial delay once the reset command has been sent, it waits for all Modules
 * to perform its own initialization */
#define OPTA_CONTROLLER_SETUP_INIT_DELAY 2000

/* update rate when placed into the main loop */
#define OPTA_CONTROLLER_UPDATE_RATE 1000

// allow update devices in update function
// #define UPDATE_DEVICES_IN_UPDATE

/* opta wait OPTA_CONTROLLER_DELAY_AFTER_MSG_SENT after sendig the set command
   in order to allow modules to update their interna state */
#define OPTA_CONTROLLER_DELAY_AFTER_MSG_SENT 2

#define OPTA_CONTROLLER_DELAY_AFTER_SET_ADDRESS 100

/* how much opta controller will wait an answer from a Module
   It exits as soon as the message is received */
#define OPTA_CONTROLLER_WAIT_REQUEST_TIMEOUT 50

#define OPTA_CONTROLLER_TIMEOUT_FOR_SETUP_MESSAGE 100

#define OPTA_CONTROLLER_DELAY_AFTER_REBOOT 600

/* this is the time the controller leaves to expansion to "set up" themselves
 * after a successfully I2C address assignment */
#define OPTA_CONTROLLER_DELAY_EXPANSION_RESET 300

/* when DETECT IN goes low it wait OPTA_CONTROLLER_DEBOUNCE_TIME *
   OPTA_CONTROLLER_DEBOUNCE_NUMBER ms before to consider the PIN really low */
#define OPTA_CONTROLLER_DEBOUNCE_LOW_TIME 1
#define OPTA_CONTROLLER_DEBOUNCE_LOW_NUMBER 50

#define OPTA_CONTROLLER_DEBOUNCE_UP_TIME 1
#define OPTA_CONTROLLER_DEBOUNCE_UP_NUMBER 50

#define OPTA_BLUE_UNDEFINED_DEVICE_NUMBER 255
/* -----------------------------------------------------------------------------
 * DEBUG CONFIGURATION DEFINES
 * -------------------------------------------------------------------------- */

// #define DEBUG_TX_CONTROLLER_ENABLE
//  #define DEBUG_RX_CONTROLLER_ENABLE
//  #define DEBUG_PARSE_CONTROLLER_ENABLE
//  #define DEBUG_PARSE_DIN_CONTROLLER_ENABLE
//  #define DEBUG_PARSE_AIN_CONTROLLER_ENABLE

// #define DEBUG_SET_FUNCTION_CONTROLLER
// #define DEBUG_GET_FUNCTION_CONTROLLER

// #define DEBUG_MSG_FUNCTION_CONTROLLER

// #define DEBUG_MSG_PARSE_ADDRESS_AND_TYPE

// #define DEBUG_CONTROLLER_PRINT_DEVICES

// #define DEBUG_ASSIGN_ADDRESS_CONTROLLER

#endif
