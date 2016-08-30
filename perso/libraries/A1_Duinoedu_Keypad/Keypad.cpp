/*
||
|| @file Keypad.cpp
|| @version 3.1
|| @author Mark Stanley, Alexander Brevig
|| @contact mstanley@technologist.com, alexanderbrevig@gmail.com
||
|| @description
|| | This library provides a simple interface for using matrix
|| | keypads. It supports multiple keypresses while maintaining
|| | backwards compatibility with the old single key library.
|| | It also supports user selectable pins and definable keymaps.
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/
#include <Keypad.h>



// <<constructor>> Default constructor 
/*EDU US*/ Keypad::Keypad(void){
		

}
// <<constructor>> Allows custom keymap, pin configuration, and keypad sizes.
/*ARD US*/ 	Keypad::Keypad(char *userKeymap, byte *row, byte *col, byte numRows, byte numCols) {
	/*Dugug*/ Serial.println("Construction normale");
	rowPins = row;
	columnPins = col;
	sizeKpd.rows = numRows;
	sizeKpd.columns = numCols;

	begin(userKeymap);

	setDebounceTime(10);
	setHoldTime(500);
	keypadEventListener = 0;

	startTime = 0;
	single_key = false;
}


//---- With 0 pin : default configuration (pin2, pin3... pin9) 
/*EDU FR*/ 	void Keypad::brancher(){

		branch();
}
/*EDU US*/ 	void Keypad::branch(){
	
	//-- Pins determination
		
	//--Effective configuration
		effectiveBranch(2,3,4,5,6,7,8,9);
}


//---- With 2 pins : starting pin and pin of arrival  
/*EDU FR*/ 	void Keypad::brancher(int startPin, int endPin){

		branch(startPin,endPin);
}
/*EDU US*/ 	void Keypad::branch(int startPin, int endPin){
	
	//-- Pins determination
		// /*Debug*/ Serial.println("branch2");
		byte nbrOfPins = endPin-startPin;
		byte maxOfPins = MAX_NBR_COLS+MAX_NBR_ROWS;
		byte pins[maxOfPins];
	
	    // Effective pins calculation
		if(nbrOfPins<=maxOfPins){
			for(int i=0;i<=nbrOfPins;i++){
				pins[i]=startPin+i;
				// /*Debug*/ Serial.println("boucle");
				byte a=pins[i];                   // Pin calculated
				// /*Debug*/ Serial.println(a);
			}
		}
	
	//--Effective configuration
		effectiveBranch(pins[0],pins[1],pins[2],pins[3],pins[4],pins[5],pins[6],pins[7]);
}


//---- With 8 pins
/*EDU FR*/ 	void Keypad::brancher(char col0, char col1, char col2, char col3, char row0, char row1, char row2, char row3){
		
		branch(col0, col1, col2, col3, row0, row1, row2, row3);
}
/*EDU FR*/ 	void Keypad::branch(char col0, char col1, char col2, char col3, char row0, char row1, char row2, char row3){
		
		effectiveBranch(col0, col1, col2, col3, row0, row1, row2, row3);
}


/*EDU US*/ 	void Keypad::effectiveBranch(char col0, char col1, char col2, char col3, char row0, char row1, char row2, char row3){
	
	// Default keypad configuration
		m_defaultNbrRows = DEFAULT_NBR_ROWS;
		m_defaultNbrCols = DEFAULT_NBR_COLS;
		m_defaultKeysCode[0][0]='1';m_defaultKeysCode[0][1]='2';m_defaultKeysCode[0][2]='3';m_defaultKeysCode[0][3]='A';	
		m_defaultKeysCode[1][0]='4';m_defaultKeysCode[1][1]='5';m_defaultKeysCode[1][2]='6';m_defaultKeysCode[1][3]='B';
		m_defaultKeysCode[2][0]='7';m_defaultKeysCode[2][1]='8';m_defaultKeysCode[2][2]='9';m_defaultKeysCode[2][3]='C';
		m_defaultKeysCode[3][0]='*';m_defaultKeysCode[3][1]='0';m_defaultKeysCode[3][2]='#';m_defaultKeysCode[3][3]='D';
		m_defaultRowPins[0]=col0;
		m_defaultRowPins[1]=col1;
		m_defaultRowPins[2]=col2;
		m_defaultRowPins[3]=col3;
		m_defaultColPins[0]=row0;
		m_defaultColPins[1]=row1;
		m_defaultColPins[2]=row2;
		m_defaultColPins[3]=row3;

	// Effective construction
		rowPins      		= m_defaultRowPins;
		columnPins 		= m_defaultColPins;
		sizeKpd.rows 	= DEFAULT_NBR_ROWS;
		sizeKpd.columns = DEFAULT_NBR_COLS;
		begin(makeKeymap(m_defaultKeysCode));
		setDebounceTime(10);
		setHoldTime(500);
		keypadEventListener = 0;
		startTime = 0;
		single_key = false;
					
	// Default
		m_passwordActive		=false;					// Pas de gestion de mot de passe par défaut
		m_serialDisplaying		=true;					
		m_bufferUpdate			=false;
		m_serialInfos			=true;					// Affichage dans le port série par défaut
		m_t1						=0;						// Date à 0 par défaut
		m_tempo					=3000;					// 1 seconde de tempo par défaut
}


// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad::begin(char *userKeymap) {
    // /*Debug*/ Serial.println("Begin");
	keymap = userKeymap;
}


char Keypad::getKey() {
	single_key = true;

	if (getKeys() && key[0].stateChanged && (key[0].kstate==PRESSED))
		return key[0].kchar;
	
	single_key = false;
	

	return NO_KEY;
}

// Populate the key list.
bool Keypad::getKeys() {
	bool keyActivity = false;

	// Limit how often the keypad is scanned. This makes the loop() run 10 times as fast.
	if ( (millis()-startTime)>debounceTime ) {
		scanKeys();
		keyActivity = updateList();
		startTime = millis();
	}

	return keyActivity;
}

// Private : Hardware scan
void Keypad::scanKeys() {
	// Re-intialize the row pins. Allows sharing these pins with other hardware.
	for (byte r=0; r<sizeKpd.rows; r++) {
		pin_mode(rowPins[r],INPUT_PULLUP);
	}

	// bitMap stores ALL the keys that are being pressed.
	for (byte c=0; c<sizeKpd.columns; c++) {
		pin_mode(columnPins[c],OUTPUT);
		pin_write(columnPins[c], LOW);	// Begin column pulse output.
		for (byte r=0; r<sizeKpd.rows; r++) {
			bitWrite(bitMap[r], c, !pin_read(rowPins[r]));  // keypress is active low so invert to high.
		}
		// Set pin to high impedance input. Effectively ends column pulse.
		pin_write(columnPins[c],HIGH);
		pin_mode(columnPins[c],INPUT);
	}
}

// Manage the list without rearranging the keys. Returns true if any keys on the list changed state.
bool Keypad::updateList() {

	bool anyActivity = false;

	// Delete any IDLE keys
	for (byte i=0; i<LIST_MAX; i++) {
		if (key[i].kstate==IDLE) {
			key[i].kchar = NO_KEY;
			key[i].kcode = -1;
			key[i].stateChanged = false;
		}
	}

	// Add new keys to empty slots in the key list.
	for (byte r=0; r<sizeKpd.rows; r++) {
		for (byte c=0; c<sizeKpd.columns; c++) {
			boolean button = bitRead(bitMap[r],c);
			char keyChar = keymap[r * sizeKpd.columns + c];
			int keyCode = r * sizeKpd.columns + c;
			int idx = findInList (keyCode);
			// Key is already on the list so set its next state.
			if (idx > -1)	{
				nextKeyState(idx, button);
			}
			// Key is NOT on the list so add it.
			if ((idx == -1) && button) {
				for (byte i=0; i<LIST_MAX; i++) {
					if (key[i].kchar==NO_KEY) {		// Find an empty slot or don't add key to list.
						key[i].kchar = keyChar;
						key[i].kcode = keyCode;
						key[i].kstate = IDLE;		// Keys NOT on the list have an initial state of IDLE.
						nextKeyState (i, button);
						break;	// Don't fill all the empty slots with the same key.
					}
				}
			}
		}
	}

	// Report if the user changed the state of any key.
	for (byte i=0; i<LIST_MAX; i++) {
		if (key[i].stateChanged) anyActivity = true;
	}

	return anyActivity;
}

// Private
// This function is a state machine but is also used for debouncing the keys.
void Keypad::nextKeyState(byte idx, boolean button) {
	key[idx].stateChanged = false;

	switch (key[idx].kstate) {
		case IDLE:
			if (button==CLOSED) {
				transitionTo (idx, PRESSED);
				holdTimer = millis(); }		// Get ready for next HOLD state.
			break;
		case PRESSED:
			if ((millis()-holdTimer)>holdTime)	// Waiting for a key HOLD...
				transitionTo (idx, HOLD);
			else if (button==OPEN)				// or for a key to be RELEASED.
				transitionTo (idx, RELEASED);
			break;
		case HOLD:
			if (button==OPEN)
				transitionTo (idx, RELEASED);
			break;
		case RELEASED:
			transitionTo (idx, IDLE);
			break;
	}
}

// New in 2.1
bool Keypad::isPressed(char keyChar) {
	for (byte i=0; i<LIST_MAX; i++) {
		if ( key[i].kchar == keyChar ) {
			if ( (key[i].kstate == PRESSED) && key[i].stateChanged )
				return true;
		}
	}
	return false;	// Not pressed.
}

// Search by character for a key in the list of active keys.
// Returns -1 if not found or the index into the list of active keys.
int Keypad::findInList (char keyChar) {
	for (byte i=0; i<LIST_MAX; i++) {
		if (key[i].kchar == keyChar) {
			return i;
		}
	}
	return -1;
}

// Search by code for a key in the list of active keys.
// Returns -1 if not found or the index into the list of active keys.
int Keypad::findInList (int keyCode) {
	for (byte i=0; i<LIST_MAX; i++) {
		if (key[i].kcode == keyCode) {
			return i;
		}
	}
	return -1;
}

// New in 2.0
char Keypad::waitForKey() {
	char waitKey = NO_KEY;
	while( (waitKey = getKey()) == NO_KEY );	// Block everything while waiting for a keypress.
	return waitKey;
}

// Backwards compatibility function.
KeyState Keypad::getState() {
	return key[0].kstate;
}

// The end user can test for any changes in state before deciding
// if any variables, etc. needs to be updated in their code.
bool Keypad::keyStateChanged() {
	return key[0].stateChanged;
}

// The number of keys on the key list, key[LIST_MAX], equals the number
// of bytes in the key list divided by the number of bytes in a Key object.
byte Keypad::numKeys() {
	return sizeof(key)/sizeof(Key);
}

// Minimum debounceTime is 1 mS. Any lower *will* slow down the loop().
void Keypad::setDebounceTime(uint debounce) {
	debounce<1 ? debounceTime=1 : debounceTime=debounce;
}

void Keypad::setHoldTime(uint hold) {
    holdTime = hold;
}

void Keypad::addEventListener(void (*listener)(char)){
	keypadEventListener = listener;
}

void Keypad::transitionTo(byte idx, KeyState nextState) {
	key[idx].kstate = nextState;
	key[idx].stateChanged = true;

	// Sketch used the getKey() function.
	// Calls keypadEventListener only when the first key in slot 0 changes state.
	if (single_key)  {
	  	if ( (keypadEventListener!=NULL) && (idx==0) )  {
			keypadEventListener(key[0].kchar);
		}
	}
	// Sketch used the getKeys() function.
	// Calls keypadEventListener on any key that changes state.
	else {
	  	if (keypadEventListener!=NULL)  {
			keypadEventListener(key[idx].kchar);
		}
	}
}


/*EDU FR*/ void Keypad::ecrireMotDePasse(String motDePasse){
		
		writePassword(motDePasse);
}
/*EDU US*/ void Keypad::writePassword(String password){
	
	//--Flag password active
		m_passwordActive=true;
	
	//--On mémorise le mot de passe
		m_password=password;
		if(m_serialInfos==true){Serial.print("PASSE\t");Serial.println(m_password);}
			
	//--Création d'une chaine au moins aussi longue que le mot de passe
		m_passLength=m_password.length();
		// /*debug*/ Serial.println("Taille du mot de passe:");
		// /*debug*/ Serial.println((int)m_passLength);
		for(int i=0;i<m_passLength;i++){
			m_keysMemory+="-";
		}
		if(m_serialInfos==true){Serial.print("CODE\t");Serial.println(m_keysMemory);}
	
	//--Curent password is not ok (default)
		m_curentPasswordOk=false;
	
}


/////////////////////////////////////////////////// PASWORD
/*EDU US*/ String Keypad::lire(void){
		
		return read();
}
/*EDU US*/ String Keypad::read(void){

	//-- No password = 1 key
		if(m_password==false){
			newKey = getKey();
			if (newKey){
				keyMemory=newKey;
				if(m_serialInfos==true){Serial.println(keyMemory);}
			}
		return (String)keyMemory;
		}
		
		
	//-- Pasword is actived
		if(m_passwordActive==true){
		//--On stoke le dernier caractère dans le buffer
			newKey = getKey();
			if (newKey){
			
				//--Ajout d'un caractère à la chaine
				m_keysMemory+=newKey;
				if(m_serialInfos){Serial.print("TOUCHE\t");Serial.println(newKey);}
				// /*debug*/ Serial.println("Etat du buffer avant traitement");
				// /*debug*/ Serial.println(m_keysMemory);
			
				//--Ajustement de la taille du buffer
				String temp = m_keysMemory.substring(1);
				m_keysMemory=temp;
				// /*debug*/ Serial.println("Etat du buffer apres traitement");
				if(m_serialInfos){Serial.print("SAISIE\t");Serial.println(m_keysMemory);}
								
				//--Update buffer
				m_bufferUpdate=true;
							
			}
		return m_keysMemory;
		}
	
	return "ERROR";
}

/*EDU US*/ boolean Keypad::testerMotDePasseOk(void){

		return testPasswordOk();

}
/*EDU US*/ boolean Keypad::testPasswordOk(void){
	//-- Reading 
		read(); 
	//-- Test changing
	if(m_bufferUpdate==true){
		//--Comparaison du buffer et du mot de passe
			if(m_keysMemory==m_password){				
				//- On regarde si le mot de pase vient juste d'être bon
					if(m_curentPasswordOk==false){
						// On marque que le mot de passe vient juste d'être détecté Ok
							m_curentPasswordOk=true;
						// On enregistre la date à laquelle le mot de passe vient d'être détecté Ok
							m_t1=millis();
						// On affiche les infos sur le port série
							if(m_serialInfos==true){Serial.println("CODE\tOK");}
					}

			}
			else{ 
				//- On marque que le mot de passe est faux
					m_curentPasswordOk=false;
				//- On affiche sur le port série
				if(m_serialInfos==true){Serial.println("CODE\tMAUVAIS\n");}
			}
		//---- Le test est terminé
			m_bufferUpdate=false;
	}
		//---- On regarde si le mot de passe n'est pas trop vieux
					if(m_curentPasswordOk==true){
						int32_t agePass=millis()-m_t1;
						if(agePass>m_tempo){
							// On RAZ le mot de passe
								clearCodes();
						}
					}
	return m_curentPasswordOk;
}

/////////////////////////////////////////////////// SERIALINFOS
/*EDU US*/ void Keypad::activerInfos(void){

		setSerialInfosOn();
		Serial.begin(9600);
}
/*EDU US*/ void Keypad::setSerialInfosOn(void){

		m_serialInfos=true;
		// /*Debug*/ Serial.println("serial infos active");
}

/*EDU US*/ void Keypad::desactiverInfos(void){

		setSerialInfosOff();
}
/*EDU US*/ void Keypad::setSerialInfosOff(void){

		m_serialInfos=false;
		// /*Debug*/ Serial.println("serial infos désactive");
}

/*EDU US*/	void Keypad::effacerCodes(void){clearCodes();}
/*EDU US*/	void Keypad::clearCodes(void){
	//---- On RAZ le mot de passe	
		m_keysMemory="";
	//---- On ajoute les tirets
		for(int8_t i=0;i<m_passLength;i++){
		
		m_keysMemory+='-';
		}
		
	//---- On affiche les infos séries
		if(m_serialInfos==true){Serial.println("\nCODE\tPERIME");}
		
		if(m_serialInfos==true){Serial.print("PASSE\t");Serial.println(m_keysMemory);Serial.print("\n");}
	//---- On marque que le mot de passe est devenu faux 
		m_curentPasswordOk=false;

	//---- On marque qu'il y a une mise à jour du buffer
		m_bufferUpdate==true;
}

/*EDU US*/ void Keypad::ecrireTempo(int32_t tempo){

		setTempo(tempo);
}
/*EDU US*/ void Keypad::setTempo(int32_t tempo){
		
		m_tempo=tempo;
		if(m_serialInfos==true){Serial.print("TEMPO\t");Serial.print(m_tempo);Serial.println(" ms\n");}
}









































/*
|| @changelog
|| | 3.2 2014-06-13 - David Souder     : Added EDU concept. No change of functionality
|| | 3.1 2013-01-15 - Mark Stanley     : Fixed missing RELEASED & IDLE status when using a single key.
|| | 3.0 2012-07-12 - Mark Stanley     : Made library multi-keypress by default. (Backwards compatible)
|| | 3.0 2012-07-12 - Mark Stanley     : Modified pin functions to support Keypad_I2C
|| | 3.0 2012-07-12 - Stanley & Young  : Removed static variables. Fix for multiple keypad objects.
|| | 3.0 2012-07-12 - Mark Stanley     : Fixed bug that caused shorted pins when pressing multiple keys.
|| | 2.0 2011-12-29 - Mark Stanley     : Added waitForKey().
|| | 2.0 2011-12-23 - Mark Stanley     : Added the public function keyStateChanged().
|| | 2.0 2011-12-23 - Mark Stanley     : Added the private function scanKeys().
|| | 2.0 2011-12-23 - Mark Stanley     : Moved the Finite State Machine into the function getKeyState().
|| | 2.0 2011-12-23 - Mark Stanley     : Removed the member variable lastUdate. Not needed after rewrite.
|| | 1.8 2011-11-21 - Mark Stanley     : Added decision logic to compile WProgram.h or Arduino.h
|| | 1.8 2009-07-08 - Alexander Brevig : No longer uses arrays
|| | 1.7 2009-06-18 - Alexander Brevig : Every time a state changes the keypadEventListener will trigger, if set.
|| | 1.7 2009-06-18 - Alexander Brevig : Added setDebounceTime. setHoldTime specifies the amount of
|| |                                          microseconds before a HOLD state triggers
|| | 1.7 2009-06-18 - Alexander Brevig : Added transitionTo
|| | 1.6 2009-06-15 - Alexander Brevig : Added getState() and state variable
|| | 1.5 2009-05-19 - Alexander Brevig : Added setHoldTime()
|| | 1.4 2009-05-15 - Alexander Brevig : Added addEventListener
|| | 1.3 2009-05-12 - Alexander Brevig : Added lastUdate, in order to do simple debouncing
|| | 1.2 2009-05-09 - Alexander Brevig : Changed getKey()
|| | 1.1 2009-04-28 - Alexander Brevig : Modified API, and made variables private
|| | 1.0 2007-XX-XX - Mark Stanley : Initial Release
|| #
*/
