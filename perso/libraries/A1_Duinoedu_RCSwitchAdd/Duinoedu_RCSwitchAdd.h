/*
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 20/03/2016
  
*/

#ifndef Duinoedu_RCSwitchAdd_H
#define Duinoedu_RCSwitchAdd_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
//#include "Wire.h"
#endif


//#include <___.h>
//#include "EDU_debug.h"
#include "utility/RCSwitch.h"

// BEBUG ?
#define EDU_RCS_DEFAULT_DEBUG false

#define EDU_RCS_DEFAULT_OPTION 200

/* INTERRUPTIONS ARDUINO 
	UNO/MEGA 	int0 => pin2    PRISE2 UNO/MEGA 
	UNO/MEGA	int1 => pin3	PRISE3 UNO
	MEGA		int2 => pin21
	MEGA		int3 => pin20
	MEGA		int4 => pin19
	MEGA 		int5 => pin18

*/
// Retrocompatibilité
#define SERIALPLUS_MEMORISER 				-1
#define IRREMOTE_MEMORISER_TOUCHE			-1
#define IRREMOTE_MEMORISER      			-1
#define DUINOEDU_MEMORISER_TOUCHE			-1
#define DUINOEDU_MEMORISER					-1

#define SERIALPLUS_MEMORISER_200_MS 		200
#define IRREMOTE_MEMORISER_200_MS 		    200
#define DUINOEDU_MEMORISER_200_MS 		    200

#define DUINOEDU_MEMORISER_500_MS 		500
#define DUINOEDU_MEMORISER_1000_MS 		1000

#define SERIALPLUS_MEMORISER_2000_MS 		2000
#define IRREMOTE_MEMORISER_2000_MS 		    2000
#define DUINOEDU_MEMORISER_2000_MS 			2000

#define DUINOEDU_MEMORISER_5000_MS 			5000
#define DUINOEDU_MEMORISER_10000_MS 		10000

#define SERIALPLUS_NE_PAS_MEMORISER 		0
#define IRREMOTE_NE_PAS_MEMORISER 		    0
#define DUINOEDU_NE_PAS_MEMORISER 			0

	
class Duinoedu_RCSwitchAdd : public RCSwitch {

	public:
	
		//========== << CONSTRUCTOR >>
			Duinoedu_RCSwitchAdd();
			
		//========== SETUP
			bool branchReceiver			(uint8_t pin);
				
		//========== METHODES OPERATIVES	
			long receiveCode			(int16_t option=200);
				uint32_t code; 										// Dernier code reçu (ou 0)				
				uint32_t codeLastTime;								// Date du dernier code reçu
				uint16_t option;									// Durée de validité du code
			
			boolean testCode			(uint32_t argCode, int16_t option=200);
				

		//========== MODIFICATION D'OPTIONS
			void SerialInf(String txt1, String txt2="", String txt3="" );
				bool serialInfos;
			
			void SerialDeb(String txt1, long val);
			void SerialDeb(String txt1, String txt2="", String txt3="" );
				bool serialDebug;		
				


	
		
	protected:
		



};
#endif










/* RECUPERATION DES CODES
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
  Serial.println("Recepteur active sur intrruption0 : broche 2");
}

void loop() {
  
  if (mySwitch.available()) {
    
    long  value = mySwitch.getReceivedValue();
    
    if (value != 0) {

         
          Serial.print("Received ");
          Serial.print( mySwitch.getReceivedValue() );
          Serial.print(" / ");
          Serial.print( mySwitch.getReceivedBitlength() );
          Serial.print("bit ");
          Serial.print("Protocol: ");
          Serial.println( mySwitch.getReceivedProtocol() );
          
          mySwitch.resetAvailable();
       
 
    }
  }
}//# Fin loop()
*/ 