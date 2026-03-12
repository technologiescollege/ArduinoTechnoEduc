/* -------------------------------------------------------------------------- */
/* FILE NAME:   testChangeFunction.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240606
   DESCRIPTION: 
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */


#include "OptaBlue.h"

/* the test run on a_test that must be placed in EXPANSION_INDEX_TEST 
   a second expansion a_output EXPANSION_INDEX_OUTPUT is used to provide
   2 known source of current (channel 1 - 10.8mA and channel 0 - 8.7V

   Test changes randomly the channel function 
   by pressing s - stop this stop the change and allow to check if the 
   configuration are correct

   by pressing c - continue the change of the function continues */


#define EXPANSION_INDEX_TEST 4
#define EXPANSION_INDEX_OUTPUT 3

AnalogExpansion a_test;
AnalogExpansion a_output;



/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
   Serial.begin(115200);
   delay(2000);

   OptaController.begin();

   a_test = OptaController.getExpansion(EXPANSION_INDEX_OUTPUT);
   if(a_test) {
      a_test.beginChannelAsVoltageDac(0);
      a_test.beginChannelAsCurrentDac(1);

      a_test.pinCurrent(1,(float)10.8);
      a_test.pinVoltage(0,(float)8.7);
      
   } else {
      if(Serial) Serial.println("Analog Expansion 1 not found... looping forever...");
      while(1) {}
   }

  
   a_test = OptaController.getExpansion(EXPANSION_INDEX_TEST);
   if(!a_test) {
      if(Serial) Serial.println("Analog Expansion 1 not found... looping forever...");
      while(1) {}
   }

   if(Serial) Serial.println("Exit setup");
     
   
}   
   
int myrand(int _min, int _max) {
  return _min + ( std::rand() % ( _max - _min + 1 ) );
}

#define RANDOMFUNCTION myrand(0,6)

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
   OptaController.update();
   
   if(a_test) {
      
      static bool set_function = true;

      char s = 'h';
      if(Serial){
         if(Serial.available()) {
            s = Serial.read();
            Serial.print("char read ");
            Serial.println(s);
            while(Serial.available()) {
               Serial.read();
            }
         }
      }

      if(s == 's' || s == 'S') {
         set_function = false;
      }
      else if(s == 'c' || s == 'C') {

         set_function = true;
      }

      static uint8_t fun[8];
      static uint8_t channel = 0;
      if(set_function) {
         fun[channel] = RANDOMFUNCTION;
      }

      if(Serial)
         Serial.print("Exp 4 ch " + String(channel));
      


      // ALTA IMPEDENZA
      if(fun[channel] == 0) {
         if(set_function) {
            if(Serial)
               Serial.println(" set to HIGH IMPEDENCE");
            a_test.beginChannelAsHighImpedance(channel);
         }
         else {
            if(Serial)
               Serial.println(" IS set to HIGH IMPEDENCE");
            delay(500);   
         }  
      }
      // DAC VOLTAGE
      if(fun[channel] == 1) {
         if(set_function) {
            if(Serial)
               Serial.println(" set to DAC VOLTAGE");
            a_test.beginChannelAsVoltageDac(channel);
         }
         else {
            float v = (float)5.6;
            if(Serial)
               Serial.println(" IS set to DAC VOLTAGE to 5.6V");
            a_test.pinVoltage(channel,v);
            delay(500); 
         }  
      }
      // DAC CURRENT
      if(fun[channel] == 2) {
         if(set_function) {
            if(Serial)
               Serial.println(" set to DAC CURRENT");
            a_test.beginChannelAsCurrentDac(channel);
         }
         else {
            float v = (float)15.7;
            if(Serial)
               Serial.println(" IS set to DAC CURRENT to 15.7 mA");
            a_test.pinCurrent(channel,v);
            delay(500); 
         }  
      }

      // ADC VOLTAGE
      if(fun[channel] == 3) {
         if(set_function) {
            if(Serial)
               Serial.println(" set to ADC VOLTAGE");
            a_test.beginChannelAsVoltageAdc(channel);
         }
         else {
            float v = a_test.pinVoltage(channel);
            if(Serial)
               Serial.println(" IS set to ADC VOLTAGE to " + String(v) + "V");
            delay(500); 
         }  
      }

      // ADC CURRENT
      if(fun[channel] == 4) {
         if(set_function) {
            if(Serial)
               Serial.println(" set to DAC CURRENT");
            a_test.beginChannelAsCurrentAdc(channel);
         }
         else {
            float v = a_test.pinCurrent(channel);
            if(Serial)
               Serial.println(" IS set to ADC CURRENT to " + String(v) + "mA");
            delay(500); 
         }  
      }

      // RTD
      if(fun[channel] == 5) {
         if(set_function) {
            if(Serial)
               Serial.println(" set to RTD");
            a_test.beginChannelAsRtd(channel,true,1.2);
         }
         else {
            float v = a_test.getRtd(channel);
            if(Serial)
               Serial.println(" IS set to RTD to " + String(v) + "ohm");
            delay(500); 
         }  
      }
      // digital input
      if(fun[channel] == 6) {
         if(set_function) {
            if(Serial)
               Serial.println(" set to Digital Input");
            a_test.beginChannelAsDigitalInput(channel);
         }
         else {
            PinStatus v = a_test.digitalRead(channel,true);
            if(Serial)
               Serial.println(" IS set to DIGITAL INPUT to " + String(v));
            delay(500); 
         }  
      }


      channel++;
      if(channel >= 8) {
         Serial.println("\n-------------------------------------------");
         channel = 0;
      }

   }
   else {
      Serial.println("Analog expansion at index 3 not found");
   }

}