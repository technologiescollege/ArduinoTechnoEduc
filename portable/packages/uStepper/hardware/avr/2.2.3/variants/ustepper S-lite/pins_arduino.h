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

#define NUM_DIGITAL_PINS            24
#define NUM_ANALOG_INPUTS           6
#define analogInputToDigitalPin(p)  ((p == 0) ? 11 : ((p == 1) ? 12 : ((p == 2) ? 13 : ((p == 3) ? 15 : ((p == 4) ? 14 : ((p == 5) ? 16 : -1))))))

#define digitalPinHasPWM(p)         ((p) == 2 || (p) == 3)

//Pins available on the user accessible headers
static const uint8_t RXD0 = 0;
static const uint8_t TXD0 = 1;

static const uint8_t RXD1 = 5;
static const uint8_t TXD1 = 4;

static const uint8_t EXTINT0 = 3;
static const uint8_t EXTINT1 = 2;

static const uint8_t SCL0 = 14;
static const uint8_t SDA0 = 15;

static const uint8_t SCK0 = 6;
static const uint8_t MISO0 = 5;
static const uint8_t MOSI0 = 4;
static const uint8_t SS0 = 17;

static const uint8_t SCK1 = 13;
static const uint8_t MISO1 = 12;
static const uint8_t MOSI1 = 11;
static const uint8_t SS1 = 16;

static const uint8_t A0 = 11;
static const uint8_t A1 = 12;
static const uint8_t A2 = 13;
static const uint8_t A4 = 14;
static const uint8_t A3 = 15;
static const uint8_t A5 = 16;

static const uint8_t AUX1 = 16;
static const uint8_t AUX2 = 9;
static const uint8_t AUX3 = 10;

//Pins NOT available to the user directly

static const uint8_t SCL1 = 7;
static const uint8_t SDA1 = 8;

static const uint8_t DIR = 17;
static const uint8_t STEP = 18;
static const uint8_t ENA = 19;

#define LED_BUILTIN 2

#define analogPinToChannel(p)	((p == 11) || (p == 0) ? 1 : ((p == 12) || (p == 1) ? 0 : ((p == 13) || (p == 2) ? 7 : ((p == 15) || (p == 3) ? 4 : ((p == 14) || (p == 4) ? 5 : ((p == 16) || (p == 5) ? 6 : -1))))))

#define digitalPinToPCICR(p)    ( \
									( \
										((p) <= 6) || ((p) >= 9 && (p) <= 16) \
									)  ? (&PCICR) : ((uint8_t *)0) \
								)

#define digitalPinToPCICRbit(p) ( \
									(((p) <= 3)) ? 2 : \
										((((p) >= 4 && (p) <= 6)) || (p) == 10 || (p) == 9 ? 0 :\
											(((p) <= 11 || (p) == 16 || (p) == 8 || (p) == 7) ? 3 : 1))\
								)

#define digitalPinToPCMSK(p)    ( \
									(((p) <= 3)) ? (&PCMSK2) : \
										((((p) >= 4 && (p) <= 6)) || (p) == 10 || (p) == 9  ? (&PCMSK0) : \
											(((p) <= 11 || (p) == 16 || (p) == 8 || (p) == 7) ? (&PCMSK3) : \
												(((p) <= 15) ? (&PCMSK1) : ((uint8_t *)0)))) \
								)

#define digitalPinToPCMSKbit(p) ( \
									((p) <= 1)? (p) : \
										(((p) == 2) ? (p) + 1 : \
											((((p) == 3) || (p) == 4) || (p) == 5 || (p) == 6 ? (p) - 1 : \
												(((p) == 12) || (p) == 13 || (p) == 19)? (p) - 12 : \
													(((p) == 7 || (p) == 9 || (p) == 10) ? (p) - 6 : \
														(((p) == 8 || (p) == 11) ? (p) - 8 : \
															((p) == 15 ? (p) - 11 : ((p) == 14 ? (p) - 10 : \
																(((p) == 16 || (p) == 18) ? (p) - 14 : \
																	(p) - 16)))))))\
								)

#define digitalPinToInterrupt(p)  ((p) == INT0 ? 0 : ((p) == INT1 ? 1 : NOT_AN_INTERRUPT))

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
	(uint16_t) &DDRE,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
	(uint16_t) &PORTE,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
	(uint16_t) &PINE,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, // D0 - RXD0 - PD0
	PD,	// D1 - TXD0 - PD1
	PD, // D2 - PD3 - PWM - INT1 - LED
	PD, // D3 - PD2 - PWM - INT0
	PB, // D4 - MOSI0 - TXD1 - PB3
	PB, // D5 - MISO0 - RXD1 - PB4
	PB, // D6 - SCK0 - PB5
	PE, // D7 - SCL1 - PE1
	PE, // D8 - SDA1 - PE0
	PD, // D9 - AUX2 - PD6
	PB, // D10 - AUX3 - PB1
	PE, // D11 - A0 - MOSI1 - PE3
	PC, // D12 - A1 - MISO1 - PC0
	PC, // D13 - A2 - SCK1 - PC1
	PC, // D14 - A3 - SCL0 - PC5
	PC, // D15 - A4 - SDA0 - PC4
	PE, // D16 - A5 - AUX1 - SS1 - PE2
	PB, // D17 - DIR - PB2
	PD, // D18 - STEP - PD7
	PD, // D19 - ENA - PD4
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), // D0 - RXD0 - PD0
	_BV(1),	// D1 - TXD0 - PD1
	_BV(3), // D2 - PD3 - PWM - INT1 - LED
	_BV(2), // D3 - PD2 - PWM - INT0
	_BV(3), // D4 - MOSI0 - TXD1 - PB3
	_BV(4), // D5 - MISO0 - RXD1 - PB4
	_BV(5), // D6 - SCK0 - PB5
	_BV(1), // D7 - SCL1 - PE1
	_BV(0), // D8 - SDA1 - PE0
	_BV(6), // D9 - AUX2 - PD6
	_BV(1), // D10 - AUX3 - PB1
	_BV(3), // D11 - A0 - MOSI1 - PE3
	_BV(0), // D12 - A1 - MISO1 - PC0
	_BV(1), // D13 - A2 - SCK1 - PC1
	_BV(5), // D14 - A4 - SCL0 - PC5
	_BV(4), // D15 - A3 - SDA0 - PC4
	_BV(2), // D16 - A5 - AUX1 - SS1 - PE2
	_BV(2), // D17 - DIR - PB2
	_BV(7), // D18 - STEP - PD7
	_BV(4), // D19 - ENA - PD4
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, // D0 - RXD0 - PD0
	NOT_ON_TIMER,	// D1 - TXD0 - PD1
	TIMER2B, // D2 - PD3 - PWM - INT1 - LED
	TIMER3B, // D3 - PD2 - PWM - INT0
	NOT_ON_TIMER, // D4 - MOSI0 - TXD1 - PB3
	NOT_ON_TIMER, // D5 - MISO0 - RXD1 - PB4
	NOT_ON_TIMER, // D6 - SCK0 - PB5
	NOT_ON_TIMER, // D7 - SCL1 - PE1
	NOT_ON_TIMER, // D8 - SDA1 - PE0
	NOT_ON_TIMER, // D9 - AUX2 - PD6
	NOT_ON_TIMER, // D10 - AUX3 - PB1
	NOT_ON_TIMER, // D11 - A0 - MOSI1 - PE3
	NOT_ON_TIMER, // D12 - A1 - MISO1 - PC0
	NOT_ON_TIMER, // D13 - A2 - SCK1 - PC1
	NOT_ON_TIMER, // D14 - A4 - SCL0 - PC5
	NOT_ON_TIMER, // D15 - A3 - SDA0 - PC4
	NOT_ON_TIMER, // D16 - A5 - AUX1 - SS1 - PE2
	NOT_ON_TIMER, // D17 - DIR - PB2
	NOT_ON_TIMER, // D18 - STEP - PD7
	NOT_ON_TIMER, // D19 - ENA - PD4
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
