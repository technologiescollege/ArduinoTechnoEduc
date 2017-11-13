/*
 * IRremote
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.htm http://arcfn.com
 * Edited by Mitra to add new controller SANYO
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 
 EDU : Librairie augmentée par David Souder (souder.d@gmail.com) pour www.duinoedu.com
 Version du 08/01/2016
 
 */

#ifndef IRremote2_h
#define IRremote2_h

// Ajout EDU :
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif
// Fin ajout

// The following are compile-time library options.
// If you change them, recompile the library.
// If DEBUG is defined, a lot of debugging output will be printed during decoding.
// TEST must be defined for the IRtest unittests to work.  It will make some
// methods virtual, which will be slightly slower, which is why it is optional.
// #define DEBUG
// #define TEST

// Results returned from the decoder

//----- <<CLASSE DECODE>>
class decode_results {
public:
  int decode_type; // NEC, SONY, RC5, UNKNOWN
  unsigned int panasonicAddress; // This is only used for decoding Panasonic data
  unsigned long value; // Decoded value
  int bits; // Number of bits in decoded value
  volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
  int rawlen; // Number of records in rawbuf.
};

// Values for decode_type
#define NEC 1
#define SONY 2
#define RC5 3
#define RC6 4
#define DISH 5
#define SHARP 6
#define PANASONIC 7
#define JVC 8
#define SANYO 9
#define MITSUBISHI 10
#define UNKNOWN -1

// Decoded value for NEC when a repeat code is received
#define REPEAT 0xffffffff


// main class for receiving IR
//----- <<CLASSE IRrecc>>
class IRrecv2{
public:
  IRrecv2(int recvpin);
  void blink13(int blinkflag);
  int decode(decode_results *results);
  void enableIRIn();
  void resume();
  
  //==== GESTION TELECOMMANDES (NEC)
  /*EDU FR*/ void brancher();
  /*EDU US*/ void branch();
  
  /*EDU FR*/ unsigned long lireCodeIr(int option=200);						
  /*EDU US*/ unsigned long codeIrReadLong(int option=200);
  
  /*EDU FR*/ bool testerTouche(String touche, int option=200);
  /*EDU FR*/ bool testTouch(String touch, int option=200);
  
  //=== GESTION ECHANGES DONNEES ENTRE 2 CARTES ARDUINO
  /*EDU US*/ int32_t receiveNumber(int16_t _memoryDelay);
				
#define SERIALPLUS_MEMORISER 				-1
#define IRREMOTE_MEMORISER_TOUCHE			-1
#define IRREMOTE_MEMORISER      			-1
#define DUINOEDU_MEMORISER_TOUCHE			-1
#define DUINOEDU_MEMORISER					-1

#define SERIALPLUS_MEMORISER_200_MS 		200
#define IRREMOTE_MEMORISER_200_MS 		    200
#define DUINOEDU_MEMORISER_200_MS 		    200

#define DUINOEDU_MEMORISER_500_MS 			500
#define DUINOEDU_MEMORISER_1000_MS 			1000

#define SERIALPLUS_MEMORISER_2000_MS 		2000
#define IRREMOTE_MEMORISER_2000_MS 		    2000
#define DUINOEDU_MEMORISER_2000_MS 			2000

#define DUINOEDU_MEMORISER_5000_MS 			5000
#define DUINOEDU_MEMORISER_10000_MS 		10000

#define SERIALPLUS_NE_PAS_MEMORISER 		0
#define IRREMOTE_NE_PAS_MEMORISER 		    0
#define DUINOEDU_NE_PAS_MEMORISER 			0

protected:
  // These are called by decode
  int getRClevel(decode_results *results, int *offset, int *used, int t1);
  long decodeNEC(decode_results *results);
  long decodeSony(decode_results *results);
  long decodeSanyo(decode_results *results);
  long decodeMitsubishi(decode_results *results);
  long decodeRC5(decode_results *results);
  long decodeRC6(decode_results *results);
  long decodePanasonic(decode_results *results);
  long decodeJVC(decode_results *results);
  long decodeHash(decode_results *results);
  int compare(unsigned int oldval, unsigned int newval);
  /*EDU US*/ decode_results serialImpuls;
  /*EDU US*/ unsigned long m_lastCode;
  /*EDU US*/ unsigned long m_dateCode;          // Date à laquelle le dernier code à été lu
  /*EDU US*/ unsigned long m_lifeCode;			// Durée de vie du code
  /*EDU US*/ bool m_longPush;					// True si appui long en cours
  /*EDU US*/ unsigned long m_dateLongPush;		// Date à laquelle la dernier appui long a été demandé
  /*EDU US*/ unsigned long m_lastDateLongPush;  // Date à laquelle l'avant dernier appui long à été enregistré
  /*EDU US*/ unsigned long m_lifeLongPush;		// Durée de vie minimum d'un appui long (évite les rebondissements)

		
  
};

// Only used for testing; can remove virtual for shorter code
#ifdef TEST
#define VIRTUAL virtual
#else
#define VIRTUAL
#endif


//----- <<CLASSE IRsend2>> 
class IRsend2{
public:
  IRsend2() {}
  
  //=== GESTION ECHANGES DONNEES ENTRE 2 CARTES ARDUINO
  /*EDU US*/ void sendNumber(int32_t _number, uint16_t _interval);
  
  
  void sendNEC(unsigned long data, int nbits);
  void sendSony(unsigned long data, int nbits);
  // Neither Sanyo nor Mitsubishi send is implemented yet
  //  void sendSanyo(unsigned long data, int nbits);
  //  void sendMitsubishi(unsigned long data, int nbits);
  void sendRaw(unsigned int buf[], int len, int hz);
  void sendRC5(unsigned long data, int nbits);
  void sendRC6(unsigned long data, int nbits);
  void sendDISH(unsigned long data, int nbits);
  void sendSharp(unsigned long data, int nbits);
  void sendPanasonic(unsigned int address, unsigned long data);
  void sendJVC(unsigned long data, int nbits, int repeat); // *Note instead of sending the REPEAT constant if you want the JVC repeat signal sent, send the original code value and change the repeat argument from 0 to 1. JVC protocol repeats by skipping the header NOT by sending a separate code value like NEC does.
  // private:
  void enableIROut(int khz);
  VIRTUAL void mark(int usec);
  VIRTUAL void space(int usec);
};

// Some useful constants

#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 100 // Length of raw duration buffer

// Marks tend to be 100us too long, and spaces 100us too short
// when received due to sensor lag.
#define MARK_EXCESS 100

#endif



/* MEMO : ENVOI VERS UNE CARTE ARDUINO
#include <IRremote2.h>
IRsend2 irsend;
void setup(){
}
void loop(){
    irsend.sendNumber(1023, 1000);		// Valeur:1023, delay:1000ms
      //irsend.sendSony(1023, 20); 
      //delay(1000); // Indispensable   
}
*/


/* MEMO : RECEPTION D'UNE CARTE ARDUINO
#include <IRremote2.h>
IRrecv2 irrecv(4);
void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn(); 
}
void loop(){
  Serial.println(irrecv.receiveNumber(2000));
}
*/