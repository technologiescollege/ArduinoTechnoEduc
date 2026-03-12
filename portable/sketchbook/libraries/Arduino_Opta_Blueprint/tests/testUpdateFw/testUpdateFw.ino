/* -------------------------------------------------------------------------- */
/* FILE NAME:   updateExpansionFw.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240115
   DESCRIPTION: 
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"
#include "utility/BossaOpta.h"
#include "fwUpdateDigital.h"
#include "fwUpdateAnalog.h"
#include "BossaArduino.h"

/* if this is defined the sketch will ask for a confirmation via serial to 
 * actually perform the fw update */
//#define ASK_FOR_FW_UPDATE 

using namespace mbed;

#define NUMBER_OF_UPGRADE  2
#define FW_MAJOR 0
#define FW_MINOR 0
#define FW_RELEASE 9

UART BossaSerial(D14, D13, NC, NC);

/* opta_analog_fw_update contains the fw update for analog expansion 
 * opta_digital_fw_update_contains the fw update for digital expansion
 * fourth from last bit contains the fw type 
 * last 3 bytes contains the version of the FW expansion */

static uint32_t oa_fw_size = sizeof(opta_analog_fw_update) - 4;
static unsigned char oa_type = opta_analog_fw_update[oa_fw_size];
static unsigned char oa_M = opta_analog_fw_update[oa_fw_size + 1];
static unsigned char oa_m = opta_analog_fw_update[oa_fw_size + 2];
static unsigned char oa_r = opta_analog_fw_update[oa_fw_size + 3];
static unsigned int  oa_version = oa_M * 100 + oa_m * 10 + oa_r;

static uint32_t od_fw_size = sizeof(opta_digital_fw_update) - 4;
static unsigned char od_type = opta_digital_fw_update[od_fw_size];
static unsigned char od_M = opta_digital_fw_update[od_fw_size + 1];
static unsigned char od_m = opta_digital_fw_update[od_fw_size + 2];
static unsigned char od_r = opta_digital_fw_update[od_fw_size + 3];
static unsigned int  od_version = od_M * 100 + od_m * 10 + od_r;

static char *fileptr = nullptr;

Controller controller; 

/* print the expansion type */
void printExpansionType(ExpansionType_t t) {
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
  else {
    Serial.print("Unknown!");
  }
}

void printVersion(unsigned char M, unsigned char m, unsigned char r) {
   Serial.print(M);
   Serial.print('.');
   Serial.print(m);
   Serial.print('.');
   Serial.print(r);
   Serial.println();
}

/* verify if the device is updatable */
bool isUpdatable(int device) {
   bool rv = true;
   uint8_t M,m,r;
   Serial.print("\nDevice n. ");
   Serial.print(device);
   Serial.print(" type ");
   ExpansionType_t type = controller.getExpansionType(device);
   printExpansionType(type);

   if(controller.getFwVersion(device,M,m,r)) {
      Serial.print(" Current FW version: ");
      printVersion(M,m,r);
      unsigned int current_version = M * 100 + m * 10 + r;
      if(EXPANSION_OPTA_DIGITAL_MEC == type || EXPANSION_OPTA_DIGITAL_STS == type) {
         if(od_version > current_version ) {
            rv = true;
         }
      }
   } else {
      Serial.println("WARNING: unable to get current FW version!");
   }

   if(rv) {
      //Serial.println("Device is Updatable");
   }
   else {
      Serial.println("Device is already updated to the last FW version");
   }
   return rv;
}

#ifdef ASK_FOR_FW_UPDATE
bool ask_for_confirmation(int i) {

   Serial.println("Expansion " + String(i) + " will be now updated... Proceed? [Y/n]");
   bool first = true;;
   char ans = 'Y';
   while(!Serial.available()) {
      
   }
   while(Serial.available()) {
      if(first) {
         ans = (char)Serial.read();
         first = false;
      }else {
         Serial.read();
      }
   }
   if(ans == 'Y' || ans == 'y') {
      Serial.println("Starting FW Update");
      return true;
   }
   else {
     Serial.println("FW will NOT be updated!");
     return false;
   }
}
#endif

void updateTask() {
   static unsigned long start = millis();
   static int upgrade_num = 0;
   static int test_failed_num = 0;
   if(upgrade_num >= NUMBER_OF_UPGRADE) {
      Serial.println("TEST FINISHED!");
      if(test_failed_num == 0) {
         Serial.println("Test Passed");
      }
      else {
         Serial.println("Test Failed");
      }
      while(1) {
         
      }
   }
   if(millis() - start > 5000) {
      start = millis();
      if(controller.getExpansionNum() > 0) {
         int expected_expansions = controller.getExpansionNum();
         Serial.println("\nUpgrade number " + String(upgrade_num));
         for(int i = 0; i < controller.getExpansionNum(); i++) {
            /* isUpgradable in this case always says yes! */
            if(isUpdatable(i)) {
               unsigned char *fw = nullptr;
               uint32_t sz = 0;

               
               ExpansionType_t type = controller.getExpansionType(i);
               if(EXPANSION_OPTA_DIGITAL_MEC == type || EXPANSION_OPTA_DIGITAL_STS == type) {
                  fw = (unsigned char *)opta_digital_fw_update;
                  sz = od_fw_size;
               }

               if(sz == 0 || fw == nullptr) {
                  continue;
               }

               Serial.print("    REBOOTING expansion at index " + String(i));

                  
               if(BOSSA.begin(BossaSerial,&controller,i)) {
                     Serial.println("    Updating FW....");
                     if(BOSSA.flash(fw,sz)) {
                        Serial.println("    OK");

                        Serial.print("    Resetting expansion...");
                        BOSSA.reset();
                        uint8_t M = 255;
                        uint8_t m = 255;
                        uint8_t r = 255;

                        controller.getFwVersion(i,M,m,r);

                        if(M == FW_MAJOR && m == FW_MINOR && r == FW_RELEASE) {
                           Serial.println("OK");
                        }
                        else {
                           Serial.println("FAILED!");
                           test_failed_num++;
                        }
                     }
                     else {
                        BOSSA.reset(); 
                        Serial.println("FAILED!");
                        test_failed_num++;
                        
                     }
               }
               else {
                  Serial.println("FAILED to initialize BOSSA...");
               }
            }
         }
         upgrade_num++;
      }
      else {
         Serial.println("No expansion detected... connect an expansion");
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

  Serial.println("\n**** OPTA Expansions Updater sketch ****\n");

  Serial.print("This update will update\n  - OPTA ** DIGITAL ** to version: ");
  printVersion(od_M, od_m, od_r);

  Serial.print("  - OPTA ** ANALOG ** to version: ");
  printVersion(oa_M, oa_m, oa_r);

  controller.begin(); 
  Serial.println("Exit setup");
}

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  /* update function will be called each OPTA_CONTROLLER_UPDATE_RATE ms */
  controller.update();
  updateTask();
  
  /*
  uint8_t M = 255;
  uint8_t m = 255;
  uint8_t r = 255;
  controller.getFwVersion(0,M,m,r);
  Serial.println("dispositivo 0 ");
  printVersion(M,m,r);
  controller.getFwVersion(1,M,m,r);
  Serial.println("dispositivo 1 ");
  printVersion(M,m,r);
  */
}


