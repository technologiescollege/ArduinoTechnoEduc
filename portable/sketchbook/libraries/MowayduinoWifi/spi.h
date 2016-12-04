/******************************************************************************

  Filename:		spi.h
  Description:	SPI bus configuration for the WiShield 1.0

 ******************************************************************************

  TCP/IP stack and driver for the WiShield 1.0 wireless devices

  Copyright(c) 2009 Async Labs Inc. All rights reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc., 59
  Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Contact Information:
  <asynclabs@asynclabs.com>

   Author               Date        Comment
  ---------------------------------------------------------------
   AsyncLabs			05/01/2009	Initial version
   AsyncLabs			05/29/2009	Adding support for new library

 *****************************************************************************/
 /* Modifyed by Daniele Denaro  2012*/


#ifndef SPI_H_
#define SPI_H_

#define USE_DIG0_INTR	  //used in other files

/***** select platform changing comment (just one define must be active) *****/

//#define ARDUINO1        //UNO
//#define TIDIGINO         //or ATMega2560
#define LEONARDO

/*****************************************************************************/

#define ZG2100_ISR_DISABLE()	(EIMSK &= ~(0x02))     
#define ZG2100_ISR_ENABLE()   (EIMSK |= 0x02)
#define ZG2100_ISR_GET(X)     (X = EIMSK)
#define ZG2100_ISR_SET(X)     (EIMSK = X)

#define BIT0                     0x01
#define BIT1                     0x02
#define BIT2                     0x04
#define BIT3                     0x08
#define BIT4                     0x10
#define BIT5                     0x20
#define BIT6                     0x40
#define BIT7                     0x80

// SS    (not used!)
#ifdef ARDUINO1                              //Same as CS !
#define SPI0_SS_BIT              BIT2             
#define SPI0_SS_DDR              DDRB
#define SPI0_SS_PORT             PORTB
#endif
                                           
#ifdef TIDIGINO                             //Not connected
#define SPI0_SS_BIT              BIT0               
#define SPI0_SS_DDR              DDRB
#define SPI0_SS_PORT             PORTB
#endif

#ifdef LEONARDO                             //Not connected
#define SPI0_SS_BIT              BIT0               
#define SPI0_SS_DDR              DDRB
#define SPI0_SS_PORT             PORTB
#endif

// SPI-CLK    socket ICSP (3)
#ifdef ARDUINO1                              //AT328 PB5 - Arduino pin 13
#define SPI0_SCLK_BIT            BIT5              
#define SPI0_SCLK_DDR            DDRB
#define SPI0_SCLK_PORT           PORTB
#endif

#ifdef TIDIGINO
#define SPI0_SCLK_BIT            BIT1       //ATMega2560 PB1 - TiDiGino pin 52         
#define SPI0_SCLK_DDR            DDRB
#define SPI0_SCLK_PORT           PORTB
#endif

#ifdef LEONARDO
#define SPI0_SCLK_BIT            BIT1       //ATMEGA32U4
#define SPI0_SCLK_DDR            DDRB
#define SPI0_SCLK_PORT           PORTB
#endif



// SPI-MOSI   socket ICSP (4)
#ifdef ARDUINO1
#define SPI0_MOSI_BIT            BIT3        //AT328 PB3 - Arduino pin 11    
#define SPI0_MOSI_DDR            DDRB
#define SPI0_MOSI_PORT           PORTB
#endif

#ifdef TIDIGINO
#define SPI0_MOSI_BIT            BIT2        //ATMega2560 PB2 - TiDiGino pin 51     
#define SPI0_MOSI_DDR            DDRB
#define SPI0_MOSI_PORT           PORTB
#endif

#ifdef LEONARDO
#define SPI0_MOSI_BIT            BIT2        //ATMEGA32U4   
#define SPI0_MOSI_DDR            DDRB
#define SPI0_MOSI_PORT           PORTB
#endif

// SPI-MISO   socket ICSP (1)
#ifdef ARDUINO1
#define SPI0_MISO_BIT            BIT4        //AT328 PB4 - Arduino pin 12    
#define SPI0_MISO_DDR            DDRB
#define SPI0_MISO_PORT           PORTB
#endif

#ifdef TIDIGINO
#define SPI0_MISO_BIT            BIT3        //ATMega2560 PB3 - TiDiGino pin 50   
#define SPI0_MISO_DDR            DDRB
#define SPI0_MISO_PORT           PORTB
#endif

#ifdef LEONARDO
#define SPI0_MISO_BIT            BIT3        //ATMEGA32U4
#define SPI0_MISO_DDR            DDRB
#define SPI0_MISO_PORT           PORTB
#endif



// SPI slave chip select CS      shield pin 10
#ifdef ARDUINO1
#define ZG2100_CS_BIT            BIT2        //AT328  PB2 - Arduino pin 10   
#define ZG2100_CS_DDR            DDRB
#define ZG2100_CS_PORT           PORTB
#endif

#ifdef TIDIGINO
#define ZG2100_CS_BIT            BIT4        //ATMega2560 PB4 -TiDiGino pin 10     
#define ZG2100_CS_DDR            DDRB
#define ZG2100_CS_PORT           PORTB
#endif

#ifdef LEONARDO
#define ZG2100_CS_BIT            BIT3        //ATMEGA32U4
#define ZG2100_CS_DDR            DDRD
#define ZG2100_CS_PORT           PORTD
#endif

/*
// Shield led  pin 9
#ifdef ARDUINO1
#define LEDConn_BIT              BIT1        //AT328 PB1 - Arduino pin 9
#define LEDConn_DDR				 DDRB              
#define LEDConn_PORT			 PORTB             
#endif

#ifdef TIDIGINO                              //ATMega2560 PH6 - TiDiGino pin 9
#define LEDConn_BIT              BIT6          
#define LEDConn_DDR              DDRH          
#define LEDConn_PORT             PORTH         
#endif

#ifdef LEONARDO                              //ATMEGA32U4
#define LEDConn_BIT              BIT4          
#define LEDConn_DDR              DDRD          
#define LEDConn_PORT             PORTD         
#endif*/

#ifdef ARDUINO1
#define SPI0_Init()	DDRB  |= SPI0_SS_BIT|SPI0_SCLK_BIT|SPI0_MOSI_BIT|LEDConn_BIT;\
										DDRB  &= ~SPI0_MISO_BIT;\
										PORTB = SPI0_SS_BIT;\
										SPCR  = 0x50;\
										SPSR  = 0x01
#endif

#ifdef TIDIGINO
#define SPI0_Init()	DDRB  |= SPI0_SS_BIT|SPI0_SCLK_BIT|SPI0_MOSI_BIT;\
										DDRB  &= ~SPI0_MISO_BIT;DDRH |= LEDConn_BIT;\
										PORTB = SPI0_SS_BIT;\
										SPCR  = 0x50;\
										SPSR  = 0x01
#endif

#ifdef LEONARDO
#define SPI0_Init()	DDRB  |= SPI0_SS_BIT|SPI0_SCLK_BIT|SPI0_MOSI_BIT;\
										DDRB  &= ~SPI0_MISO_BIT;DDRD;\
										PORTB = SPI0_SS_BIT;\
										SPCR  = 0x50;\
										SPSR  = 0x01
#endif


#define SPI0_WaitForReceive()
#define SPI0_RxData()                (SPDR)

#define SPI0_TxData(Data)            (SPDR = Data)
#define SPI0_WaitForSend()			     while( (SPSR & 0x80)==0x00 )        

#define SPI0_SendByte(Data)          SPI0_TxData(Data);SPI0_WaitForSend()
#define SPI0_RecvBute()              SPI0_RxData()


#define ZG2100_SpiInit                SPI0_Init
#define ZG2100_SpiSendData            SPI0_SendByte
#define ZG2100_SpiRecvData            SPI0_RxData


#define ZG2100_CSInit()             (ZG2100_CS_DDR |= ZG2100_CS_BIT)
#define ZG2100_CSon()               (ZG2100_CS_PORT |= ZG2100_CS_BIT)
#define ZG2100_CSoff()					    (ZG2100_CS_PORT &= ~ZG2100_CS_BIT)    
//#define ZG2100_CSon()               (ZG2100_CS_PORT &= ~ZG2100_CS_BIT)         
//#define ZG2100_CSoff()					    (ZG2100_CS_PORT |= ZG2100_CS_BIT)  

//#define LEDConn_on()   (LEDConn_PORT |= LEDConn_BIT)


//#define LEDConn_off()	(LEDConn_PORT &= ~LEDConn_BIT)


#endif /* SPI_H_ */
