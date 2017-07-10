// Displays text sent over the serial port (e.g. from the Serial Monitor) on
// an attached LCD, using 2-wire connection to the LCD.

#include <ShiftRegLCD123.h>

const byte datapin  = 10;    // SR Data from Arduino pin 10
const byte clockpin = 11;    // SR Clock from Arduino pin 11

// Instantiate an LCD object
ShiftRegLCD123 srlcd(datapin, clockpin, SRLCD123);

void setup()
{
  // Initialize LCD and set display size
  // LCD size 20 columns x 2 lines, defaults to small (normal) font
  srlcd.begin(20,2);
  
  // Turn on backlight (if used)
  srlcd.backlightOn();

  // Start the serial port
  Serial.begin(9600);
}

void loop()
{
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    srlcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      srlcd.write(Serial.read());
    }
  }
}

