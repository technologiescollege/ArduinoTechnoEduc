/* -------------------------------------------------------------------------- */
/* FILENAME:    Protocol.h
   AUTHOR:      Daniele Aimo (d.aimo@arduino.cc)
   DATE:        20230801
   REVISION:    0.0.1
   DESCRIPTION: This file contains configuration defines and the definition of
                some types
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef OPTA_PROTOCOL_COMMON
#define OPTA_PROTOCOL_COMMON

/* -----------------------------------------------------------------------------
 * PROTOCOL DEFINES
 * -------------------------------------------------------------------------- */
#define BP_USE_CRC

#define BP_HEADER_DIM 0x03

#define BP_CMD_POS 0x00
#define BP_ARG_POS 0x01
#define BP_LEN_POS 0x02
#define BP_PAYLOAD_START_POS 0x03

#define BP_CMD_SET 0x01
#define BP_CMD_GET 0x02
#define BP_ANS_GET 0x03
#define BP_ANS_SET 0x04

#endif
