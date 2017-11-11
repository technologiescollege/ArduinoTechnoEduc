
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Duinoedu_SerialUartGrove.h"


#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)										
/*EDU FR*/	Duinoedu_SerialUartGrove::Duinoedu_SerialUartGrove(HardwareSerial &_uart){
#else																														
/*EDU FR*/	Duinoedu_SerialUartGrove::Duinoedu_SerialUartGrove(SoftwareSerial &_uart){										
#endif	
	esp = &_uart;
	
	code_recu 				= ' ' ;
    requete_recue			= false ;
    header_reponse_put 		= "" ;
	ID_connection 			= 0 ;
	
	esp->setTimeout(1000);

}

/*EDU US*/	void Duinoedu_SerialUartGrove::begin(uint32_t _speed){
	esp->begin(9600);
	delay(100);
}



//========== OPERATIVES
/*EDU US*/	uint32_t	Duinoedu_SerialUartGrove::findBaudrate(bool _verbose){
	
	uint8_t succes=0;
	
	static const uint32_t baudrate[] = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 74800, 115200, 0};
	
	//---- Vidage du buffer
	while(esp->available()){
		esp->read();
	}

	if(_verbose)Serial.print("MESURE VITESSE ACTUELLE");
	for(uint8_t i=0;i<10;i++){
		if(_verbose)Serial.print(".");
		esp->begin(baudrate[i]);
		//esp->println("AT");
		//esp->setTimeout(100);
		succes = sendAt("AT","",100);
		if(succes){
			effectiveBaudrate=baudrate[i];
			if(_verbose){
				Serial.print("\r\n ");
				Serial.println(effectiveBaudrate);
				Serial.println("");
			}
			return effectiveBaudrate;
		}
		
	}

}

/*EDU US*/	void		Duinoedu_SerialUartGrove::factoryReset(){
	
	up_factory:
	//---- On détermine la vitesse et la stocke dans : effectiveBaudrate
		if(!findBaudrate()){
			Serial.println("PB baudrate");
			return;
		}
	//---- On démarre le port série sur la vitesse déterminée
		esp->begin(effectiveBaudrate);										
	
	//---- On envoie la commande de RESET d'usine => le module passe à 115200 bauds
		sendAt("AT+RESTORE", "FORMATAGE USINE", 5000);
}

/*EDU US*/	void		Duinoedu_SerialUartGrove::changeSpeed(uint32_t _speed){
	// Mode manuel obligatoire
	String comment;
	String envoi;

	//---- On détermine la vitesse actuelle et on la stocke dans : effectiveBaudrate
		esp->begin(findBaudrate(true));
		if(effectiveBaudrate==_speed)return;

	//---- On change la vitesse : AT+UART_DEF=9600,8,1,0,0
		comment="FORCAGE VITESSE A "; comment+=_speed; 
		envoi ="AT+UART_DEF="; envoi+=String(_speed); envoi+=",8,1,0,0";
		
		Serial.println(comment);
		Serial.print(" ");Serial.println(envoi);
		
		esp->println(envoi);
		esp->find("OK");		// Difficiel de trouver OK
		delay(1000);
	
	//---- On lance une vérif manuelle (plus sûr)
		esp->begin(_speed);
		esp->println("AT");
		(esp->find("OK"))?Serial.println(" OK\r\n"):Serial.println(" PB\r\n");
		
	//---- On affiche la vérif
		findBaudrate();
}


/*EDU US*/	void		Duinoedu_SerialUartGrove::setAp(String _Ssid, String _password, String _Ip, String _Geteway, String _Mask){
	
	uint8_t succes=0;
	String envoi;
	
	//---- Configuration en mode AP
		succes += sendAt("AT+CWMODE_DEF=2", "ACTIVATION MODE POINT ACCES", 2000);
		
	//---- Config des IPs
		envoi ="AT+CIPAP_DEF=\""; envoi +=_Ip; envoi +="\",\""; envoi +=_Geteway; envoi +="\",\""; envoi +=_Mask; envoi +="\"";
		succes += sendAt(envoi, "CONFIG. IP", 2000);
	
	//---- Config. SSID
		(_password=="/")? _password="" : _password=_password;
		envoi = "AT+CWSAP_DEF=\""+_Ssid+"\",\""+_password+"\",1,0";
		succes += sendAt(envoi, "CONFIG. SSID", 2000);

	//---- Message de succès
		Serial.println(F("[MODE POINT ACCES]"));
		(succes==3)?Serial.println(F(" OK")):Serial.println(F(" PB"));
		Serial.print(F(" IP:"));Serial.println(_Ip);
		Serial.print(F(" SSID:"));Serial.println(_Ssid);
		Serial.print(F(" PASS:"));Serial.println(_password);
		Serial.println(F("\r\n"));
}

/*EDU US*/	void		Duinoedu_SerialUartGrove::setStation(String _Ssid, String _password, String _Ip, String _Geteway="192.168.1.1", String _Mask="255.255.255.0"){

	uint8_t succes=0;
	String envoi;
	
	//---- Configuration en mode STATION
		succes += sendAt("AT+CWMODE_DEF=1", "ACTIVATION MODE STATION", 2000);
		
	//---- Config des IPs
		envoi ="AT+CIPSTA_DEF=\""; envoi+=_Ip; envoi+="\",\""; envoi+=_Geteway; envoi+="\",\""; envoi+=_Mask; envoi+="\"";
        succes +=  sendAt(envoi, "CONFIG. IP", 2000);
		
	//---- Connexion à une box
		envoi = "AT+CWJAP_DEF=\""+_Ssid+"\",\""+_password+"\"";
		succes +=  sendAt(envoi, "CONNEXION A LA BOX", 20000);
		
	//---- Auto connexion
		succes +=  sendAt("AT+CWAUTOCONN=1","AUTO CONNEXION",2000);

	//---- Message de succès
		Serial.println(F("[MODE STATION]"));
		(succes==4)?Serial.println(F(" OK")):Serial.println(F(" PB"));
		Serial.print(F(" IP:"));Serial.println(_Ip);
		Serial.println(F("\r\n"));
     	
}


/*EDU US*/	void Duinoedu_SerialUartGrove::startServer(uint32_t _port=80){
  
  requete_recue = false ;
  code_recu = 'A';
 
  header_reponse_put 	 = 		"HTTP/1.1 200 OK\r\n" 			;
  header_reponse_put	+=		"Content-type: text/html\r\n"	;
  header_reponse_put	+=		"Content-length: 0\r\n"			;
  header_reponse_put	+=		"Connection: Close\r\n\r\n"		;
  delay( 4000 );
  
  esp->println("AT+RST");
  delay( 1000 );
  
  esp->println("AT+CIPMUX=1");
  delay( 1000 );
  
  esp->println("AT+CIPSERVER=1,80");
  delay( 1000 );
  
  esp->flush();
  delay( 20 );
}

/*EDU US*/	uint16_t Duinoedu_SerialUartGrove::available(){
	return esp->available();
}

/*EDU US*/  char Duinoedu_SerialUartGrove::readPostChar(){

	requete_recue		=	esp->find("+IPD,");
	delay( 100 );
	
	if (requete_recue){
		ID_connection = ( esp->read() - 48 ) ;   // Id 0,1..
		String donnees_recues	=	esp->readString();
		code_recu = donnees_recues.charAt(donnees_recues.length()-1);  // Dernier caractère (donnée)
	}
	
	reponseHeader();


return code_recu;

}

/*EDU US*/	void Duinoedu_SerialUartGrove::reponseHeader(){
  
if (requete_recue)
  {
    String envoi="AT+CIPSEND=";
    envoi	+=	ID_connection;
    envoi	+=	",";
    envoi	+=	header_reponse_put.length();
    esp->println(envoi);
    delay( 100 );
    esp->print(header_reponse_put);
    delay( 200 );
  }	
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVEES
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx



/*EDU US*/	bool Duinoedu_SerialUartGrove::sendAt(String _at, String _comment="", uint16_t _timeout){
	
	bool comment = (_comment=="") ? comment=false : comment=true;
	bool succes=0;
	
	if(comment)Serial.println(_comment);
	if(comment)Serial.print(" ");
	if(comment)Serial.println(_at);

	esp->setTimeout(_timeout);
	esp->println(_at);
	bool ok = esp->find("OK");
	if(ok){
		if(comment)Serial.println(F(" OK\r\n"));
		succes = true;
	}else{
		if(comment)Serial.println(F(" PB\r\n"));
		succes = false;
	}
	
	while(esp->available()) esp->read();
	return succes;
}











