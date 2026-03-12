/* -------------------------------------------------------------------------- */
/* FILENAME: assignAddress.ino
   AUTHOR: Daniele Aimo
   DATE: 20231020
   REVISION: 0.0.1
   DESCRIPTION: Performs I2C assign address process on Opta device
                This is a sort of minimal sketch to use Opta controller
                Always call begin() in the setup function
                Alway call update() in the loop function
   PRODUCT: Arduino Opta
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
  OptaController.begin();
}

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  OptaController.update();
}
