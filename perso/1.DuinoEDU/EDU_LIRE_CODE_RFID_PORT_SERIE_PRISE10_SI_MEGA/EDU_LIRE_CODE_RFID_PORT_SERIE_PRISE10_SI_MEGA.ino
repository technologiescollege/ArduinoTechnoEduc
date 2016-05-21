#include <RFID125.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <rgb_lcd.h>

//libraries at http://duinoedu.com/dl/lib/grove/EDU_RFID125_Grove/ 
RFID125 monRFID;
//libraries at http://duinoedu.com/dl/lib/grove/EDU_LCD_RGB_Backlight_Grove/ 
rgb_lcd monRgb;

void setup()
{
  monRFID.brancher(10,11);
  monRgb.branch();
}

void loop()
{
  monRFID.ecrireCode("12345678");
  monRgb.placerCurseurEn(0);
  monRgb.ecrire(monRFID.lireCode() );
}


