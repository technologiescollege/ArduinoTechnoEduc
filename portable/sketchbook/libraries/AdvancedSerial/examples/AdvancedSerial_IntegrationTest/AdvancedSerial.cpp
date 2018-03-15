/*
 *        File: AdvancedSerial.c
 *      Author: Nick Dodds <Nick1787@gmail.com>
 * Description: An Adnvancer Serial Interface for Interfacing with Arduino
 */
 
#include "AdvancedSerial.h"
#include "Arduino.h"

AdvancedSerial::~AdvancedSerial(){
  delete Signals;
}

void AdvancedSerial::addSignal(String Name, bool * value){
  Signals[signalCount].Name = Name;
  Signals[signalCount].Type = asi_bool;
  Signals[signalCount].addr = value;
  signalCount++;
}


void AdvancedSerial::addSignal(String Name, double * value){
  Signals[signalCount].Name = Name;
  Signals[signalCount].Type = asi_double;
  Signals[signalCount].addr = value;
  signalCount++;
}

void AdvancedSerial::addSignal(String Name, float * value){
  Signals[signalCount].Name = Name;
  Signals[signalCount].Type = asi_float;
  Signals[signalCount].addr = value;
  signalCount++;
}

void AdvancedSerial::addSignal(String Name, unsigned long * value){
  Signals[signalCount].Name = Name;
  Signals[signalCount].Type = asi_ulong;
  Signals[signalCount].addr = value;
  signalCount++;
}

void AdvancedSerial::addSignal(String Name, int * value){
  Signals[signalCount].Name = Name;
  Signals[signalCount].Type = asi_int;
  Signals[signalCount].addr = value;
  signalCount++;
}


void AdvancedSerial::exec(){
  this->Read();
  TransmitSignalData(123);
}

void AdvancedSerial::Read(){
  char buffer[_advancedserial_inbuffer];
  
  while ((SerialRef->available() > 0) && (ReadBuffPos<_advancedserial_inbuffer)){
    buffer[ReadBuffPos] = SerialRef->read();
    //Serial.print("Data!!!");
    if ((buffer[ReadBuffPos] == '\n')&&(buffer[ReadBuffPos-1] == '\r')){
      ProcessInMsg(buffer);
      ReadBuffPos=0;
      break;
    }
    ReadBuffPos = ReadBuffPos+1;
  }

  //Wrap around buffer
  if(ReadBuffPos == _advancedserial_inbuffer){
    ReadBuffPos = 0;
  }
}

void AdvancedSerial::ProcessInMsg(char * buffer){
   byte msg_key = buffer[5];
   unsigned int msg_id = (buffer[10] << 24) | (buffer[9] << 16) | (buffer[8] << 8) | (buffer[7]);
   switch(msg_key){
      case(0xA0):{
        Serial.println("Transmit Signals!!");
        this->TransmitSignalList(msg_id);
      }
      case(0xA1):{
        unsigned int Signaldid = (buffer[8] << 24) | (buffer[7] << 16) | (buffer[6] << 8) | (buffer[5]);
        this->TransmitSignalData(msg_id);
      }
   }
}

void AdvancedSerial::TransmitSignalList(unsigned int msg_id){
  SerialRef->write("#ASI:");
  byte msg_key = 0xB0;
  SerialRef->write(msg_key);
  SerialRef->write(":");
  SerialRef->write(msg_id);
  SerialRef->write(":");
  
  for(int i=0; i<signalCount; i++){
    intCvt.val = i;
    SerialRef->write(intCvt.bval,2);
    LoggedSignal sym = Signals[i];
    SerialRef->print(sym.Name);
    SerialRef->write('\0');
    
    switch(sym.Type){
      case(asi_bool):{
        SerialRef->write(0x0);
        break;
      }
      case(asi_byte):{
        SerialRef->write(0x1);
        break;
      }
      case(asi_short):{
        SerialRef->write(0x2);
        break;
      }
      case(asi_int):{
        SerialRef->write(0x3);
        break;
      }
      case(asi_uint):{
        SerialRef->write(0x4);
        break;
      }
      case(asi_long):{
        SerialRef->write(0x5);
        break;
      }
      case(asi_ulong):{
        SerialRef->write(0x6);
        break;
      }
      case(asi_float):{
        SerialRef->write(0x7);
        break;
      }
      case(asi_double):{
        SerialRef->write(0x8);
        break;
      }
    }
  }
  SerialRef->write("\r\n");
  SerialRef->flush();
}

void AdvancedSerial::TransmitSignalData(unsigned int msg_id){
  SerialRef->write("#ASI:");
  byte msg_key = 0xB1;
  SerialRef->write(msg_key);
  SerialRef->write(":");
  SerialRef->write(msg_id);
  SerialRef->write(":");
  
  for(int i=0; i<signalCount; i++){
    intCvt.val = i;
    SerialRef->write(intCvt.bval,2);
    LoggedSignal sym = Signals[i];
    switch(sym.Type){
      case(asi_bool):{
        boolCvt.val = *((bool*)sym.addr);
        SerialRef->write(boolCvt.bval,1);
      } break;
      case(asi_byte):{
        SerialRef->write(*((byte*)sym.addr));
      } break;
      case(asi_short):{
        shortCvt.val = *((short*)sym.addr);
        SerialRef->write(shortCvt.bval,2);
      } break;
      case(asi_int):{
        intCvt.val = *((int*)sym.addr);
        SerialRef->write(intCvt.bval,2);
      } break;
      case(asi_uint):{
        uintCvt.val = *((unsigned int*)sym.addr);
        SerialRef->write(uintCvt.bval,2);
      } break;
      case(asi_long):{
        lngCvt.val = *((long*)sym.addr);
        SerialRef->write(lngCvt.bval,4);
      } break;
      case(asi_ulong):{
        ulngCvt.val = *((unsigned long*)sym.addr);
        SerialRef->write(ulngCvt.bval,4);
      } break;
      case(asi_float):{
        fltCvt.val = *((float*)sym.addr);
        SerialRef->write(fltCvt.bval,4);
      } break;
      case(asi_double):{
        dblCvt.val = *((double*)sym.addr);
        //Serial.print("Double!");
        SerialRef->write(dblCvt.bval,8);
      } break;
    }
  }
  SerialRef->write("\r\n");
  SerialRef->flush();
}

