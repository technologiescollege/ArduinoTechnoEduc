/* 1-wire connection demo to a HD44780-compatible LCD via shiftregister

ShiftRegLCD123 wiring
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
*/

#include <ShiftRegLCD123.h>

// LCD signals from Arduino pin 10
const byte lcdPin  = 10;

// Instantiate a LCD object, using SRLCD123 wiring as default
ShiftRegLCD123 srlcd(lcdPin);

void setup()
{
  // Initialize LCD and set display size
  // LCD size 20 columns x 2 lines, small (normal) font
  srlcd.begin(20,2);
  
  // Turn on backlight (if used)
  srlcd.backlightOn();
  
  // Print a message to the LCD.
  srlcd.print("HELLO, WORLD!");

  // move to next line
  srlcd.setCursor(0,1);
  srlcd.print("SRLCD123 1-wire test");
}

void loop()
{
}

