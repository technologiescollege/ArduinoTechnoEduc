/*
 
  Original Author: David Souder - souder.d@gmail.com
  Pour www.duinoedu.com
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "duinoeduGpsAdd.h"

#include <avr/pgmspace.h>
#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>


//========== << CONSTRUCTOR >>

/*EDU US*/	duinoeduGpsAdd::duinoeduGpsAdd(){
	
	//-- On définit les valeurs par défaut
		baudRate = 9600;
		m_usingInterrupt = true;    // Juste la variable initialisée
		serialInfos=false;
		timeDifference =0;
}


//========== SETUP
//== SI SOFTWARE SERIAL
/*EDU FR*/	void	duinoeduGpsAdd::brancher(int8_t pin1, int8_t pin2){branch(pin1,pin2);}
/*EDU US*/	void 	duinoeduGpsAdd::branch(int8_t pin1, int8_t pin2){
	//-- Création du softwareSerial interne à l'objet
		softwareSerial = new SoftwareSerial(pin1, pin2);
		m_serialType = EDUGPS_SOFTWARESERIAL;
	//-- Création de l'objet GPS Adafruit connecté au sofwareSerial
		gps = new Adafruit_GPS(softwareSerial);
		//if(serialInfos==true) Serial.println("SoftwareSerial ok");
	//-- Fin de branchement commun à tous les modes 
		endBranch();
	}

//== SI HARDWARE SERIAL
/*EDU FR*/	void	duinoeduGpsAdd::brancher(HardwareSerial *uartx){branch(uartx);}
/*EDU US*/	void 	duinoeduGpsAdd::branch(HardwareSerial *uartx){
	//-- Création du softwareSerial interne à l'objet
		
	//-- Création de l'objet GPS Adafruit connecté au hardwareSerial
		gps = new Adafruit_GPS(uartx);
		//if(serialInfos==true) Serial.println("HardwareSerial ok");
		m_serialType = EDUGPS_HARDWARESERIAL;
	//-- Fin de branchement commun à tous les modes 
		endBranch();
}

//== COMMUN A TOUS LES MODES DE BRANCHEMENT
/*EDU PR*/	void	duinoeduGpsAdd::endBranch(){
		//-- On fixe la vitesse du port série virtuel via l'objet Adafruit
		gps->begin(baudRate);
	//-- On définit la masse d'information transmise par le GPS au minimum par défaut
		gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
	//-- On fixe un rafraichissement à 1Hz par défaut
		gps->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
	//-- Antenne
		gps->sendCommand(PGCMD_ANTENNA);
	//-- Version
	/*
		if(m_serialType==EDUGPS_SOFTWARESERIAL){
			softwareSerial->println(PMTK_Q_RELEASE);
		}else{
			hardwareSerial->println(PMTK_Q_RELEASE);
		}
	*/
	//-- On configure l'utilisation des interruptions par défaut	
		useInterrupt(true);
	//-- On fixe le décalage des heures
		setTimeDifference(timeDifference);
	//-- Pause
		delay(1000);
}

//========== METHODES OPERATIVES
// LOCALISATION
/*EDU US*/	String	duinoeduGpsAdd::latitudeTxt(uint8_t nbrOfDec){
	return floatToString(latitudeNbr(), nbrOfDec);
}
/*EDU US*/	String	duinoeduGpsAdd::longitudeTxt(uint8_t nbrOfDec){
	return floatToString(longitudeNbr(), nbrOfDec);
}
/*EDU US*/	float 	duinoeduGpsAdd::latitudeNbr(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	float f = 0.0;
	if(isSynchronized()) f = gps->latitude;
	return f;
}
/*EDU US*/	float 	duinoeduGpsAdd::longitudeNbr(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	float f = 0.0;
	if(isSynchronized()) gps->longitude;
	return f;
}
/*EDU US*/	char	duinoeduGpsAdd::latitudeDirection(){
	char c = '?';
	if(isSynchronized()) c=gps->lat;
	return c;
}
/*EDU US*/	char	duinoeduGpsAdd::longitudeDirection(){
		char c = '?';
	if(isSynchronized()) c=gps->lon;
	return c;
}

/*EDU US*/	String	duinoeduGpsAdd::positionGoogleMaps(){
	String result = "";
	result += latitudeGoogleMapsTxt();
	result += ',';
	result += longitudeGoogleMapsTxt();
	
	return result;
}
/*EDU US*/	String	duinoeduGpsAdd::latitudeGoogleMapsTxt(uint8_t nbrOfDec){
	return floatToString(latitudeGoogleMapsNbr(), nbrOfDec);
}
/*EDU US*/	String	duinoeduGpsAdd::longitudeGoogleMapsTxt(uint8_t nbrOfDec){
	return floatToString(longitudeGoogleMapsNbr(), nbrOfDec);
}
/*EDU US*/	float 	duinoeduGpsAdd::latitudeGoogleMapsNbr(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	float f = 0.0;
	if(isSynchronized()) f=gps->latitudeDegrees;
	return f;
}
/*EDU US*/	float 	duinoeduGpsAdd::longitudeGoogleMapsNbr(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	float f = 0.0;
	if(isSynchronized()) f=gps->longitudeDegrees;
	return f;
}


// DATE
/*EDU FR*/	uint8_t	duinoeduGpsAdd::jour(){return day();}
/*EDU US*/	uint8_t	duinoeduGpsAdd::day(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	uint8_t d = gps->day;
	return d;
}

/*EDU FR*/	uint8_t	duinoeduGpsAdd::mois(){return month();}
/*EDU US*/	uint8_t	duinoeduGpsAdd::month(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	uint8_t m = gps->month;
	return m;
}

/*EDU FR*/	uint16_t duinoeduGpsAdd::annee(uint8_t format){return year(format);}
/*EDU US*/	uint16_t duinoeduGpsAdd::year(uint8_t format){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	uint16_t y = gps->year;
	if(format==4) y+=2000;
	return y;
}

// HEURE
/*EDU US*/	String duinoeduGpsAdd::heure(String pattern){return time(pattern);}
/*EDU US*/	String duinoeduGpsAdd::time(String pattern){
	String t = "";
	for(int8_t i=0;i<pattern.length();i++){
		if(i==0) t+=hours();
		if(i==1) t+=minutes();
		if(i==2) t+=seconds();
		if(i==3) t+=milliseconds();
		t+=pattern[i];
	}
	return t;
}

/*EDU FR*/	uint8_t duinoeduGpsAdd::heures(){return hours();}
/*EDU US*/	uint8_t duinoeduGpsAdd::hours(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	uint8_t h = gps->hour;
	h+=timeDifference;
	return h;
}

/*EDU US*/	uint8_t duinoeduGpsAdd::minutes(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	uint8_t m = gps->minute;
	return m;
}

/*EDU FR*/	uint8_t duinoeduGpsAdd::secondes(){return seconds();}
/*EDU US*/	uint8_t duinoeduGpsAdd::seconds(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	uint8_t s = gps->seconds;
	return s;
}

/*EDU FR*/	uint16_t duinoeduGpsAdd::millisecondes(){return milliseconds();}
/*EDU US*/	uint16_t duinoeduGpsAdd::milliseconds(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	uint8_t ms = gps->milliseconds;
	return ms;
}

// ENVIRONNEMENTAL
/*EDU US*/	uint16_t duinoeduGpsAdd::vitesse(){
	//-- Conversion knots (fournis par le GPS) en km/h
		float knots = speed();
		float km	= knots * 1.852;
	return km;}
/*EDU US*/	uint16_t duinoeduGpsAdd::speed(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	uint8_t sp = gps->speed;
	return sp;
}

/*EDU US*/	uint16_t duinoeduGpsAdd::angle(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	uint8_t a = gps->angle;
	return a;
}

/*EDU US*/	float	 duinoeduGpsAdd::altitude(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	float a = gps->altitude;
	return a;
}


//========== STATUT
/*EDU US*/	boolean duinoeduGpsAdd::estSynchronise(){return isSynchronized();}
/*EDU US*/	boolean duinoeduGpsAdd::isSynchronized(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	boolean s = gps->fix;
	return s;
}


//========== MODIFICATION D'OPTIONS
/*EDU FR*/	void 	duinoeduGpsAdd::activerInterruption(){
	useInterrupt(true);
}
/*EDU FR*/	void 	duinoeduGpsAdd::desactiverInterruption(){
	useInterrupt(false);
}
/*EDU US*/	void 	duinoeduGpsAdd::useInterrupt(boolean v){
// Choix utilisateur entre une utilisation des interruptions ou non
  if (v) {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    m_usingInterrupt = true;
  } else {
    TIMSK0 &= ~_BV(OCIE0A);
    m_usingInterrupt = false;
  }
}

/*EDU FR*/	void 	duinoeduGpsAdd::definirQuantiteInformation(int8_t level){setVerboseLevel(level);}
/*EDU US*/	void	duinoeduGpsAdd::setVerboseLevel(int8_t level){
	switch (level) {
		case 1 : gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY ); break;	// RMC
		case 2 : gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA  ); break;	// RMC+GGA
		case 3 : gps->sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA ); break;	// ALL
		case 0 : gps->sendCommand(PMTK_SET_NMEA_OUTPUT_OFF     ); break;	// NON
	}
}

/*EDU FR*/	void	duinoeduGpsAdd::definirFrequenceTrames(int8_t freq){setUpdateFrequency(freq);}
/*EDU US*/	void	duinoeduGpsAdd::setUpdateFrequency(int8_t freq){
		switch (freq) {
		case EDUGPS_100MLHZ : gps->sendCommand(PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ); 
		case EDUGPS_200MLHZ : gps->sendCommand(PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ);
		case EDUGPS_1HZ     : gps->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ           );
		case EDUGPS_5HZ     : gps->sendCommand(PMTK_SET_NMEA_UPDATE_5HZ           );
		case EDUGPS_10HZ    : gps->sendCommand(PMTK_SET_NMEA_UPDATE_10HZ          );
	}
}

/*EDU FR*/	void	duinoeduGpsAdd::definirFrequencePortSerie(uint16_t baud){setBaudRate(baud);}
/*EDU US*/	void	duinoeduGpsAdd::setBaudRate(uint16_t baud){
	baudRate = baud;
	gps->begin(baudRate);
}

/*EDU FR*/	void	duinoeduGpsAdd::definirDecalageHoraire(int8_t decalage){setTimeDifference(decalage);}
/*EDU US*/	void	duinoeduGpsAdd::setTimeDifference(int8_t decalage){
	timeDifference=decalage;
}

/*EDU FR*/	boolean duinoeduGpsAdd::mettreEnVeille(){return standby();}
/*EDU US*/	boolean duinoeduGpsAdd::standby(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	boolean stb = gps->standby();
	return stb;
}

/*EDU FR*/	boolean duinoeduGpsAdd::reveiller(){return wakeup();}
/*EDU US*/	boolean duinoeduGpsAdd::wakeup(){
	effectiveUpdate();				// Uniquement si une trame est reçue (pas de perte réelle de temps)
	boolean wu = gps->wakeup();
	return wu;
}


//========== LECTURE BAS NIVEAU
/*EDU FR*/	char 	duinoeduGpsAdd::lireUnCaractere(){return readOneChar();}
/*EDU US*/	char 	duinoeduGpsAdd::readOneChar(){
	return gps->read();
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES INTERNES
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/*EDU US*/	void 	duinoeduGpsAdd::effectiveUpdate(){
	
	//---- On lit tout ce qui est disponible (stockage dans l'objet Adafruit) si pas déjà géré par l'interruption
		if(!m_usingInterrupt){
			readAllInSerial();
			//Serial.println("ReadAll");
		}
	//---- On teste si une trame est complète
		if(gps->newNMEAreceived()){
			if(serialInfos==true) Serial.println("Nouvelle trame");
			if(!gps->parse(gps->lastNMEA())){
				return;
			}
		}
}

/*EDU US*/	void 	duinoeduGpsAdd::readAllInSerial(){
	//-- On marque l'heure à laquelle on démarre la lecture
		uint16_t start = millis();
		uint16_t maxDuration = 50;           // On estime que l'on peut perdre 100ms pour une lecture sans interruption
	reloop:
//== Gestion de la lecture si interruptions désactivées
		while(softwareSerial->available()){
			char c = gps->read();
			//-- On regarde si une trame ne serait pas complète 
				if(gps->newNMEAreceived()) break; 
		}
		if (millis() < start + maxDuration) goto reloop;

}

/*EDU US*/	String	duinoeduGpsAdd::floatToString(float val, uint8_t nbrOfDec, uint8_t widthNbr, uint8_t minWidth){

	//-- On sauvegarde la valeur;
		float floatVal=val;
	//-- On initialise un tableau;
		char resultTabChar[widthNbr]; 
		for(uint8_t i=0;i<widthNbr;i++){
			resultTabChar[i]=NULL;
		}
	//-- On utilise la fonction de conversion
		dtostrf(floatVal, minWidth, nbrOfDec, resultTabChar);
	//-- On construit une chaîne String
		String resultString ="";
		for(uint8_t i=0;i<widthNbr;i++){
			if(resultTabChar[i]==NULL) break;
			resultString+=resultTabChar[i];
		}
	return resultString;
}






