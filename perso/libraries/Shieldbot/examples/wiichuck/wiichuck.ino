/* You need a nintendo wiichuck and you probably want one of these 
 * from Seeed to make connecting it easy!
 * http://www.seeedstudio.com/depot/wiichuck-a-wiinunchuck-break-out-board-p-586.html?cPath=175_177
 *
 * And finally you need the wiichuck library from 
 * https://github.com/coopermaa/Wiichuck
 * (click the little zip icon to download)
 *
 * IMPORTANT NOTE: you MUST turn your A2 and A3 (S3 and S4) switches OFF to use
 * as the wiichuck adapter uses those for power and ground.
 */

#include <Shieldbot.h>

#include <Wiichuck.h> 
#include <Wire.h> //Must be included for wiichuck

Shieldbot shieldbot = Shieldbot();
Wiichuck wii = Wiichuck();

void setup(){
  Serial.begin(9600);
  shieldbot.setMaxSpeed(255);//255 is max
  wii.init();
  wii.calibrate();
}

void loop(){
  wii.poll();
  int joy_x_axis = wii.joyX();
  int joy_y_axis = wii.joyY();

  Serial.print("nunx = ");
  Serial.println(joy_x_axis);
  Serial.print("nuny= ");
  Serial.println(joy_y_axis);
  
  int X1 = map(joy_x_axis, 26, 228, 127, -128);
  int Y = map(joy_y_axis, 24, 228, -128, 127);
  int rVal = constrain(X1+Y, -128, 127);
  
  int X2 = map(joy_x_axis, 26, 228, -128, 127);
  int lVal = constrain(X2+Y, -128, 127);
  
  shieldbot.leftMotor(lVal);
  shieldbot.rightMotor(rVal);
  
  Serial.print("LMval = ");
  Serial.println(lVal);
  Serial.print("RMval= ");
  Serial.println(rVal);

}