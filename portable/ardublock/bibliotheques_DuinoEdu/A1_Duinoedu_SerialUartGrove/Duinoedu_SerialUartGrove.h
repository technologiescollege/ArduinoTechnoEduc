/*
	Programme initial : Julien Launay : julien.launay@ac-lyon.fr
	Encapsulation : David Souder : souder.d@gmail.com
	
*/


#ifndef Duinoedu_SerialUartGrove_H
#define Duinoedu_SerialUartGrove_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif

#include <SoftwareSerial.h>




class Duinoedu_SerialUartGrove {

	public:

	#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)										
	/*EDU FR*/	Duinoedu_SerialUartGrove(HardwareSerial &_uart);
	#else																														
	/*EDU FR*/	Duinoedu_SerialUartGrove(SoftwareSerial &_uart);							    		
	#endif	
	
	
	
	
	
	//========== SETUP
	/*EDU US*/	void begin(uint32_t _speed);
	
	

	
	
	//========== OPERATIVES
	/*EDU US*/	uint32_t	findBaudrate(bool _verbose=true);
							uint32_t effectiveBaudrate = 0;
		
	/*EDU US*/	void		factoryReset();															// Formatage complet => 115200
	/*EDU US*/	void		changeSpeed(uint32_t _speed);	
	
	
	/*EDU US*/	void		setAp(String _Ip, String _Geteway, String _Mask, String _Ssid, String _password);
	/*EDU US*/	void		setStation(String _Ip, String _Geteway, String _Mask, String _Ssid, String _password);
	
	
	/*EDU US*/	void 		startServer(uint32_t _port=80);
	
	  
	
	
	/*EDU US*/	uint16_t	available();														
	/*EDU US*/	char 		readPostChar();															// Lecture et réponse d'une requête POST
	/*EDU US*/	
	
	
	/*EDU US*/	bool 		itsOk();
	
	/*EDU US*/	bool        sendAt(String _at, String _comment="", uint16_t _timeout=1000);
	
	
	
	//========== PRIVEES
	/*EDU US*/	void 		reponseHeader();
	
	

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)		// MEGA ? 
	HardwareSerial *esp;						    				// =>hard					
#else																// OTHER (__AVR_ATmega328P__)															
	SoftwareSerial 	*esp;							    			// =>soft
#endif


	
	
	
	
	
	
	
	//HardwareSerial  *espHard;

	char 		code_recu 				= ' ' ;
    bool 		requete_recue			= false ;
    String 		header_reponse_put 		= "" ;
	int 		ID_connection 			= 0 ;
	
	
	protected:



};
#endif


/* EXEMPLE - ALLUMER 1 LED : MODULE DEJA CONFIGURE
#include <Duinoedu_SerialUartGrove.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerialWifi(2,3);
Duinoedu_SerialUartGrove myWifi(mySerialWifi);

void setup(){
  Serial.begin(9600);
  myWifi.begin(9600);
  pinMode( 8 , OUTPUT);
  myWifi.initialisation();
}

void loop(){  
  if ( myWifi.available() ) {
    Serial.println("av");
     myWifi.lireCodeEtRepondre();
        if ( myWifi.code_recu == '3' ){
          digitalWrite(8 , HIGH);
        }
        if (myWifi.code_recu == '7' ){
          digitalWrite(8 , LOW);
        }
    }
}

*/


/* EXEMPLE2 - ALLUMER 1 LED : MODULE NON CONFIGURE
#include <Duinoedu_SerialUartGrove.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerialWifi(2,3);
Duinoedu_SerialUartGrove myWifi(mySerialWifi);

void setup(){
  //---- Arduino
  
  Serial.begin(9600);
  pinMode( 8 , OUTPUT);

  //---- ESP : préalable
  myWifi.begin(9600);
  myWifi.factoryReset();
  myWifi.changeSpeed(9600);

  //---- ESP : config point d'accès
  myWifi.setAp("Wifi_techno2","","192.168.1.40","192.168.1.1","255.255.255.0");

  //---- ESP : activation du serveur
  myWifi.startServer(); 
}

void loop(){  

    
}
*/

/* EXEMPLE 3 AP/STATION

#include <Duinoedu_SerialUartGrove.h>
#include <SoftwareSerial.h>

// SI MEGA
#if defined(__AVR_ATmega2560__)
Duinoedu_SerialUartGrove myWifi(Serial1);

// SI UNO NANO
#else
SoftwareSerial mySerialWifi(2,3);
Duinoedu_SerialUartGrove myWifi(mySerialWifi);
#endif 

char monCaractere='X';

void setup(){
  //---- Arduino
  
  Serial.begin(9600);
  pinMode( 8 , OUTPUT);

  //---- ESP : préalable : bloc 01
  myWifi.begin(9600);
  myWifi.factoryReset();
  myWifi.changeSpeed(19200);
  //myWifi.findBaudrate();
  
  //---- ESP : config point d'accès : bloc 02/03
  //myWifi.setAp("Wifi_techno2","","192.168.0.40","192.168.0.1","255.255.255.0");
  myWifi.setStation("freebox_Dave","0712760712","192.168.0.40","192.168.0.254","255.255.255.0");
  myWifi.startServer(80); 

}

void loop(){  
  if ( myWifi.available() ) monCaractere = myWifi.readPostChar();
  if ( monCaractere == '3') digitalWrite(8 , HIGH);
  if ( monCaractere == '7') digitalWrite(8 , LOW);  
}

*/

