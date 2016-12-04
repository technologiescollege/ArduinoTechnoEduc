/************************************************************************
 *  
 *  NAME:  MowayduinoRf.h
 *
 *  DESCRIPTION:  Header file for RF library.
 *
 *  DATE: 07/01/2013    Rev 1.0    Minirobots, S.L.
 *
 ************************************************************************/

#ifndef mowayduinorf_h
#define mowayduinorf_h

//*******************************************************
//    Libraries
//*******************************************************
#include "Arduino.h"

#define SIZE_RF 8

//*******************************************
//    Class definition
//*******************************************
class Rf
{
public:
  Rf();

  unsigned char beginRF(unsigned char chan, unsigned char add);
  unsigned char On();
  unsigned char Off();
  unsigned char Send(unsigned char RF_DIR_OUT, unsigned char RF_DATA_OUT[]);
  unsigned char Receive(unsigned char* RF_DIR_IN, unsigned char* RF_DATA_IN);	
  void InterruptsRF(byte ENABLE_DISABLE);	
   
private:
  unsigned char DATA_N_SND;
  unsigned char DATA_N_RCV;
  boolean flgInt; 
};

#endif


