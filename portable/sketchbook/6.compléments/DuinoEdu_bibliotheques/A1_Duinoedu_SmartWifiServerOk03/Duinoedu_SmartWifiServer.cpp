/*
  Original Author: David Souder - staff@duinoedu.com
  www.duinoedu.com
*/

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Duinoedu_SmartWifiServer.h"


#include <avr/pgmspace.h>

#define SERIAL_INFOS(arg1)						(serialInfos)	?Serial.println		(arg1)								:Serial.print("")

//DEBUG
#define SERIAL_PRINT(arg1)						(SERIALDEBUG)   ?Serial.print   	(arg1)      						:Serial.print("")
#define SERIAL_WRITE(arg1)						(SERIALDEBUG)   ?Serial.write  		(arg1)      						:Serial.print("")
#define SERIAL_PRINTLN(arg1)					(SERIALDEBUG)   ?Serial.println		(arg1)      						:Serial.print("")			
		






//========== << CONSTRUCTOR >>

//Duinoedu_SmartWifiServer::Duinoedu_SmartWifiServer(SoftwareSerial &uart){
Duinoedu_SmartWifiServer::Duinoedu_SmartWifiServer(AltSoftSerial &uart){
//Duinoedu_SmartWifiServer::Duinoedu_SmartWifiServer(HardwareSerial &uart){	
	espSerial 		= &uart;	
	serialInfos 	= 0;
	ip				="";	
	id				=-1;							// N° du client -1=pas de client en cours
	replyMe			=0;								// Vaut 1 si une réponse doit être envoyée au client en cours
	parametersSent	="";							// ex: /?button1=ON&..
	sendingState	=0;								// Par défaut, rien à faire... 1:calculer; 2:envoyer; 3 fermer
	globalLength	=0;								// Taille de la chaîne globale à envoyer à 0
	parameters		="#";							// Doit contenir les parametres transmis
	buffer = "1234567890123456789012345678901234567890";

}

//========== SETUP
/*EDU US 01*/	void 	Duinoedu_SmartWifiServer::begin								(uint16_t _speed){
	
	//---- On initialise le serial passé  en argument
		espSerial->begin(_speed);
		rx_empty();
	return;
}

/*EDU US 02*/	void 	Duinoedu_SmartWifiServer::reset								(){
	
		bool restartOk = restart();
		(restartOk)?SERIAL_INFOS(F("RESSET:ok")):SERIAL_INFOS(F("RESSET:error"));
	return;
}

/*EDU US 03*/	void 	Duinoedu_SmartWifiServer::activeAcceesPoint					(){
		
		bool activeAccessPointOk = setOprToSoftAP();
		(activeAccessPointOk)?SERIAL_INFOS(F("AP:ok")):SERIAL_INFOS(F("AP:error"));
	return;
}		

/*EDU US 04*/	String 	Duinoedu_SmartWifiServer::getLocalIp						(){
	//---- On récupère toute la chaîne de retour
		String getLocalIpString = getLocalIP();
		SERIAL_PRINTLN(getLocalIpString);
		
	return getLocalIpString;
}
					
/*EDU US 05*/	void 	Duinoedu_SmartWifiServer::enableMultiClients				(){
		bool enableMultipleClientsOk = enableMUX();
		(enableMultipleClientsOk)?SERIAL_INFOS(F("ENABLE MUX:ok")):SERIAL_INFOS(F("ENABLE MUX:error"));
	return;
}	

/*EDU US 06*/	void 	Duinoedu_SmartWifiServer::disableMultiClients				(){
		bool enableMultipleClientsOk = enableMUX();
		(enableMultipleClientsOk)?SERIAL_INFOS(F("DISABLE MUX:ok")):SERIAL_INFOS(F("DISABLE MUX:error"));
	return;
}	

/*EDU US 07*/	void 	Duinoedu_SmartWifiServer::startTcpServer					(uint32_t _port){
		bool startTcpServerOk = startTCPServer(_port);
		(startTcpServerOk)?SERIAL_INFOS(F("SERVER:ok")):SERIAL_INFOS(F("SERVEUR:error"));
	return;
}		

/*EDU US 08*/	void 	Duinoedu_SmartWifiServer::setTcpServerTimeOutInSeconds		(uint8_t _timeOut){
		bool setTcpServerTimeOutInSecondsOk = setTCPServerTimeout(_timeOut);
		(setTcpServerTimeOutInSecondsOk)?SERIAL_INFOS(F("TIMEOUT:ok")):SERIAL_INFOS(F("TIMEOUT:error"));
	return;
}



//========== LOOP

/*EDU US 12*/	int8_t 	Duinoedu_SmartWifiServer::requestReceivedAndParametersSaved	(){
//////////////////////////////////////////////////////////////////////////////////////////////////	
	// Permet de récupérer l'id client et parse les données client(s)
	// Nous attendons : +IPD,{id client},{taille des données}:{données envoyées par le client}
	// LES PETITES PAUSES SONT INDISPENSABLES !!!!!!!!!!!
	
	uint32_t lastTime;
	char c;
	
	//---- On remet les paramètres reçus à 0
		parametersSent="";
	
	//---- On recherche le motif 							[curseur : ▓+IDP,0,789:données] 	
		if(espSerial->available()){
			c = espSerial->read();
		}
		if(c!='+'){
			delay(2);	//1
			//SERIAL_PRINTLN("NR");
			return 0;
		}
		
		/*
		char car;
		if(espSerial->available()){
			while(espSerial->available()){
				car = espSerial->read();
				if(car=='+'){
					SERIAL_WRITE(car);
					break;
				}
			}
		}else{
			return 0;
		}
		*/
		
		delay(5);
		if(espSerial->available()){
			char c = espSerial->read();
			SERIAL_PRINTLN(c);
			if( c != 'I' ){ //I
				return 0;
			}	
			
		}else{
			return 0;
		}
			c = espSerial->read();					//P
			//SERIAL_WRITE( c );	
			c = espSerial->read();					//D
			//SERIAL_WRITE( c );
			c = espSerial->read();					//,
			//SERIAL_WRITE( c );
			
			//SERIAL_WRITE( "\r\n" );
			
		
	
	//---- On enregistre l'id client (0..3)					[curseur : +IDP,▓0,789:données]			
		delay(5);
		if( espSerial->available() ){
			id=espSerial->read()-48; //0..3
			//SERIAL_PRINT("i");SERIAL_PRINTLN(id);
		}
	
	//---- On se décale au premier chiffre					[curseur : +IDP,0▓,789:données]			
		if( espSerial->available() ){
			espSerial->read();	//,
		}

	
	//---- On récupère la taille des données				[curseur : +IDP,0,▓789:données] » [curseur : +IDP,0,789:▓données]
		delay(5);
		String   lenSt="";
		//--- On lit jusqu'aux ':'							
			for(uint8_t i=0;i<4;i++){
				char c;
				if( espSerial->available() ){
					c = espSerial->read();
				}
				if(c==':'){
					break;
					
				}else{
					lenSt+=c;
				}
			}
		//--- On stocke la taille 
			dataLength = lenSt.toInt();
			//SERIAL_PRINT('t');SERIAL_PRINTLN(dataLength);
	
	//---- On se place avant le '/'							[curseur : +IDP,0,789:▓GET /?bouton1=ON] » [curseur : +IDP,0,789:GET ▓/?bouton1=ON] OU [curseur : +IDP,0,789:GET ▓/favicon.ico]
			delay(2);
			for(uint8_t i=0;i<4;i++){
				if( espSerial->available() ){
					c = espSerial->read();
				}
			}
			if(c==' '){
					//SERIAL_PRINTLN("/+");
			}else{
					//SERIAL_PRINTLN("/?");
			}

	//---- On lit jusqu'au premier retour chariot 
		delay(15); //15
		c = NULL;
		lastTime=millis();
		
		while(c!='\r' && c!='\n' && millis()<(lastTime+100) ){
			if(espSerial->available()){
				c = espSerial->read();
				parametersSent += c;
			}else{
				delay(1);
			}
		}
		//SERIAL_PRINT("p"); SERIAL_PRINTLN(parametersSent);

	//---- On regarde quelle resource a été demandée (page web ou favicon ?)
		
		if(parametersSent=="/ HTTP/1.1\r"){
			//SERIAL_PRINTLN(F("SP"));
			//--- Aucun paramètre à sauvegarder
			replyMe=1;
		}else if(parametersSent.indexOf('?')>=0) {
			//SERIAL_PRINTLN(F("AP"));
			//--- Il faut stoker les paramètres
				saveParameters(parametersSent);
			replyMe=2;
		}else if(parametersSent.indexOf("favicon")>=0 || parametersSent.indexOf(".ico")>=0 ){
			//SERIAL_PRINTLN(F("FA"));
			//--- On ne stocke pas les paramètres
			replyMe=3;
		}else{
			// Requête imcomplète
			//SERIAL_PRINTLN(F("P?"));
			replyMe=4;						
		}
				
	
	//---- On vide le buffer correspondant à la reqête demandée

		delay(10);	//15  //30:pb
		uint16_t nbrInBuf = dataLength;
		while (espSerial->available()){				
			char c = espSerial->read();
			//nbrInBuf--;
			//SERIAL_PRINT(c); 		// 
			delay(1); 				// Ne fonctionne pas sans petite pause
			//delay(5); //?
		}
		//SERIAL_PRINTLN(F("\r\n"));
	
	return replyMe;
		
}


///// A DEVELOPPER : METHODE AVEC STRING BUFFER
/*EDU US 20*/	uint32_t	Duinoedu_SmartWifiServer::requestReceived				(){
	return recvPkg(buffer,&id,4000);		//4000
}

/*EDU US 21*/	void		Duinoedu_SmartWifiServer::choseAnAnswer					(){
		
		if(buffer.indexOf("/ HTTP/1.1\r")){
			SERIAL_PRINTLN(F("SP"));
			//--- Aucun paramètre à sauvegarder
			replyMe=1;
		}else if(buffer.indexOf('?')>=0) {
			SERIAL_PRINTLN(F("AP"));
			//--- Il faut stoker les paramètres
				////////////////////////////////////////////////////// A MODIFIER
				saveParameters("LED10=ON");
			replyMe=2;
		}else if(buffer.indexOf("favicon")>=0 || parametersSent.indexOf(".ico")>=0 ){
			SERIAL_PRINTLN(F("FA"));
			//--- On ne stocke pas les paramètres
			replyMe=3;
		}else{
			// Requête imcomplète
			SERIAL_PRINTLN(F("P?"));
			replyMe=4;						
		}
}


/*EDU US 30*/	uint8_t Duinoedu_SmartWifiServer::sendData							(String &_data, uint8_t _id){
	
	//----[1] Préparation de commande AT
		sendWebPagebegin( _id, _data.length() );
		delay(100);
	
	//----[2] On envoie les données
		espSerial->print(_data);
	
	//----[4] On attend la réponse de l'ESP [SEND OK] 
		return sendWebPageFinish();
}

/*EDU US 40*/	void    Duinoedu_SmartWifiServer::manageAll							(){
	//---- A t-on besoin d'une réponse ?
		if(replyMe==0){
			return;
		}
	
	//---- PAGE WEB ? (GET / ou GET/?..)
		if(replyMe==1 || replyMe==2){
			//--- On passe à l'étape suivante 1:calculer; 2:envoyer; 3 fermer
				sendingState++;
				SERIAL_PRINTLN(sendingState);
				// 1 => les fct suivantes calculent la longeur
				// 2 => les fct suivantes envoient les chaînes
				// 3 => les fct suivantes n'ont rien à faire mais il faut envoyer CLOSE.
				if(sendingState==2){
					//-- ENVOI - partie 01
						SERIAL_PRINT("CIP1:");SERIAL_PRINTLN(globalLength);
						sendWebPagebegin(id, globalLength);
						/* TEMPS ABSOLUMENT NECESSAIRE /////////////////////////// */ delay(100);	//100
					//-- ON ECRIT LA PAGE WEB
						// Fait par chaque fonction séparément
					//-- Vérifier que l'envoi est fait
						// Fait au passage sendingState==3
				}
			
			//--- On ferme la connexion client
				if(sendingState==3){
					//-- Attente réponse ESP
						sendWebPageFinish();					
					//-- Tout est normalement reçu
						closeClient(id);
					//-- On marque que les fonctions n'ont plus rien à faire.
						sendingState=0;
					//-- On remet à 0 la taille globale de la chîne à transmettre
						globalLength=0;
					//-- On marque que le client à été servi
						replyMe=0;
				}
			return;
		}
	
	//---- Est-ce une demande de favicon ?
		if(replyMe==3){
			//--- On répond 	
				String data;
				//data  += "HTTP/1.1 404	Not Found\r\n";
				data  += "HTTP/1.1 501 Not Implemented\r\n";
				data  += "Connection: close\r\n";
				data  += "\r\n";
				sendData(data, id);
				closeClient(id);		
			//--- On marque que le client en cours à été servi 
				replyMe=0;
				return;			
		}	
		
		if(replyMe==4){
			//-- La requête est mauvaise
				String data;
				//data  += "HTTP/1.1 400	Bad Request\r\n";
				data  += "HTTP/1.1 501 Not Implemented\r\n";
				data  += "Connection: close\r\n";
				data  += "\r\n";
				sendData(data, id);
				closeClient(id);
			//--- On marque que le client en cours à été servi 
				replyMe=0;
				return;			
		}
}

/*EDU US 41*/	void  	Duinoedu_SmartWifiServer::addText							(String _text){
	
	//---- Doit-on faire qq chose ?
		if(sendingState==0){
			return;		
		}

	//---- On prépare les chaînes
		String data = _text;
	
	//---- PASSAGE 01 : calcul de longeur
		if(sendingState==1){
			globalLength	+=	data.length();
			return;
		}
		
	//---- PASSAGE 02 : envoie des chaînes sans fermeture
		if(sendingState==2){
			SERIAL_PRINT(data);
			espSerial->print(data);
			return;
		}
	//---- Fermeture
		if(sendingState==3 ){
			// Ceci est fait par manageAll()
			return;		
		}
	
}

/*EDU US 42*/	void  	Duinoedu_SmartWifiServer::addHttpHeaders					(){	
	
	addText("HTTP/1.1 200 OK\r\n");
	addText("Content-Type: text/html\r\n");
	addText("Cache-Control: no-cache\r\n");
	addText("Connection: keep-alive\r\n");
	addText("\r\n");
	return;
}

/*EDU US 43*/	void 	Duinoedu_SmartWifiServer::addHead							(String _title){

	addText("<!doctype html>");
	addText("<html>");
	  addText("<head>");
	    addText("<title>");
	      addText(_title);
	    addText("</title>");
	  addText("</head>");
	  
	  
	return;
	
}

/*EDU US 44*/	void 	Duinoedu_SmartWifiServer::beginBody							(){
	
	addText("<body>");	
		
}

/*EDU US 5*/	void 	Duinoedu_SmartWifiServer::finishBody						(){
	addText("</body>");
	addText("</html>");

}

/*EDU US 46*/	void 	Duinoedu_SmartWifiServer::beginFrom							(String _page){
	addText("<form method=\"get\" action=\"/\">");
}

/*EDU US 47*/	void 	Duinoedu_SmartWifiServer::addButton							(String _name, String _value){

	addText("<input ");
	addText("type=");
	addText("\"submit\"");
	  addText(" ");
	  addText(" name=\"");
	  addText(_name);
	  addText("\"");
	  addText(" value=\"");
	  addText(_value);
	  addText("\"");
	addText(">");
	
}

/*EDU US 47*/	void 	Duinoedu_SmartWifiServer::finishFrom						(){
	addText("</form>");
}


/*EDU US 60*/	void	Duinoedu_SmartWifiServer::saveParameters					(String _parameters){
// Stocke le paramètre s'il est nouveau dans une chaîne/
// Exemple de paramètre transmis : /?LED01=ON HTTP/1.1

	//==== ON RECUPERE LE PARAMETRE
		SERIAL_PRINTLN(F("-------------------"));
	//---- Quelques variables temporaires
		uint8_t parameterLen;														// Taille du paramètre transmis 
		String reqTemp="";															// ex: LED01=ON
		String varTemp="";															// ex: LED1
		String valTemp="";															// ex: ON
		uint8_t start;																// Pointage d'un emplacement dans une chaîne
		uint8_t stop;																// Pointage d'un emplacement dans une chaîne	
		
	//---- On met en forme le paramètre												[/?LED01=ON HTTP/1.1] » [LED01=ON&]	
		
		//--- On détermine l'emplacement du paramètre 								[...start...stop...]
			parameterLen   = _parameters.length();															
			start = 2;																// Retir: /?
			stop  = parameterLen-10;												// Retir: HTTP/1.1
		
		//--- On récupère le paramètre												[LED01=ON]
			reqTemp = _parameters.substring(start, stop);
		
		//--- On ajoute &															[LED01=ON&]			
			reqTemp+="&";
			SERIAL_PRINT(F("TEP:")); SERIAL_PRINTLN(reqTemp);
	
		// A ce stade, 3 situations :
			/**
			[1] : Var & Val EXISTENT en l'état dans la chaîne
				  => Il n'y a rien à faire
			[2] : Var & Val N'EXISTE PAS
				  => On enregistre VarVal à la fin de la chaîne de paramètres
			[3] : Var EXISTE avec la MAUVAISE VALEUR
				  => On reherche et supprime le paramètre 
				  => On enregistre VarVal à la fin de la chaîne de paramètres
			**/
	
	

	//[1]==== VarVal EXISTENT
		if(parameters.indexOf(reqTemp)>=0){
			SERIAL_PRINTLN(F("NE!"));
			SERIAL_PRINT(F("PA!"));SERIAL_PRINTLN(parameters);
			return;
		}
		
		SERIAL_PRINT(F("PA:"));SERIAL_PRINTLN(parameters);
	
		
	//---- La variable existe t-elle déjà 											[LED01]
		
		//--- On sépare variable/valeur 											[LED01=ON&] » [LED01] + [ON]
			parameterLen = reqTemp.length();
			//-- Récup nom variable
				start   = 0;
				stop  	= reqTemp.indexOf('=')+1;
				varTemp = reqTemp.substring(start, stop);
				SERIAL_PRINT(F("var:")); SERIAL_PRINTLN(varTemp);
			//-- Récup valeur
				start = stop;
				stop  = parameterLen-1; 
				valTemp = reqTemp.substring(start, stop);
				SERIAL_PRINT(F("val:")); SERIAL_PRINTLN(valTemp);
	
		
			int8_t posVar = parameters.indexOf(varTemp) ;
			SERIAL_PRINTLN(F("pos:")); SERIAL_PRINTLN(posVar);
		
	//[2]==== VarVal n'EXISTENT PAS
		if( posVar <0 ){
			//--- On ajoute la variable et la valeur
				parameters += reqTemp;	
				SERIAL_PRINT(F("PA/"));SERIAL_PRINTLN(parameters);
			return;			
		
	//[3]==== Var EXISTENT avec la MAUVAISE VALEUR
		}else{
			
			//-- On retire le couple var/val qui n'est pas à jour
				//-- On sait que le motif à retirer xx : LED.....&xxxx&........&
			 
				uint8_t parametersLen = parameters.length();
				uint8_t endPosVal = posVar;
			
				for(uint8_t i=posVar; i<parametersLen;i++){
					if(parameters[i]!='&'){
						endPosVal++;
					}else{
						break;
					}
				}
				SERIAL_PRINTLN("fin");SERIAL_PRINTLN(endPosVal);
				
				//-- A ce stade, la partie à retirer commence à posVar et fini à endPosVal
					SERIAL_PRINTLN(F("ValVarPos")); 
					SERIAL_PRINTLN(posVar);
					SERIAL_PRINTLN(endPosVal);
					
					String parameters1="";
					String parameters2="";
	
					//- P1 : Premier partie des paramètres	
						start = 0;
						stop  = posVar;
							SERIAL_PRINTLN(F("P1Pos")); 
							SERIAL_PRINTLN(start);
							SERIAL_PRINTLN(stop);
						parameters1 = parameters.substring(start, stop);
						SERIAL_PRINT(F("P1:"));SERIAL_PRINTLN(parameters1);
						
					//- P2 : Deuxième partie des paramètres => Dépassement possible de l'indice stop
						start  = endPosVal+1;
						stop   = parametersLen;
							SERIAL_PRINTLN(F("P2Pos")); 
							SERIAL_PRINTLN(start);
							SERIAL_PRINTLN(stop);
						parameters2 = parameters.substring(start, stop);
						SERIAL_PRINT(F("P2:"));SERIAL_PRINTLN(parameters2);
			
					//- PT : On rassemble les paramètres	
						parameters = parameters1 + parameters2;
						parameters1="";
						parameters2="";
			
			//-- On enregister le bon couple var/val à la fin de la chaîne
				parameters+=reqTemp;

			SERIAL_PRINTLN("exis");
			SERIAL_PRINT(F("PA->"));SERIAL_PRINTLN(parameters);
		}
		
}

/*EDU US 61*/	String	Duinoedu_SmartWifiServer::findStringValue					(String _var){
// Permet de renvoyer la valeur (Val) associée à la variable (Var)	
// La chaine dans laquelle on doit trouver la variable et la valeur est "parameters" 
// Ex de contenu de la chaine parametres: #LED02=ON&LED1=OFF&
		
		int16_t posVar 			= parameters.indexOf(_var);
		uint8_t parametersLen 	= parameters.length();
		uint8_t posVal=0;
		uint8_t posValEnd;
		String  returnVal;
		
		//SERIAL_PRINTLN(parameters);
		//SERIAL_PRINT(F("parLen:"));SERIAL_PRINTLN(parametersLen);
		//SERIAL_PRINT(F("posVar:"));SERIAL_PRINTLN(posVar);
		
	
	//==== On recherche la présence de la variable 									// ex: [LED1]
		if(posVar==-1){	
			//SERIAL_PRINTLN(F("NEAN"));
			return "0";
		}
		
		else{
			//--- On recherche les délimiteurs de la valeur
				//-- On reherche le début de Val
				//-- On se déplace de posVar jusqu'au '='
					for(posVal=posVar; posVal<parametersLen; posVal++){
						if( parameters[posVal] == '=' ){
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
						if( parameters[posValEnd] == '&' ){
							break;
						}
						//SERIAL_PRINTLN(F("posValEnd:"));
						//SERIAL_PRINTLN(posValEnd);
					}
			
			//--- On récupère la valeur
				//SERIAL_PRINTLN(F("posValDeA:"));
				//SERIAL_PRINTLN(posVal);
				//SERIAL_PRINTLN(posValEnd);
				
				returnVal = parameters.substring(posVal, posValEnd);

				
		}
	return returnVal;

}

//***************************************************************************************************
//                                        METHODES DE BAS NIVEAU
//***************************************************************************************************

/*EDU US 60*/	bool Duinoedu_SmartWifiServer::sendWebPagebegin						(uint8_t mux_id, uint32_t len){
// 01 Envoie la commande ATCIPSEND + vérifie que l'esp est ok (>) pour la réception d'une page web	
// Inspiré de la méthode ESP8266::sATCIPSENDMultiple(..)
// Procédure : 01:cette fonction prépare l'envoi de la page web; 02:la page web est envoyée par les fonctions elles-mêmes; 03:il faut vérifier l'envoi par la fonction part2
   
	//---- AT+CIPSEND,id,length
		rx_empty();
		espSerial->print("AT+CIPSEND=");
		espSerial->print(mux_id);
		espSerial->print(",");
		espSerial->println(len);
    
	//---- Attente de confirmation de l'ESP
	
		if (recvFind(">", 3000)) {										//500
			rx_empty();
			SERIAL_PRINTLN(">+");
			return true;
		}else{
			SERIAL_PRINTLN(">?");
			// TO DO : Il faudra traiter cet échec
			return false;
		}
	
		/* TEMPS ABSOLUMENT NECESSAIRE /////////////////////////// */ delay(200); //100 //200 grosses pages
	
	//---- La page web doit être envoyée
		// Fait par les fonctions utilisateur
    
}

/*EDU US 61*/	bool Duinoedu_SmartWifiServer::sendWebPageFinish					(){
// SEND OK ?
		
	
		if(recvFind("SEND OK", 4000)){			//500
			SERIAL_PRINTLN("S+");
			return true;
		}else{
			SERIAL_PRINTLN("S?");
			return false;
		}

	
}

/*EDU US 62*/	uint8_t Duinoedu_SmartWifiServer::closeClient						(uint8_t _id){
// De la lib ESP8266	
	String data;
    rx_empty();
 	espSerial->print("AT+CIPCLOSE=");
    espSerial->println(_id);
    
	data = recvString("OK", "link is not", 2000);
    if (data.indexOf("OK") != -1 || data.indexOf("link is not") != -1) {
        SERIAL_PRINTLN("C+");
		return 1;
    }else{
		SERIAL_PRINTLN("C?");
		return 0;
	}

}




//========== ESP - Méthodes constructeur
bool Duinoedu_SmartWifiServer::recvFind(String target, uint32_t timeout){
    String data_tmp;
    data_tmp = recvString(target, timeout);
    if (data_tmp.indexOf(target) != -1) {
        return true;
    }
    return false;
}

String Duinoedu_SmartWifiServer::recvString(String target, uint32_t timeout){
    String data;
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
   
		while(espSerial->available() > 0) {
            a = espSerial->read();
			if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target) != -1) {
            break;
        }   
    }
    return data;
}

String Duinoedu_SmartWifiServer::recvString(String target1, String target2, uint32_t timeout){
    String data;
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(espSerial->available() > 0) {
            a = espSerial->read();
			if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target1) != -1) {
            break;
        } else if (data.indexOf(target2) != -1) {
            break;
        }
    }
    return data;
}

bool Duinoedu_SmartWifiServer::restart(void){
    unsigned long start;
    if (eATRST()) {
        delay(2000);
        start = millis();
        while (millis() - start < 3000) {
            if (eAT()) {
                delay(500); /* Waiting for stable */ //1500 d'origine
                return true;
            }
            delay(100);
        }
    }
    return false;
}

bool Duinoedu_SmartWifiServer::eATRST(void){
    rx_empty();
    espSerial->println("AT+RST");
    return recvFind("OK");
}

bool Duinoedu_SmartWifiServer::eAT(void){
    rx_empty();
    espSerial->println("AT");
    return recvFind("OK");
}

void Duinoedu_SmartWifiServer::rx_empty(void) {
    while(espSerial->available() > 0) {
        espSerial->read();
    }
}

bool Duinoedu_SmartWifiServer::setOprToSoftAP(void){
    uint8_t mode;
    if (!qATCWMODE(&mode)) {
        return false;
    }
    if (mode == 2) {
        return true;
    } else {
        if (sATCWMODE(2) && restart()) {
            return true;
        } else {
            return false;
        }
    }
}

bool Duinoedu_SmartWifiServer::qATCWMODE(uint8_t *mode){
    String str_mode;
    bool ret;
    if (!mode) {
        return false;
    }
    rx_empty();
    espSerial->println("AT+CWMODE?");
    ret = recvFindAndFilter("OK", "+CWMODE:", "\r\n\r\nOK", str_mode); 
    if (ret) {
        *mode = (uint8_t)str_mode.toInt();
        return true;
    } else {
        return false;
    }
}

bool Duinoedu_SmartWifiServer::sATCWMODE(uint8_t mode){
    String data;
    rx_empty();
    espSerial->print("AT+CWMODE=");
    espSerial->println(mode);
    
    data = recvString("OK", "no change");
    if (data.indexOf("OK") != -1 || data.indexOf("no change") != -1) {
        return true;
    }
    return false;
}

String Duinoedu_SmartWifiServer::getLocalIP(void){
    String list;
    eATCIFSR(list);
    return list;
}

bool Duinoedu_SmartWifiServer::eATCIFSR(String &list){
    rx_empty();
    espSerial->println("AT+CIFSR");
    return recvFindAndFilter("OK", "\r\r\n", "\r\n\r\nOK", list);
}

bool Duinoedu_SmartWifiServer::recvFindAndFilter(String target, String begin, String end, String &data, uint32_t timeout){
    String data_tmp;
    data_tmp = recvString(target, timeout);
    if (data_tmp.indexOf(target) != -1) {
        int32_t index1 = data_tmp.indexOf(begin);
        int32_t index2 = data_tmp.indexOf(end);
        if (index1 != -1 && index2 != -1) {
            index1 += begin.length();
            data = data_tmp.substring(index1, index2);
            return true;
        }
    }
    data = "";
    return false;
}

bool Duinoedu_SmartWifiServer::sATCIPMUX(uint8_t mode){
    String data;
    rx_empty();
    espSerial->print("AT+CIPMUX=");
    espSerial->println(mode);
    
    data = recvString("OK", "Link is builded");
    if (data.indexOf("OK") != -1) {
        return true;
    }
    return false;
}

bool Duinoedu_SmartWifiServer::enableMUX(void){
    return sATCIPMUX(1);
}

bool Duinoedu_SmartWifiServer::startTCPServer(uint32_t port){
    if (sATCIPSERVER(1, port)) {
        return true;
    }
    return false;
}

bool Duinoedu_SmartWifiServer::sATCIPSERVER(uint8_t mode, uint32_t port){
    String data;
    if (mode) {
        rx_empty();
        espSerial->print("AT+CIPSERVER=1,");
        espSerial->println(port);
        
        data = recvString("OK", "no change");
        if (data.indexOf("OK") != -1 || data.indexOf("no change") != -1) {
            return true;
        }
        return false;
    } else {
        rx_empty();
        espSerial->println("AT+CIPSERVER=0");
        return recvFind("\r\r\n");
    }
}

bool Duinoedu_SmartWifiServer::setTCPServerTimeout(uint32_t timeout){
    return sATCIPSTO(timeout);
}

bool Duinoedu_SmartWifiServer::sATCIPSTO(uint32_t timeout){
    rx_empty();
    espSerial->print("AT+CIPSTO=");
    espSerial->println(timeout);
    return recvFind("OK");
}


/*----------------------------------------------------------------------------*/
/* +IPD,<id>,<len>:<data> */
/* +IPD,<len>:<data> */

uint32_t Duinoedu_SmartWifiServer::recvPkg(String &buffer, uint8_t *coming_mux_id, uint32_t timeout){
    String data;
    char a;
    int32_t index_PIPDcomma = -1;
    int32_t index_colon = -1; /* : */
    int32_t index_comma = -1; /* , */
    int32_t len = -1;
    int8_t id = -1;
    bool has_data = false;
    uint32_t ret;
    unsigned long start;
    uint32_t i;
	/*EDU*/ uint32_t buffer_size = buffer.length(); 
	/*EDU*/ uint32_t *data_len = NULL;
    
    if (buffer == NULL) {
        return 0;
    }
    
    start = millis();
    while (millis() - start < timeout) {
        if(espSerial->available() > 0) {
            a = espSerial->read();
            data += a;
        }
        
        index_PIPDcomma = data.indexOf("+IPD,");
        if (index_PIPDcomma != -1) {
            index_colon = data.indexOf(':', index_PIPDcomma + 5);					// Recherche ':' à partir de +IPD, + 5
            if (index_colon != -1) {
                index_comma = data.indexOf(',', index_PIPDcomma + 5);
                /* +IPD,id,len:data */
                if (index_comma != -1 && index_comma < index_colon) { 
                    id = data.substring(index_PIPDcomma + 5, index_comma).toInt();
                    if (id < 0 || id > 4) {
                        return 0;
                    }
                    len = data.substring(index_comma + 1, index_colon).toInt();
                    if (len <= 0) {
                        return 0;
                    }
                } else { /* +IPD,len:data */
                    len = data.substring(index_PIPDcomma + 5, index_colon).toInt();
                    if (len <= 0) {
                        return 0;
                    }
                }
                has_data = true;
                break;
            }
        }
    }
    
    if (has_data) {
        i = 0;
        ret = len > buffer_size ? buffer_size : len;
        start = millis();
        while (millis() - start < 3000) {
            while(espSerial->available() > 0 && i < ret) {
                a = espSerial->read();
                buffer[i++] = a;
            }
            if (i == ret) {
                rx_empty();
               	if (data_len) {
                    *data_len = len;    
                }
                if (index_comma != -1 && coming_mux_id) {
                    *coming_mux_id = id;
                }
                return ret;
            }
        }
    }
    return 0;
}



