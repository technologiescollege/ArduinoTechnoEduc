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

#include <cstdlib>
#include "OptaBlue.h"
#include "R4DisplayExpansion.h"

/* This sketch uses at least 3 Analog expansion + 0..2 Digital Expansion */

/* Two analog expansions are used to test "each other" 
   - one is used to set all outputs are DAC (half current and half voltage) 
   - second one is used to set verify the output of the first one 
     (using ADC) 
   These 2 expansions "exchange" roles periodically during the test 
   These 2 expansions must have the channels wired together (0 <-> 0, 1 <-> 1 ...)
   The following 2 defines establich the position of the these 2 expansions
   in the chain 

   The third Analog expansion (at index RTD_EXPANSION_INDEX) is used to set
   up some RTD and DIGITAL INPUTS
*/


#define T1_EXPANSION_INDEX 1
#define T2_EXPANSION_INDEX 2

AnalogExpansion out_expansion;
AnalogExpansion in_expansion;

#define RTD_EXPANSION_INDEX 4

#define TIMEOUT_FOR_EXPANSIONS 5000


#define DIGITAL_TASK_PERIOD 1000
#define PRINT_TASK_PERIOD   10000
#define MUTUAL_TEST_TASK    1000
#define CHANGE_PWM_TASK     5000
#define RTD_EXPANSION_TASK  3000
#define TIMEOUT_TASK        5000

#define TIMEOUT   8000


void printExpansionType(ExpansionType_t t) {

  if(t == EXPANSION_NOT_VALID) {
    if(Serial) Serial.print("TYPE NOT VALID (perhaps custom expansion has not been registered?)");
  }
  else if(t == EXPANSION_OPTA_DIGITAL_MEC) {
    if(Serial) Serial.print("Opta --- DIGITAL [Mechanical]  ---");
  }
  else if(t == EXPANSION_OPTA_DIGITAL_STS) {
    if(Serial) Serial.print("Opta --- DIGITAL [Solid State] ---");
  }
  else if(t == EXPANSION_DIGITAL_INVALID) {
    if(Serial) Serial.print("Opta --- DIGITAL [!!Invalid!!] ---");
  }
  else if(t == OptaController.getExpansionType(R4DisplayExpansion::getProduct()))
  {
     if(Serial) Serial.print("*** UnoR4 Display ***");
  }
  else if(t == OptaController.getExpansionType("ARDUINO UNO R4 WIFI"))
  {
     if(Serial) Serial.print("+++ UnoR4 WIFI ++++");
  }
  else if(t == EXPANSION_OPTA_ANALOG) {
    if(Serial) Serial.print("Opta ~~~ ANALOG ~~~ ");
  }
  else {
    if(Serial) Serial.print("Unknown!");
  }
}

std::string debug_getProduct() {
  std::string rv("ARDUINO UNO R4 WIFI");
  return rv;
}

void setExpansionAsOutput_1(AnalogExpansion &a) {
   /* channels from 0 to 3 voltage DAC */
   a.beginChannelAsVoltageDac(0);
   a.beginChannelAsVoltageDac(1);
   a.beginChannelAsVoltageDac(2);
   a.beginChannelAsVoltageDac(3);
   a.setDefaultPinVoltage(0, 1.0);
   a.setDefaultPinVoltage(1, 3.0);
   a.setDefaultPinVoltage(2, 7.0);
   a.setDefaultPinVoltage(3, 4.7);
   /*channels from 4 to 7 are current DAC */
   a.beginChannelAsCurrentDac(4);
   a.beginChannelAsCurrentDac(5);
   a.beginChannelAsCurrentDac(6);
   a.beginChannelAsCurrentDac(7);
   a.setDefaultPinCurrent(4, 4.0);
   a.setDefaultPinCurrent(5, 5.0);
   a.setDefaultPinCurrent(6, 1.0);
   a.setDefaultPinCurrent(7, 6.0);

   a.setTimeoutForDefaultValues(TIMEOUT);
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
   a.setDefaultPinVoltage(4, 1.0);
   a.setDefaultPinVoltage(5, 3.0);
   a.setDefaultPinVoltage(6, 7.0);
   a.setDefaultPinVoltage(7, 4.7);
   /*channels from 0 to 0 are current DAC */
   a.beginChannelAsCurrentDac(0);
   a.beginChannelAsCurrentDac(1);
   a.beginChannelAsCurrentDac(2);
   a.beginChannelAsCurrentDac(3);
   a.setDefaultPinCurrent(0, 4.0);
   a.setDefaultPinCurrent(1, 5.0);
   a.setDefaultPinCurrent(2, 1.0);
   a.setDefaultPinCurrent(3, 6.0);

   a.setTimeoutForDefaultValues(TIMEOUT);
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
   out_expansion = OptaController.getExpansion(T1_EXPANSION_INDEX);
   if(out_expansion) {
      setExpansionAsOutput_1(out_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion at index " +  
                                 String(T1_EXPANSION_INDEX) + 
                                 "NOT FOUND looping forever...");
      while(1) {}
   }

   
   in_expansion = OptaController.getExpansion(T2_EXPANSION_INDEX);
   if(in_expansion) {
      setExpansionAsInput_1(in_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion at index " +  
                                 String(T2_EXPANSION_INDEX) + 
                                 "NOT FOUND looping forever...");
      while(1) {}
   }
}


void initForward_2() {
   /* expansion 0 out / expansion 1 is in with configuration 2 */
   out_expansion = OptaController.getExpansion(T1_EXPANSION_INDEX);
   if(out_expansion) {
      setExpansionAsOutput_2(out_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion at index " +  
                                 String(T1_EXPANSION_INDEX) + 
                                 "NOT FOUND looping forever...");
      while(1) {}
   }

   
   in_expansion = OptaController.getExpansion(T2_EXPANSION_INDEX);
   if(in_expansion) {
      setExpansionAsInput_2(in_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion at index " +  
                                 String(T2_EXPANSION_INDEX) + 
                                 "NOT FOUND looping forever...");
      while(1) {}
   }
}


void initReverse_1() {
   /* expansion 0 in / expansion 1 is out with configuration 1 */
   out_expansion = OptaController.getExpansion(T2_EXPANSION_INDEX);
   if(out_expansion) {
      setExpansionAsOutput_1(out_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion at index " +  
                                 String(T2_EXPANSION_INDEX) + 
                                 "NOT FOUND looping forever...");
      while(1) {}
   }

   
   in_expansion = OptaController.getExpansion(T1_EXPANSION_INDEX);
   if(in_expansion) {
      setExpansionAsInput_1(in_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion at index " +  
                                 String(T1_EXPANSION_INDEX) + 
                                 "NOT FOUND looping forever...");
      while(1) {}
   }
}


void initReverse_2() {
   /* expansion 0 input / expansion 1 is output with configuration 2 */
   out_expansion = OptaController.getExpansion(T2_EXPANSION_INDEX);
   if(out_expansion) {
      setExpansionAsOutput_2(out_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion at index " +  
                                 String(T2_EXPANSION_INDEX) + 
                                 "NOT FOUND looping forever...");
      while(1) {}
   }

   
   in_expansion = OptaController.getExpansion(T1_EXPANSION_INDEX);
   if(in_expansion) {
      setExpansionAsInput_2(in_expansion);
   }
   else {
      if(Serial) Serial.println("Analog Expansion at index " +  
                                 String(T1_EXPANSION_INDEX) + 
                                 "NOT FOUND looping forever...");
      while(1) {}
   }
}


/* TASK for DIGITAL expansions (accende tutte le uscite se spente, le spegne
   se accese) 
*/

void digitalTask() {
   static unsigned int start = millis();
   static bool status[5] = {true,true,true,true,true};
   if(millis() - start > DIGITAL_TASK_PERIOD) {
      start = millis();
      for(int i = 0; i < 5; i++) {
         if(Serial) Serial.print("Expansions " + String(i));
         DigitalExpansion d = OptaController.getExpansion(i);
         if(d) {
            if(Serial) Serial.print(" Index " + String(d.getIndex()));
            
            if(Serial) Serial.println("DIGITAL FOUND!");
            if(status[i]) {
               for(int i = 0; i < 8; i++) {
                  d.digitalWrite(i,HIGH,true);
                  delay(100);
               }
               
            }
            else {
               for(int i = 0; i < 8; i++) {
                  d.digitalWrite(i,LOW,true);
                  delay(100);
               }
               
            }
         }
         else {
            if(Serial) Serial.println(" DIGITAL ~ NOT ~ FOUND!");
         }
         if(status[i]) {
            status[i] = false;
         }
         else {
            status[i] = true;
         }
      }
   }
}



/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
   Serial.begin(115200);
   delay(5000);

   srand(millis());

   OptaController.begin();

   int type = OptaController.registerCustomExpansion(R4DisplayExpansion::getProduct(),
                                         R4DisplayExpansion::makeExpansion,
                                         R4DisplayExpansion::startUp);

   if(Serial) Serial.println("Custom expansion type for R4 display " + String(type));
   type = OptaController.registerCustomExpansion(debug_getProduct(),
                                         R4DisplayExpansion::makeExpansion,
                                         R4DisplayExpansion::startUp);
   if(Serial) Serial.println("Custom expansion type for R4 WIFI " + String(type));

   /* setting default value and timeout */

   initForward_1();

   AnalogExpansion rtd_in = OptaController.getExpansion(RTD_EXPANSION_INDEX);
   if(rtd_in) {
      rtd_in.beginChannelAsRtd(0, true, 1.2);
      rtd_in.beginChannelAsRtd(1, true, 1.2);
      rtd_in.beginChannelAsRtd(2, true, 1.2);
      rtd_in.beginChannelAsRtd(3, true, 1.2);
      /*channels from 0 to 3 are current ADC */
      rtd_in.beginChannelAsRtd(4, true, 1.2);
      rtd_in.beginChannelAsVoltageDac(5);
      rtd_in.beginChannelAsDigitalInput(6);
      rtd_in.beginChannelAsDigitalInput(7);
   }
   else {
      if(Serial) Serial.println("Analog Expansion for RTD test not found... looping forever...");
      while(1) {}
   }
   
   rtd_in.pinVoltage(5,(float)9.0);
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

uint32_t pwm_periods[10] = {100, 50000, 0, 45000, 27000, 38000, 0, 750000, 80000, 106000};
uint32_t pwm_pulse[10] = {14, 2500, 10000, 36000, 4000, 12000, 15000, 47000, 40000, 75000};



void printExpansionTask() {
   static unsigned int start = millis();
   
   if(millis() - start > PRINT_TASK_PERIOD) {
      start = millis();
      for(int i = 0; i < OptaController.getExpansionNum(); i++) {
         if(Serial) Serial.print("Expansion n. ");
         if(Serial) Serial.print(i);
         if(Serial) Serial.print(" type ");
         printExpansionType(OptaController.getExpansionType(i));
         if(Serial) Serial.print(" I2C address ");
         if(Serial) Serial.println(OptaController.getExpansionI2Caddress(i));
      } 
   }
}


#define index my_own_index

static int8_t index = -1;
static uint8_t configuration = 0;

void testMutualExpansionTask() {
   static unsigned int start = millis();
   
   
   
   if(millis() - start > MUTUAL_TEST_TASK + std::rand()%500) {
      start = millis();
      
      index++;
      if(index >= MAX_TEST_VALUES) {
         index = 0;

         configuration++;
         if(configuration >= 4) {
            configuration = 0;
         }

         if(Serial) Serial.println("configuration = " + String(configuration));

         if(configuration == 0) {
            if(Serial) Serial.println("\n------ Configurazione FORWARD 1");
            initForward_1();
         }
         else if(configuration == 1) {
            if(Serial) Serial.println("\n------ Configurazione REVERSE 1");
            initReverse_1();
         }
         else if(configuration == 2) {
            if(Serial) Serial.println("\n------ Configurazione FORWARD 2");
            initForward_2();
         }
         else if(configuration == 3) {
            if(Serial) Serial.println("\n------ Configurazione REVERSE 2");
            initReverse_2();
            }
      }
      if(Serial) Serial.println("index = " + String(index));

      if(in_expansion && out_expansion) {
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
         
      }
      else {
         if(Serial) Serial.println("Analog expansion FOR MUTUAL TEST NOT FOUND");
      }
   }
}


void changePwmTask() {
   static unsigned int start = millis();
   
   if(millis() - start > PRINT_TASK_PERIOD + std::rand()%2001) {
      start = millis();
      static int in = 0;

      if(in_expansion) {
         for(int i = OA_PWM_CH_FIRST; i <= OA_PWM_CH_LAST; i++) {
            
            if(Serial) Serial.println("Exp " + String(in_expansion.getIndex()) + " SET PWM " + String(i - OA_PWM_CH_FIRST) + " periodo " +
               String(pwm_periods[in]) + " pulse " +  String(pwm_pulse[in]));
            
            in_expansion.setPwm(i,pwm_periods[in],pwm_pulse[in]);
            
            if(Serial) {
               Serial.println("  * Getting back PWM information:  - periodo " + String(in_expansion.getPwmPeriod(i)) + "  - pulse " + String(in_expansion.getPwmPulse(i)) + "  - freq " + String(in_expansion.getPwmFreqHz(i)) + "  - duty " + String(in_expansion.getPwmPulsePerc(i)));
            }
         }   
      }

      if(out_expansion) {
         for(int i = OA_PWM_CH_FIRST; i <= OA_PWM_CH_LAST; i++) {
            
            if(Serial) Serial.println("Exp " + String(out_expansion.getIndex()) + " SET PWM " + String(i - OA_PWM_CH_FIRST) + " periodo " +
               String(pwm_periods[in]) + " pulse " +  String(pwm_pulse[in]));
            
               out_expansion.setPwm(i,pwm_periods[in],pwm_pulse[in]);
            if(Serial) {
               Serial.println("  * Getting back PWM information:  -  periodo " + String(out_expansion.getPwmPeriod(i)) + "  - pulse " + String(out_expansion.getPwmPulse(i)) + "  - freq " + String(out_expansion.getPwmFreqHz(i)) + "  - duty " + String(out_expansion.getPwmPulsePerc(i)));
               
            }
         }   
      }

      in++;
      if(in >= 10) {
       in = 0;
      }

      AnalogExpansion rtd_in = OptaController.getExpansion(RTD_EXPANSION_INDEX);
      static bool on = true;

      if(rtd_in) {
         if(on) {
            if(Serial) Serial.println("++++++++++++++  PWM ON on RTD expansion");
            rtd_in.setPwm(OA_PWM_CH_0,10000,5000);
            rtd_in.setPwm(OA_PWM_CH_1,8000,2000);
            rtd_in.setPwm(OA_PWM_CH_2,20000,5000);
            rtd_in.setPwm(OA_PWM_CH_3,20000,10000);
            on = false;
         }
         else {
            if(Serial) Serial.println("--------------  PWM OFF on RTD expansion");
            rtd_in.setPwm(OA_PWM_CH_0,0,5000);
            rtd_in.setPwm(OA_PWM_CH_1,0,2000);
            rtd_in.setPwm(OA_PWM_CH_2,0,5000);
            rtd_in.setPwm(OA_PWM_CH_3,0,10000);
            on = true;
         }
      }
   }
}


void rtdExpansionTask() {
   static unsigned int start = millis();
   
   if(millis() - start > RTD_EXPANSION_TASK + std::rand()%2001) {
      start = millis();
      AnalogExpansion rtd_in = OptaController.getExpansion(RTD_EXPANSION_INDEX);
      if(rtd_in) {
         for(int i = 0; i < 8; i++) {
            if(rtd_in.isChRtd(i)) {
               if(Serial) Serial.println("RTD expansion " + String(rtd_in.getIndex()) + " - channel " + String(i) + " reading value " + String(rtd_in.getRtd(i)) + " ohm");
            }
            else if(rtd_in.isChDigitalInput(i)) {
               rtd_in.updateDigitalInputs();
               if(rtd_in.digitalRead(i)==HIGH){
                  if(Serial) Serial.println("RTD expansion " + String(rtd_in.getIndex()) + " - channel " + String(i) + " reading value HIGH (DIGITAL)");
               } else if(rtd_in.digitalRead(i)==LOW){
                  if(Serial) Serial.println("RTD expansion " + String(rtd_in.getIndex()) + " - channel " + String(i) + " reading value LOW (DIGITAL)");
               }
            }
            else if(rtd_in.isChDac(i)) {
               if(Serial) Serial.println("RTD expansion " + String(rtd_in.getIndex()) + " - channel " + String(i) + " is DAC");
            }

            else {
               if(Serial) Serial.print("Exp " + String(rtd_in.getIndex()) + " ch " + String(i));  
               if(Serial) Serial.println(" BOH?");
            }    
         }  
      } 
   }
}


void timeoutTask() {
   static unsigned int start = millis();
   
   if(millis() - start > TIMEOUT_TASK) {
      start = millis();

      if( (std::rand() % 10000) > 9000) { /* capita circa 1 volta su 10 (dovrebbe...) */
         if(Serial) Serial.println("********************** TIMEOUT ************************************* ");

         delay(TIMEOUT + 500); // qui si verifica il timeout
         AnalogExpansion ain;
         AnalogExpansion aout;
         if(configuration == 0 || configuration == 2) {
            /* Le configurazioni 0 e 1 usano T1 come espansione che fornisce le uscite 
               che è quella che va in timeout ...
               => uso T2 per leggere i valori */
            ain = OptaController.getExpansion(T2_EXPANSION_INDEX);
            aout = OptaController.getExpansion(T1_EXPANSION_INDEX);
            if(Serial) Serial.println("TIMEOUT on expansion " + String(T1_EXPANSION_INDEX) + " reading from " + String(T2_EXPANSION_INDEX));
         }
         else {
            /* Le configurazioni 2 e 3 usano T2 come espansione che fornisce le uscite 
               che è quella che va in timeout ...
               => uso T1 per leggere i valori */
            ain = OptaController.getExpansion(T1_EXPANSION_INDEX);
            aout = OptaController.getExpansion(T2_EXPANSION_INDEX);
            if(Serial) Serial.println("TIMEOUT on expansion " + String(T2_EXPANSION_INDEX) + " reading from " + String(T1_EXPANSION_INDEX));
         }

         if(configuration == 0 || configuration == 1) {  

            for(int ch = 0; ch < 8; ch++ ) {
               if(ch == 0) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 1.0V ");
                  while(value > (1.0 + 0.3) || value < (1.0 - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 1) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 3.0V ");
                  while(value > (3.0 + 0.3) || value < (3.0 - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 2) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 7.0V ");
                  while(value > (7.0 + 0.3) || value < (7.0 - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 3) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 4.7V ");
                  while(value > (4.7 + 0.3) || value < (4.7 - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 4) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 4.0mA ");
                  while(value > (4.0 + 0.3) || value < (4.0 - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 5) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 5.0mA ");
                  while(value > (5.0 + 0.3) || value < (5.0 - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 6) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 1.0mA ");
                  while(value > (1.0 + 0.3) || value < (1.0 - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 7) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 6.0mA ");
                  while(value > (6.0 + 0.3) || value < (6.0 - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
            }
            
         }
         else {
            for(int ch = 0; ch < 8; ch++ ) {
               if(ch == 4) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 1.0V ");
                  while(value > (1.0 + 0.3) || value < (1.0 - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 5) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 3.0V ");
                  while(value > (3.0 + 0.3) || value < (3.0 - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 6) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 7.0V ");
                  while(value > (7.0 + 0.3) || value < (7.0 - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 7) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 4.7V ");
                  while(value > (4.7 + 0.3) || value < (4.7 - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 0) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 4.0mA ");
                  while(value > (4.0 + 0.3) || value < (4.0 - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 1) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 5.0mA ");
                  while(value > (5.0 + 0.3) || value < (5.0 - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 2) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 1.0mA ");
                  while(value > (1.0 + 0.3) || value < (1.0 - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 3) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("Timeout Expected value on channel " + String(ch) + " 6.0mA ");
                  while(value > (6.0 + 0.3) || value < (6.0 - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
            }

         }

         if(Serial) Serial.println("^^^^^^^^^^^^^^^^^^^^^ REMOVING TIMEOUT ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");

         aout.isChDac(0,true);

         if(configuration == 0 || configuration == 1) {  

            for(int ch = 0; ch < 8; ch++ ) {
               if(ch == 0) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(voltages[index]) + "V");
                  while(value > (voltages[index] + 0.3) || value < (voltages[index] - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 1) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(voltages[index]) + "V");
                  while(value > (voltages[index] + 0.3) || value < (voltages[index] - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 2) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(voltages[index]) + "V");
                  while(value > (voltages[index] + 0.3) || value < (voltages[index] - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 3) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(voltages[index]) + "V");
                  while(value > (voltages[index] + 0.3) || value < (voltages[index] - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 4) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(currents[index]) + "mA");
                  while(value > (currents[index] + 0.3) || value < (currents[index] - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 5) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(currents[index]) + "mA");
                  while(value > (currents[index] + 0.3) || value < (currents[index] - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 6) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(currents[index]) + "mA");
                  while(value > (currents[index] + 0.3) || value < (currents[index] - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 7) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(currents[index]) + "mA");
                  while(value > (currents[index] + 0.3) || value < (currents[index] - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
            }
            
         }
         else {
            for(int ch = 0; ch < 8; ch++ ) {
               if(ch == 4) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(voltages[index]) + "V");
                  while(value > (voltages[index] + 0.3) || value < (voltages[index] - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 5) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(voltages[index]) + "V");
                  while(value > (voltages[index] + 0.3) || value < (voltages[index] - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 6) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(voltages[index]) + "V");
                  while(value > (voltages[index] + 0.3) || value < (voltages[index] - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 7) {
                  float value = ain.pinVoltage((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(voltages[index]) + "V");
                  while(value > (voltages[index] + 0.3) || value < (voltages[index] - 0.3)) {
                     value = ain.pinVoltage((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 0) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(currents[index]) + "mA");
                  while(value > (currents[index] + 0.3) || value < (currents[index] - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 1) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(currents[index]) + "mA");
                  while(value > (currents[index] + 0.3) || value < (currents[index] - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 2) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(currents[index]) + "mA");
                  while(value > (currents[index] + 0.3) || value < (currents[index] - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
               else if(ch == 3) {
                  float value = ain.pinCurrent((uint8_t)ch);
                  if(Serial) Serial.println("EXIT from Expected value on channel " + String(ch) + " " + String(currents[index]) + "mA");
                  while(value > (currents[index] + 0.3) || value < (currents[index] - 0.3)) {
                     value = ain.pinCurrent((uint8_t)ch);  
                  }
                  if(Serial) Serial.println( "-> OK!");
               }
            }
         }
      }
   }
}


/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
   static uint8_t configuration = 0;

   OptaController.update();
   
   printExpansionTask();
   testMutualExpansionTask();
   changePwmTask(); 
   rtdExpansionTask();
   timeoutTask();
   digitalTask();
}
