#include "G4D.h"

G4D::G4D(uint8_t pinReset, Stream * serialObjectPtr){
  _resetPin = pinReset;
  _serialObjectPtr = serialObjectPtr;

  if(_resetPin>-1)
  {
	  digitalWrite(_resetPin, HIGH);
	  pinMode(_resetPin, OUTPUT);
  }
}

bool G4D::wait_ok()
{
  	uint8_t count = 0;
  	char retVal = G4D_NAK;
	while (retVal != G4D_ACK){
		retVal = getResponse();
		count++;
		if(count > G4D_STARTUP_MAX_RETRIES) return(false);
	}
    return(true); 
}

bool G4D::reset()
{
  uint8_t count = 0;
  char retVal = G4D_NAK;
  if(_resetPin>-1) digitalWrite(_resetPin, LOW);
  delay(1000);
  if(_resetPin>-1) digitalWrite(_resetPin, HIGH);
  // if we don't get an ACK, spin until we do.  Fail after max retries
  while (retVal != G4D_ACK){
	delay(G4D_STARTUP_DELAY);
	_serialObjectPtr->flush();
	_serialObjectPtr->print(char(G4D_CMD_AUTOBAUD));
	retVal = getResponse();
	count++;
	if(count > G4D_STARTUP_MAX_RETRIES) return(false);
  }
  return(true);    
}

bool G4D::getTimeoutFlag()
{
	return(_serialReciveTimedOut);
}

void G4D::resetTimeoutFlag()
{
	_serialReciveTimedOut = false;
}

void G4D::version(bool screenFlag)
{
	_serialObjectPtr->print((char)G4D_CMD_VERSION);
	_serialObjectPtr->print((char)screenFlag);
	device_type = getResponse();// device type
	hardware_rev = getResponse();// hardware rev
	firmware_rev = getResponse();// firmware rev
	horizontal_res = getResponse();// horiz res
	vertical_res = getResponse();// vert res
}

bool G4D::replaceBackground(uint8_t r, uint8_t g, uint8_t b)
{
	bool retVal;
	uint16_t color;
	color = getColor(r,g,b);
	retVal = replaceBackground(color);
  	wait_ok(); 
}

bool G4D::replaceBackground(uint16_t color)
{
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_REPLACE_BACKGROUND);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
	wait_ok(); 
}

bool G4D::clear()
{
	char retVal;
    _serialObjectPtr->print((char)G4D_CMD_CLEAR);
  	wait_ok(); 
}

bool  G4D::control(uint8_t mode, uint8_t value)
{
	char retVal;

    _serialObjectPtr->print((char)G4D_CMD_CONTROL);
    _serialObjectPtr->print((char)mode);
    _serialObjectPtr->print((char)value);
  	wait_ok(); 
}

bool  G4D::sleep(uint8_t mode)
{
	char retVal;

    _serialObjectPtr->print((char)G4D_CMD_SLEEP);
    _serialObjectPtr->print((char)mode);
    _serialObjectPtr->print((char)0x00);//unused 2 byte delay
    _serialObjectPtr->print((char)0x00);//unused 2 byte delay
  	wait_ok(); 
}

uint8_t  G4D::joystick(uint8_t option)
{
	char retVal;

    _serialObjectPtr->print((char)G4D_CMD_JOYSTICK);
    _serialObjectPtr->print((char)option);
  	retVal = getResponse(); 
  	return retVal;
}

uint8_t  G4D::waitJoystick(uint8_t option,uint16_t waitTime)
{
	char retVal;
	merged waitBytes;
	waitBytes.l = waitTime;
	 
    _serialObjectPtr->print((char)G4D_CMD_WAITJOYSTICK);
    _serialObjectPtr->print((char)option);
	_serialObjectPtr->print((char)waitBytes.i[1]);
	_serialObjectPtr->print((char)waitBytes.i[0]);
  	retVal = getResponse(); 
  	return retVal;
}

bool  G4D::sound(uint16_t note, uint16_t duration)
{
	char retVal;
	merged soundBytes;
	soundBytes.l = note;
	 
    _serialObjectPtr->print((char)G4D_CMD_SOUND);
	_serialObjectPtr->print((char)soundBytes.i[1]);
	_serialObjectPtr->print((char)soundBytes.i[0]);
	soundBytes.l = duration;
	_serialObjectPtr->print((char)soundBytes.i[1]);
	_serialObjectPtr->print((char)soundBytes.i[0]);
  	wait_ok(); 
}

// ##################TODO########################
// change parameter list to take an array of notes and durations
bool  G4D::tune(uint16_t note, uint16_t duration)
{
	char retVal;
	merged soundBytes;
	soundBytes.l = note;
	 
    _serialObjectPtr->print((char)G4D_CMD_TUNE);
    _serialObjectPtr->print((char)1);//number of note pairs (max 64)
	_serialObjectPtr->print((char)soundBytes.i[1]);
	_serialObjectPtr->print((char)soundBytes.i[0]);
	soundBytes.l = duration;
	_serialObjectPtr->print((char)soundBytes.i[1]);
	_serialObjectPtr->print((char)soundBytes.i[0]);
  	wait_ok(); 
}

// convert RGB color to a single 16 bit value
uint16_t G4D::getColor(uint8_t r, uint8_t g, uint8_t b)
{
	// shift off the least sig bits
	uint8_t redFull = r >> 3;
	uint8_t greenFull = g >> 2;
	uint8_t blueFull = b >> 3;
	//load each byte in at the lowest and shift result up
	uint16_t colorFull = redFull;
	colorFull = colorFull << 6;
	colorFull = colorFull | greenFull;
	colorFull = colorFull << 5;
	colorFull = colorFull | blueFull;
	return (colorFull);
}
	
bool G4D::usdImage (uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t colorMode, uint32_t sectorAdd)
{
	char retVal;
	merged address;
	address.l=sectorAdd;

	//char msgString[10];
	//msgString[0] = (char)G4D_CMD_EXTENDED;
	//msgString[1] = (char)G4D_CMD_SDIMAGE;
	//msgString[2] = (char)x;
	//msgString[3] = (char)y;
	//msgString[4] = (char)width;
	//msgString[5] = (char)height;
	//msgString[6] = (char)colorMode;
	//msgString[7] = (char)address.i[2];
	//msgString[8] = (char)address.i[1];
	//msgString[9] = (char)address.i[0];


    _serialObjectPtr->print((char)G4D_CMD_EXTENDED);
    _serialObjectPtr->print((char)G4D_CMD_SDIMAGE);
    _serialObjectPtr->print((char)x);//start
    _serialObjectPtr->print((char)y);
    _serialObjectPtr->print((char)width);//size
    _serialObjectPtr->print((char)height);

    _serialObjectPtr->print((char)colorMode);//mode
    
	_serialObjectPtr->print((char)address.i[2]); //adddy
	_serialObjectPtr->print((char)address.i[1]);
	_serialObjectPtr->print((char)address.i[0]);
  	wait_ok(); 
}

bool G4D::addUserBitmap(uint8_t charIdx, uint8_t data[8])
{
	char retVal;
    _serialObjectPtr->print((char)G4D_CMD_ADD_USER_BITMAP);
    _serialObjectPtr->print((char)charIdx);

	int i;
	for(i=0;i<=7; i++)
	{
		_serialObjectPtr->print((char)data[i]);
	} 
  	wait_ok(); 
}

bool G4D::drawUserBitmap(uint8_t charIdx, uint8_t x, uint8_t y, uint16_t color)
{
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_DRAW_USER_BITMAP);
    _serialObjectPtr->print((char)charIdx);
    _serialObjectPtr->print((char)x);
    _serialObjectPtr->print((char)y);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
  	wait_ok(); 
}

bool G4D::drawUserBitmap(uint8_t charIdx, uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{
	bool retVal;
	uint16_t color;
	color = getColor(r,g,b);
	retVal = drawUserBitmap(charIdx, x, y, color);
  	wait_ok(); 
}

bool G4D::circle(uint8_t x, uint8_t y, uint8_t radius, uint16_t color)
{
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_CIRCLE);
    _serialObjectPtr->print((char)x);
    _serialObjectPtr->print((char)y);
    _serialObjectPtr->print((char)radius);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
  	wait_ok(); 
}

bool G4D::circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t r, uint8_t g, uint8_t b)
{
	bool retVal;
	uint16_t color;
	color = getColor(r,g,b);
	retVal = circle(x, y, radius, color);
  	wait_ok(); 
}

bool G4D::triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3,  uint16_t color)
{
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_TRIANGLE);
    _serialObjectPtr->print((char)x1);
    _serialObjectPtr->print((char)y1);
    _serialObjectPtr->print((char)x2);
    _serialObjectPtr->print((char)y2);
    _serialObjectPtr->print((char)x3);
    _serialObjectPtr->print((char)y3);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
  	wait_ok(); 
}

bool G4D::triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t r, uint8_t g, uint8_t b)
{
	bool retVal;
	uint16_t color;
	color = getColor(r,g,b);
	retVal = triangle(x1, y1, x2, y2, x3, y3, color);
  	wait_ok(); 
}

// #############################TODO######################################
// stub function
bool G4D::image (uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t colorMode)
{
	return(false);

	char retVal;
    _serialObjectPtr->print((char)G4D_CMD_IMAGE);
    _serialObjectPtr->print((char)x);//start
    _serialObjectPtr->print((char)y);
    _serialObjectPtr->print((char)width);//size
    _serialObjectPtr->print((char)height);
    _serialObjectPtr->print((char)colorMode);//mode
  	wait_ok(); 
}

bool G4D::setBackground(uint8_t r, uint8_t g, uint8_t b)
{
	bool retVal;
	uint16_t color;
	color = getColor(r,g,b);
	retVal = setBackground(color);
  	wait_ok(); 
}

bool G4D::setBackground(uint16_t color)
{
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_SET_BACKGROUND);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
  	wait_ok(); 
}

bool G4D::line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_LINE);
    _serialObjectPtr->print((char)x1);
    _serialObjectPtr->print((char)y1);
    _serialObjectPtr->print((char)x2);
    _serialObjectPtr->print((char)y2);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
  	wait_ok(); 
}

bool G4D::line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t r, uint8_t g, uint8_t b)
{
	bool retVal;
	uint16_t color;
	color = getColor(r,g,b);
	retVal = line(x1, y1, x2, y2, color);
  	wait_ok(); 
}

bool G4D::pixel(uint8_t x, uint8_t y, uint16_t color)
{
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_PIXEL);
    _serialObjectPtr->print((char)x);
    _serialObjectPtr->print((char)y);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
  	wait_ok(); 
}

bool G4D::pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{
	bool retVal;
	uint16_t color;
	color = getColor(r,g,b);
	retVal = pixel(x, y, color);
  	wait_ok(); 
}

uint16_t G4D::readPixel(uint8_t x, uint8_t y)
{
	char retVal;
	merged colorBytes;
    _serialObjectPtr->print((char)G4D_CMD_READ_PIXEL);
    _serialObjectPtr->print((char)x);
    _serialObjectPtr->print((char)y);

	colorBytes.i[1] = getResponse(); 
	colorBytes.i[0] = getResponse();

	//TODO check for timeout to getResponse
	return(colorBytes.l);

}

bool G4D::screenCopyPaste(uint8_t xs, uint8_t ys, uint8_t xd, uint8_t yd, uint8_t width, uint8_t height)
{
	char retVal;
    _serialObjectPtr->print((char)G4D_CMD_SCREEN_COPY_PASTE);
    _serialObjectPtr->print((char)xs);
    _serialObjectPtr->print((char)ys);
    _serialObjectPtr->print((char)xd);
    _serialObjectPtr->print((char)yd);
    _serialObjectPtr->print((char)width);
    _serialObjectPtr->print((char)height);
  	wait_ok(); 
}

// #############################TODO######################################
// stub function
bool G4D::polygon(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
	return(false);
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_POLYGON);
    _serialObjectPtr->print((char)x1);
    _serialObjectPtr->print((char)y1);
    _serialObjectPtr->print((char)x2);
    _serialObjectPtr->print((char)y2);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
  	wait_ok(); 
}

bool G4D::replaceColor(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t oldColor, uint16_t newColor)
{
	char retVal;
	merged colorBytes;
	colorBytes.l = oldColor;
    _serialObjectPtr->print((char)G4D_CMD_REPLACE_COLOR);
    _serialObjectPtr->print((char)x1);
    _serialObjectPtr->print((char)y1);
    _serialObjectPtr->print((char)x2);
    _serialObjectPtr->print((char)y2);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
	colorBytes.l = newColor;
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
  	wait_ok(); 
}

bool G4D::replaceColor(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t oldr, uint8_t oldg, uint8_t oldb, uint8_t newr, uint8_t newg, uint8_t newb)
{
	bool retVal;
	uint16_t oldColor,newColor;
	oldColor = getColor(oldr,oldg,oldb);
	newColor = getColor(newr,newg,newb);
	retVal = replaceColor(x1, y1, x2, y2, oldColor, newColor);
  	wait_ok(); 
}

bool  G4D::pen(uint8_t size)
{
	char retVal;
    _serialObjectPtr->print((char)G4D_CMD_PEN);
    _serialObjectPtr->print((char)size);
  	wait_ok(); 
}

bool G4D::rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_RECTANGLE);
    _serialObjectPtr->print((char)x1);
    _serialObjectPtr->print((char)y1);
    _serialObjectPtr->print((char)x2);
    _serialObjectPtr->print((char)y2);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
  	wait_ok(); 
}

bool G4D::rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t r, uint8_t g, uint8_t b)
{
	bool retVal;
	uint16_t color;
	color = getColor(r,g,b);
	retVal = rectangle(x1, y1, x2, y2, color);
  	wait_ok(); 
}

bool  G4D::font(uint8_t fontSet)
{
	char retVal;

    _serialObjectPtr->print((char)G4D_CMD_FONT);
    _serialObjectPtr->print((char)fontSet);
  	wait_ok(); 
}

bool  G4D::opacity(bool mode)
{
	char retVal;
    _serialObjectPtr->print((char)G4D_CMD_OPACITY);
    _serialObjectPtr->print((char)mode);
  	wait_ok(); 
}

bool G4D::stringG(uint8_t x, uint8_t y, uint8_t font, uint16_t color,  uint8_t width,  uint8_t height, char str[] )
{
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_STRING);
    _serialObjectPtr->print((char)x);
    _serialObjectPtr->print((char)y);
    _serialObjectPtr->print((char)font);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
    _serialObjectPtr->print((char)height);
    _serialObjectPtr->print((char)width);
    _serialObjectPtr->print(str);
    _serialObjectPtr->print((char)0x00); // terminate the string
  	wait_ok(); 
}

bool G4D::stringG(uint8_t x, uint8_t y, uint8_t font, uint8_t r,  uint8_t g,  uint8_t b,  uint8_t width,  uint8_t height, char str[] )
{
	bool retVal;
	uint16_t color;
	color = getColor(r,g,b);
	retVal = stringG(x,y,font,color,width,height,str);
  	wait_ok(); 
}

bool G4D::asciiChar(char character, uint8_t column, uint8_t row, uint16_t color )
{
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_ASCII_CHAR);
    _serialObjectPtr->print((char)character);
    _serialObjectPtr->print((char)column);
    _serialObjectPtr->print((char)row);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
  	wait_ok(); 
}

bool G4D::asciiChar(char character, uint8_t column, uint8_t row, uint8_t r, uint8_t g, uint8_t b )
{
	bool retVal;
	uint16_t color;
	color = getColor(r,g,b);
	retVal = asciiChar(character, column, row, color);
  	wait_ok(); 
}

bool G4D::button(bool state, uint8_t x, uint8_t y, uint16_t buttonColor, uint8_t font, uint16_t stringColor, uint8_t width,  uint8_t height, char str[] )
{
	char retVal;
	merged colorBytes;
	colorBytes.l = buttonColor;
    _serialObjectPtr->print((char)G4D_CMD_BUTTON);
    _serialObjectPtr->print((char)state);
    _serialObjectPtr->print((char)x);
    _serialObjectPtr->print((char)y);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
    _serialObjectPtr->print((char)font);
	colorBytes.l = stringColor;
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
	_serialObjectPtr->print((char)height);
	_serialObjectPtr->print((char)width);
    _serialObjectPtr->print(str);
    _serialObjectPtr->print((char)0x00); // terminate the string
  	wait_ok(); 
}

bool G4D::button(bool state, uint8_t x, uint8_t y, uint8_t buttonr, uint8_t buttong, uint8_t buttonb, uint8_t font, uint8_t stringr, uint8_t stringg, uint8_t stringb, uint8_t width,  uint8_t height, char str[] )
{
	bool retVal;
	uint16_t buttonColor,stringColor;
	buttonColor = getColor(buttonr,buttong,buttonb);
	stringColor = getColor(stringr,stringg,stringb);
	retVal = button(state,x,y,buttonColor,font,stringColor,width,height,str);
  	wait_ok(); 
}

bool G4D::string(uint8_t column, uint8_t row, uint8_t font, uint16_t color, char str[] )
{
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_STRING_CHAR_COORD);
    _serialObjectPtr->print((char)column);
    _serialObjectPtr->print((char)row);
    _serialObjectPtr->print((char)font);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
    _serialObjectPtr->print(str);
    _serialObjectPtr->print((char)0x00); // terminate the string
  	wait_ok(); 
}

bool G4D::string(uint8_t column, uint8_t row, uint8_t font, uint8_t r,  uint8_t g,  uint8_t b, char str[] )
{
	bool retVal;
	uint16_t color;
	color = getColor(r,g,b);
	retVal = string(column,row,font,color,str);
  	wait_ok(); 
}

bool G4D::asciiCharG(char character, uint8_t x, uint8_t y, uint16_t color, uint8_t width,  uint8_t height )
{
	char retVal;
	merged colorBytes;
	colorBytes.l = color;
    _serialObjectPtr->print((char)G4D_CMD_ASCII_CHAR_GRAPHICS);
    _serialObjectPtr->print((char)character);
    _serialObjectPtr->print((char)x);
    _serialObjectPtr->print((char)y);
	_serialObjectPtr->print((char)colorBytes.i[1]);
	_serialObjectPtr->print((char)colorBytes.i[0]);
    _serialObjectPtr->print((char)width);
    _serialObjectPtr->print((char)height);
  	wait_ok(); 
}

bool G4D::asciiCharG(char character, uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t width,  uint8_t height )
{
	bool retVal;
	uint16_t color;
	color = getColor(r,g,b);
	retVal = asciiCharG(character, x, y, color, width, height);
  	wait_ok(); 
}

bool  G4D::setAddress(uint32_t address)
{
	char retVal;
	merged addressFlip;
	addressFlip.l=address;
    _serialObjectPtr->print((char)G4D_CMD_EXTENDED);
    _serialObjectPtr->print((char)G4D_CMD_SET_ADDRESS);
	_serialObjectPtr->print((char)addressFlip.i[3]); 
	_serialObjectPtr->print((char)addressFlip.i[2]); 
	_serialObjectPtr->print((char)addressFlip.i[1]);
	_serialObjectPtr->print((char)addressFlip.i[0]);
  	wait_ok(); 
}

bool G4D::screenCopyuSD (uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint32_t sectorAdd)
{
	char retVal;
	merged address;
	address.l=sectorAdd;
    _serialObjectPtr->print((char)G4D_CMD_EXTENDED);
    _serialObjectPtr->print((char)G4D_CMD_SCREEN_COPY);
    _serialObjectPtr->print((char)x);
    _serialObjectPtr->print((char)y);
    _serialObjectPtr->print((char)width);
    _serialObjectPtr->print((char)height);
	_serialObjectPtr->print((char)address.i[2]); //adddy
	_serialObjectPtr->print((char)address.i[1]);
	_serialObjectPtr->print((char)address.i[0]);
  	wait_ok(); 
}

bool  G4D::object(uint32_t address)
{
	char retVal;
	merged addressFlip;
	addressFlip.l=address;
    _serialObjectPtr->print((char)G4D_CMD_EXTENDED);
    _serialObjectPtr->print((char)G4D_CMD_OBJECT);
	_serialObjectPtr->print((char)addressFlip.i[3]); 
	_serialObjectPtr->print((char)addressFlip.i[2]); 
	_serialObjectPtr->print((char)addressFlip.i[1]);
	_serialObjectPtr->print((char)addressFlip.i[0]);
  	wait_ok(); 
}

// NOTE: this command will always return NAK because the RunScript command does not return ACK
// on failure you will get a NAK right away (if unsuccessful or card not present)
// on success you will get a NAK after timeout
bool  G4D::runScript(uint32_t address)
{
	char retVal;
	merged addressFlip;
	addressFlip.l=address;
    _serialObjectPtr->print((char)G4D_CMD_EXTENDED);
    _serialObjectPtr->print((char)G4D_CMD_RUN_SCRIPT);
	_serialObjectPtr->print((char)addressFlip.i[3]); 
	_serialObjectPtr->print((char)addressFlip.i[2]); 
	_serialObjectPtr->print((char)addressFlip.i[1]);
	_serialObjectPtr->print((char)addressFlip.i[0]);
  	wait_ok(); 
}

bool  G4D::readSector(uint32_t address, char data[] )
{
	merged addressFlip;
	addressFlip.l=address;
    _serialObjectPtr->print((char)G4D_CMD_EXTENDED);
    _serialObjectPtr->print((char)G4D_CMD_READ_SECTOR);
	_serialObjectPtr->print((char)addressFlip.i[2]); 
	_serialObjectPtr->print((char)addressFlip.i[1]);
	_serialObjectPtr->print((char)addressFlip.i[0]);
	int i;
	for(i=0;i<512; i++)
	{
		data[i] = getResponse(6); //use a very short timeout so if something goes wrong on recive we don't get locked in forever
	} 
	return(true);
}

bool G4D::video(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t colorMode, uint8_t delay, uint16_t frames, uint32_t sectorAdd)
{
	char retVal;
	merged util;
	util.l=frames;
    _serialObjectPtr->print((char)G4D_CMD_EXTENDED);
    _serialObjectPtr->print((char)G4D_CMD_VIDEO);
    _serialObjectPtr->print((char)x);
    _serialObjectPtr->print((char)y);
    _serialObjectPtr->print((char)width);
    _serialObjectPtr->print((char)height);
    _serialObjectPtr->print((char)colorMode);
    _serialObjectPtr->print((char)delay);
    _serialObjectPtr->print((char)colorMode);
	_serialObjectPtr->print((char)util.i[1]);//frames
	_serialObjectPtr->print((char)util.i[0]);
	util.l=sectorAdd;
	_serialObjectPtr->print((char)util.i[2]);//sectorAdd
	_serialObjectPtr->print((char)util.i[1]);
	_serialObjectPtr->print((char)util.i[0]);
  	wait_ok(); 
}

bool  G4D::writeSector(uint32_t address, char data[] )
{
	char retVal;
	merged addressFlip;
	addressFlip.l=address;
    _serialObjectPtr->print((char)G4D_CMD_EXTENDED);
    _serialObjectPtr->print((char)G4D_CMD_WRITE_SECTOR);
	_serialObjectPtr->print((char)addressFlip.i[2]); 
	_serialObjectPtr->print((char)addressFlip.i[1]);
	_serialObjectPtr->print((char)addressFlip.i[0]);
	int i;
	for(i=0;i<512; i++)
	{
		_serialObjectPtr->print((char)data[i]);
	} 
  	wait_ok(); 
}

bool G4D::inituSD()
{
	char retVal;
    _serialObjectPtr->print((char)G4D_CMD_EXTENDED);
    _serialObjectPtr->print((char)G4D_CMD_INIT_USD);
  	wait_ok(); 
}

uint8_t G4D::readByte()
{
	uint8_t retVal;
    _serialObjectPtr->print((char)G4D_CMD_EXTENDED);
    _serialObjectPtr->print((char)G4D_CMD_READ_BYTE);
  	wait_ok(); 
}

bool G4D::writeByte(uint8_t data)
{
	char retVal;
    _serialObjectPtr->print((char)G4D_CMD_EXTENDED);
    _serialObjectPtr->print((char)G4D_CMD_WRITE_BYTE);
    _serialObjectPtr->print((char)data);
  	wait_ok(); 
}

char G4D::getResponse()
{
	return getResponse(G4D_SERIAL_RESPONSE_TIMEOUT);
}

char G4D::getResponse(unsigned long waitTime)
{
	unsigned long startTime;
	startTime = millis();
	byte response;
	while (!_serialObjectPtr->available()) { 
		if (  millis() - startTime >= waitTime ){
			_serialReciveTimedOut = true;

			return(G4D_NAK);
		}
    }
	response = _serialObjectPtr->read();
	return response;
}
