
#include "EduMp3.h"
#include <avr/pgmspace.h>


// << CONSTRUCTOR >>
/*EDU US*/ EduMp3::EduMp3(int pin1, int pin2){
		
		m_pin1=0;							// D2 par défaut (1er fil)
		m_pin2=m_pin1+1;				// D3 par défaut (2ème fil)
		
		mySerial = new SoftwareSerial(pin1, pin2);				// Création du port série virtuel à la construction
}

/*EDU FR*/	void EduMp3::brancher(){branch();}
/*EDU US*/ 	void EduMp3::branch(){

		//---- Réglage du port série virtuel
			mySerial->begin(9600);					// Vitesse fixée par Grove
			delay(100);	
			SetPlayMode(0x01);							// Mode de lecture par défaut
			delay(1000);
	
}

//------------ MODE DE LECTURE
/*EDU FR*/	boolean EduMp3::choisirModeDeLecture(uint8_t playMode){return SetPlayMode(playMode);}
/*ARD US*/	boolean EduMp3::SetPlayMode(uint8_t playmode){
	if (((playmode==0x00)|(playmode==0x01)|(playmode==0x02)|(playmode==0x03))==false)  {
		Serial.println("Erreur : Mode de lecture");
		return false;
	}

	//--- Envoi des paramètres
		mySerial->write(0x7E);						// Appel du module
		mySerial->write(0x03);
		mySerial->write(0xA9);
		
		mySerial->write(playmode);
		mySerial->write(0x7E);
		delay(10);
 
		while(mySerial->available()){
			if (0xA9==mySerial->read())		// Lecture effective de la réponse du module MP3
				return true;
			else
				return false;
		}
}

//----------- CHOIX MORCEAU
/*EDU FR*/ 	boolean EduMp3::lireMorceau(uint8_t track){return SetMusicPlay(0x00, track);}
/*EDU FR*/ 	boolean EduMp3::lire(uint8_t track){return SetMusicPlay(0x00, track);}
/*ARD US*/	boolean EduMp3::SetMusicPlay(uint8_t hbyte,uint8_t lbyte){
	mySerial->write(0x7E);
	mySerial->write(0x04);
	mySerial->write(0xA0);
	mySerial->write(hbyte);
	mySerial->write(lbyte);
	mySerial->write(0x7E);
    delay(10);
	 
	while(mySerial->available())	{
		if (0xA0==mySerial->read())
			return true;
		else 
			return false;
    }
}

//----------- VOLUME
/*EDU FR*/	boolean EduMp3::volume(int volume){return SetVolume(volume);}
/*EDU FR*/	boolean EduMp3::reglerVolume(int volume){return SetVolume(volume);}
/*ARD US*/	boolean EduMp3::SetVolume(int volume){
	// Plage de réglage 0x00 to 0x1F = 00011111=16+8+4+2+1 = 32 niveaux ! = 0 à 31
	//-- Adaptation des valeurs
	if(volume>1000){volume=1000;}
	volume = map(volume,0,1000,0,31);
	
	mySerial->write(0x7E);
	mySerial->write(0x03);
	mySerial->write(0xA7);
	mySerial->write(volume);
	mySerial->write(0x7E);
	delay(10);
	while(mySerial->available()) {
 if (0xA7==mySerial->read())
 return true;
 else 
 return false;
 }
}

//----------- PAUSE
/*EDU FR*/	boolean EduMp3::mettreEnPause(void){PauseOnOffCurrentMusic();}
/*EDU FR*/	boolean EduMp3::pause(void){PauseOnOffCurrentMusic();}
/*ARD US*/	boolean EduMp3::PauseOnOffCurrentMusic(void){
 mySerial->write(0x7E);
 mySerial->write(0x02);
 mySerial->write(0xA3);
 mySerial->write(0x7E);
 delay(10);
 while(mySerial->available())
 {
 if (0xA3==mySerial->read())
 return true;
 else 
 return false;
 }
}
