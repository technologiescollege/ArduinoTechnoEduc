/* -------------------------------------------------------------------------- */
/* FILE NAME:   getChannelFunction.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240920
   DESCRIPTION: This example shows and test the use of the "isChannel..." functions
                in the Analog Expansion, when used to request the actual set up to the expansion
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:       Channels are set up randomly at every iteration of the main
                loop and then the function to get if the function has been
                actally set up in the expansion is used                       */
/* -------------------------------------------------------------------------- */


#include "OptaBlue.h"
#include <cstdlib>
#define PERIODIC_UPDATE_TIME 5000
#define DELAY_AFTER_SETUP 200

/* -------------------------------------------------------------------------- */
void printExpansionType(ExpansionType_t t) {
/* -------------------------------------------------------------------------- */
  if(t == EXPANSION_NOT_VALID) {
    Serial.print("Unknown!");
  }
  else if(t == EXPANSION_OPTA_DIGITAL_MEC) {
    Serial.print("Opta --- DIGITAL [Mechanical]  ---");
  }
  else if(t == EXPANSION_OPTA_DIGITAL_STS) {
    Serial.print("Opta --- DIGITAL [Solid State] ---");
  }
  else if(t == EXPANSION_DIGITAL_INVALID) {
    Serial.print("Opta --- DIGITAL [!!Invalid!!] ---");
  }
  else if(t == EXPANSION_OPTA_ANALOG) {
    Serial.print("~~~ Opta  ANALOG ~~~");
  }
  else {
    Serial.print("Unknown!");
  }
}

/* -------------------------------------------------------------------------- */
void printExpansionInfo() {
/* -------------------------------------------------------------------------- */
  static long int start = millis();
  
  if(millis() - start > 5000) {
    start = millis();
    Serial.print("Number of expansions: ");
    Serial.println(OptaController.getExpansionNum());

    for(int i = 0; i < OptaController.getExpansionNum(); i++) {
      Serial.print("Expansion n. ");
      Serial.print(i);
      Serial.print(" type ");
      printExpansionType(OptaController.getExpansionType(i));
      Serial.print(" I2C address ");
      Serial.println(OptaController.getExpansionI2Caddress(i));
    }
  }  
}

  int8_t oa_index = -1;
/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  Serial.begin(115200);
  delay(2000);

  srand(millis());

  OptaController.begin();
}



/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  OptaController.update();

  for(int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {

    AnalogExpansion ae = OptaController.getExpansion(i);

    if(ae) {

      Serial.println("\n********* ANALOG EXPANSION n. " + String(i));

      for(int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
        
        /* RANDOMLY SELECT A function for the channel ch */
        int requested_function = (rand()%6)+1;
        
        /* each case handles a function (so it set channel ch to the random 
           function choose before and then ask if the function is actually 
           been set up by the expansion 

           the functions isCh... are called very quicly (as fast as
           possible, because this is a test sketch 
           consider to relax a bit introducing a small delay ... */


        switch (requested_function) {
        case 0: {
          bool exit = false;
          Serial.println("\n + set channel " + String(ch) + " to HIGH IMPEDENCE");
          ae.beginChannelAsHighImpedance(ch);
          int i = 0;
          for(i = 0; !exit; i++) {
            if(ae.isChHighImpedance(ch,true)){
              break;
            }
          }
          Serial.println("  -> set in " + String(i) + " cycle");
          }
          break;
        case 1: {
          bool exit = false;
          Serial.println("\n + set channel " + String(ch) + " to VOLTAGE ADC");
          ae.beginChannelAsVoltageAdc(ch);
          int i = 0;
          for(i = 0; !exit; i++) {
            if(ae.isChVoltageAdc(ch,true)){
              break;
            }
          }
          if(ae.isChAdc(ch,true) == false) {
            Serial.println("Something wrong whit isChAdc");
          }
          Serial.println("  -> set in " + String(i) + " cycle ");
          }
          break;
        case 2: {
          bool exit = false;
          Serial.println("\n + set channel " + String(ch) + " to CURRENT ADC");
          ae.beginChannelAsCurrentAdc(ch);
          int i = 0;
          for(i = 0; !exit; i++) {
            if(ae.isChCurrentAdc(ch,true)){
              break;
            }
          }
          if(ae.isChAdc(ch,true) == false) {
            Serial.println("Something wrong with isChAdc");
          }
          Serial.println("  -> set in " + String(i) + " cycle ");
          }
          break;
          
        case 3:{
          bool exit = false;
          Serial.println("\n + set channel " + String(ch) + " to VOLTAGE DAC");
          ae.beginChannelAsVoltageDac(ch);;
          int i = 0;
          for(i = 0; !exit; i++) {
            if(ae.isChVoltageDac(ch,true)){
              break;
            }
          }
          if(ae.isChDac(ch,true) == false) {
            Serial.println("Something wrong with isChDac");
          }
          Serial.println("  -> set in " + String(i) + " cycle ");
          }
          

          break;
        case 4: {  
          bool exit = false;
          Serial.println("\n + set channel " + String(ch) + " to CURRENT DAC");
          ae.beginChannelAsCurrentDac(ch);;
          int i = 0;
          for(i = 0; !exit; i++) {
            if(ae.isChCurrentDac(ch,true)){
              break;
            }
          }
          if(ae.isChDac(ch,true) == false) {
            Serial.println("Something wrong with isChDac");
          }
          Serial.println("  -> set in " + String(i) + " cycle ");
          }
          

          break;
        case 5:{
          bool exit = false;
          Serial.println("\n + set channel " + String(ch) + " to DIGITAL INPUT");
          ae.beginChannelAsDigitalInput(ch);
          int i = 0;
          for(i = 0; !exit; i++) {
            if(ae.isChDigitalInput(ch,true)){
              break;
            }
          }
          Serial.println("  -> set in " + String(i) + " cycle");
          }
          

          break;
        case 6:
          if(ch == 0 || ch == 1) {
            bool exit = false;
            Serial.println("\n + set channel " + String(ch) + " to RTD 3 WIRES");
            ae.beginChannelAsRtd(ch, true, 1.2);
            int i = 0;
            for(i = 0; !exit; i++) {
              if(ae.isChRtd3Wires(ch,true)){
                break;
              }
            }
            Serial.println("  -> set in " + String(i) + " cycle");
            /* 3 wire RTD is available only on channel 0 and 1*/
          }
          else {
            bool exit = false;
            Serial.println("\n + set channel " + String(ch) + " to RTD 3 WIRES");
            ae.beginChannelAsRtd(ch, true, 1.2);
            int i = 0;
            for(i = 0; !exit; i++) {
              if(ae.isChRtd2Wires(ch,true)){
                break;
              }
            }
            Serial.println("  -> set in " + String(i) + " cycle");
          }
          break;
        case 7:{
            bool exit = false;
            Serial.println("\n + set channel " + String(ch) + " to RTD 3 WIRES");
            ae.beginChannelAsRtd(ch, false, 1.2);
            int i = 0;
            for(i = 0; !exit; i++) {
              if(ae.isChRtd2Wires(ch,true)){
                break;
              }
            }
            Serial.println("  -> set in " + String(i) + " cycle");
          }

          break;
       
        }
      }
    }
  }
}
