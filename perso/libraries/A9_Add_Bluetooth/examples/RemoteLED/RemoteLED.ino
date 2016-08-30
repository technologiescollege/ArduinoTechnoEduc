/***************************************************************************************
 *
 * Title:       Remote LED
 * Description: Turn the on-board LED on and off by Bluetooth.
 * Tip:         After upload, select the Bluetooth port with Tools > Port and send 1 
 *              or 0 with the Serial Monitor. Don't forget to select the USB port back
 *              before uploading.
 *
 ***************************************************************************************/
#include <prismino.h>
#include <Bluetooth.h>

void setup()
{
  // open Bluetooth communication and wait for port to open
  delay(1000);
  Bluetooth.begin(9600);
  delay(1000);
  
  // set pin output mode (sources current)
  pinMode(LED, OUTPUT);
}

void loop()
{
  if(Bluetooth.available())
  {
    // read value received by Bluetooth
    char val = Bluetooth.read();
    
    switch (val)
    {
      case '1':
        // turn LED on
        digitalWrite(LED, HIGH);
        break;
      case '0':
        // turn LED off
        digitalWrite(LED, LOW);
        break;
    }
  }
  delay(1);
}
