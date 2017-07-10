/* 3-wire connection demo to a HD44780-compatible LCD via shiftregister

LCD3wire compatible wiring

SR output: LCD3wire wiring:
 Bit  #0   - Zero if 1-wire mode, LCD backlight in 3-wire mode.
             Do not connect directly! Use a driver / transistor!
 Bit  #1   - connects to RS (Register Select) on the LCD
 Bit  #2   - LCD R/!W bit, always 0 (zero).
 Bit  #3   - LCD enable
 Bits #4-7 - connects to LCD data inputs D7 - D4 (in reverse order).

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
      lines   : Nr. of lines in the LCD
      font    : 0 = small (default), 1 = large font for some 1-line LCD's only.
*/

#include <ShiftRegLCD123.h>


const byte datapin  = 10;    // SR Data from Arduino pin 10
const byte clockpin = 11;    // SR Clock from Arduino pin 11
const byte latchpin = 12;    // SR Latch (or Strobe, RCK / Register Clock, etc) from Arduino pin 12

// Using LCD3wire compatible wiring  
ShiftRegLCD123 srlcd(datapin, clockpin, latchpin, LCD3WIRE);

void setup()
{
  // initialize LCD and set display size
  // LCD size 20 columns x 2 lines, small (normal) font
  srlcd.begin(20,2);
  
  // Turn on backlight (if used)
  srlcd.backlightOn();
  
  // Print a message to the LCD.
  srlcd.print("HELLO, WORLD!");
  // move to next line
  srlcd.setCursor(0,1);
  srlcd.print("LCD3wire 3-wire test");
}

void loop()
{
}

