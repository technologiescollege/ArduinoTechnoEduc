/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            20
#define NUM_ANALOG_INPUTS           6
#define analogInputToDigitalPin(p)  ((p < 6) ? (p) + 14 : -1)

#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 4 || (p) == 5 || (p) == 8 || (p) == 9)

static const uint8_t SS   = 8;
static const uint8_t MOSI = 9;
static const uint8_t MISO = 10;
static const uint8_t SCK  = 11;
#define LED_BUILTIN 11

static const uint8_t STEP  = 12;
static const uint8_t DIR  = 13;
static const uint8_t ENA  = 18;
static const uint8_t ASDIR  = 19;

static const uint8_t SDA = 7;
static const uint8_t SCL = 6;

static const uint8_t A0 = 14;
static const uint8_t A1 = 15;
static const uint8_t A2 = 16;
static const uint8_t A3 = 17;
static const uint8_t ASA = 21;
static const uint8_t TEMP = 20;

#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) ((((p) <= 5) || ((p) == 12) || ((p) == 13)) ? 2 : ((((p) <= 11) || ((p) >= 8)) ? 0 : 1))
#define digitalPinToPCMSK(p)    ((((p) <= 5) || ((p) == 12) || ((p) == 13)) ? (&PCMSK2) : ((((p) <= 11) || ((p) >= 8)) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0)))) // FIX DENNE LINJE!
#define digitalPinToPCMSKbit(p) (((p) <= 5) ? (p) : (((p) <= 7) ? ((p) - 2) : (((p) == 12) ? ((p) - 8) : (((p) <= 13) ? ((p) - 6) : (((p) <= 19) ? ((p) - 18) : ((p) - 14))))))

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))

#ifdef ARDUINO_MAIN

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, // D0 - RXD - PD0
	PD,	// D1 - TXD - PD1
	PD, // D2 - PD2
	PD, // D3 - PD3
	PD, // D4 - PD5
	PD, // D5 - PD6
	PC, // D6 - SCL - PC5
	PC, // D7 - SDA - PC4
	PB, // D8 - SS - LED - PB2
	PB, // D9 - MOSI - PB3
	PB, // D10 - MISO - PB4
	PB, // D11 - SCK - PB5
	PD, // D12 - STEP - PD4
	PD, // D13 - DIR - PD7
	PC, // D14 - A0 - PC0
	PC, // D15 - A1 - PC1
	PC, // D16 - A2 - PC2
	PC, // D17 - A2 - PC3
	PB, // D18 - ENA - PB0
	PB, // D19 - ASDIR - PB1
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), // D0 - RXD - PD0
	_BV(1), // D1 - TXD - PD1
	_BV(2), // D2 - PD2
	_BV(3), // D3 - PD3
	_BV(5), // D4 - PD5
	_BV(6), // D5 - PD6
	_BV(5), // D6 - SCL - PC5
	_BV(4), // D7 - SDA - PC4
	_BV(2), // D8 - SS - LED - PB2
	_BV(3), // D9 - MOSI - PB3
	_BV(4), // D10 - MISO - PB4
	_BV(5), // D11 - SCK - PB5
	_BV(4), // D12 - STEP - PD4
	_BV(7), // D13 - DIR - PD7
	_BV(0), // D14 - A0 - PC0
	_BV(1), // D15 - A1 - PC1
	_BV(2), // D16 - A2 - PC2
	_BV(3), // D17 - A2 - PC3
	_BV(0), // D18 - ENA - PB0
	_BV(1), // D19 - ASDIR - PB1
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, 	// D0 - RXD - PD0
	NOT_ON_TIMER, 	// D1 - TXD - PD1
	NOT_ON_TIMER,	// D2 - PD2
	TIMER2B, 		// D3 - PD3		
	TIMER0B,		// D4 - PD5
	TIMER0A,		// D5 - PD6
	NOT_ON_TIMER,	// D6 - SCL - PC5
	NOT_ON_TIMER,	// D7 - SDA - PC4
	NOT_ON_TIMER,	// D8 - SS - LED - PB2
	TIMER1B, 		// D9 - MOSI - PB3
	TIMER2A,		// D10 - MISO - PB4
	NOT_ON_TIMER,	// D11 - SCK - PB5
	NOT_ON_TIMER,	// D12 - STEP - PD4
	NOT_ON_TIMER,	// D13 - DIR - PD7
	NOT_ON_TIMER,	// D14 - A0 - PC0
	NOT_ON_TIMER, 	// D15 - A1 - PC1
	NOT_ON_TIMER,	// D16 - A2 - PC2
	NOT_ON_TIMER,	// D17 - A2 - PC3
	NOT_ON_TIMER,	// D18 - ENA - PB0
	NOT_ON_TIMER,	// D19 - ASDIR - PB1
};	

#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR   Serial
#define SERIAL_PORT_HARDWARE  Serial

#endif
