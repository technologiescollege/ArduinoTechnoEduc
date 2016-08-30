/*
 
  Original Author: David Souder
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Duinoedu_RCSwitchAdd.h"


#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>
Duinoedu_RCSwitchAdd::Duinoedu_RCSwitchAdd() 
: RCSwitch()
{
	//---- On initialise les messages série
		serialInfos=true;
		serialDebug=EDU_RCS_DEFAULT_DEBUG;
	
	//---- On initialise les attributs internes
		code = 0;
		codeLastTime = 0;
		option = EDU_RCS_DEFAULT_OPTION;
		
}

//========== SETUP

bool Duinoedu_RCSwitchAdd::branchReceiver(uint8_t pin){
	
	//---- On vérifie les broches
		if(pin!=2 && pin!=3){
			Serial.println("Pin2/3 only !");
			return false;
		}
	//---- On associe les broches aux interruptions
		uint8_t interrupt;
		switch(pin){
			case 2 : interrupt=0;break;
			case 3 : interrupt=1;break;
		}
	//---- On initialise effectivement le récepteur
		this->enableReceive(interrupt);
		SerialInf("Receiver:ok");
		SerialInf("Pin:",(String)pin);
		SerialInf("Int:",(String)interrupt);
		return true;
	
}

//========== METHODES OPERATIVES


	
long Duinoedu_RCSwitchAdd::receiveCode(int16_t option){
	//---- Si le code est disponible, on le transmet tout de suite
		if (this->available()) {
			code = this->getReceivedValue();
			if(serialInfos){
				String ch = "Code "+(String)code+" / "+(String)this->getReceivedBitlength()+" bit "+"Ptl:"+(String)this->getReceivedProtocol();
				Serial.println(ch);
			}
			//--- On enregistre l'heure du dernier code valide
				codeLastTime=millis();
				SerialDeb("codeLastTime:",codeLastTime);
			
			//--- On efface le buffer
				this->resetAvailable();	
			
			//--- Le code est immédiatement transmis car disponible
				return code;
		}
	
	//---- Mais que faire si aucun code n'est disponible
		else{
			
			//--- MODE SANS EFFET MEMOIRE : Retourner 0
				if(option==0){
					code = 0;
					return 0;
				}
			
			//--- MODE AVEC EFFET MEMOIRE MOMENTANE : Retourner dernière valeur pendant un certain temps
				else if(option>0){
					if(millis()<codeLastTime+option){
						return code;
					}else{
						code = 0;
						return 0;
					}
				}
			
			//--- MODE AVEC EFFET MEMOIRE PERMANANT : Retourner la dernière valeur 
				else if(option==-1){
					return code;
				}		
				
			
			
		}
}


boolean Duinoedu_RCSwitchAdd::testCode(uint32_t argCode, int16_t option){
	
	//---- On essaye de récupérer un code
		this->receiveCode(option);
		
	
	SerialDeb("code:",code);
	
	if(argCode==code){
		SerialDeb("vrai");
		return true;
	}else{
		SerialDeb("faux");
		return false;
	}
}



//========== MODIFICATION D'OPTIONS





//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


void Duinoedu_RCSwitchAdd::SerialInf(String txt1, String txt2, String txt3){
	if(serialInfos){
		if(txt2==""){
			Serial.println(txt1);
		}else{
			if(txt3==""){
				Serial.print(txt1); 	Serial.print(' ');
				Serial.println(txt2);
			}else{
				Serial.print(txt1);		Serial.print(' ');
				Serial.print(txt2);		Serial.print(' ');
				Serial.println(txt3);
			}
		}
	}
}

void Duinoedu_RCSwitchAdd::SerialDeb(String txt1, long val){
	SerialDeb(txt1, (String)val);
}
void Duinoedu_RCSwitchAdd::SerialDeb(String txt1, String txt2, String txt3){
	if(serialDebug){
		if(txt2==""){
			Serial.println(txt1);
		}else{
			Serial.print(txt1);
			Serial.println(txt2);
		}
	}
}



