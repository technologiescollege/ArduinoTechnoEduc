/* -------------------------------------------------------------------------- */
/* FILE NAME:   OptaCfg.h
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240506
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef OPTABLUEPRINTCFG
#define OPTABLUEPRINTCFG

#define EXPANSION_NOT_VALID (0)
#define EXPANSION_DIGITAL_INVALID (1)
#define EXPANSION_OPTA_DIGITAL_MEC (2)
#define EXPANSION_OPTA_DIGITAL_STS (3)
#define EXPANSION_OPTA_ANALOG (4)
#define EXPANSION_CUSTOM (5) // not actually used

typedef union float_union {
  float value;
  unsigned char bytes[4];
} Float_u;

/* MAX Dimension of the I2C buffer for transmission and reception both for
 * controller and Expansions */
#define OPTA_I2C_BUFFER_DIM 48

#define OPTA_CONTROLLER_MAX_EXPANSION_NUM 5
/* the first addressed assigned to a MODULE */
#define OPTA_CONTROLLER_FIRST_AVAILABLE_ADDRESS 0x0B
/* first temporary address */
#define OPTA_CONTROLLER_FIRST_TEMPORARY_ADDRESS                                \
  (OPTA_CONTROLLER_FIRST_AVAILABLE_ADDRESS + OPTA_CONTROLLER_MAX_EXPANSION_NUM)

#define OPTA_DEFAULT_SLAVE_I2C_ADDRESS 0x0A
#define OPTA_CONTROLLER_CUSTOM_MIN_TYPE 100
#endif
