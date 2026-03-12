/* -------------------------------------------------------------------------- */
/* FILE NAME:   testReboot.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240115
   DESCRIPTION: This 
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"
#include "utility/BossaOpta.h"
#include "BossaArduino.h"
#include <string>

using namespace mbed;

#define NUMBER_OF_REBOOT  100

#define FW_MAJOR 0
#define FW_MINOR 0
#define FW_RELEASE 4

UART BossaSerial(D14, D13, NC, NC);
static std::string ver("Arduino Bootloader (SAM-BA extended) 2.0 [Arduino:IKXYZ]"); 

Controller controller; 


void rebootTask() {
   static int reboot_num = 0;
   static int test_failed_num = 0;
   static unsigned long start = millis();
   if(reboot_num >= NUMBER_OF_REBOOT) {
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

   if(millis() - start > 2000) {
      start = millis();
      if(controller.getExpansionNum() > 0) {
         Serial.println("\nReboot number " + String(reboot_num));
         for(int i = 0; i < controller.getExpansionNum(); i++) {
               

               Serial.println("   REBOOTING expansion at index " + String(i));
               if(BOSSA.begin(BossaSerial, &controller,i)) {
                  delay(5000);
                  
                  std::string version = BOSSA.version();
                  if(strcmp(version.c_str(), ver.c_str()) == 0) {
                     Serial.println(" OK!");
                  }
                  else {
                     test_failed_num++;

                  }
                  
                  Serial.print("   Return in APPLICATION mode: ");
                  BOSSA.reset();
                  delay(12000);
                  uint8_t M = 255;
                  uint8_t m = 255;
                  uint8_t r = 255;

                  controller.getFwVersion(i,M,m,r);

                  if(M == FW_MAJOR && m == FW_MINOR && r == FW_RELEASE) {
                     Serial.println("OK!");
                  }
                  else {
                     Serial.println("FAILED!");
                     test_failed_num++;
                  }
                  delay(10); 

               }
               else {
                  test_failed_num++;
                  Serial.println(" FAILED! (+++)");
                  BOSSA.begin(BossaSerial, &controller,i);
                  delay(100);
                  BOSSA.reset();
               }
         }
         reboot_num++;
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

  Serial.println("\n**** OPTA Test REBOOT sketch ****\n");
  controller.begin();
}

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  /* update function will be called each OPTA_CONTROLLER_UPDATE_RATE ms */
  controller.update();
  rebootTask();
}


