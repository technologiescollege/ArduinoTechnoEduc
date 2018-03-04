/*
 
  Original Author: David Souder
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Duinoedu_RF24Add.h"


#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>

/*EDU FR*/	Duinoedu_RF24Add::Duinoedu_RF24Add(uint8_t _cepin, uint8_t _cspin) : RF24(_cepin, _cspin){
	
							
			
}



//========== SETUP

//========== METHODES OPERATIVES

//========== MODIFICATION D'OPTIONS





//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/* EXEMPLE EMETTEUR
// EMETTEUR
#include <SPI.h>
//#include <Duinoedu_RF24Add.h>
#include <RF24.h>


RF24 radio(9,10);

const uint64_t pipeA = 0xF0F0F0F0E1LL;
const uint64_t pipeB = 0xF0F0F0F0D2LL;


void setup(){
  Serial.begin(9600);
  radio.begin();

  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setPALevel(RF24_PA_MIN); // MIJ LOW HIGH

  radio.openWritingPipe(pipeB);
  radio.openReadingPipe(1,pipeA);

  radio.startListening();
}


void loop(){

  unsigned long tempR;
  unsigned long tempE = millis();
  
  // ENVOI
  radio.stopListening();
  radio.write(&tempE,sizeof(unsigned long));
  delay(1000);
  Serial.println(tempE);  
}
*/


/*EXEMPLE RECEPTEUR
// RECEPTEUR
#include <SPI.h>
//#include <Duinoedu_RF24Add.h>
#include <RF24.h>

RF24 radio(9,10);

const uint64_t pipeA = 0xF0F0F0F0E1LL;
const uint64_t pipeB = 0xF0F0F0F0D2LL;

void setup(){
  Serial.begin(9600);
  radio.begin();

  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setPALevel(RF24_PA_MIN); // HIGH

  radio.openWritingPipe(pipeA);
  radio.openReadingPipe(1,pipeB);

  radio.startListening();

  Serial.println("go");
}


void loop(){

  unsigned long tempR;
 
  // RECEPTION
  if(radio.available()){
      radio.read(&tempR, sizeof(unsigned long));
      Serial.println(tempR);
  }  
}

*/