
#include "Guino.h"

#include <avr/pgmspace.h>

//////////////////// PUBLIC

// << CONSTRUCTOR >>
/*EDU US*/ Guino::Guino(){
	
	guidino_initialized = false;
	eepromKey = 1234;
	// Find a way to dynamically allocate memory
	guino_maxGUIItems = 100;
	guino_item_counter = 0;
	//guino_item_values[100]; 
	gTmpInt = 0; // temporary int for items without a variable
	internalInit = true; // boolean to initialize before connecting to serial
	//ET = new EasyTransfer();
	//int test=AA;

}

// Initialisation des graphs
// This is where you setup your interface 
void Guino::gInit(){
	/*
  gAddLabel("GRAPHS",1);
  gAddSpacer(1);
  gAddMovingGraph("ENTREE A0",0,1023, &graphA0, 10);
  gAddSpacer(1);
  gAddMovingGraph("ENTREE A1",0,1023, &graphA1, 10);
  gAddSpacer(1);
  gAddMovingGraph("ENTREE A2",0,1023, &graphA2, 10);
  gAddSpacer(1);
  gAddMovingGraph("ENTREE A3",0,1023, &graphA3, 10);
  */
}

//Lecture des boutons
// Method called everytime a button has been pressed in the interface.
void Guino::gButtonPressed(int id){
 
}

void Guino::gItemUpdated(int id){

}


//This function will write a 2 byte integer to the eeprom at the specified address and address + 1
void Guino::EEPROMWriteInt(int p_address, int p_value){
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}




//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int Guino::EEPROMReadInt(int p_address){
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}


void Guino::update(){

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

void Guino::gInitEEprom(){
  if(EEPROMReadInt(0) != eepromKey)
  {
    EEPROMWriteInt(0, eepromKey);
    for (int i =1; i <guino_maxGUIItems;i++)
    {
      EEPROMWriteInt(i*2+2, -3276);
    }
  }

}

void Guino::gSetColor(int _red, int _green, int _blue){
  gSendCommand(guino_setColor, 0, _red);
  gSendCommand(guino_setColor, 1, _green);
  gSendCommand(guino_setColor, 2, _blue);
}

void Guino::gGetSavedValue(int item_number, int *_variable){

  if(EEPROMReadInt(0) == eepromKey && internalInit)
  {

    int tmpVar =  EEPROMReadInt((item_number)*2+2);
    if(tmpVar != -3276)
      *_variable = tmpVar;
  }

}




void Guino::gBegin(int _eepromKey){

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

int Guino::gAddButton(char * _name){
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

void Guino::gAddColumn(){

  gSendCommand(guino_addColumn,0,0);

}




int Guino::gAddLabel(char * _name, int _size){
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

int Guino::gAddSpacer(int _size){
  if(guino_maxGUIItems > guino_item_counter)
  {
    gSendCommand(guino_addSpacer,(byte)guino_item_counter,_size);

    guino_item_counter++;
    return guino_item_counter-1;
  }
  return -1;

}   


int Guino::gAddToggle(char * _name, int * _variable){
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

int Guino::gAddFixedGraph(char * _name,int _min,int _max,int _bufferSize, int * _variable, int _size){
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

int Guino::gAddMovingGraph(char * _name,int _min,int _max, int * _variable, int _size){
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


int Guino::gUpdateLabel(int _item, char * _text){

  gSendCommand(guino_clearLabel,(byte)_item,0);
  for (int i = 0; i < strlen(_text); i++){
    gSendCommand(guino_addChar,(byte)_item,(int)_text[i]);
  }



}



int Guino::gAddRotarySlider(int _min,int _max, char * _name, int * _variable){
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

int Guino::gAddSlider(int _min,int _max, char * _name, int * _variable){
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

void Guino::gUpdateValue(int _item){

  gSendCommand(guino_setValue,_item, *guino_item_values[_item]); 
}


void Guino::gUpdateValue(int * _variable){

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



void Guino::gSendCommand(byte _cmd, byte _item, int _value){
  if(!internalInit && (guidino_initialized || guino_executed || _cmd == guino_iamhere)  )
  {
    guino_data.cmd = _cmd;
    guino_data.item = _item;
    guino_data.value = _value;
    ET.sendData();
  }

}















