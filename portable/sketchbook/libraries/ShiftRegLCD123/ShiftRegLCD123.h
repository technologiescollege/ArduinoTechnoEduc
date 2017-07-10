#ifndef ShiftRegLCD123_h
#define ShiftRegLCD123_h

#include <inttypes.h>
#include "Print.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// Shift register to LCD wiring scheme
#define SRLCD123 123
#define LCD3WIRE 3

// Flag to use only the upper nibble during LCD initialization
#define LCD_UPPER_NIBBLE 0xFF


// Shift register bits connected to LCD control pins

// ShiftRegLCD / arduinoshiftreglcd compatible wiring
// (except formerly unused backlight control bit addition for ShiftRegLCD123)
#define SRLCD123_BACKLIGHT 0x02
#define SRLCD123_RS_BIT 0x04
#define SRLCD123_EN_BIT 0x80

// LCD3wire library compatible wiring
// (in reverse order because of wiring and using LSBFIRST to shiftregister)
#define LCD3WIRE_BACKLIGHT 0x80
#define LCD3WIRE_RS_BIT 0x40
#define LCD3WIRE_RW_BIT 0x20
#define LCD3WIRE_EN_BIT 0x10



class ShiftRegLCD123 : public Print {
public:
  ShiftRegLCD123(uint8_t srdata);
  ShiftRegLCD123(uint8_t srdata, uint8_t scheme);
  ShiftRegLCD123(uint8_t srdata, uint8_t srclock, uint8_t scheme);
  ShiftRegLCD123(uint8_t srdata, uint8_t srclock, uint8_t enable, uint8_t scheme);
  void begin( uint8_t cols, uint8_t lines);
  void begin( uint8_t cols, uint8_t lines, uint8_t font);
  void clear();
  void home();
  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();
  void backlightOn();
  void backlightOff();
  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t);

#if defined(ARDUINO) && ARDUINO >= 100
  virtual size_t write(uint8_t);
#else
  virtual void write(uint8_t);
#endif

  void command(uint8_t);
private:
  void init(uint8_t srdata, uint8_t srclock, uint8_t enable, uint8_t scheme);
  void send(uint8_t, uint8_t);
  void init4bits(uint8_t);
  void shiftOut1wire(uint8_t, uint8_t, uint8_t);
  uint8_t _srdata_pin;
  uint8_t _srclock_pin;
  uint8_t _enable_pin;
  uint8_t _numwires;
  uint8_t _scheme;   // wiring type from SR to LCD: SRLCD123 or LCD3wire.

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t _numlines;
  uint8_t _cols;
  uint8_t _backlight;

};

#endif
