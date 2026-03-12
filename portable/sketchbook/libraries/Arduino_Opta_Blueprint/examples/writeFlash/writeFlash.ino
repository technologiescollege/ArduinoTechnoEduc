/* -------------------------------------------------------------------------- */
/* FILE NAME:   writeFlash.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240124
   DESCRIPTION: This sketch write example of production information on the flash of 
                the expansions.
                On Digital it is also interactively asked the kind of Digital
                (Mechanical or State Solid) and also that information is stored
                in the Digital Expansion Flash memory
                Writing is performed just once for each expansion
                Type 'W' to write again
   LICENSE:     Copyright (c) 2024 Arduino SA 
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

/* PAY ATTENTION: by using this script you are going to modify data that could
 * make your device no more usable !!!!! */


#include "OptaBlue.h"

using namespace Opta;

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
  else if(t == EXPANSION_OPTA_ANALOG) {
    Serial.print("Opta ~~~ ANALOG ~~~ ");
  }
  else {
    Serial.print("Unknown!");
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
}

char getCmdFromSerial(const char *message, bool block = false) {
  char cmd = 'n';
  bool cmd_set = false;
  
  Serial.println(message);

  while(!Serial.available()) {
      if(!block) {
        break;
      }
  }

  if(Serial.available()) {
    while(Serial.available()) {
      if(!cmd_set) {
        cmd = (char)Serial.read();
        cmd_set = true;
      }
      else {
        Serial.read();
      }
    }
  }
  return cmd;
}


char serial_number[8];


/* -------------------------------------------------------------------------- */
void getHEXNumberFromSerial() {
/* -------------------------------------------------------------------------- */
  int i = 0;
  serial_number[0] = 'F';
  serial_number[1] = 'F';
  serial_number[2] = 'F';
  serial_number[3] = 'F';
  serial_number[4] = 'F';
  serial_number[5] = 'F';
  serial_number[6] = 'F';
  serial_number[7] = 'F';


  while(!Serial.available()) {

  }

  while(Serial.available()) {
    
    int num = Serial.read();
    
    if( (num >= 0x30 && num <= 0x39) || (num >= 0x41 && num <= 0x46)) {
      serial_number[i] = (char)num;
      i++;
    }
  }
}


void readInformation(DigitalExpansion &dexp, ExpansionType_t ex_type) {
  Serial.print("Current expansion Serial Number: ");

  char buffer[32];
  uint8_t dbuf = 32;
  uint16_t add = PRODUCTION_DATA_FLASH_ADDRESS;
  dexp.getFlashData((uint8_t *)buffer,dbuf,add);
  for(int i = 0;  i < dbuf; i++) {
    if(std::isprint(buffer[i])) {
      Serial.print((char)buffer[i]);
    }
  }
  Serial.println();
  if(ex_type == EXPANSION_OPTA_DIGITAL_MEC ||
     ex_type == EXPANSION_OPTA_DIGITAL_STS ||
     ex_type == EXPANSION_DIGITAL_INVALID) {
    add = EXPANSION_TYPE_ADDITIONA_DATA;
    dexp.getFlashData((uint8_t *)buffer,dbuf,add);
    Serial.print("Digital type: ");
    if(buffer[0] == 0) {
      Serial.println("MECHANICAL RELE");
    }
    else if(buffer[0] == 1) {
      Serial.println("STATE SOLID");
    }
    else {
      Serial.println("NOT SET");
    }
  }
}

void readInformation(AnalogExpansion &aexp) {
  Serial.print("Current expansion Serial Number: ");

  char buffer[32];
  uint8_t dbuf = 32;
  uint16_t add = PRODUCTION_DATA_FLASH_ADDRESS;
  aexp.getFlashData((uint8_t *)buffer,dbuf,add);
  for(int i = 0;  i < dbuf; i++) {
    if(std::isprint(buffer[i])) {
      Serial.print((char)buffer[i]);
    }
  }
  Serial.println();

}

char askToWrite() {
  Serial.println("If you continue, this data will be overwritten.");
  Serial.println("Pay attention this may make the expansion unusable!!!");
  Serial.println("Continue? [N/y]");
  
  char ans = 'N';
  while(!Serial.available()) {
    
  }
  int read = 0;
  while(Serial.available()) {
    if(read == 0) {
      ans = (char)Serial.read();
    }
    else {
      Serial.read();
    }
    read++;
  }

  return ans;
}



/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */
   static bool written[10] = {false,false,false,false,false,false,false,false,
   false,false};
  /* update function will be called each OPTA_CONTROLLER_UPDATE_RATE ms */
  OptaController.update();
  /* update check for expansion automatically, it can also update the "shadow"
     copy of the devices if UPDATE_DEVICES_IN_UPDATE is defined */

  /* if update() is not called then the user has to call checkForExpansions()
     explicity to verify if new devices are plugged in */
  
  char cmd = getCmdFromSerial("To write again expansions type 'w' and hit return");

  if(cmd == 'w' || cmd == 'W') {
    /* to write again */
    for(int i = 0; i < 10; i++) {
      written[i] = false;
    }
  }
  Serial.println();
  for(int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
    
    DigitalExpansion dexp = OptaController.getExpansion(i);

    if(dexp) {
      
      /* this code will be executed only if dexp is valid i.e. if at the index
      i there is a digital expansion */

      Serial.print("Expansion n. ");
      Serial.print(dexp.getIndex());
      Serial.print(" type ");
      ExpansionType_t ex_type = dexp.getType();
      printExpansionType(ex_type);
      Serial.println();
      
      readInformation(dexp,ex_type);
      
      if(!written[i]) {

        char ans = askToWrite();

        if( !(ans == 'Y' || ans == 'y') ){
          continue;
        }

        /*
         *  ------------ WRITING TO FLASH ---------- (just once)
         */
        Serial.print("\nWriting to ");
        char product_data_example[32]; // Max 32 bytes
        memset(product_data_example,0x00,32);
        if(ex_type == EXPANSION_OPTA_DIGITAL_MEC || 
           ex_type == EXPANSION_OPTA_DIGITAL_STS ||
           ex_type == EXPANSION_DIGITAL_INVALID) {
        
           Serial.println(" DIGITAL flash");
            /*
             * Writing the kind of Digital (STATE SOLID or MECHANICAL)
             */

            cmd = getCmdFromSerial("Type '1' for Opta Digital State Solid Output\nType '2' for Opta Digital Mechanical rele\n and hit return", true); 
            if(cmd == '1') {
              dexp.setIsStateSolid();
            }
            else if(cmd == '2') {
              dexp.setIsMechanical();
            }
           /*
            * WRITING INFORMATION FOR OPTA DIGITAL
            */
            Serial.println("Write the serial number as hex value on 8 number: ");
            getHEXNumberFromSerial();
            if(cmd == '1') {
            sprintf(product_data_example, "EXT-AFX00006%c%c%c%c%c%c%c%c",
            serial_number[0], serial_number[1], serial_number[2],
            serial_number[3], serial_number[4], serial_number[5],
            serial_number[6], serial_number[7]);
            }
            else if(cmd == '2') {
            sprintf(product_data_example, "EXT-AFX00005%c%c%c%c%c%c%c%c",
            serial_number[0], serial_number[1], serial_number[2],
            serial_number[3], serial_number[4], serial_number[5],
            serial_number[6], serial_number[7]);

            }

            Serial.println(product_data_example);
            uint8_t len = strlen(product_data_example) + 1;
            len = (len >= 32) ? 31 : len;
            // MAX 32 bytes can be written 
            product_data_example[len] = 0;
            dexp.setProductData((uint8_t *)product_data_example, len);
        
            written[i] = true;
            Serial.println("WRITTEN");
         }
      }
    }

    
    /* ANALOG EXPANSION */
    AnalogExpansion aexp = OptaController.getExpansion(i);

    if(aexp) {
      
      /* this code will be executed only if aexp is valid i.e. if at the index
      i there is an analog expansion */

      Serial.print("Expansion n. ");
      Serial.print(aexp.getIndex());
      Serial.print(" type ");
      ExpansionType_t ex_type = aexp.getType();
      printExpansionType(ex_type);
      Serial.println();
      
      readInformation(aexp);
      
      if(!written[i]) {

        char ans = askToWrite();

        if( !(ans == 'Y' || ans == 'y') ){
          continue;
        }

        Serial.print("\nWriting to ");
        Serial.println(" ANALOG flash");
        char product_data_example[32]; // Max 32 bytes
        memset(product_data_example,0x00,32);

        /*
         * WRITING INFORMATION FOR OPTA DIGITAL
         */
        Serial.println("Write the serial number as hex value on 8 number: ");
        getHEXNumberFromSerial();

        sprintf(product_data_example, "EXT-AFX00007%c%c%c%c%c%c%c%c",
            serial_number[0], serial_number[1], serial_number[2],
            serial_number[3], serial_number[4], serial_number[5],
            serial_number[6], serial_number[7]);

        Serial.println(product_data_example);
        uint8_t len = strlen(product_data_example) + 1;
        len = (len >= 32) ? 31 : len;
        // MAX 32 bytes can be written 
        product_data_example[len] = 0;
        aexp.setProductData((uint8_t *)product_data_example, len);
        
        written[i] = true;
        Serial.println("WRITTEN");

      }
    }
  }
  delay(3000);
}


