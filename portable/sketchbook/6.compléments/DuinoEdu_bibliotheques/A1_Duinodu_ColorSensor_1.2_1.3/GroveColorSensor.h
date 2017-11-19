/****************************************************************************/	
//	Hardware: Grove - I2C Color Sensor
//  Arduino IDE: Arduino-1.6
//  
//  Refactored version of the library by FrankieChu - www.seeedstudio.com
//	
/******************************************************************************/

#ifndef GROVECOLORSENSOR
#define GROVECOLORSENSOR

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include <Registers.h>

class GroveColorSensor
{
public:

	// Color Sensor LED Status
	int ledStatus;
	// Default constructor
	GroveColorSensor();
	// Constructor with parameters
	GroveColorSensor(
		  const int& triggerMode
		, const int& interruptSource
		, const int& interruptMode
		, const int& gainAndPrescaler
		, const int& sensorAddress);

	void readRGB();
	void readRGB(int *red, int *green, int *blue);
	void calculateCoordinate();
	void clearInterrupt();
	
				
	
private:	 
	 
	// Set trigger mode. Including free mode, manually mode, single synchronization mode or so.
	void setTimingReg();
	// Set interrupt source
	void setInterruptSourceReg();
	// Set interrupt mode
	void setInterruptControlReg();
	// Set gain value and pre-scaler value
	void setGain();
	// Start ADC of the colour sensor
	void setEnableADC();
	
	// Used for storing the colour data
	int readingdata_[8];
	int green_;
	int red_;
	int blue_;
	int clear_;
	
	int triggerMode_;	
	int interruptSource_;
	int interruptMode_;
	int gainAndPrescaler_;
	int sensorAddress_;
	
};

#endif

/* Exemple EDU : base

#include <Wire.h>
#include <GroveColorSensor.h>

void setup(){
    Serial.begin(9600);
    Wire.begin();
}
 
void loop(){
  int red, green, blue;
          
  GroveColorSensor *colorSensor = new GroveColorSensor();
    colorSensor->ledStatus = 1;            
    colorSensor->readRGB(&red, &green, &blue); 
    colorSensor->clearInterrupt();
  delete colorSensor;

  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.println(blue);
}

*/

/* 
#include <Wire.h>
#include <GroveColorSensor.h>

int Duinoedu_red, Duinoedu_green, Duinoedu_blue;

void setup(){
    Serial.begin(9600);
    Wire.begin();
}
 
void loop(){

 readAllColors();

 Serial.print(Duinoedu_red);    
 Serial.print(", ");
 Serial.print(Duinoedu_green);
 Serial.print(", ");
 Serial.print(Duinoedu_blue); 
 Serial.print("\r\n");    
  
}


// BONNE FONCTION
int readAllColors(){
  GroveColorSensor *colorSensor = new GroveColorSensor();
    colorSensor->ledStatus = 1;            
    colorSensor->readRGB(&Duinoedu_red, &Duinoedu_green, &Duinoedu_blue); 
    colorSensor->clearInterrupt();
}

*/


