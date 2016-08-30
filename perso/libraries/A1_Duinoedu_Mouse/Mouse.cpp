
#include "Mouse.h"

#include <avr/pgmspace.h>

// << CONSTRUCTOR >>
/*EDU US*/ Mouse::Mouse(){

}


/*EDU FR*/	void Mouse::brancher(int8_t pinData, int8_t pinClk){
		
		branch(pinData, pinClk);
}//*/
/*EDU US*/	void Mouse::branch(int8_t pinData, int8_t pinClk){
	
	//---- On configure les broches
		//--- DATA est toujours passée en argument
		m_DATA = pinData;
		//--- CLK peut être sous enttendue
		if(pinClk==0){
			//-- Configuration implicite
			m_CLK=m_DATA+1;
		}
		else{
			//-- Configuration explicite
			m_CLK=pinClk;
		}
		
	//---- Initialisation des variables
		m_s2[0] 	= 0;	// Vitesse nulle de la souris en mémoire
		m_t2[0] 	= -1;	// Date du dernier échantillon (-1 = ne peut pas être pris en compte)
				
		m_pos1000[0] 	= 500000;	// Souris par défaut au centre 0=X
		m_pos1000[1]	= 500000;	// Souris par défaut au centre 1=Y
		
		m_sensitivity = 500;	// Sensibilité (0 à 1000)
			
	//----- On lance une initialisation
		mouse_init();

}//*/

/*ARD US*/	void Mouse::mouse_init(){

		gohi(m_CLK);
		gohi(m_DATA);
		//Serial.print("Sending reset to mouse\n");
		mouse_write(0xff);
		mouse_read();  					// ack byte
		//  Serial.print("Read ack byte1\n");
		mouse_read();  					// blank
		mouse_read();  					// blank
		//  Serial.print("Sending remote mode code\n");
		mouse_write(0xf0);  			// remote mode
		mouse_read();  					//ack
		//  Serial.print("Read ack byte2\n");
		delayMicroseconds(100);

}//*/

/*ARD US*/	void Mouse::mouse_write(char data){
		char i;
		char parity = 1;

		//  Serial.print("Sending ");
		//  Serial.print(data, HEX);
		//  Serial.print(" to mouse\n");
		//  Serial.print("RTS");
	//---- put pins in output mode
		gohi(m_DATA);
		gohi(m_CLK);
		delayMicroseconds(300);
		golo(m_CLK);
		delayMicroseconds(300);
		golo(m_DATA);
		delayMicroseconds(10);
	//---- start bit
		gohi(m_CLK);
	//---- wait for mouse to take control of clock
		while (digitalRead(m_CLK) == HIGH)
			;
	//---- clock is low, and we are clear to send data
		for (i=0; i < 8; i++) {
			if (data & 0x01) {
				gohi(m_DATA);
			} 
			else {
				golo(m_DATA);
			}
		//--- wait for clock cycle
			while (digitalRead(m_CLK) == LOW)
				;
			while (digitalRead(m_CLK) == HIGH)
				;
			parity = parity ^ (data & 0x01);
			data = data >> 1;
		}  
	//---- parity
		if (parity) {
			gohi(m_DATA);
		} 
		else {
			golo(m_DATA);
		}
		while (digitalRead(m_CLK) == LOW)
			;
		while (digitalRead(m_CLK) == HIGH)
			;
	//---- stop bit
		gohi(m_DATA);
		delayMicroseconds(50);
		while (digitalRead(m_CLK) == HIGH)
			;
	//---- wait for mouse to switch modes
		while ((digitalRead(m_CLK) == LOW) || (digitalRead(m_DATA) == LOW))
			;
	//---- put a hold on the incoming data
		golo(m_CLK);
		//  Serial.print("done.\n");
}//*/


/*ARD US*/	char Mouse::mouse_read(void){
  
		char data = 0x00;
		int i;
		char bit = 0x01;

		//  Serial.print("reading byte from mouse\n");
	//---- start the clock
		gohi(m_CLK);
		gohi(m_DATA);
		delayMicroseconds(50);
		while (digitalRead(m_CLK) == HIGH)
			;
		delayMicroseconds(5);  				// not sure why
		while (digitalRead(m_CLK) == LOW) 	//eat start bit
			;
		for (i=0; i < 8; i++) {
			while (digitalRead(m_CLK) == HIGH)
				;
			if (digitalRead(m_DATA) == HIGH) {
				data = data | bit;
			}
			while (digitalRead(m_CLK) == LOW)
				;
			bit = bit << 1;
		}
	//---- eat parity bit, which we ignore
		while (digitalRead(m_CLK) == HIGH)
			;
		while (digitalRead(m_CLK) == LOW)
			;
	//---- eat stop bit
		while (digitalRead(m_CLK) == HIGH)
			;
		while (digitalRead(m_CLK) == LOW)
			;

	//---- put a hold on the incoming data
		golo(m_CLK);
		//  Serial.print("Recvd data ");
		//  Serial.print(data, HEX);
		//  Serial.print(" from mouse\n");
  
	return data;


}//*/


/*EDU FR*/	int Mouse::lireX(){return readX();}
/*EDU US*/	int Mouse::readX(){return read("x");}

/*EDU FR*/	int Mouse::lireY(){return readY();}
/*EDU US*/	int Mouse::readY(){return read("y");}

/*EDU FR*/	int Mouse::lire(String quoi){return read(quoi);}
/*EDU US*/	int Mouse::read(String what){
	
		int8_t axe;
		int8_t mstat;
		int8_t m[2];
		int32_t theDate[2];
		
	//---- On détermine quel-est l'axe concerné (0=X / 1=Y)
		if(what=="x"){ axe=0;}
		if(what=="y"){ axe=1;}
		DEBUG2("AXE = ", axe);
		
		
	//---- Calcule de l'intégral entre t1 et t2
		//--- On décale les valeurs (effet mémoire)
		m_s1[axe] = m_s2[axe];			// L'ancinne valeur de vitesse devient la nouvelle
		//DEBUG2("Vitesse = ", m_s1[axe]);
		m_t1[axe] = m_t2[axe];			// L'ancienne date devient la nouvelle
		//DEBUG2("La date en t1 vaut maintenant : ", m_t1[axe]);
		
	//---- Enregistrement des vitesses instantanées (2ème point)
		//--- Programme initial de lecture (X&Y)
			effectiveRead(&mstat,m,theDate);
			
		//--- Enregistrement des valeurs de vitesse instantanée et date	
			//-- Pour le point X2 ou Y2
				m_s2[axe]=m[axe];					// vitesse instantanée en X
				m_t2[axe]=theDate[axe];				// date d'enregistrement
			
	//---- Calcul de intégral à partir des valeurs échantillonnées

		//--- Calcul des la surface du rectangle  
			int32_t width;							// Base du rectangle
			width=m_t2[axe]-m_t1[axe];				// durée en millisecondes entre 2 échantillons

			int32_t hRect;							// Hauteur du rectangle
			hRect=m_s1[axe];						// Vitesse instantanée au premier échantillon
			
			int32_t surfaceRect;
			surfaceRect=width*hRect;
		
		//--- Calcul de la surface du triangle
			int32_t hTri;
			hTri=m_s2[axe]-m_s1[axe];

			int32_t surfaceTri;
			surfaceTri=(width*hTri)/2;
		
		//--- Calcul de la surface totale X
			int32_t surface;
			surface=surfaceRect+surfaceTri;
			
	//---- Calcul de la nouvelle position X
		//--- Calcul position théorique sans bornes
			m_pos1000[axe]+=surface*m_sensitivity*((m_sensitivity+40)/40)/(width);
		//--- Mise en place de limites
			if(m_pos1000[axe]<0){m_pos1000[axe]=0;}
			if(m_pos1000[axe]>1000000){m_pos1000[axe]=1000000;}
			m_pos[axe]=m_pos1000[axe]/1000;


	return m_pos[axe];	

	
}















/////////////////////// PRIVATE

/*EDU US*/	void Mouse::effectiveRead(int8_t *mstat, int8_t m[], int32_t theDate[]){
	
		mouse_write(0xeb);    
		mouse_read();        
		*mstat = mouse_read();				// On ignore l'acquitement
		m[0] = mouse_read();				// Lecture vitesse en X
		theDate[0] = millis();
		m[1] = mouse_read();				// Lecture vitesse en Y
		theDate[1] = millis();

}//*/


/*ARD US*/	void Mouse::gohi(int8_t pin){

		pinMode(pin, INPUT);
		digitalWrite(pin, HIGH);

}//*/


/*ARD US*/	void Mouse::golo(int8_t pin){

		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);

}//*/
