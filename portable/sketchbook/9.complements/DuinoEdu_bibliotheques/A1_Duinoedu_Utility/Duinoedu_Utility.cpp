/*
 
  Original Author: David Souder
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <Duinoedu_Utility.h>

//#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>
Duinoedu_Utility::Duinoedu_Utility(){
	
}

void irGenerator(uint8_t _pin, uint8_t on, unsigned int frequency){
	if(on==1){
		tone(_pin, frequency);
	}else{
		noTone(_pin);
	}
}


String SendStringSerial(String _key, String _Value, char _separator, char _end){
	_Value+= _key+ _separator+_Value ;
	if(_end!=-1){_Value+=_end;}
	return _Value;
}

// STRING : récupération d'une valeur à partir d'une clé
int findIntValue(String _bigString, String _key, char _separator, char _end) {
	return findStringValue( _bigString, _key, _separator, _end).toInt();
}
double findDoubleValue(String _bigString, String _key, char _separator, char _end){
	return findStringValue( _bigString, _key, _separator, _end).toFloat();
}

String	findStringValue(String _bigString, String _key, char _separator, char _end){
// Permet de renvoyer la valeur (Val) associée à la variable (Var)	
// La chaine dans laquelle on doit trouver la variable et la valeur est "parameters" 
// Ex de contenu de la chaine parametres: #LED02=ON&LED1=OFF&
// Ex = _separator, & _end [#LED02=ON&LED1=OFF]
		
		int16_t posVar 			= _bigString.indexOf(_key);
		uint8_t parametersLen 	= _bigString.length();
		uint8_t posVal=0;
		uint8_t posValEnd;
		String  returnVal;
		
		//SERIAL_PRINTLN(parameters);
		//SERIAL_PRINT(F("parLen:"));SERIAL_PRINTLN(parametersLen);
		//SERIAL_PRINT(F("posVar:"));SERIAL_PRINTLN(posVar);
		
	
	//==== On recherche la présence de la variable 									// ex: [LED1]
		if(posVar==-1){	
			//SERIAL_PRINTLN(F("NEAN"));
			return "";
		}
		
		else{
			//--- On recherche les délimiteurs de la valeur
				//-- On reherche le début de Val
				//-- On se déplace de posVar jusqu'au séparateur (=,:)
					for(posVal=posVar; posVal<parametersLen; posVal++){
						if( _bigString[posVal] == _separator ){
							posVal++;
							break;
						}
						//SERIAL_PRINTLN(F("posVal:"));
						//SERIAL_PRINTLN(posVal);
					}
					//SERIAL_PRINTLN(F("posVal:verif:"));
					//SERIAL_PRINTLN(posVal);
				
	
				
				//-- On recherche la fin de Val
				
					//-- On se déplace de posVal jusqu'au "&"
					for(posValEnd=posVal; posValEnd<parametersLen; posValEnd++){
						//--- Si on a choisi un terminateur : [var1=1023 var2=512]
						if(_end!=0){
							if( _bigString[posValEnd] == _end ){
								break;
							}
						}
						//--- Si on a pas choisi de terminateur [var1=1023var2=512] 
						//https://www.arduino.cc/en/Tutorial/ASCIITable
						//  +       ASCii 43
						//  ,		ASCII 44
						//	- 		ASCII 45
						//  .       ASCII 46
						//  /		ASCII 47
						//	0..9	ASCII 48..57

						else{
							if( _bigString[posValEnd] < 44 ||  _bigString[posValEnd] > 57){
								break;
							}							
						}
						
						//SERIAL_PRINTLN(F("posValEnd:"));
						//SERIAL_PRINTLN(posValEnd);
					}
				
			
			//--- On récupère la valeur
				//SERIAL_PRINTLN(F("posValDeA:"));
				//SERIAL_PRINTLN(posVal);
				//SERIAL_PRINTLN(posValEnd);
				
				returnVal = _bigString.substring(posVal, posValEnd);

				
		}
	return returnVal;

}




Chrono::Chrono(){
}

void Chrono::start(){
	lastTime = millis();
}

uint32_t  Chrono::duration(){
	uint32_t d;
	d = millis()-lastTime;
	return d;
}
void Chrono::reset(){
	lastTime = millis();
}





//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx













