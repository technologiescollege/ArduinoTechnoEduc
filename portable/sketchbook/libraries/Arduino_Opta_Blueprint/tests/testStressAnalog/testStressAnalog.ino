/* -------------------------------------------------------------------------- */
/* FILE NAME:   testStressAnalog.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240605
   DESCRIPTION: 
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

/* this is testDAC setting plus an additional expansion with RTD and Digital Input*/


#include "OptaBlue.h"

AnalogExpansion out_expansion;
AnalogExpansion in_expansion;

#define RTD_EXPANSION_INDEX 3



/* This test uses 2 opta analog to performs DAC and ADC test 
   from the HW point of view hw must be "paired" 
   so channel 0 of expansion 0 is wired with channel 0 of expansion 1,
   channel 1 of expansion 0 is wired with channel 0 of expansion 1 
   and so on

   at the beginning 
   - expansion 0 is DAC --> provides output
   - expansion 1 is ADC --> read the input

   after a while and then ciclycally roles are inverted 


   */


void setExpansionAsOutput_1(AnalogExpansion &a) {
   /* channels from 0 to 3 voltage DAC */
   a.beginChannelAsVoltageDac(0);
   a.beginChannelAsVoltageDac(1);
   a.beginChannelAsVoltageDac(2);
   a.beginChannelAsVoltageDac(3);
   /*channels from 4 to 7 are current DAC */
   a.beginChannelAsCurrentDac(4);
   a.beginChannelAsCurrentDac(5);
   a.beginChannelAsCurrentDac(6);
   a.beginChannelAsCurrentDac(7);
} 


void setExpansionAsInput_1(AnalogExpansion &a) {
   /* channels from 0 to 3 voltage ADC */
   a.beginChannelAsAdc(0, OA_VOLTAGE_ADC, true, false, false, 0);
   a.beginChannelAsAdc(1, OA_VOLTAGE_ADC, true, false, false, 0);
   a.beginChannelAsAdc(2, OA_VOLTAGE_ADC, true, false, false, 0);
   a.beginChannelAsAdc(3, OA_VOLTAGE_ADC, true, false, false, 0);
   /*channels from 4 to 7 are current ADC */
   a.beginChannelAsAdc(4, OA_CURRENT_ADC, false, false, false, 0);
   a.beginChannelAsAdc(5, OA_CURRENT_ADC, false, false, false, 0);
   a.beginChannelAsAdc(6, OA_CURRENT_ADC, false, false, false, 0);
   a.beginChannelAsAdc(7, OA_CURRENT_ADC, false, false, false, 0);
} 

void setExpansionAsOutput_2(AnalogExpansion &a) {
   /* channels from 4 to 7 voltage DAC */
   a.beginChannelAsVoltageDac(4);
   a.beginChannelAsVoltageDac(5);
   a.beginChannelAsVoltageDac(6);
   a.beginChannelAsVoltageDac(7);
   /*channels from 0 to 0 are current DAC */
   a.beginChannelAsCurrentDac(0);
   a.beginChannelAsCurrentDac(1);
   a.beginChannelAsCurrentDac(2);
   a.beginChannelAsCurrentDac(3);
} 

void setExpansionAsInput_2(AnalogExpansion &a) { //rejection active
   /* channels from 4 to 7 voltage ADC */
   a.beginChannelAsAdc(4, OA_VOLTAGE_ADC, true, true, false, 0);
   a.beginChannelAsAdc(5, OA_VOLTAGE_ADC, true, true, false, 0);
   a.beginChannelAsAdc(6, OA_VOLTAGE_ADC, true, true, false, 0);
   a.beginChannelAsAdc(7, OA_VOLTAGE_ADC, true, true, false, 0);
   /*channels from 0 to 3 are current ADC */
   a.beginChannelAsAdc(0, OA_CURRENT_ADC, false, true, false, 0);
   a.beginChannelAsAdc(1, OA_CURRENT_ADC, false, true, false, 0);
   a.beginChannelAsAdc(2, OA_CURRENT_ADC, false, true, false, 0);
   a.beginChannelAsAdc(3, OA_CURRENT_ADC, false, true, false, 0);
} 


void initForward_1() {
   /* expansion 0 out / expansion 1 is in with configuration 1 */
   out_expansion = OptaController.getExpansion(0);
   if(out_expansion) {
      setExpansionAsOutput_1(out_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion 0 not found... looping forever...");
      while(1) {}
   }

   
   in_expansion = OptaController.getExpansion(1);
   if(in_expansion) {
      setExpansionAsInput_1(in_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion 1 not found... looping forever...");
      while(1) {}
   }
}


void initForward_2() {
   /* expansion 0 out / expansion 1 is in with configuration 2 */
   out_expansion = OptaController.getExpansion(0);
   if(out_expansion) {
      setExpansionAsOutput_2(out_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion 0 not found... looping forever...");
      while(1) {}
   }

   
   in_expansion = OptaController.getExpansion(1);
   if(in_expansion) {
      setExpansionAsInput_2(in_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion 1 not found... looping forever...");
      while(1) {}
   }
}


void initReverse_1() {
   /* expansion 0 in / expansion 1 is out with configuration 1 */
   out_expansion = OptaController.getExpansion(1);
   if(out_expansion) {
      setExpansionAsOutput_1(out_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion 0 not found... looping forever...");
      while(1) {}
   }

   
   in_expansion = OptaController.getExpansion(0);
   if(in_expansion) {
      setExpansionAsInput_1(in_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion 1 not found... looping forever...");
      while(1) {}
   }
}


void initReverse_2() {
   /* expansion 0 input / expansion 1 is output with configuration 2 */
   out_expansion = OptaController.getExpansion(1);
   if(out_expansion) {
      setExpansionAsOutput_2(out_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion 0 not found... looping forever...");
      while(1) {}
   }

   
   in_expansion = OptaController.getExpansion(0);
   if(in_expansion) {
      setExpansionAsInput_2(in_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion 1 not found... looping forever...");
      while(1) {}
   }
}

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
   Serial.begin(115200);
   delay(5000);

   OptaController.begin();
   initReverse_1();

   AnalogExpansion rtd_in = OptaController.getExpansion(RTD_EXPANSION_INDEX);
   if(rtd_in) {
      rtd_in.beginChannelAsRtd(0, true, 1.2);
      rtd_in.beginChannelAsRtd(1, true, 1.2);
      rtd_in.beginChannelAsRtd(2, true, 1.2);
      rtd_in.beginChannelAsRtd(3, true, 1.2);
      /*channels from 0 to 3 are current ADC */
      rtd_in.beginChannelAsRtd(4, true, 1.2);
      rtd_in.beginChannelAsRtd(5, true, 1.2);
      rtd_in.beginChannelAsDigitalInput(6);
      rtd_in.beginChannelAsDigitalInput(7);
   }
   else {
      if(Serial) Serial.println("Analog Expansion for RTD test not found... looping forever...");
      while(1) {}
   }
   

   if(Serial) Serial.println("Exit setup");
     
   
}
   
#define MAX_TEST_VALUES 10

static const float voltages[MAX_TEST_VALUES] = {1.4, 9.7, 3.6, 8.8, 5.2, 2.3, 7.8,
9.2, 4.1, 8.6};
static const float currents[MAX_TEST_VALUES] = {2.3, 12.0, 8.5, 9.2, 7.8, 19.3,
1.9, 14.5, 9.3, 2.2};


int myrand(int _min, int _max) {
  return _min + ( std::rand() % ( _max - _min + 1 ) );
}

#define RANDOMFUNCTION myrand(0,6)

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
   static uint8_t configuration = 0;

   OptaController.update();
   
   /* TEST adc and dac ---- expansion 0 and 1 */
   
   if(in_expansion && out_expansion) {
      static uint8_t index = 0;
      
      for(int ch = 0; ch < 8; ch++ ) {

         if(out_expansion.isChCurrentDac(ch)){
            if(Serial) Serial.print("Exp " + String(out_expansion.getIndex()) + " SET out ch " + String(ch) + " to " +
            String(currents[index]) + " mA... ");
            out_expansion.pinCurrent((uint8_t)ch, currents[index]);
            unsigned int start = millis();

            float value = in_expansion.pinCurrent((uint8_t)ch);
            while(value > (currents[index] + 0.3) || value < (currents[index] - 0.3)) {
               value = in_expansion.pinCurrent((uint8_t)ch);  
            }
            if(Serial) Serial.println( " | Exp " +  String(in_expansion.getIndex()) + " read " + String(value) + "V -->" " OK! in " + String(millis()-start));

         }
         else if(out_expansion.isChVoltageDac(ch)) {
            if(Serial) Serial.print("Exp " + String(out_expansion.getIndex()) + " SET out ch " + String(ch) + " to " +
            String(voltages[index]) + " V... ");
            out_expansion.pinVoltage((uint8_t)ch, voltages[index]);
            unsigned int start = millis();

            float value = in_expansion.pinVoltage((uint8_t)ch);
            while(value > (voltages[index] + 0.3) || value < (voltages[index] - 0.3)) {
               value = in_expansion.pinVoltage((uint8_t)ch);  
            }

            if(Serial) Serial.println( " | Exp " +  String(in_expansion.getIndex()) + " read " + String(value) + "V -->" " OK! in " + String(millis()-start));
         }
      }
      index++;
      if(index >= MAX_TEST_VALUES) {
         index = 0;

         configuration++;
         if(configuration >= 4) {
            configuration = 0;
         }

         Serial.println("configuration = " + String(configuration));

         if(configuration == 0) {
            Serial.println("\n------ Configurazione FORWARD 1");
            initForward_1();
         }
         else if(configuration == 1) {
            Serial.println("\n------ Configurazione REVERSE 1");
            initReverse_1();
         }
         else if(configuration == 2) {
            Serial.println("\n------ Configurazione FORWARD 2");
            initForward_2();
         }
         else if(configuration == 3) {
            Serial.println("\n------ Configurazione REVERSE 2");
            initReverse_2();
         }
      }
      Serial.println("index = " + String(index));
   }
   else {
      Serial.println("Analog expansion at index 0 and 1 not found");
   }

   AnalogExpansion rtd_in = OptaController.getExpansion(RTD_EXPANSION_INDEX);
   if(rtd_in) {
   	for(int i = 0; i < 8; i++) {
   		if(rtd_in.isChRtd(i)) {
   			Serial.println("RTD expansion " + String(rtd_in.getIndex()) + " - channel " + String(i) + " reading value " + String(rtd_in.getRtd(i)) + " ohm");
   		}
   		else if(rtd_in.isChDigitalInput(i)) {
   			rtd_in.updateDigitalInputs();
   			if(rtd_in.digitalRead(i)==HIGH){
   				Serial.println("RTD expansion " + String(rtd_in.getIndex()) + " - channel " + String(i) + " reading value HIGH (DIGITAL)");
   			} else if(rtd_in.digitalRead(i)==LOW){
   				Serial.println("RTD expansion " + String(rtd_in.getIndex()) + " - channel " + String(i) + " reading value LOW (DIGITAL)");
   			}
   		}
   	}
   	delay(500);
	}
}
