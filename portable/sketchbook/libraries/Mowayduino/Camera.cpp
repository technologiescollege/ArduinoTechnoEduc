//********************************************************************************
//*    Camera.cpp                                                                *
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

#include "Camera.h"
#include <SPI.h>
#include "MowayduinoIo.h"



void Camera::beginCamera()
{
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  digitalWrite(CS_IO,HIGH);
  digitalWrite(RF_CS,HIGH);
  digitalWrite(CS_ACC,HIGH);
  digitalWrite(CS_EXP,LOW);
  pinMode(CS_EXP, OUTPUT); 
  pinMode(IRQ_EXP, INPUT); 
  pinMode(CS_IO, OUTPUT);
  pinMode(RF_CS, OUTPUT);
  pinMode(CS_ACC, OUTPUT);
  

  cam_command(CAM_WR,CAM_IODIR_ADD,0xC0);
  cam_command(CAM_WR,CAM_OLAT_ADD,0x00);

}

void Camera::cameraOn(byte channel)
{
  byte i;
  switch(channel)
  {
  case 1:
    // CH1 pin output, others input (high impedance)
    cam_command(CAM_WR, CAM_IODIR_ADD, 0xCC);
    break;
  case 2:
    // CH2 pin output, others input
    cam_command(CAM_WR, CAM_IODIR_ADD, 0xD4);
    break;
  case 3:
    // CH3 pin output, others input
    cam_command(CAM_WR, CAM_IODIR_ADD, 0xD8);
    break;
  case 4:
    // CH4 all input
    cam_command(CAM_WR, CAM_IODIR_ADD, 0xDC);
    break;
  }

  cam_command(CAM_WR, CAM_OLAT_ADD, 0x02);

  //Led blink
  for(i=0;i<10;i++)
  {
    cam_command(CAM_WR, CAM_OLAT_ADD, 0x22);
    delay (50);
    cam_command(CAM_WR, CAM_OLAT_ADD, 0x02);
    delay (50);
  }
  //Led on
  cam_command(CAM_WR, CAM_OLAT_ADD, 0x22);
}

void Camera::cameraOff()
{
  cam_command(CAM_WR, CAM_OLAT_ADD, 0x00);
}

void Camera::cam_command (byte command, byte address, byte data)
{
  digitalWrite(CS_EXP,1);
  SPI.transfer(command);
  SPI.transfer(address);
  SPI.transfer(data);
  digitalWrite(CS_EXP,0);
}

