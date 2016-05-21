/*
   Original Author: David Souder - souder.d@gmail.com
  Pour www.duinoedu.com
  Date de dernière modification : 27/07/2014
  Adafruit_GPS par Adafruit_GPS
  SofwareSerial par Arduino
  
 */

#ifndef duinoeduGpsAdd_H
#define duinoeduGpsAdd_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include <WProgram.h>
	#include "Wire.h"
#endif

#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>
#include "EDU_debug.h"

//#define DEBUG
#define OUI true
#define NON false

// ISR
#define EDUGPS_INTERRUPTION ISR
#define HORLOGE_0_DEPASSE_VALEUR_A TIMER0_COMPA_vect



class duinoeduGpsAdd{

  public:
	//========== << CONSTRUCTEUR>>
		/*EDU US*/	duinoeduGpsAdd();       
		
		
	//========== SETUP
		//== SI SOFTWARE SERIAL
		/*EDU FR*/	void		brancher(int8_t pin1, int8_t pin2);
		/*EDU US*/	void 		branch  (int8_t pin1, int8_t pin2);
		//== SI HARDWARE SERIAL
		/*EDU FR*/	void		brancher(HardwareSerial *uartx);
		/*EDU US*/	void 		branch  (HardwareSerial *uartx);
		//== COMMUN A TOUS LES MODES DE BRANCHEMENT
		/*EDU PR*/	void		endBranch();

		
	
	//========== OPERATIVES
		//== COORDONNEES NORMALES
		/*EDU US*/	String		latitudeTxt(uint8_t nbrOfDec=4);
		/*EDU US*/	String		longitudeTxt(uint8_t nbrOfDec=4);
		/*EDU US*/	float 		latitudeNbr();
		/*EDU US*/	float 		longitudeNbr();
	
		//== COORDONNEES ANGULAIRES GOOGLE MAPS COMPATIBLES
		/*EDU US*/	String		latitudeGoogleMapsTxt(uint8_t nbrOfDec=4);
		/*EDU US*/	String		longitudeGoogleMapsTxt(uint8_t nbrOfDec=4);
		/*EDU US*/	float 		latitudeGoogleMapsNbr();
		/*EDU US*/	float 		longitudeGoogleMapsNbr();	
		/*EDU US*/	String		positionGoogleMaps();
	
		//== DATE
		/*EDU US*/	uint8_t		day();
		/*EDU US*/	uint8_t		jour();
		/*EDU US*/	uint8_t		month();
		/*EDU US*/	uint8_t		mois();
		/*EDU US*/	uint16_t	year(uint8_t format=4);
		/*EDU US*/	uint16_t	annee(uint8_t format=4);
	
		//== HEURE
		/*EDU FR*/	String 		heure(String pattern="h s");
		/*EDU US*/	String		time(String pattern="h s"); 		
		/*EDU FR*/	uint8_t		heures();	
		/*EDU US*/	uint8_t		hours();
		/*EDU US*/	uint8_t		minutes();
		/*EDU FR*/	uint8_t		secondes();
		/*EDU US*/	uint8_t		seconds();
		/*EDU FR*/	uint16_t	millisecondes();
		/*EDU US*/	uint16_t	milliseconds();
	
		//== ENVIRONNEMENTAL
		/*EDU US*/	uint16_t	vitesse();
		/*EDU US*/	uint16_t	speed();
	
		/*EDU US*/	uint16_t	angle();
	
		/*EDU US*/	char		latitudeDirection();			// N S
		/*EDU US*/	char  	 	longitudeDirection();			// E W
		
		/*EDU FR*/	float		altitude();
	
	//========== VEILLE
		/*EDU FR*/	boolean 	reveiller();
		/*EDU US*/	boolean		wakeup();
		
		/*EDU FR*/	boolean 	mettreEnVeille();
		/*EDU US*/	boolean		standby();
	
	//========== STATUT
		/*EDU FR*/	boolean 	estSynchronise();
		/*EDU US*/	boolean 	isSynchronized();
	
	//========== LECTURE BAS NIVEAU
		/*EDU FR*/	char		lireUnCaractere();
		/*EDU US*/	char 		readOneChar();
	
	

		
	//========== MODIFICATION D'OPTIONS
		/*EDU FR*/	void 		activerInterruption();
		/*EDU FR*/	void 		desactiverInterruption();
		/*EDU US*/	void 		useInterrupt(boolean v=true);
	
		/*EDU FR*/	void 		definirQuantiteInformation(int8_t level=1);
		/*EDU US*/	void		setVerboseLevel(int8_t level);					// 1=RMC 2=RMC+GGA 3=ALL 0=NONE
		#define EDUGPS_100MLHZ			1
		#define EDUGPS_200MLHZ			2
		#define EDUGPS_1HZ				10
		#define EDUGPS_5HZ				50
		#define EDUGPS_10HZ				100
		/*EDU FR*/	void		definirFrequenceTrames(int8_t freq=EDUGPS_1HZ);
		/*EDU US*/	void		setUpdateFrequency(int8_t freq=EDUGPS_1HZ);

		/*EDU FR*/	void		definirFrequencePortSerie(uint16_t baud);
		/*EDU US*/	void		setBaudRate(uint16_t baud=9600);
		
		/*EDU FR*/	void		definirDecalageHoraire(int8_t decalage);
		/*EDU US*/	void		setTimeDifference(int8_t decalage);	
		
	
		

	//========== METHODE INTERNES
		/*EDU US*/	void 		effectiveUpdate();

		/*EDU US*/	void		readAllInSerial();
		/*EDU US*/	String		floatToString(float val, uint8_t nbrOfDec=4, uint8_t widthNbr=10, uint8_t minWidth=4);

	
	//========== ATTRIBUTS
		SoftwareSerial 			*softwareSerial;
		HardwareSerial			*hardwareSerial;
		Adafruit_GPS 			*gps;
		uint32_t				baudRate;
		uint8_t					timeDifference;
		boolean					serialInfos;
	
	
  protected:
		boolean					m_usingInterrupt;	
		uint8_t					m_serialType;
		#define EDUGPS_SOFTWARESERIAL 1	
		#define EDUGPS_HARDWARESERIAL 2

	//extern int abc;

};
#endif
