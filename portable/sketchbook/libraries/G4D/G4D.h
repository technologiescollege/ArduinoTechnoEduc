/*
GOLDELOX SGC Library by Mark Medrano 2011
Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php

*/

#include "colors.h"

#ifndef GOLDELOX_4D
#define GOLDELOX_4D

#include <inttypes.h>
#include <arduino.h>

#define G4D_SERIAL_RESPONSE_TIMEOUT		1000
#define G4D_STARTUP_DELAY				1200
#define G4D_STARTUP_MAX_RETRIES			10

#define G4D_ACK						0x06
#define G4D_NAK 					0x15

// 4DSL Commands
#define	G4D_CMD_AUTOBAUD			0x55
#define	G4D_CMD_VERSION				0x56
#define	G4D_CMD_REPLACE_BACKGROUND	0x42
#define	G4D_CMD_CLEAR				0x45
#define	G4D_CMD_CONTROL				0x59
#define	G4D_CMD_SLEEP				0x5A
#define	G4D_CMD_JOYSTICK			0x4A
#define	G4D_CMD_WAITJOYSTICK		0x6A
#define	G4D_CMD_SOUND				0x4E
#define	G4D_CMD_TUNE				0x6E
#define	G4D_CMD_ADD_USER_BITMAP		0x41
#define	G4D_CMD_DRAW_USER_BITMAP	0x44
#define	G4D_CMD_CIRCLE				0x43
#define	G4D_CMD_TRIANGLE			0x47
#define	G4D_CMD_IMAGE				0x49
#define	G4D_CMD_SET_BACKGROUND		0x48
#define	G4D_CMD_LINE				0x4C
#define	G4D_CMD_PIXEL				0x50
#define	G4D_CMD_READ_PIXEL			0x52
#define	G4D_CMD_SCREEN_COPY_PASTE	0x63
#define	G4D_CMD_POLYGON				0x67
#define	G4D_CMD_REPLACE_COLOR		0x6B
#define	G4D_CMD_PEN					0x70
#define	G4D_CMD_RECTANGLE			0x72
#define	G4D_CMD_FONT				0x46
#define	G4D_CMD_OPACITY				0x4F
#define	G4D_CMD_STRING				0x53
#define	G4D_CMD_ASCII_CHAR			0x54
#define	G4D_CMD_BUTTON				0x62
#define	G4D_CMD_STRING_CHAR_COORD	0x73
#define	G4D_CMD_ASCII_CHAR_GRAPHICS	0x74

#define	G4D_CMD_EXTENDED			0x40
#define	G4D_CMD_SET_ADDRESS			0x41
#define	G4D_CMD_SCREEN_COPY			0x43
#define	G4D_CMD_SDIMAGE				0x49
#define	G4D_CMD_OBJECT				0x4F
#define	G4D_CMD_RUN_SCRIPT			0x50
#define	G4D_CMD_READ_SECTOR			0x52
#define	G4D_CMD_VIDEO				0x56
#define	G4D_CMD_WRITE_SECTOR		0x57
#define	G4D_CMD_INIT_USD			0x69
#define	G4D_CMD_READ_BYTE			0x72
#define	G4D_CMD_WRITE_BYTE			0x77

// command flags

#define G4D_COLORMODE_8BIT		0x08
#define G4D_COLORMODE_16BIT		0x10
#define G4D_DISPLAY_ON			0x01
#define G4D_DISPLAY_CONTRAST	0x02
#define G4D_DISPLAY_POWER_MODE	0x03
#define G4D_DISPLAY_SHUTDOWN	0x00
#define G4D_DISPLAY_POWERUP		0x01
#define G4D_WAKEON_JOYSTICK		0x02
#define G4D_WAKEON_SERIAL		0x01
#define G4D_TURN_OFF_SD			0x80
#define G4D_GET_BUTTON			0x08
#define G4D_WAIT_ON_ANY_CLICK	0x0F
#define G4D_WAIT_ON_ANY			0x00
#define G4D_WAIT_ON_SW1			0x01
#define G4D_WAIT_ON_SW2			0x02
#define G4D_WAIT_ON_SW3			0x03
#define G4D_WAIT_ON_SW4			0x04
#define G4D_WAIT_ON_SW5			0x05
#define G4D_PEN_SOLID			0x00
#define G4D_PEN_WIRE			0x01
#define G4D_FONT_SMALL			0x00
#define G4D_FONT_MEDIUM			0x01
#define G4D_FONT_LARGE			0x02
#define G4D_OPAQUE				true
#define G4D_TRANSPARENT			false

// response codes

#define G4D_DEVICE_OLED			0x00
#define G4D_DEVICE_LCD			0x01
#define G4D_DEVICE_VGA			0x02

#define G4D_RES_220				0x22
#define G4D_RES_128				0x28
#define G4D_RES_320				0x32
#define G4D_RES_160				0x60
#define G4D_RES_64				0x64
#define G4D_RES_176				0x76
#define G4D_RES_93				0x96

#define NONE					0x00
#define UP						0x01
#define LEFT					0x02
#define DOWN					0x03
#define RIGHT					0x04
#define FIRE					0x05
#define G4D_BUTTON_TIMEOUT		0x00
#define G4D_BUTTON_UP			true
#define G4D_BUTTON_DOWN			false

union merged {
  uint32_t l;
  uint8_t i[4];
};

class G4D
{

  public:
    G4D(uint8_t pinReset, Stream * test);

	bool wait_ok();
	bool reset();
	bool getTimeoutFlag();
	void resetTimeoutFlag();
	void version(bool screenFlag);
	bool replaceBackground(uint8_t r, uint8_t g, uint8_t b);
	bool replaceBackground(uint16_t color);
	bool clear();
	bool control(uint8_t mode, uint8_t value);
	bool sleep(uint8_t mode);
	uint8_t joystick(uint8_t option);
	uint8_t waitJoystick(uint8_t option,uint16_t waitTime);
	bool sound(uint16_t note, uint16_t duration);
	bool tune(uint16_t note, uint16_t duration);
	bool addUserBitmap(uint8_t charIdx, uint8_t data[8]);
	bool drawUserBitmap(uint8_t charIdx, uint8_t x, uint8_t y, uint16_t color);
	bool drawUserBitmap(uint8_t charIdx, uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
	bool circle(uint8_t x, uint8_t y, uint8_t radius, uint16_t color);
	bool circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t r, uint8_t g, uint8_t b);
	bool triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3,  uint16_t color);
	bool triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t r, uint8_t g, uint8_t b);
	bool image(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t colorMode);
	bool setBackground(uint8_t r, uint8_t g, uint8_t b);
	bool setBackground(uint16_t color);
	bool line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
	bool line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t r, uint8_t g, uint8_t b);
	bool pixel(uint8_t x, uint8_t y, uint16_t color);
	bool pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
	uint16_t readPixel(uint8_t x, uint8_t y);
	bool screenCopyPaste(uint8_t xs, uint8_t ys, uint8_t xd, uint8_t yd, uint8_t width, uint8_t height);
	bool polygon(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
	bool replaceColor(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t oldColor, uint16_t newColor);
	bool replaceColor(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t oldr, uint8_t oldg, uint8_t oldb, uint8_t newr, uint8_t newg, uint8_t newb);
	bool pen(uint8_t size);
	bool rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
	bool rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t r, uint8_t g, uint8_t b);
	bool font(uint8_t fontSet);
	bool opacity(bool mode);
	bool stringG(uint8_t x, uint8_t y, uint8_t font, uint16_t color,  uint8_t width,  uint8_t height, char str[] );
	bool stringG(uint8_t x, uint8_t y, uint8_t font, uint8_t r, uint8_t g, uint8_t b,  uint8_t width,  uint8_t height, char str[] );
	bool asciiChar(char character, uint8_t column, uint8_t row, uint16_t color);
	bool asciiChar(char character, uint8_t column, uint8_t row, uint8_t r, uint8_t g, uint8_t b);
	bool button(bool state, uint8_t x, uint8_t y, uint16_t buttonColor, uint8_t font, uint16_t stringColor, uint8_t width,  uint8_t height, char str[] );
	bool button(bool state, uint8_t x, uint8_t y, uint8_t buttonr, uint8_t buttong, uint8_t buttonb, uint8_t font, uint8_t stringr, uint8_t stringg, uint8_t stringb, uint8_t width,  uint8_t height, char str[] );
	bool string(uint8_t column, uint8_t row, uint8_t font, uint16_t color,  char str[] );
	bool string(uint8_t column, uint8_t row, uint8_t font, uint8_t r, uint8_t g, uint8_t b,  char str[] );
	bool asciiCharG(char character, uint8_t x, uint8_t y, uint16_t color, uint8_t width,  uint8_t height );
	bool asciiCharG(char character, uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t width,  uint8_t height );
	bool usdImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t colorMode, uint32_t sectorAdd);
	bool setAddress(uint32_t address);
	bool screenCopyuSD(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint32_t sectorAdd);
	bool object(uint32_t address);
	bool runScript(uint32_t address);
	bool readSector(uint32_t address, char data[]);
	bool video(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t colorMode, uint8_t delay, uint16_t frames, uint32_t sectorAdd);
	bool writeSector(uint32_t address, char data[]);
	bool inituSD();
	uint8_t readByte();
	bool writeByte(uint8_t data);

	uint16_t getColor(uint8_t r, uint8_t g, uint8_t b);

	char device_type;
	char hardware_rev;
	char firmware_rev;
	char horizontal_res;
	char vertical_res;

  private:
	uint8_t _powerPin;
	uint8_t _resetPin;
	Stream * _serialObjectPtr;

	bool _serialReciveTimedOut;
	uint8_t _startUpTimer;
	char getResponse();
	char getResponse(unsigned long timeout);
		  
};

#endif