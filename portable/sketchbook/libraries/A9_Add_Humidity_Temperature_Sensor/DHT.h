#ifndef __DHT_H__
#define __DHT_H__
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

/* DHT library 
MIT license
written by Adafruit Industries
*/

// how many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85

// Version standard
#define DHT11 11
// Version PRO
#define DHT22 22
#define DHT_PRO 22
// Autres versions (?)
#define DHT21 21
#define AM2301 21


#define lireHumidite    readHumidity
#define lireTemperature readTemperature
#define humidite        readHumidity
#define temperature     readTemperature

class DHT 
{

public:

    DHT(uint8_t pin, uint8_t type=DHT11, uint8_t count=6);
	/*EDU FR*/	void	brancher(void);
	/*EDU US*/	void	branch(void);	
    void begin(void);
    float readTemperature(bool S=false);
    float convertCtoF(float);
    float readHumidity(void);


private:
    uint8_t data[6];
    uint8_t _pin, _type, _count;
    boolean read(void);
    unsigned long _lastreadtime;
    boolean firstreading;


};

#endif
