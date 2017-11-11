/*
  Author: 
  Méthodes /EDU/ : David Souder - staff@duinoedu.com - www.duinoedu.com
  Méthodes /ESP/ : ai-thinker 
  Integration Ardublock : Karl Thomas
  
  Date de dernière modification : 11/07/2016
*/

#ifndef Duinoedu_SmartWifiServer_H
#define Duinoedu_SmartWifiServer_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
//#include "Wire.h"
#endif


//#define DEBUG

#include <SoftwareSerial.h>
#include <AltSoftSerial.h>					//https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html#speed D8/D9

#define SERIALDEBUG 0

/********************************** TO DO 
 » Gérer les caractères spéciaux qui génèrent des erreurs
 » Passer à altSoftSerial (simultanéité em/rec)
 » Ajouter un switch sur UART UNO
 » delay(2) dans toutes les fonctions d'attente en mode polling
 */

class Duinoedu_SmartWifiServer {

	public:
		//===== << CONSTRUCTORS >>
			/*EDU US 00*/	//Duinoedu_SmartWifiServer						(SoftwareSerial &uart);
			/*EDU US 00*/	//Duinoedu_SmartWifiServer						(AltSoftSerial &uart);
			/*EDU US 00*/	Duinoedu_SmartWifiServer						(HardwareSerial &uart);
							//uint8_t *buffer;	
													
							//SoftwareSerial 	*espSerial;																		// softwareSerial(Rx,Tx)
							//AltSoftSerial		*espSerial;
							HardwareSerial		*espSerial;
			
		//========== SETUP
			
			/*EDU US 01*/	void 		begin								(uint16_t _speed);
						 
			/*EDU US 02*/	void 		reset								();													// AT+RST
			
			/*EDU US 03*/	void 		activeAcceesPoint					();													// AT+CWMODE=2
			
			/*EDU US 04*/	String 		getLocalIp							();													// AT+CIFSR
										String 		ip;
			
			/*EDU US 05*/	void 		enableMultiClients					();													// AT+CIPMUX=1
			
			/*EDU US 06*/	void 		disableMultiClients					();													// AT+CIPMUX=0
				
			/*EDU US 07*/	void 		startTcpServer						(uint32_t _port);									// AT+CIPSERVER=1,80

			/*EDU US 08*/	void 		setTcpServerTimeOutInSeconds		(uint8_t _timeOut);									// AT+CIPSTO=timeOut
			
			
			
		//========== LOOP
			
			/*EDU US 12*/	int8_t 		requestReceivedAndParametersSaved	();													// +IPD,{id client},{taille des données}:{données envoyées par le client}
										uint8_t 	id;																			// n° du client: -1;0,1,2,3
										uint16_t 	dataLength;																	// Taille de l'envoi
										int8_t 		replyMe;																	// Vaut 1 si un clien à besoin d'une réponse
										String 		parametersSent;																// Paramètres reçus favicon compris
										String		parameters;

			
			
			//// A DEVELOPPER : METHODE AVEC STRING BUFFER						
			/*EDU US 20*/	uint32_t	requestReceived();					
										String buffer;
			
			/*EDU US 21*/	void		choseAnAnswer();
			
		
			
			
			
			
			/*EDU US 30*/	uint8_t 	sendData							(String &_data, uint8_t _id);
										uint16_t 	lengthRequest;																// Taille calculée de toute la requête
			

			/*EDU US 40*/	void    	manageAll();
										uint16_t globalLength;
										int8_t  sendingState;																	// 0:rien; 1:calcul; 2:envoi; 3:close
			
			/*EDU US 41*/	void  		addText								(String _text);
			
			/*EDU US 42*/	void  		addHttpHeaders						();
			
			/*EDU US 43*/	void 		addHead								(String _title);
			
			/*EDU US 44*/	void 		beginBody							();
			/*EDU US 45*/	void 		finishBody							();
			
			/*EDU US 46*/	void 		beginFrom							(String _page="/");
			
			/*EDU US 47*/	void 		addButton							(String _name, String _value);
			
			
			/*EDU US 47*/	void 		finishFrom							();
			
			/*EDU US 60*/	void		saveParameters						(String _parameters);
			/*EDU US 61*/	String		findStringValue						(String _var);
			
			
	//***************************************************************************************************
	//                                        METHODES DE BAS NIVEAU
	//***************************************************************************************************
			/*EDU US 60*/	bool 		sendWebPagebegin					(uint8_t mux_id, uint32_t len);
			/*EDU US 61*/	bool 		sendWebPageFinish					();
			/*EDU US 62*/	uint8_t 	closeClient							(uint8_t _id);	
													
							bool 		serialInfos;
							bool 		serialdebug;
			
		//========== ESP : fonctions constructeur
		
							bool 		recvFind							(String target, uint32_t timeout = 1000);
							bool 		recvFindAndFilter					(String target, String begin, String end, String &data, uint32_t timeout = 1000);
							String 		recvString							(String target, uint32_t timeout = 1000);
							String 		recvString							(String target1, String target2, uint32_t timeout = 1000);
							bool 		restart								(void);
							bool 		eATRST								(void); 
							bool 		eAT									(void);
							void 		rx_empty							(void);
							bool 		setOprToSoftAP						(void);
							bool 		qATCWMODE							(uint8_t *mode); 
							bool 		sATCWMODE							(uint8_t mode);
							String 		getLocalIP							(void);
							bool 		eATCIFSR							(String &list);
							bool 		sATCIPMUX							(uint8_t mode);
							bool 		enableMUX							(void);
							bool 		startTCPServer						(uint32_t port = 333);
							bool 		sATCIPSERVER						(uint8_t mode, uint32_t port = 333);
							bool 		setTCPServerTimeout					(uint32_t timeout = 180);
							bool 		sATCIPSTO							(uint32_t timeout);
							uint32_t 	recvPkg								(String &buffer, uint8_t *coming_mux_id, uint32_t timeout);
							

	protected:
			


};
#endif

/*########## Les différents Serials
	+ Hard
	+ SoftwareSerial
	+ AltSoftSerial
	+ http://forum.arduino.cc/index.php?topic=251764
*/


//V1

/*
//#include <SoftwareSerial.h>
//#include <AltSoftSerial.h>
#include <Duinoedu_SmartWifiServer.h>

//#define SSID        "SSID"
//#define PASSWORD    "Pass"

//SoftwareSerial espSerial(8,9);
//AltSoftSerial espSerial;              // D8/9 auto


Duinoedu_SmartWifiServer wifi(Serial1);

void setup(void)
{
    Serial.begin(115200);
    // wifi.reset();
    wifi.begin(38400);
    wifi.activeAcceesPoint();  
    //if (wifi.joinAP(SSID, PASSWORD)) {
    //    Serial.print("Join AP success\r\n");
    //    Serial.print("IP: ");
    //    Serial.println(wifi.getLocalIP().c_str());    
    //} else {
    //    Serial.print("Join AP failure\r\n");
    //}    
    wifi.enableMultiClients();
    //smartWifi.disableMultiClients();
    wifi.startTcpServer(80);
    wifi.setTcpServerTimeOutInSeconds(10);
    wifi.getLocalIp();
    pinMode(A0, INPUT);
    pinMode(2, OUTPUT);
 }
 
void loop(void){
 
  if ( wifi.requestReceivedAndParametersSaved() ){
    for (int i=0;i<3;i++){
      wifi.manageAll();
      wifi.addHttpHeaders();
      wifi.addHead("DuinoEDU");
      wifi.beginBody();
        wifi.beginFrom();
    
            wifi.addText("Pilotage d'une LED 0 <br>");
            wifi.addButton("LED00","ON");
            wifi.addButton("LED00","OFF");
            wifi.addText("<br>");
   
            wifi.addText("Pilotage d'une LED 2 <br>");
            wifi.addButton("LED02","ON");
            wifi.addButton("LED02","OFF");
            wifi.addText("<br>");
  /*        
            wifi.addText("Pilotage d'une LED 3 <br>");
            wifi.addButton("LED03","ON");
            wifi.addButton("LED03","OFF");
            wifi.addText("<br>");

            wifi.addText("Pilotage d'une LED 4 <br>");
            wifi.addButton("LED04","ON");
            wifi.addButton("LED04","OFF");
            wifi.addText("<br>");
 
            wifi.addText("Pilotage d'une LED 5 <br>");
            wifi.addButton("LED05","ON");
            wifi.addButton("LED05","OFF");
            wifi.addText("<br>");
  /*          
            wifi.addText("Pilotage d'une LED 6 <br>");
            wifi.addButton("LED06","ON");
            wifi.addButton("LED06","OFF");
            wifi.addText("<br>");
            
            wifi.addText("Pilotage d'une LED 7 <br>");
            wifi.addButton("LED07","ON");
            wifi.addButton("LED07","OFF");
            wifi.addText("<br>");
            
            wifi.addText("Pilotage d'une LED 8 <br>");
            wifi.addButton("LED08","ON");
            wifi.addButton("LED08","OFF");
            wifi.addText("<br>");
            
            wifi.addText("Pilotage d'une LED 8 <br>");
            wifi.addButton("LED08","ON");
            wifi.addButton("LED08","OFF");
            wifi.addText("<br>");
 
          wifi.finishFrom();
      wifi.finishBody();
    }
  }

     if ( wifi.findStringValue("LED00")=="ON" ){ 
       digitalWrite(2,HIGH); 
     }
     if ( wifi.findStringValue("LED00")=="OFF" ){ 
       digitalWrite(2,LOW); 
     }
 
}
*/





//V2

/*

//#include <SoftwareSerial.h>
//#include <AltSoftSerial.h>
#include <Duinoedu_SmartWifiServer.h>

//AltSoftSerial espSerial;              // D8/9 auto
//SoftwareSerial espSerial(8,9);

Duinoedu_SmartWifiServer wifi(Serial1);

void setup(){
  Serial.begin(115200);
  wifi.begin(38400);
  wifi.activeAcceesPoint();
  wifi.enableMultiClients();
  wifi.startTcpServer(80);
  wifi.setTcpServerTimeOutInSeconds(10);
  wifi.getLocalIp();
  pinMode(A0, INPUT);
  pinMode(2, OUTPUT);
}



void loop(){
  if(wifi.requestReceived()){
    
    //Serial.println(wifi.buffer);
    //Serial.print("ID:");Serial.println(wifi.id);
    wifi.choseAnAnswer();
    for (int i=0;i<3;i++){
        wifi.manageAll();
        
        wifi.addHttpHeaders();
        wifi.addHead("DuinoEDU");
        wifi.beginBody();
        wifi.beginFrom();
         
          wifi.addText("Pilotage d'une LED 30 <br>");
          wifi.addButton("LED30","ON");
          wifi.addButton("LED30","OFF");
          wifi.addText("<br>");

          wifi.addText("Pilotage d'une LED 31 <br>");
          wifi.addButton("LED31","ON");
          wifi.addButton("LED31","OFF");
          wifi.addText("<br>");
       
        wifi.finishFrom();
        wifi.finishBody();
          
    }
  }
}
*/


//////////// FORCAGE DE LA VITESSE
/*
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8,9);
void sendReq();
void setup(){
  Serial.begin(9600);			
  
  mySerial.begin(4800);		// Tentative à 4800 bauds
  sendReq();
  mySerial.begin(9600);		// Tentative à 9600 bauds
  sendReq();
  mySerial.begin(14400);		//...
  sendReq();
  mySerial.begin(19200);
  sendReq();
  mySerial.begin(28800);
  sendReq();
  mySerial.begin(38400);
  sendReq();
  mySerial.begin(57600);
  sendReq();
  mySerial.begin(115200);
  sendReq();  
}
void loop(){
  
}
void sendReq(){
  mySerial.print("AT+CIOBAUD=38400\r\n");  // Vitesse est forcée à 38400 bauds
  delay(10);
  while( mySerial.available()){
    Serial.write( mySerial.read() );
  }
  Serial.print("\r\n");
}


*/

