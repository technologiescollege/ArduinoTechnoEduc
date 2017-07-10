/*
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 17/05/2017
  
*/

#ifndef Duinoedu_HMC5883LAdd_H
#define Duinoedu_HMC5883LAdd_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif


#include <HMC5883L.h>


class Duinoedu_HMC5883LAdd : public HMC5883L {

	public:
	
	/*EDU FR*/	Duinoedu_HMC5883LAdd();


	protected:



};
#endif



// Exemple de départ SeeedStudio
/*
#include <Wire.h>
#include <HMC5883L.h>
//				If X-axis points to the North, it is 0 degree.
//				If X-axis points to the East, it is 90 degrees.
//				If X-axis points to the South, it is 180 degrees.
//				If X-axis points to the West, it is 270 degrees.


HMC5883L compass;
int error = 0;


void setup()
{
  Serial.begin(9600);
  Wire.begin(); 
  
  Serial.println("Setting scale to +/- 1.3 Ga");
  error = compass.setScale(1.3);  if(error != 0) Serial.println(compass.getErrorText(error));
  
  Serial.println("Setting measurement mode to continous.");
  error = compass.setMeasurementMode(MEASUREMENT_CONTINUOUS);  if(error != 0) Serial.println(compass.getErrorText(error));
}


void loop()
{
  MagnetometerRaw     raw     = compass.readRawAxis();
  MagnetometerScaled  scaled  = compass.readScaledAxis();
  
  // Values are accessed like so:
  int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

  float heading = atan2(scaled.YAxis, scaled.XAxis);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -2??37' which is -2.617 Degrees, or (which we need) -0.0456752665 radians, I will use -0.0457
  float declinationAngle = -0.0457;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  float headingDegrees = heading * 180/M_PI; 

  Output(raw, scaled, heading, headingDegrees);

  // Temps de mesure
  delay(66);//of course it can be delayed longer.
}

// Output the data down the serial port.
void Output(MagnetometerRaw raw, MagnetometerScaled scaled, float heading, float headingDegrees)
{
   Serial.print("Raw:\t");
   Serial.print(raw.XAxis);
   Serial.print("   ");   
   Serial.print(raw.YAxis);
   Serial.print("   ");   
   Serial.print(raw.ZAxis);
   Serial.print("   \tScaled:\t");
   
   Serial.print(scaled.XAxis);
   Serial.print("   ");   
   Serial.print(scaled.YAxis);
   Serial.print("   ");   
   Serial.print(scaled.ZAxis);

   Serial.print("   \tHeading:\t");
   Serial.print(heading);
   Serial.print(" Radians   \t");
   Serial.print(headingDegrees);
   Serial.println(" Degrees   \t");
}
*/