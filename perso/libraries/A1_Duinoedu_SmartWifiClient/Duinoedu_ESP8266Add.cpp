/*
 
  Original Author: David Souder - souder.d@gmail.com
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Duinoedu_ESP8266Add.h"


#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>

Duinoedu_ESP8266Add::Duinoedu_ESP8266Add(SoftwareSerial &uart, uint32_t baud)
: ESP8266(uart, baud)
{
	buffer = (uint8_t*)malloc(100*sizeof(uint8_t));
	serialInfos=true;
	softSerial = &uart;	
		
	mode=-1;
}
		

					
			




//========== SETUP
// Activation du Serial
extern  HardwareSerial *myHS = &Serial;

void 	Duinoedu_ESP8266Add::setSerialInfos(bool _serialInfos ){
	if(_serialInfos==true){
		
		myHS->begin(9600); 
	}
	
	
}

void 	Duinoedu_ESP8266Add::setClientServer(){
// Active le mode Client+Seveur 
	//---- Choix mode
		/*
		setOprToStation       => MODE 1	= mode Station
		setOprToSoftAP        => MODE 2 = mode Point d'accès
		setOprToStationSoftAP => MODE 3 = mode Station + Point accès
		*/
		Serial.print("SET MODE STATION & ACCES POINT:");
		if (this->setOprToStationSoftAP()) {
			Serial.print(" ok\r\n");
		} else {
			Serial.print(" error or already done\r\n");
		}
		mode=3;

}

void 	Duinoedu_ESP8266Add::login					(String ssid, String pass){
		
	//---- On regarde si le mode Client+Serveur est actif
		if(mode!=3){
			setClientServer();
		}
	
	//---- Tentative de connexion
		Serial.print("CONNECTION WITH SSID & PASS:");
		if (this->joinAP(ssid, pass)) {
			Serial.print(" ok\r\n");
			//Serial.print("IP: ");       
			//Serial.println(this->getLocalIP().c_str());
		} else {
			Serial.print(" error\r\n");
		}
}


//========== METHODES OPERATIVES COLLEGE
void 	Duinoedu_ESP8266Add::choseIotServer(String _server, uint32_t _port){
	
	if(_server=="thingspeak.com" || _server=="www.thingspeak.com" || _server=="184.106.153.149"){
		Serial.print("SERVER SELECTED: ");
		Serial.print("thingspeak.com ");
		iotServerIp="184.106.153.149";
		Serial.println('('+iotServerIp+')');
		//--- On enregistre la bonne IP
			iotServerIp = "184.106.153.149";
	}else{
		iotServerIp=_server;
	}
	
	
	
}

void 	Duinoedu_ESP8266Add::choseIotServerAdafruit(String _Name, String _aioKey){
	
	
		Serial.print("SERVER SELECTED: ");
		Serial.print("io.adafruit.com ");
		//--- On enregistre la bonne IP
		iotServerIp="52.70.203.194";
		Serial.println('('+iotServerIp+')');
		Name=_Name;
		AioKey=_aioKey;

}



void	Duinoedu_ESP8266Add::choseApiKey(String _apiKey){
	apiKey=_apiKey;
}

//---------- Envois
void 	Duinoedu_ESP8266Add::sendValue(uint32_t _value, String _apiKey, uint16_t _minIntervalSeconds ){

	
	//==== Est-ce trop top pour envoyer une valeur ?
	static uint32_t lastTime = 0;
	if(millis() < lastTime + _minIntervalSeconds*1000 && lastTime!=0){
		return;
	}
	lastTime=millis();

	
	//==== Envoi effectife des valeurs
	//---- On configure un TCP
		configTcp(iotServerIp);
	//---- On met à jour l'apiKey si transmis en argument
		if(_apiKey!=0){
			apiKey=_apiKey;
		}
	
	//---- On soumet 1 requête 
		//--- Fabrication
		String req 	= 		"GET /";
		req 	   	+= 		"update?";
		req			+=		"key=";
		req			+=		apiKey;
		req			+=		"&";
		req			+=  	"field";
		req			+=  	"1=";
		req		   	+= 		_value ;
		req			+= 		"\r\n";
		
		//--- Envoi
		sendRequest(req);
}

void	Duinoedu_ESP8266Add::sendTweet(String _tweet, String _apiKeyTweet, uint16_t _minIntervalSeconds ){
	
	
	//==== Est-ce trop top pour envoyer une valeur ?
	static uint32_t lastTime = 0;
	if(millis() < lastTime + _minIntervalSeconds*1000 && lastTime!=0){
		return;
	}
	lastTime=millis();
	
	
	//==== Incrément des Tweets (2 Tweets identiques ne peuvent pas être envoyés)
	static uint8_t tweetCounter = 0;
	String stringAdd;
	(tweetCounter==0)?stringAdd="":stringAdd="_("+(String)tweetCounter+')';
	tweetCounter++;
	Serial.print("tweetCount:");
	Serial.println(tweetCounter);
	Serial.print("stringAdd:");
	Serial.println(stringAdd);
	
	//---- On configure un TCP
		configTcp(iotServerIp);
		
	//---- On soumet 1 requête  (GET /apps/thingtweet/1/statuses/update?api_key=...&status=texte)
		//--- Fabrication
		String req 	= 		"GET /";
		req 		+=		"apps/thingtweet/1/statuses/";
		req 	   	+= 		"update?";
		req			+=		"api_key=";
		req			+=		_apiKeyTweet;
		req			+=		"&";
		req 		+= 		"status=";
		req		   	+= 		_tweet ;
		req		   	+= 		stringAdd ;
		req			+= 		"\r\n";
		Serial.print(req);
		//--- Envoi
		sendRequest(req);
		

}


void 	Duinoedu_ESP8266Add::sendValueAdafruit(uint32_t _value, String _Feed, uint16_t _minIntervalSeconds ){

	
	//==== Est-ce trop top pour envoyer une valeur ?
	static uint32_t lastTime = 0;
	if(millis() < lastTime + _minIntervalSeconds*1000 && lastTime!=0){
		return;
	}
	lastTime=millis();

	
	//==== Envoi effectife des valeurs
	//---- On configure un TCP
		configTcp(iotServerIp);

	
	//---- On soumet 1 requête 
		//--- Fabrication
		String req 	= 		"GET /";
		req 	   	+= 		"api/groups/";
		req			+=		Name;
		req			+=		"/send.json?x-aio-key=";
		req			+=		AioKey;
		req			+=		"&";
		req			+=  	_Feed;
		req			+=  	"=";
		req		   	+= 		_value ;
		req			+= 		"\r\n";
		
		//--- Envoi
		Serial.print(req);
		sendRequest(req);
}



void	Duinoedu_ESP8266Add::sendAt(String _at, uint16_t _timeout){
	m_puart->println(_at);
	unsigned long start = millis();
	// Méthode timeout bloquant sans sortie par break
    while (millis() - start < _timeout) {
		while(m_puart->available()){
			Serial.write(m_puart->read());
		}
	}
}


//========== MODIFICATION D'OPTIONS



//========== FONCTIONS PLUS BAS NIVEAU

void 	Duinoedu_ESP8266Add::configTcp(String addr, uint32_t port){
		
		Serial.print("SET TCP: "+addr+' '+port+':');
		if (createTCP(addr, port)) {
			Serial.print(" ok\r\n");
		} else {
			Serial.print(" error\r\n");
		}

    //char *hello = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\nConnection: close\r\n\r\n";
    //wifi.send((const uint8_t*)hello, strlen(hello));

}

void 	Duinoedu_ESP8266Add::sendRequest(String _request){
	//Serial.print("SENDING REQUEST: ");
	//String request = "GET /update?key=EGN3KW0V38BVA1L1&field1=66\r\n";
	
	//char *hello = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\nConnection: close\r\n\r\n";
    char *request = &_request[0];
	
	
    this->send((const uint8_t*)request, strlen(request));
	
	// Réponse 
    uint32_t len = this->recv(buffer, sizeof(buffer), 10000);
    if (len > 0) {
        Serial.print("Received:[");
        for(uint32_t i = 0; i < len; i++) {
            Serial.print((char)buffer[i]);
        }
		
        Serial.print("]\r\n");
    }
	else {
			Serial.print(" no received\n");
		}

}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx





