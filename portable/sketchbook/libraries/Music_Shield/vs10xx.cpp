/*
 * vs10xx.cpp
 * A library for MusicShield 2.0
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Jack Shao (jacky.shaoxg@gmail.com)
 * Create Time: Mar 2014
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <SPI.h>
#include "vs10xx.h"
#include "pins_config.h"
VS10XX vs1053;

//software patch for MIDI Play
const unsigned short gVS1053_MIDI_Patch[28]={
/*if you don't let GPIO1 = H,please send this patch by spi*/
0x0007, 0x0001, 0x8050, 0x0006, 0x0014, 0x0030, 0x0715, 0xb080, /* 0 */
0x3400, 0x0007, 0x9255, 0x3d00, 0x0024, 0x0030, 0x0295, 0x6890, /* 8 */
0x3400, 0x0030, 0x0495, 0x3d00, 0x0024, 0x2908, 0x4d40, 0x0030, /* 10 */
0x0200, 0x000a, 0x0001, 0x0050,
};

/*
 **@ function name: init
 **@ usage:init the vs10xx chip
 **@ input:none
 **@ retval:none
*/
void VS10XX::init(void)
{
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  initIO();
  reset();
}

/*
 **@ function name: initForMidiFmt
 **@ usage:init the vs10xx chip for midi Mode
 **@ input:none
 **@ retval:none
*/
void VS10XX::initForMidiFmt(void)
{
  //init IO
  initIO();

  //init SPI
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  //SPI.transfer(0xFF);	//transfer a dump

  //reset the chip
  reset();
  //load MIDI plugin
  loadMidiPlugin();
}

void VS10XX::initIO(void)
{
  pinMode(VS_XRESET, OUTPUT);
  pinMode(VS_DREQ, INPUT);
  pinMode(VS_XDCS, OUTPUT);
  pinMode(VS_XCS, OUTPUT);
  digitalWrite(VS_XDCS, HIGH);
  digitalWrite(VS_XCS, HIGH);
}

void VS10XX::writeRegister(unsigned char addressbyte, unsigned char highbyte, unsigned char lowbyte)
{
  // while (!readDREQ());
  deselectDataBus();
  selectControlBus();
  SPI.transfer(VS_WRITE_COMMAND);
  SPI.transfer((addressbyte));
  SPI.transfer((highbyte));
  SPI.transfer((lowbyte));
  deselectControlBus();
}

void VS10XX::writeRegister(unsigned char addressbyte, unsigned int value)
{
  writeRegister(addressbyte, value >>8, value & 0xff);
}

void VS10XX::writeData(unsigned char *databuf, unsigned char datalen)
{
  selectDataBus();
  while (!readDREQ());
  while (datalen--)
  {
    SPI.transfer(*databuf++);
  }
  deselectDataBus();
}

unsigned int VS10XX::readRegister(unsigned char addressbyte)
{
  unsigned int result = 0;
  unsigned char highbyte = 0;
  unsigned char lowbyte = 0;
  deselectDataBus();
  selectControlBus(); //XCS = 0
  SPI.transfer(VS_READ_COMMAND); //send read command
  SPI.transfer(addressbyte);    //send register address
                                //be careful: when read 0xff,dump it,and read agin
                                //while((h = SPIGetChar())== 0xff);
  highbyte = SPI.transfer(0xff);
  //while((l = VsReadByte())== 0xff);
  lowbyte = SPI.transfer(0xff);

  result = (((unsigned int)highbyte << 8) | lowbyte);
  deselectControlBus();
  return result;
}
// reset for vs10xx
void VS10XX::reset(void)
{
  putInReset();
  delay(100); //it is a must
  /* Send dummy SPI byte to initialize atmel SPI */
  ////SPIPutCharWithoutWaiting(0xFF);
  deselectControlBus();
  deselectDataBus();
  releaseFromReset();
  while (!readDREQ());
  /* Set clock register, doubler etc. */
  writeRegister(SPI_CLOCKF, 0xc0, 0x00);
  Serial.print("\r\nVS10xx Clock Frq: 0x");
  Serial.println(readRegister(SPI_CLOCKF), HEX);
  /* Wait for DREQ */
  while (!readDREQ());
  softReset(); //comment this, as it will be executed everytime playing a music file.

  writeRegister(SPI_WRAMADDR, 0xc0, 0x13);

  /* Switch on the analog parts */
  setVolume(40, 40);
  //setVolume(0xff,0xff);
  //SPISetFastClock();
}
/** Soft Reset of VS10xx (Between songs) */
void VS10XX::softReset(void)
{
  /* Soft Reset of VS10xx */
  writeRegister(SPI_MODE, 0x08, 0x04); /* Newmode, Reset, No L1-2 */
  //Delay(1);
  while (!readDREQ()); /* wait for startup */
  //setup I2S (see page77 of the datasheet of vs1053 )
  //set GPIO0 as output
  writeRegister(SPI_WRAMADDR, 0xc0, 0x17);
  writeRegister(SPI_WRAM, 0x00, 0xf0);
  //enable I2S (MCLK enabled, 48kHz sample rate)
  writeRegister(SPI_WRAMADDR, 0xc0, 0x40);
  writeRegister(SPI_WRAM, 0x00, 0x0C);
  while (!readDREQ());
  //LoadUserPatch();
}
/** Set VS10xx Volume Register */
void VS10XX::setVolume(unsigned char leftchannel, unsigned char rightchannel)
{
  writeRegister(11, (leftchannel), (rightchannel));
}

void VS10XX::sendZerosToVS10xx(void)
{
  unsigned int i;
  selectDataBus();
  for (i = 0; i < 2048; i++)
  {
    while (!readDREQ());
    SPI.transfer(0);
  }
  deselectDataBus();
}

/*
 **@ function name: loadMidiPatch
 **@ usage:load a software patch for vs10xx
 **@ input:none
 **@ retval:none
*/
void VS10XX::loadMidiPlugin(void)
{
  int i=0;
  Serial.print("load MIDI Plugin...\r\n");
  while(i < sizeof(gVS1053_MIDI_Patch)/sizeof(gVS1053_MIDI_Patch[0]))
  {
    unsigned short addr, n, val;
    addr = gVS1053_MIDI_Patch[i++];
    n = gVS1053_MIDI_Patch[i++];
    while(n--)
    {
      val = gVS1053_MIDI_Patch[i++];
      writeRegister(addr, val >> 8, val&0xff);
    }
  }
  Serial.print("done\r\n");
}
/****************The end***********************************************/
