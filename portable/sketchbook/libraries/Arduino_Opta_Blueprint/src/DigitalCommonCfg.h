/* -------------------------------------------------------------------------- */
/* FILE NAME:   CommonDigital.h
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

#ifndef COMMONDIGITAL
#define COMMONDIGITAL

/* number of digital output in opta digital expansion */
#define OPTA_DIGITAL_OUT_NUM 8
#define OPTA_DIGITAL_IN_NUM 16
/* the DEFAULT ADDRESS the SLAVE uses before the address initialization process
 */

/* this define (if defined) allow the reading of OPTA DIGITAL input as ANALOG
   INPUT */
#define OPTA_DIGITAL_ALLOW_ANALOG_USE

#define MAX_FLASH_BUFFER_DIM 32
#define PRODUCTION_DATA_FLASH_ADDRESS 0x1C00
#define EXPANSION_TYPE_ADDITIONA_DATA                                          \
  (PRODUCTION_DATA_FLASH_ADDRESS + MAX_FLASH_BUFFER_DIM)
#define FLASH_OD_TYPE_MECHANICAL 0
#define FLASH_OD_TYPE_STATE_SOLID 1

#define OPTA_DIGITAL_DESCRIPTION "ARDUINO OPTA DIGITAL"
#define OPTA_DIGITAL_MECH_DESCRIPTION "ARDUINO OPTA DIGITAL MECH"
#define OPTA_DIGITAL_STSOLID_DESCRIPTION "ARDUINO OPTA DIGITAL STSOLID"
#endif
