/* -------------------------------------------------------------------------- */
/* FILE NAME:   genericAnalog.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240308
   DESCRIPTION: This sketch uses an Opta Controller to configure
	              an Opta Analog with different functions for each channel
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:       This sketch only uses the first OA found                      */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"

#define PERIODIC_UPDATE_TIME_ANALOG 2000
 
#define DAC_VALUE_NUM 10

#define INC_DAC_INDEX(x) (x = ((++x) % DAC_VALUE_NUM))

typedef enum {
  OA_HIGH_IMPEDENCE,
  OA_DIGITAL_INPUT,
  OA_DAC_CURRENT,
  OA_DAC_VOLTAGE,
  OA_ADC_CURRENT,
  OA_ADC_VOLTAGE,
  OA_RTD_3_WIRES,
  OA_RTD_2_WIRES,
  OA_PWM_FUNC
} OaChFunction_t;

/* current are in mA */
float currents[DAC_VALUE_NUM] = { 10.2, 4.7, 8.3, 9.4, 1.2, 1.36, 1.54, 4.75, 8.65, 9.4};
/* voltages are in V */
float voltages[DAC_VALUE_NUM] = { 3.5, 2.8, 7.6, 0.58, 4.86, 9.85, 10.48, 3.9, 6.7, 4.1};

/* change the configuration of the device by changing this array */
OaChFunction_t device1_cfg[8] = { OA_ADC_VOLTAGE, OA_RTD_3_WIRES, OA_ADC_CURRENT, OA_DIGITAL_INPUT, OA_RTD_2_WIRES, OA_DIGITAL_INPUT, OA_DAC_VOLTAGE, OA_ADC_CURRENT};

void setupAnalogExpansion(AnalogExpansion &aexp, OaChFunction_t cfg[8]) {
   for(int i = 0; i < 8; i++) {
      /* ####################################################### */
      /*                      ADC VOLTAGE                        */
      /* ####################################################### */ 

      if(cfg[i] == OA_ADC_VOLTAGE){
         Serial.println("SETUP channel " + String(i) + " as VOLTAGE ADC");
         aexp.beginChannelAsAdc(i,               //channel index
                                OA_VOLTAGE_ADC,  //ADC type
                                true,            //No Pull Down
                                false,           //No rejection
                                false,           //No diagnosis
                                0);              //No average
      }
      /* ####################################################### */
      /*                      ADC CURRENT                        */
      /* ####################################################### */ 
      else if(cfg[i] == OA_ADC_CURRENT){
         Serial.println("SETUP channel " + String(i) + " as CURRENT ADC");
         aexp.beginChannelAsAdc(i,               //channel index
                                OA_CURRENT_ADC,  //ADC type
                                false,            //Pull Down
                                false,           //No rejection
                                false,           //No diagnosis
                                0);              //No average
      }
      /* ####################################################### */
      /*                      RTD 3 WIRES                        */
      /* ####################################################### */ 
      else if(cfg[i] == OA_RTD_3_WIRES){
         Serial.println("SETUP channel " + String(i) + " as RTD 3 WIRE");
         aexp.beginChannelAsRtd(i,            //channel index
                                true,         //use 3 wires
                                1.0);         //1mA current
      }
      /* ####################################################### */
      /*                      RTD 2 WIRES                        */
      /* ####################################################### */ 
      else if(cfg[i] == OA_RTD_2_WIRES){
         Serial.println("SETUP channel " + String(i) + " as RTD 2 WIRE");
         aexp.beginChannelAsRtd(i,               //channel index
                                 false,           //use 2 wires
                                 1.0);            //1mA current (ignored in case of 2 wires)
      }
      /* ####################################################### */
      /*                      DAC VOLTAGE                        */
      /* ####################################################### */
      else if(cfg[i] == OA_DAC_VOLTAGE){
         Serial.println("SETUP channel " + String(i) + " as VOLTAGE DAC");
         aexp.beginChannelAsDac(i,               //channel index
                                OA_VOLTAGE_DAC,  //DAC type
                                true,            //limit current
                                false,           //No slew rate
                                OA_SLEW_RATE_0); //Slew rate setting.
      } 
      /* ####################################################### */
      /*                      DAC CURRENT                        */
      /* ####################################################### */
      else if(cfg[i] == OA_DAC_CURRENT){
         Serial.println("SETUP channel " + String(i) + " as CURRENT DAC");
         aexp.beginChannelAsDac( i,               //channel index
                                 OA_CURRENT_DAC,  //DAC type
                                 true,            //limit current
                                 false,           //No slew rate
                                 OA_SLEW_RATE_0); //Slew rate setting
      }
      /* ####################################################### */
      /*                      DIGITAL INPUT                      */
      /* ####################################################### */ 
      else if(cfg[i] == OA_DIGITAL_INPUT) {
         Serial.println("SETUP channel " + String(i) + " as DIGITAL INPUT");
         aexp.beginChannelAsDigitalInput(i);      //DI with default settings
      } 
   }
}

void updateAnalogExpansion(OaChFunction_t cfg[8]) {
   static int dac_v = -1;
   static int dac_c = -1;
   Serial.println("\n*** OPTA ANALOG expansion ");
   
   AnalogExpansion aexp = OptaController.getExpansion(0);
   
   if(!aexp) {
     return;
   }


   for(int i = 0; i < 8; i++) {
      /* ####################################################### */
      /*                      ADC VOLTAGE                        */
      /* ####################################################### */ 

      if(cfg[i] == OA_ADC_VOLTAGE){
         float va = aexp.pinVoltage(i);
         Serial.print("   - ADC channel ");
         Serial.print(i);
         Serial.println(" value: " + String(va) + " Volts");
            
      }
      /* ####################################################### */
      /*                      ADC CURRENT                        */
      /* ####################################################### */ 
      else if(cfg[i] == OA_ADC_CURRENT){
      
         float va = aexp.pinCurrent(i);
         Serial.print("   - ADC channel ");
         Serial.print(i);
         Serial.println(" value: " + String(va) + " milli Amperes");
                    
      }
      /* ####################################################### */
      /*                      RTD 3 WIRES                        */
      /* ####################################################### */ 
      else if(cfg[i] == OA_RTD_3_WIRES){
         float value =  aexp.getRtd((uint8_t)i);
         Serial.print("   - RTD channel ");
         Serial.print(i); 
         Serial.println(" value: " + String(value) + " Ohms [3 Wires]");
         
      }
      /* ####################################################### */
      /*                      RTD 2 WIRES                        */
      /* ####################################################### */ 
      else if(cfg[i] == OA_RTD_2_WIRES){
         float value =  aexp.getRtd((uint8_t)i);
         Serial.print("   - RTD channel ");
         Serial.print(i); 
         Serial.println(" value: " + String(value) + " Ohms [2 Wires]");
        
      }
      /* ####################################################### */
      /*                      DAC VOLTAGE                        */
      /* ####################################################### */
      else if(cfg[i] == OA_DAC_VOLTAGE){
         INC_DAC_INDEX(dac_v);
         aexp.pinVoltage(i,voltages[dac_v]);
         Serial.print("   - DAC channel ");
         Serial.print(i); 
         Serial.println(" value: " + String(voltages[dac_v]) + " Volts");
      } 
      /* ####################################################### */
      /*                      DAC CURRENT                        */
      /* ####################################################### */
      else if(cfg[i] == OA_DAC_CURRENT){
         INC_DAC_INDEX(dac_c);
         aexp.pinCurrent(i,currents[dac_c]);
         Serial.print("   - DAC channel ");
         Serial.print(i); 
         Serial.println(" value: " + String(currents[dac_c]) + " milli Amperes");
         
      }
      /* ####################################################### */
      /*                      DIGITAL INPUT                      */
      /* ####################################################### */ 
      else if(cfg[i] == OA_DIGITAL_INPUT) {
         int va = aexp.digitalRead(i,true);
         Serial.print("   - DI  channel ");
         Serial.print(i); 
         Serial.println(" value: " + String(va));
      } 
   }
}




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
  
  if(millis() - start > 30000) {
    start = millis();
    Serial.print("\n*** Number of expansions: ");
    Serial.println(OptaController.getExpansionNum());

    for(int i = 0; i < OptaController.getExpansionNum(); i++) {
      Serial.print("    Expansion n. ");
      Serial.print(i);
      Serial.print(" type ");
      printExpansionType(OptaController.getExpansionType(i));
      Serial.print(" I2C address ");
      Serial.println(OptaController.getExpansionI2Caddress(i));
    }
  }  
}

int8_t oa_index_1 = -1;

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  Serial.begin(115200);
  delay(2000);

  Serial.println("*** Opta GENERIC example ***");

  /* always call begin before any other OptaController function */
  OptaController.begin();
  
  /* wait until some expansion is found */
  while(OptaController.getExpansionNum() == 0) {
    Serial.println("Looking for expansions...");
    OptaController.update(); // call update to look for new expansion
    delay(1000);
  }

  /* analog expansion must be in position 0 */
  AnalogExpansion aexp = OptaController.getExpansion(0);

  if(aexp) {
     setupAnalogExpansion(aexp, device1_cfg);
  }
  else {
    while(1) {
      Serial.println("NO Opta Analog found at position 0... looping forever...");
      delay(2000);
    }
  }
  
}
 

/* -------------------------------------------------------------------------- */
void optaAnalogTask() {
/* -------------------------------------------------------------------------- */
   
  static long int start = millis();
  if(millis() - start > PERIODIC_UPDATE_TIME_ANALOG) {
    start = millis();
    updateAnalogExpansion(device1_cfg);
  }
}




/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  OptaController.update();
  printExpansionInfo();
  optaAnalogTask();
}
