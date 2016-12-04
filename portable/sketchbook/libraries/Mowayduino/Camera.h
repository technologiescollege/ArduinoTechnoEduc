//********************************************************************************
//*    Camera.h                                                                  *
//*    version: 0.1                                                              *
//*    This library contains  functions to control mowayduino camera             *
//*    				                                                 *
//*    Copyright (C) 2010  Minirobots S.L.                                       *
//*                                                                              *
//********************************************************************************
//*    This program is free software; you can redistribute it and/or modify      *
//*    it under the terms of the GNU General Public License as published by      *
//*    the Free Software Foundation; either version 3 of the License, or         *
//*    (at your option) any later version.                                       *
//*                                                                              *
//*    This program is distributed in the hope that it will be useful,           *
//*    but WITHOUT ANY WARRANTY; without even the implied warranty of            *
//*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
//*    GNU General Public License for more details.                              *
//*                                                                              *
//*    You should have received a copy of the GNU General Public License along   *
//*    with this program; if not, write to the Free Software Foundation, Inc.,   *
//*    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
//********************************************************************************

#ifndef Camera_h
#define Camera_h

#include "Arduino.h"


//*******************************************
//		DEFINES FOR MCP23S08 REGISTERS
//*******************************************
// Write opcode
#define CAM_WR			0x40
#define CAM_RD			0x41

// Registro IODIR
#define CAM_IODIR_ADD		0x00

// Registro OLAT
#define CAM_OLAT_ADD		0x0A

class  Camera{
  
public:

  void beginCamera();
  void cameraOn(byte channel);
  void cameraOff();
  
private:

  void cam_command (byte command, byte address, byte data);
};

#endif

