
#include "AnalogicAccelerometer.h"
#include <avr/pgmspace.h>

// << CONSTRUCTOR >>
/*EDU US*/ AnalogicAccelerometer::AnalogicAccelerometer(){
		
		m_pinX=0;		// A0 par défaut
		m_pinY=-1;		// Y non utilisé par défaut
		
		m_ofsetX=0;		// Valeur brute : 200 > 300 initialement (on ramène au plus proche de 500)
		
		
		correctionEnX=0;
		correctionEnY=0;
		
		buteeHauteX=330;
		buteeBasseX=200;
		
		buteeHauteY=330;
		buteeBasseY=200;
}

/*EDU FR*/	void AnalogicAccelerometer::brancher(int pinX, int pinY){branch(pinX, pinY);}
/*EDU US*/ 	void AnalogicAccelerometer::branch(int pinX, int pinY){
		
		m_pinX=pinX;
		pinMode(pinX, INPUT);
		
		if(pinY!=-1){
			m_pinY=pinY;
			pinMode(pinY, INPUT);
		}

}


/*EDU US*/	int AnalogicAccelerometer::lire(char axis){return read(axis);}
/*EDU US*/	int AnalogicAccelerometer::read(char axis){
	float temp=0;
	if(axis=='X'){
		temp=analogRead(m_pinX);							// Valeur brute (216 > 316)
		temp=map(temp,buteeBasseX,buteeHauteX,0,1000);
		temp+=correctionEnX;								// Décalage utilisateur
	}	
	if(axis=='Y'){
		temp=analogRead(m_pinY);							// Valeur brute (216 > 316)
		temp=map(temp,buteeBasseY,buteeHauteY,0,1000);
		temp+=correctionEnY;								// Décalage utilisateur
	}
		
		
		
		
		return temp;
}



