/*
 * SharpIR
 * Library for the acquisition of distance data from Sharp IR sensors
 *
 * Created by Giuseppe Masino, 28 may 2016
 * Author URL http://www.facebook.com/dev.hackerinside
 * Author email: dev.giuseppemasino@outlook.it
 *
 * This library and the relative example files are released under the
 * CreativeCommons ShareAlike-Attribution 4.0 International license 
 * and a custom MIT license.
 *
 * The license terms can be read at the following url: https://www.facebook.com/notes/giuseppe-masino/faqs-about-my-works/197854657355058
 *
 */

#pragma once

#include <Arduino.h>

const bool GP2YA41SK0F = 0;
const bool GP2Y0A21YK0F = 1;
const uint8_t GP2Y0A02YK0F = 2;

class SharpIR
{
  public:
    
    SharpIR(uint8_t _sensorType, uint8_t _sensorPin);
    
    uint8_t getDistance();
		uint8_t getDistance(bool avoidBurstRead);
  
  protected:
    
    uint8_t sensorType, pin;	
  
  private:
    
    uint8_t distance;
    uint32_t lastTime, sampleTime;
};
