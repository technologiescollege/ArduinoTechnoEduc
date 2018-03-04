/*
	Reprise du projet GUINO DASHBOARD disponible sur cette page :
	http://www.instructables.com/id/Guino-Dashboard-for-your-Arduino/
	
	Les contributeurs initiaux sont :
		Programming and idea by: Mads Hobye
		Easytransfer library by: Bill Porter
		GUI library by:  Reza Ali
	
	Modifications apportées :
		Descriptif XML Ardublock : Karl Thomas - techno94@gmail.com
		Librarie : David Souder - souder.d@gmail.com
		
	Librairie modifiée par David Souder - 08/07/2014 (voir version en bas de section)
	Cette librairie est librement utilisable
	www.duinoedu.com
	
	Version 0.8 - du 12/12/2014
*/


#ifndef GUINO_H
#define GUINO_H

//#include <SoftwareSerial.h>
#include <EasyTransfer.h>
#include <EEPROM.h>
#include "EDU_debug.h"
#include <EEPROM.h>

//----- Nécessaire si utilisation de String
	#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
	#else
	#include <WProgram.h>
	#include "Wire.h"
	#endif

//----- Langage EDU
#define faire void
#define initialisation setup()
#define boucle loop()
#define portSerie Serial.begin(9600)
#define lectureEtat digitalRead
#define utiliser 
#define chaine String
#define nombre int
#define grandNombre long
#define si if
#define sinon else


//////////////////////////////////////////////// Propre à Guino

//----- Includes propres à Guino
#define guino_executed -1
#define guino_init 0
#define guino_addSlider 1
#define guino_addButton 2
#define guino_iamhere 3
#define guino_addToggle 4
#define guino_addRotarySlider 5
#define guino_saveToBoard 6
#define guino_setFixedGraphBuffer 8
#define guino_clearLabel 7
#define guino_addWaveform 9
#define guino_addColumn 10
#define guino_addSpacer 11
#define guino_addMovingGraph 13
#define guino_buttonPressed 14
#define guino_addChar 15
#define guino_setMin 16
#define guino_setMax 17
#define guino_setValue 20
#define guino_addLabel 12
#define guino_large 0
#define guino_medium 1
#define guino_small 2
#define guino_setColor  21

#define GUINO_BRANCHER() gBegin(34236)


//---- Utilisées dans loop()
#define GUINO_GERER_INTERFACE() guino_update() 	// Inconditionnel 
#define GUINO_LIRE(val) gUpdateValue(&val);				// Met à jour les graphiques

//---- Utilisées pour définir l'apparence de l'interface
	//-- La fonction de description de l'apparence elle-même
	#define GUINO_DEFINIR_INTERFACE() gInit()
	//-- Les fonctions utilisables dans la précédente fonction
		//- Les outils disponibles
			// Ajouter un graphique
			#define GUINO_AFFICHER_GRAPH(titre, adr, min, max) gAddMovingGraph(titre,min, max, &adr,10)  	
			// Ajouter un potentiomètre virtuel
			#define GUINO_AFFICHER_POTENTIOMETRE(titre, adr, min, max) gAddSlider(min,max,titre, &adr)
			// Ajouter un interrupteur vituel (toogle)
			#define GUINO_AFFICHER_INTERRUPTEUR(titre, adr) gAddToggle(titre, &adr)
			// Ajouter une pause à l'endroit du bouton
			#define GUINO_AFFICHER_PAUSE(adr) gAddPause(&adr)
			#define GUINO_GERER_PAUSE(adr) gUpdatePause(&adr)
			
		//- Gestion d'apparence
		#define GUINO_AFFICHER_TITRE(arg1) gAddLabel(arg1, 1)
		#define GUINO_AJOUTER_COLONNE()   gAddColumn()
		#define GUINO_AFFICHER_LIGNE() gAddSpacer(1)
		




static boolean guidino_initialized = false;



	//---- Fonctions opératives
	
		static void 					EEPROMWriteInt(int p_address, int p_value);
		static unsigned int 			EEPROMReadInt(int p_address);
		static void 					guino_update();
		static void 					gInitEEprom();
		static void 					gSetColor(int _red, int _green, int _blue);
		static void 					gGetSavedValue(int item_number, int *_variable);
		static void 					gBegin(int _eepromKey);
		static int 						gAddButton(char * _name);
		static void 					gAddColumn();
		static int 						gAddLabel(char * _name, int _size);
		static int 						gAddSpacer(int _size);
		static int 						gAddToggle(char * _name, int * _variable, int min=0, int max=0);
		static int 						gAddFixedGraph(char * _name,int _min,int _max,int _bufferSize, int * _variable, int _size);
		/*EDU US*/ 	static int			gAddPause(int * _variable);
		
		/*EDU US*/	static int 	gAddMovingGraphEDU(char * _name,int _min,int _max, int _valeur, int _size);
		static int 						gAddMovingGraph(char * _name,int _min,int _max, int * _variable, int _size);
		static int 						gUpdateLabel(int _item, char * _text);
		static int 						gAddRotarySlider(int _min,int _max, char * _name, int * _variable);
		static int 						gAddSlider(int _min,int _max, char * _name, int * _variable);
		
		
		static void 					gUpdateValue(int _item);
		/*EDU US*/	static void gUpdateValueEDU(int _valeur);
		/*ARD US*/	static void gUpdateValue(int * _variable);
		static void 					gSendCommand(byte _cmd, byte _item, int _value);
		//---
		void 								gInit();
		static void 					gButtonPressed(int id);
		static void 					gItemUpdated(int id);



 struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  char cmd;
  char item;
  int value;
};

static void EEPROMWriteInt(int p_address, int p_value){
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
static unsigned int EEPROMReadInt(int p_address){
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

//create object
static EasyTransfer ET; 

/*
struct SEND_DATA_STRUCTURE
{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  char cmd;
  char item;
  int value;
};
*/

// Find a way to dynamically allocate memory
static int guino_maxGUIItems = 100;
static int guino_item_counter = 0;
static int *guino_item_values[100]; 
static int gTmpInt = 0; // temporary int for items without a variable
static boolean internalInit = true; // boolean to initialize before connecting to serial

// COMMAND STRUCTURE

//give a name to the group of data
static SEND_DATA_STRUCTURE guino_data;
static int eepromKey = 1234;
static void guino_update(){

  while(Serial.available())
  {

    if(ET.receiveData())
    {
      switch (guino_data.cmd) 
      {
      case guino_init:

        guino_item_counter = 0;
        guidino_initialized = true;
        gInit();
        break;
      case guino_setValue:
        *guino_item_values[guino_data.item] = guino_data.value;
        guino_data.cmd = guino_executed;
        gItemUpdated(guino_data.item);
        break;
      case guino_buttonPressed:
        gButtonPressed(guino_data.item);
        break;
      case guino_saveToBoard:
        {

          gInitEEprom();
          for (int i =0; i < guino_item_counter;i++)
          {
            EEPROMWriteInt(i*2+2, *guino_item_values[i]);
          }
        }
        break;
      }
    }
  }
}

static void gInitEEprom()
{
  if(EEPROMReadInt(0) != eepromKey)
  {
    EEPROMWriteInt(0, eepromKey);
    for (int i =1; i <guino_maxGUIItems;i++)
    {
      EEPROMWriteInt(i*2+2, -3276);
    }
  }

}

static void gSetColor(int _red, int _green, int _blue)
{
  gSendCommand(guino_setColor, 0, _red);
  gSendCommand(guino_setColor, 1, _green);
  gSendCommand(guino_setColor, 2, _blue);
}

static void gGetSavedValue(int item_number, int *_variable)
{

  if(EEPROMReadInt(0) == eepromKey && internalInit)
  {

    int tmpVar =  EEPROMReadInt((item_number)*2+2);
    if(tmpVar != -3276)
      *_variable = tmpVar;
  }

}

static void gBegin(int _eepromKey){

  // Sets all pointers to a temporary value just to make sure no random memory pointers.
  for(int i = 0; i < guino_maxGUIItems; i++)
  {
    guino_item_values[i] = &gTmpInt;
  }
  eepromKey = _eepromKey;

  gInit(); // this one needs to run twice only way to work without serial connection.
  internalInit = false;
  Serial.begin(115200);
  ET.begin(details(guino_data), &Serial);
  gSendCommand(guino_executed, 0, 0);
  gSendCommand(guino_executed, 0, 0);
  gSendCommand(guino_executed, 0, 0);
  gSendCommand(guino_iamhere, 0, 0); 

}



static int gAddButton(char * _name)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    gSendCommand(guino_addButton,(byte)guino_item_counter,0);
    for (int i = 0; i < strlen(_name); i++){
      gSendCommand(guino_addChar,(byte)guino_item_counter,(int)_name[i]);
    }
    guino_item_counter++;
    return guino_item_counter-1;
  }
  return -1;
}


static void gAddColumn()
{

  gSendCommand(guino_addColumn,0,0);

}


static int gAddLabel(char * _name, int _size)
{
  if(guino_maxGUIItems > guino_item_counter)
  { 
    gSendCommand(guino_addLabel,(byte)guino_item_counter,_size);

    for (int i = 0; i < strlen(_name); i++){
      gSendCommand(guino_addChar,(byte)guino_item_counter,(int)_name[i]);
    }

    guino_item_counter++;

    return guino_item_counter-1;
  }
  return -1;


} 
static int gAddSpacer(int _size)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    gSendCommand(guino_addSpacer,(byte)guino_item_counter,_size);

    guino_item_counter++;
    return guino_item_counter-1;
  }
  return -1;

}   


static int gAddToggle(char * _name, int * _variable, int min, int max)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    guino_item_values[guino_item_counter] =_variable ;
    gGetSavedValue(guino_item_counter, _variable);
    gSendCommand(guino_addToggle,(byte)guino_item_counter,*_variable);

    for (int i = 0; i < strlen(_name); i++){
      gSendCommand(guino_addChar,(byte)guino_item_counter,(int)_name[i]);
    }

    guino_item_counter++;

    return guino_item_counter-1;


  }
  return -1;
}   

/*EDU US*/ 	static int			gUpdatePause(int * _variable){
	while(*_variable==1){
		GUINO_GERER_INTERFACE();
	}
}

/*EDU US*/ 	static int			gAddPause(int * _variable){

	gAddToggle("PAUSE", _variable);

}




static int gAddFixedGraph(char * _name,int _min,int _max,int _bufferSize, int * _variable, int _size)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    gAddLabel(_name,guino_small);
    guino_item_values[guino_item_counter] =_variable ;
    gGetSavedValue(guino_item_counter, _variable);
    gSendCommand(guino_addWaveform,(byte)guino_item_counter,_size);
    gSendCommand(guino_setMax,(byte)guino_item_counter,_max);
    gSendCommand(guino_setMin,(byte)guino_item_counter,_min);
    gSendCommand(guino_setFixedGraphBuffer,(byte)guino_item_counter,_bufferSize);


    guino_item_counter++;

    return guino_item_counter-1;
  }
  return -1;
}


/*EDU US*/	static int gAddMovingGraphEDU(char * _name,int _min,int _max, int _valeur, int _size){
	//---On sauvegarde la valeur
		int valeur = _valeur;
	//---On récupère l'adresse de cette variable
		int* p_valeur = &_valeur;
	//---On appelle la fonction de dessin
		gAddMovingGraph(_name, _min, _max, p_valeur, _size);
}
/*ARD US*/	static int gAddMovingGraph(char * _name,int _min,int _max, int * _variable, int _size){
  if(guino_maxGUIItems > guino_item_counter)
  {
    gAddLabel(_name,guino_small);
    guino_item_values[guino_item_counter] =_variable ;
    gGetSavedValue(guino_item_counter, _variable);
    gSendCommand(guino_addMovingGraph,(byte)guino_item_counter,_size);
    gSendCommand(guino_setMax,(byte)guino_item_counter,_max);
    gSendCommand(guino_setMin,(byte)guino_item_counter,_min);


    guino_item_counter++;

    return guino_item_counter-1;
  }
  return -1;


}   


static int gUpdateLabel(int _item, char * _text)
{

  gSendCommand(guino_clearLabel,(byte)_item,0);
  for (int i = 0; i < strlen(_text); i++){
    gSendCommand(guino_addChar,(byte)_item,(int)_text[i]);
  }



}



static int gAddRotarySlider(int _min,int _max, char * _name, int * _variable)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    guino_item_values[guino_item_counter] =_variable ;
    gGetSavedValue(guino_item_counter, _variable);
    gSendCommand(guino_addRotarySlider,(byte)guino_item_counter,*_variable);
    gSendCommand(guino_setMax,(byte)guino_item_counter,_max);
    gSendCommand(guino_setMin,(byte)guino_item_counter,_min);
    for (int i = 0; i < strlen(_name); i++){
      gSendCommand(guino_addChar,(byte)guino_item_counter,(int)_name[i]);
    }

    guino_item_counter++;
    gUpdateValue(_variable);
    return guino_item_counter-1;
  }
  return -1;

}

static int gAddSlider(int _min,int _max, char * _name, int * _variable)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    guino_item_values[guino_item_counter] =_variable ;
    gGetSavedValue(guino_item_counter, _variable);
    gSendCommand(guino_addSlider,(byte)guino_item_counter,*_variable);
    gSendCommand(guino_setMax,(byte)guino_item_counter,_max);
    gSendCommand(guino_setMin,(byte)guino_item_counter,_min);
    for (int i = 0; i < strlen(_name); i++){
      gSendCommand(guino_addChar,(byte)guino_item_counter,(int)_name[i]);
    }

    guino_item_counter++;
    gUpdateValue(_variable);
    return guino_item_counter-1;
  }
  return -1;

}


static void gUpdateValue(int _item)
{

  gSendCommand(guino_setValue,_item, *guino_item_values[_item]); 
}

/*EDU US*/	static void gUpdateValueEDU(int _valeur){
}
static void gUpdateValue(int * _variable){

  int current_id = -1;
  for(int i = 0; i < guino_item_counter; i++)
  {

    if(guino_item_values[i] == _variable)
    {

      current_id = i;
      gUpdateValue(current_id);
    }
  }
  // if(current_id != -1)

}



static void gSendCommand(byte _cmd, byte _item, int _value)
{
  if(!internalInit && (guidino_initialized || guino_executed || _cmd == guino_iamhere)  )
  {
    guino_data.cmd = _cmd;
    guino_data.item = _item;
    guino_data.value = _value;
    ET.sendData();
  }

}


/*
static void gInit()
{
  /*
  gAddLabel("SLIDERS",1);
  gAddSpacer(1); 

  
  gAddSlider(3,127,"AMPLITUDE", &amplitude);


  gAddColumn();

  gAddLabel("GRAPHS",1);
  gAddSpacer(1);

  // Last parameter in moving graph defines the size 10 = normal
  gAddMovingGraph("SINUS",0,255, &graphValue, 10);


  gAddMovingGraph("POTMETER",0,1023, &potMeter, 10);
  
  gAddSlider(0,1023,"POTMETER",&potMeter);
  
}
*/


// Method called everytime a button has been pressed in the interface.
static void gButtonPressed(int id)
{
 
}

static void gItemUpdated(int id)
{

}

/*
static void gInit(){
}
*/

//////////////////////////////////////////////////////////////////////// Plus tard
//----- Début classe
class Guino {

public:

		/*EDU US*/	Guino();

	//---- Initialisation
		void gInit();
		void gButtonPressed(int id);
		void gItemUpdated(int id);

		
	//---- Fonctions opératives
		void EEPROMWriteInt(int p_address, int p_value);
		unsigned int EEPROMReadInt(int p_address);
		void update();
		void gInitEEprom();
		void gSetColor(int _red, int _green, int _blue);
		void gGetSavedValue(int item_number, int *_variable);
		void gBegin(int _eepromKey);
		int gAddButton(char * _name);
		void gAddColumn();
		int gAddLabel(char * _name, int _size);
		int gAddSpacer(int _size);
		int gAddToggle(char * _name, int * _variable);
		int gAddFixedGraph(char * _name,int _min,int _max,int _bufferSize, int * _variable, int _size);
		int gAddMovingGraph(char * _name,int _min,int _max, int * _variable, int _size);
		int gUpdateLabel(int _item, char * _text);
		int gAddRotarySlider(int _min,int _max, char * _name, int * _variable);
		int gAddSlider(int _min,int _max, char * _name, int * _variable);
		void gUpdateValue(int _item);
		void gUpdateValue(int * _variable);
		void gSendCommand(byte _cmd, byte _item, int _value);

		
		
	//---- Modification options

		
		
	//---- Variables publiques
			
	//---- Propriétés 

		

private:
		
	//---- Méthodes

	

	//---- Objets internes
		//give a name to the group of data
		SEND_DATA_STRUCTURE guino_data;
		EasyTransfer ET; 
	
	//---- Propriétés EDU
	
	//---- Propriéts Guino
		boolean guidino_initialized;
		int eepromKey;
		int guino_maxGUIItems;
		int guino_item_counter;
		int *guino_item_values[100]; 
		int gTmpInt; // temporary int for items without a variable
		boolean internalInit; // boolean to initialize before connecting to serial
		int test;
		
	//---- Gestion de la mémoire 


};


#endif

