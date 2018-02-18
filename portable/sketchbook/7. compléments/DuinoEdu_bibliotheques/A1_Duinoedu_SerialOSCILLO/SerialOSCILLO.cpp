
#include "SerialOSCILLO.h"

#include <avr/pgmspace.h>


//////////////////// PUBLIC

// << CONSTRUCTOR >>
/*EDU US*/ SerialOSCILLO::SerialOSCILLO(){
}

/*EDU FR*/	void SerialOSCILLO::brancher(int32_t speed){branch(speed);}//*/
/*EDU US*/	void SerialOSCILLO::begin(int32_t speed){branch(speed);}//*/
/*EDU US*/	void SerialOSCILLO::branch(int32_t speed){
		
		Serial.begin(speed);
		Serial.println("OSCILLO OK");
												
}//*/


/*EDU FR*/	void		SerialOSCILLO::nombreCadrans(int8_t nbrCadrans){
	
	//---- On enregistre le nombre de cadrans
		m_nbrCadrans = nbrCadrans;
	
	//---- On met en place un protocole avec un motif 
		String motif = "CDR-"+(String)nbrCadrans;
	
	//---- On écrit sur le port série (on envoit 4 fois pour être sûr);
		Serial.println(motif);
		Serial.println(motif);
		Serial.println(motif);
		Serial.println(motif);


}//*/



/*EDU US*/	void SerialOSCILLO::analogWrite(int16_t pin, int8_t cadran, char graduation){

	//---- On lit les valeurs
		uint16_t analogVal1023 = analogRead(pin);
		char  analogVal255  = analogVal1023/4;
		char  pinChar = 54;
		char  nbrXnCadrans = 8*m_nbrCadrans+cadran;

	
	//---- On met en place un protocole avec un motif 
		//String can = (String)analogVal1023;
		//can = "~"+(String)pin+"&"+m_nbrCadrans+cadran+graduation+":"+can+"/";
		//delay(10);
	
	//---- On écrit sur le port série 
	// On est passé de 17 caractères à 4
		Serial.print(graduation);
		Serial.print(pinChar);
		Serial.print(nbrXnCadrans);
	    Serial.print(analogVal255);
	    delay(5);                        // Ancienne valeur 7
		
	
}




			
	
		



SerialOSCILLO	SerialOscillo;







////////////////////:: Reste







