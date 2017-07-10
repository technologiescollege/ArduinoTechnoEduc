// Example creating custom characters 0-7 on a HD44700 compatible LCD
// Using SRLCD123 2-wire connection to LCD

#include <ShiftRegLCD123.h>

const byte dataPin  = 10;    // SR Data from Arduino pin 10
const byte clockPin = 11;    // SR Clock from Arduino pin 11

// Instantiate a LCD object
ShiftRegLCD123 srlcd(dataPin, clockPin, SRLCD123);

void setup()
{
  // Initialize LCD to 20 columns, 2 lines.
  srlcd.begin(20,2);

  // Turn on backlight (if used)
  srlcd.backlightOn();

  // Some example custom characters
  uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
  uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
  uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
  uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
  uint8_t duck[8]  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
  uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
  uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
  uint8_t retarrow[8] = {0x1,0x1,0x5,0x9,0x1f,0x8,0x4};
  
  srlcd.createChar(0, bell);
  srlcd.createChar(1, note);
  srlcd.createChar(2, clock);
  srlcd.createChar(3, heart);
  srlcd.createChar(4, duck);
  srlcd.createChar(5, check);
  srlcd.createChar(6, cross);
  srlcd.createChar(7, retarrow);

  // Print the custom characters to the LCDs 1. line
  for (uint8_t i=8;i>0;i--) srlcd.write((uint8_t)i);

  // Move to next line
  srlcd.setCursor(0,1);

  // Print another message on line 2
  srlcd.print("CUSTOM CHARACTERS");
}

void loop()
{
}

