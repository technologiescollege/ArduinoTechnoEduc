/*
 
  Original Author: David Souder - souder.d@gmail.com
  www.duinoedu.com
  Date de dernière modification : 22/06/2016
  
*/

#ifndef Duinoedu_ESP8266Add_H
#define Duinoedu_ESP8266Add_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
//#include "Wire.h"
#endif


//#include <___.h>
//#include "EDU_debug.h"

//#define DEBUG

#include <ESP8266.h>

class Duinoedu_ESP8266Add : public ESP8266 {

	public:
		//===== << CONSTRUCTORS >>
			Duinoedu_ESP8266Add						(SoftwareSerial &uart, uint32_t baud=9600);
			uint8_t *buffer;
													// softwareSerial(Rx,Tx)
			SoftwareSerial 	*softSerial;
		
		//===== setup collège
			void 	setSerialInfos					(bool _serialInfos=true );
					bool serialInfos;
			
			void 	setClientServer					();
					int8_t mode;					// -1:non configuré,1,2,3	
			
			void 	login							(String ssid, String pass);
		
		//===== méthodes opératives collège
			void 	choseIotServer					(String _server, uint32_t _port=80);
			//void 	choseIotServerAdafruit			(String _Name, String _aioKey);
					String iotServerIp;
					String Name;
					String AioKey;
			
			void	choseApiKey						(String _apiKey);
					String apiKey;
					
			void 	sendValue						(uint32_t _value, 	String _apiKey, 		uint16_t _minIntervalSeconds=20);
			void	sendTweet						(String _tweet, 	String _apiKeyTweet, 	uint16_t _minIntervalSeconds=60);
			//void 	sendValueAdafruit				(uint32_t _value, 	String _Feed=0, 		uint16_t _minIntervalSeconds=1);
			
			

		//===== fonction plus bas niveau
			void 	configTcp						(String addr, uint32_t port=80);		// Ex: http://www.thingspeak.com:80
			void    sendRequest						(String _request);						// Requêtes web
			void	sendAt							(String _at, uint16_t _timeout=1000);				// Commandes AT
		
		

		


	protected:



};
#endif


/* THING_SPEAK
	Channel =>


#include "ESP8266.h"
#include <Duinoedu_ESP8266Add.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8,9);

Duinoedu_ESP8266Add smartWifi(mySerial);

void setup(void)
{
    Serial.begin(9600);
    pinMode(A0, INPUT);
    smartWifi.login("freebox_Dave", "0712760712");		  // Pass provisoir
    smartWifi.choseIotServer("thingspeak.com");
   
}

void loop(void){
    int      valeur = analogRead(A0);
    
    smartWifi.sendValue(valeur, "N637JQU8ESWSEV1G", 20);                // Valeur,  API_KEY, Interval mini
    smartWifi.sendTweet("Message", "2LUGYMNZVRS60DOG", 60);   // Message, API KEY, Interval mini


}





/* TESTS

	Envoi toutes les 10s => 1 point sur 2000ms
	Envoi toutes les 20s => tous les points
*/
