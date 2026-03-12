/* -------------------------------------------------------------------------- */
/* FILENAME:    Controller.ino
   AUTHOR:      Daniele Aimo
   DATE:        20230801
   REVISION:    0.0.1
   DESCRIPTION: Sketch for Opta DIGITAL used to develop and test the I2C discover
                and assign addresses protocol 
                This sketch is for the MODULE of the BUS
   PRODUCT:     OPTA DIGITAL - RENESAS CORE 
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */
/* REVISION LIST:
   
   DATE:
   AUTHOR:
   CHANGES:                                                                   */
/* -------------------------------------------------------------------------- */

#include "OptaDigital.h"

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
   OptaExpansion = new OptaDigital();

   if(OptaExpansion != nullptr) {
      OptaExpansion->begin();
   }
}

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  if(OptaExpansion != nullptr) {
      OptaExpansion->update();
   }
   
}
