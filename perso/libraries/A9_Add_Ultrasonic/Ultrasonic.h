/*
	Cette librairie est la compilation de 2 libraries existentes portant le même nom:
	  -ultrasonic.h Grove     : 1 librarie SeeedGrove de 2011 (mesure avec une seule pin)
	  -ultrasonic.h générique : 1 librarie Arduino.cc (mesure avec 2 pins : trig & echo)
	
	Cette librairie apporte les compléments suivants : 
	  -un seul fichier pour 2 librairies (Grove et générique)
	  -mesure à partir de plusieurs échantillons (1 à 100)
	  -optimisation pour produits Grove
	
  	EDU : Modifié par David Souder (souder.d@gmail.com) pour www.duinoedu.com
    
	Version : du 03/01/215 - OpenSource
*/

#ifndef Ultrasonic_H
#define Ultrasonic_H

#define prendreUneMesure  MeasureInCentimeters
#define distance          RangeInCentimeters
#define duree             duration

class Ultrasonic
{
	public:
		//<<CONSTRUCTOR>>
		Ultrasonic(int pin, int pinTrig=0);

		/*EDU US*/ long RangeInMillimeters;
		
		/*ARD US*/ long RangeInCentimeters;								// Résultat en cm
		/*ARD US*/ long RangeInInches;									// Résultat en pouces
		/*ARD US*/ long duration;                                       // Retard de l'écho
	
		/*EDU FR*/ long mesurerEnCm(int nbrOfEch=1);    							// Mesure en cm avec 1 éch.
		/*ARD US*/ void MeasureInCentimeters(int nbrOfEch=1);				    	// IDEM
		
		/*EDU FR*/ long mesurer(int nbrOfEch=1);					// Mesure en cm avec x éch. (x:0>100)
		/*EDU US*/ long MultiMeasureInCentimeters(int nbrOfEch=1);	// IDEM
		
		/*EDU FR*/ long mesurerEnMm(void);								// Mesure en mm avec 1 éch.					
        /*EDU US*/ long MeasureInMillimeters(void);						// IDEM
						
		/*ARD US*/ void MeasureInInches(void);
	
	protected:
		int _pin;														// PIN1 : Si Grove ou générique
		int _pinTrig;    												// PIN2 : Si Générique=SR04 (Trig+Echo)
		#define ULTRASONIC_NBR_OF_ECH_MAX 100							// Nbr d'échantillons max autorisés pour 1 mesure
		int _nbrOfEch;												// Nbr d'échantillons courant
		int _ech[ULTRASONIC_NBR_OF_ECH_MAX];							// Récupération des échantillons
};
#endif
