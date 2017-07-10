/*

ShiftRegLCD123  - 1,2 and 3-wire shiftregister-based HD44780 compatible LCD library for Arduino

Connects a Hitachi HD44780 compatible LCD using 1, 2 or 3 pins from the Arduino
via an 8-bit shift register (SR from now on).
HW compatible with LCD3Wire and old ShiftRegLCD libraries.



Copyright (C) 2009-2012 raron

GNU GPLv3 license
 
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


  Contact: raronzen@gmail.com   (Not always checked)



Acknowledgements:

  Based very much on the "official" LiquidCrystal library for the Arduino:
    http://arduino.cc/en/Reference/Libraries
  And the improved version (with examples CustomChar1 and SerialDisplay) from LadyAda:
    http://web.alfredstate.edu/weimandn/arduino/LiquidCrystal_library/LiquidCrystal_index.html
  Plus slightly (very) inspired by this schematics from an unknown author
    (thanks to mircho on the arduino playground forum!):
    http://www.scienceprog.com/interfacing-lcd-to-atmega-using-two-wires/
  And also the -excellent- explanation of the "Shift1 system" for the PIC, at
    http://www.romanblack.com/shift1.htm
  And FINALLY: A proper initialization routine for HD44780 compatible LCD's:
    http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html



This is just modified to work serially with an 8-bit shift register and an LCD.

1-wire connections requires a latched shift register.
  Note that 1-wire mode is rather slow because the digitalWrite method is slow,
  and the RC networks timings needed to rely on that. Also I didn't want to
  mess with direct port access to keep compatibility between Arduino's.
  In any event 1-wire is just a novelty, last resort if you just have ONE pin left.
2-wire is slightly easier to use with an unlatched shift register
  (but latched is also possible - see schematics by Nystrom and Bill).
3-wire mode latched, unless using older ShiftRegLCD schematics, in which case
  an unlatched shift register can be used together with LCD enable directly
  from an Arduino pin.

For reference / schematics:

Project homepage     : http://code.google.com/p/shiftreglcd123/
Old ShiftRegLCD page : http://code.google.com/p/arduinoshiftreglcd/
LCD3Wire library page: http://www.arduino.cc/playground/Code/LCD3wires



CONNECTION DESCRIPTIONS

ShiftRegLCD123

  Requires 1, 2 or 3 pins from the Arduino.
  Also arduinoshiftreglcd / ShiftRegLCD compatible, except added 1-wire mode.

SR output:  SRLCD123 wiring:
  Bit  #0   - N/C - not connected. Must be 0 in 1-wire and 2-wire mode.
  Bit  #1   - N/C or LCD backlight. Do not connect directly!
              Use a driver / transistor!
  Bit  #2   - connects to RS (Register Select) on the LCD
  Bits #3-6 - connects to LCD data inputs D4 - D7.
  Bit  #7   - 1-wire mode: To LCD Enable input.
              2-wire mode: To diode cathode of the diode-resistor AND "gate"
              3-wire mode: N/C, unused. LCD Enable directly from Arduino.

  LCD R/!W-pin hardwired to LOW (only writing to LCD).



LCD3Wire connections

  Requires 1 or 3 pins from the arduino.
  (2-wire is not possible, since LCD Enable is not on MSB of shift register)

SR output: LCD3wire wiring:
  Bit  #0   - Zero if 1-wire mode, LCD backlight in 3-wire mode.
              Do not connect directly! Use a driver / transistor!
  Bit  #1   - connects to RS (Register Select) on the LCD
  Bit  #2   - LCD R/!W pin, always 0 (zero).
  Bit  #3   - LCD enable
  Bits #4-7 - connects to LCD data inputs D7 - D4 (in reverse order).


In both schemes the backlight is optional, used to hold a zero otherwise.
If using a 1-wire mode with LCD3wire, bit#0 is zero regardless (IE no backlight).
Busy Flag (BF, data bit D7) is not read.


USAGE:

  1: Make an LCD object, set arduino output pins and LCD wiring scheme:

        ShiftRegLCD123 LCDobject( Datapin [, scheme] )
    or  ShiftRegLCD123 LCDobject( Datapin , Clockpin, scheme )
    or  ShiftRegLCD123 LCDobject( Datapin , Clockpin, Latchpin, scheme )

    where:
      Datapin : Arduino pin to shiftregister serial data input.
      Clockpin: Arduino pin to shiftregister clock input.
      Latchpin: Arduino pin to shiftregister latch/strobe/register clock input.
      scheme  : SRLCD123 or LCD3WIRE.
                Indicates shiftregister to LCD wiring type / variant.

  2: Initialize the LCD by calling begin() function with LCD size and font:

        LCDobject.begin( cols, lines [, font] )

    where:
      cols    : Nr. of columns in the LCD
      lines   : Nr. of "logical lines" in the LCD (not neccesarily physical)
      font    : 0 = small (default), 1 = large font for some 1-line LCD's only.



History
2012.02.12 raron - removed the unneccesary SRLCD123 3-wire method, going for the
           "old" and slightly faster ShiftRegLCD way renamed to SRLCD123.
           Thus reducing SR to LCD connection schemes to two and less confusing.
2012.02.10 raron - Implemented fully old ShiftRegLCD 3-wire compatibility
           (Still backlight at bit # 1 possible though)
2012.02.07 raron - Finally implemented a proper LCD initialization!
           Thanks to arduino forum member florista's posts.
2012.02.05 raron - Modified library to be similar in use as LiquidCrystal.
           Namely, included a begin() function.
2012.02.02 raron - Added LCD3Wire compatibility, added 1-wire shiftout method.
           Renamed 4 functions to be the same as in the LiquidCrystal library:
             shiftLeft(), shiftRight(), shiftIncrement() and shiftDecrement()
             to leftToRight(), rightToLeft(), autoscroll() and noAutoscroll()
           Borrowed the 16x4 LCD setCursor memory map from Francisco's
           NewLiquidCrystal library, and some comments.
2012.02.01 raron - namechange: ShiftRegLCD123, to reflect 1,2 or 3 wires
           via shiftregisters.
2012.01.31 raron - Updated for Arduino 1.0, thanks to suggestions
           from florian.fida. Also (I think) debugged setCursor method, but
           haven't coped for 16x4 LCD's yet since I don't specify LCD size.
2011.07.02 raron - Fixed a minor flaw in setCursor function. No functional
           change, just a bit more memory efficient.
           Thanks to CapnBry (from google code and github) who noticed it.
           URL to his version of shiftregLCD:
           https://github.com/CapnBry/HeaterMeter/commit/c6beba1b46b092ab0b33bcbd0a30a201fd1f28c1
2009.07.30 raron - minor corrections to the comments.
           Fixed timing to datasheet safe. Fixed keyword highlights.
2009.07.28 Mircho / raron - a new modification to the schematics, and a
           more streamlined interface
2009.07.27 raron - Thanks to an excellent suggestion from mircho from Arduino 
           playground forum, the number of wires now required is only two!
2009.07.25 raron - Fixed comments. I really messed up the comments before 
           posting this, so I had to fix it.
           Renamed a function, but no improvements or functional changes.
2009.07.23 raron - Incorporated some proper initialization routines
           inspired (lets say copy-paste-tweaked) from LiquidCrystal
           library improvements from LadyAda.
2009.05.23 raron - first version, but based mostly (as in almost verbatim)
           on the "official" LiquidCrystal library.
*/


// Indicates wether or not a diode + small resistor quicker charging
// is used for 1-wire shiftregister circuit, if defined.
// Comment out next line if not charging quicker via diodes.
#define LCD_SLIGHTLY_FASTER_1_WIRE

#include "ShiftRegLCD123.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// CONSTRUCTORS
// ---------------------------------------------------------------------------

// Assume ONE_WIRE connection, SRLCD123 wiring
// Only sets Arduino output pin to the shiftregister.
ShiftRegLCD123::ShiftRegLCD123( uint8_t srdata )
{
	init( srdata, srdata, srdata, SRLCD123 );
}

// Assume ONE_WIRE connection
// Only set Arduino output pin and wiring scheme
ShiftRegLCD123::ShiftRegLCD123( uint8_t srdata, uint8_t scheme )
{
	init( srdata, srdata, srdata, scheme );
}

// Assume TWO_WIRE connection
// Set data, clock pin and wiring scheme
ShiftRegLCD123::ShiftRegLCD123( uint8_t srdata, uint8_t srclock, uint8_t scheme )
{
	init( srdata, srclock, srdata, scheme);
}

// Assume THREE_WIRE connection
// Set data, clock pins and wiring scheme
ShiftRegLCD123::ShiftRegLCD123( uint8_t srdata, uint8_t srclock, uint8_t enable,
                                uint8_t scheme )
{
	init( srdata, srclock, enable, scheme);
}


// PRIVATE METHODS
// ---------------------------------------------------------------------------

void ShiftRegLCD123::init( uint8_t srdata, uint8_t srclock, uint8_t enable,
                           uint8_t scheme )
{
  _srdata_pin  = srdata;
  _srclock_pin = srclock;
  _enable_pin  = enable;
  _scheme      = scheme;  // Indicates connection scheme used from SR to LCD
  _numwires    = 3;       // Indicates nr. of pins connected from Arduino to SR
  _backlight   = 0;       // Defaults to backlight off.

  // Figure out nr. of control pins from the Arduino to the shift register
  // if using LCD3WIRE wiring, _numwires should never be 2 (can be 1 or 3 then)
  if (srdata == enable)  _numwires = 2;   // At most 2 wires in use
  if (srdata == srclock) _numwires = 1;   // Only 1 wire in use

  pinMode(_srclock_pin, OUTPUT);
  pinMode(_srdata_pin, OUTPUT);
  pinMode(_enable_pin, OUTPUT);
  
  // In 1-wire mode, signal line needs to be high (to charge capacitors).
  if (_numwires == 1)
  {
    shiftOut1wire (_srdata_pin, MSBFIRST, 0x00);
    digitalWrite(_srdata_pin, HIGH);
  } else {
    shiftOut (_srdata_pin, _srclock_pin, MSBFIRST, 0x00);
    digitalWrite(_enable_pin, LOW);
  }
}


void ShiftRegLCD123::begin( uint8_t cols, uint8_t lines )
{
  begin(cols,lines,0);
}

void ShiftRegLCD123::begin( uint8_t cols, uint8_t lines, uint8_t font )
{
  _numlines    = lines;
  _cols        = cols;    // Only used for identifying 16x4 LCD, nothing else.

  if (lines>1)
    _displayfunction = LCD_4BITMODE | LCD_2LINE;
  else
    _displayfunction = LCD_4BITMODE | LCD_1LINE;


  // For some 1-line displays you can select a 10 pixel high font
  if (font != 0 && lines == 1)
    _displayfunction |= LCD_5x10DOTS;
  else
    _displayfunction |= LCD_5x8DOTS;

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 
  // more.
  // Also check this -excellent- guide on HD44780-based LCD initializations:
  // http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html
  // (From which this initialization and comments are taken from)
  // ---------------------------------------------------------------------------

  // Step 1.   Power on, then delay > 100 ms
  //delay(200);
  for (int i=0; i<200; ++i) delayMicroseconds(1000);  // Waits 200 ms just in case.
  
  // Step 2.   Instruction 0011b (3h), then delay > 4.1 ms 
  send(LCD_FUNCTIONSET | LCD_8BITMODE, LCD_UPPER_NIBBLE);
  delayMicroseconds(4500);  // wait more than 4.1ms
  
  // Step 3.   Instruction 0011b (3h), then delay > 100 us
  //   NOTE: send()/command() and init4bits() functions adds 60 us delay
  send(LCD_FUNCTIONSET | LCD_8BITMODE, LCD_UPPER_NIBBLE);
  delayMicroseconds(40);
  
  // Step 4.   Instruction 0011b (3h), then delay > 100 us 
  send(LCD_FUNCTIONSET | LCD_8BITMODE, LCD_UPPER_NIBBLE);
  delayMicroseconds(40);

  // Step 5.   Instruction 0010b (2h), then delay > 100 us (speculation)
  //   This is where display is set to 4-bit interface
  send(LCD_FUNCTIONSET | LCD_4BITMODE, LCD_UPPER_NIBBLE);
  delayMicroseconds(40);
  
  // Step 6.   Instruction 0010b (2h), then 1000b (8h), then delay > 53 us or check BF 
  //   Set nr. of logical lines (not neccesarily physical lines) and font size
  //   (usually 2 lines and small font)
  command(LCD_FUNCTIONSET | _displayfunction);
  //delayMicroseconds(60);

  // Step 7.   Instruction 0000b (0h), then 1000b (8h) then delay > 53 us or check BF
  command(LCD_DISPLAYCONTROL);
  //delayMicroseconds(60);

  // Step 8.   Instruction 0000b (0h), then 0001b (1h) then delay > 3 ms or check BF 
  //   This is a Clear display instruction and takes a lot of time
  command(LCD_CLEARDISPLAY);
  delayMicroseconds(3000);
  
  // Step 9.   Instruction 0000b (0h), then 0110b (6h), then delay > 53 us or check BF 
  //   Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  command(LCD_DISPLAYCONTROL | _displaymode);
  //delayMicroseconds(60);

  // Step 10. Not really a step, but initialization has ended. Except display is turned off.
  
  // Step 11.   Instruction 0000b (0h), then 1100b (0Ch), then delay > 53 us or check BF
  //   Turn display on, no cursor, no cursor blinking.
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
  //delayMicroseconds(60);
}



// Commands used by the other functions to control the LCD

void ShiftRegLCD123::command(uint8_t value)
{
  send(value, LOW);
}

#if defined(ARDUINO) && ARDUINO >= 100
size_t ShiftRegLCD123::write(uint8_t value)
#else
void ShiftRegLCD123::write(uint8_t value)
#endif
{
  send(value, HIGH);
}


// For sending data via the shiftregister

void ShiftRegLCD123::send(uint8_t value, uint8_t mode)
{
  uint8_t nibnr = 0;
  uint8_t direction;
  
  do
  {
    uint8_t nibble1 = 0;
    uint8_t nibble2 = 0;
    
    // Clear shiftregister between nibbles in 2-wire mode
    if (_numwires == 2) shiftOut ( _srdata_pin, _srclock_pin, MSBFIRST, 0x00 );

    // Format nibble according to shiftregister wiring to the LCD
    if (_scheme == SRLCD123)
    {
      if (nibnr == 0)
      {
        direction = MSBFIRST;
        if (mode != LCD_UPPER_NIBBLE)
        {
          // RS bit: LOW = command,  HIGH = character
          mode = mode ? SRLCD123_RS_BIT : 0;
          mode = _backlight ? mode | SRLCD123_BACKLIGHT : mode;  // Backlight
          nibble1 = mode;
        }
        // UPPER NIBBLE
        nibble1 |= SRLCD123_EN_BIT | ((value >> 1) & 0x78);      // LCD Enable On
        nibble2 = nibble1 & ~SRLCD123_EN_BIT;                    // LCD Enable Off
      } else {
        // LOWER NIBBLE
        nibble1 = mode | SRLCD123_EN_BIT | ((value << 3) & 0x78);// LCD Enable On
        nibble2 = nibble1 & ~SRLCD123_EN_BIT;                    // LCD Enable Off
      }
    }

    if (_scheme == LCD3WIRE)
    {
      if (nibnr == 0)
      {
        direction = LSBFIRST;
        if (mode != LCD_UPPER_NIBBLE)
        {
          // RS bit: LOW = command,  HIGH = character
          mode = mode ? LCD3WIRE_RS_BIT : 0;
          mode = _backlight ? mode | LCD3WIRE_BACKLIGHT : mode;  // Backlight
          nibble1 = mode;
        }
        // UPPER NIBBLE
        nibble1 |= LCD3WIRE_EN_BIT | ((value >> 4) & 0x0F);      // Enable ON
        nibble2 = nibble1 & ~LCD3WIRE_EN_BIT;                    // Enable OFF
      } else {
        // LOWER NIBBLE
        nibble1 = mode | LCD3WIRE_EN_BIT | (value & 0x0F);       // Enable ON
        nibble2 = nibble1 & ~LCD3WIRE_EN_BIT;                    // Enable OFF
      }
    }

    // Push out the nibble to the LCD
    if (_numwires == 1)
    {
      // Nibble + LCD enable
      shiftOut1wire (_srdata_pin, direction, nibble1);
      // enable pulse must be >450ns  
      delayMicroseconds(1);
      // same nibble again without LCD enable
      shiftOut1wire (_srdata_pin, direction, nibble2);
    } else {
      shiftOut (_srdata_pin, _srclock_pin, direction, nibble1);
      // LCD ENABLE PULSE and / or shiftregister latch
      digitalWrite( _enable_pin, HIGH );
      // enable pulse must be >450ns  
      delayMicroseconds(1);
      digitalWrite( _enable_pin, LOW );
    }
    if (_numwires == 3 && _scheme != SRLCD123)
    {
      // same nibble again without LCD enable
      shiftOut (_srdata_pin, _srclock_pin, direction, nibble2);
      // Shiftregister latch
      digitalWrite( _enable_pin, HIGH );
      digitalWrite( _enable_pin, LOW );
    }
    
    // skip lower nibble when initializing
    if (mode == LCD_UPPER_NIBBLE) ++nibnr;
  } while (++nibnr<2);

  // commands need > 37us to settle (for normal LCD with clock = 270 kHz)
  // Since LCD's may be slow (190 kHz clock = 53 us), we'll wait 60 us
  delayMicroseconds(60);

}



// 1-wire shiftregister data output

void ShiftRegLCD123::shiftOut1wire(uint8_t datapin, uint8_t dir, uint8_t value)
{
  long bitwait;
  uint8_t bit;
  
  // Push out 7 bits (last must be zero)
  for (int i=0; i<7; i++)
  {
    if (dir == MSBFIRST) bit = (value & 0x80);
    else bit = (value & 0x01);
    
    if (bit > 0) bitwait = 1; else bitwait = 100;
    
    digitalWrite(datapin, LOW);
    delayMicroseconds (bitwait);
    digitalWrite(datapin, HIGH);
    
    if (dir == MSBFIRST) value = value << 1;
    else value = value >> 1;

    // Wait before next bit can be transmitted
    // not needed to wait much if recharging quicker via diode and small resistor
#if !defined (LCD_SLIGHTLY_FASTER_1_WIRE)
    delayMicroseconds (bitwait*2);
#else
    // Also, digitalWrite is really slow enough, but waiting a bit just in case
    delayMicroseconds (5);
#endif
  }

  // Latch output, makes last bit = 0
  digitalWrite(datapin, LOW);
  delayMicroseconds (1000);
  digitalWrite(datapin, HIGH);

  // not needed to wait much if recharging quicker via diode and small resistor
#if !defined (LCD_SLIGHTLY_FASTER_1_WIRE)
  delayMicroseconds (1500);
#else
  delayMicroseconds (5);
#endif

}




// PUBLIC METHODS
// high level commands, for the user
// ---------------------------------------------------------------------------

// Backlight On / Off control
void ShiftRegLCD123::backlightOn()
{
  _backlight = 1;
  // Just to force a write to the shift register
  command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
}

void ShiftRegLCD123::backlightOff()
{
  _backlight = 0;
  // Just to force a write to the shift register
  command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
}

void ShiftRegLCD123::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(3000);    // this command takes a long time!
}

void ShiftRegLCD123::home()
{
  command(LCD_RETURNHOME);    // set cursor position to zero
  delayMicroseconds(3000);    // this command takes a long time!
}


void ShiftRegLCD123::setCursor(uint8_t col, uint8_t row)
{
   const uint8_t row_offsetsDef[]   = { 0x00, 0x40, 0x14, 0x54 }; // For regular LCDs
   const uint8_t row_offsetsLarge[] = { 0x00, 0x40, 0x10, 0x50 }; // For 16x4 LCDs
   
   if (row >= _numlines) row = _numlines-1;    // rows start at 0
   
   // 16x4 LCDs have special memory map layout
   // ----------------------------------------
   if ( _cols == 16 && _numlines == 4 )
      command(LCD_SETDDRAMADDR | (col + row_offsetsLarge[row]));
   else 
      command(LCD_SETDDRAMADDR | (col + row_offsetsDef[row]));
}


// Turn the display on/off (quickly)
void ShiftRegLCD123::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void ShiftRegLCD123::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void ShiftRegLCD123::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void ShiftRegLCD123::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void ShiftRegLCD123::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void ShiftRegLCD123::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void ShiftRegLCD123::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void ShiftRegLCD123::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void ShiftRegLCD123::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void ShiftRegLCD123::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void ShiftRegLCD123::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void ShiftRegLCD123::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations with custom characters
void ShiftRegLCD123::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7;              // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | location << 3);
  for (int i=0; i<8; i++) {
    write((uint8_t)charmap[i]);
  }
  command(LCD_SETDDRAMADDR);    // Reset display to display text (from pos. 0)
}

