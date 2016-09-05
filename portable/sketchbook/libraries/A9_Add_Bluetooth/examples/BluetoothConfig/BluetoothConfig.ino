/***************************************************************************************
 *
 * Title:       Bluetooth configuration
 * Description: Sketch to help configuring the Bluetooth module on the Robopoly shield.
 *
 ***************************************************************************************/
#include <Bluetooth.h>

char buffer[32];

void serialInput(void)
{
  memset(buffer, 0, sizeof(buffer));
  while(!Serial.available());
  Serial.readBytesUntil('\n', buffer, 31); 
}

void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  //Bluetooth.verbose = 1;
  Bluetooth.begin(9600);
}
      
void loop()
{
  if(Serial.available() > 0)
  {
    char cmd = Serial.read();
    
    // get rid of other characters in serial buffer
    while(Serial.available())
    {
      Serial.read();
    }
    
    uint16_t baud;
    
    switch(cmd)
    {
      case 'n':
      Serial.println("Enter new name:");
      serialInput();
      if(Bluetooth.setName(buffer))
      {
        Serial.println("New name set");
      }
      else
      {
        Serial.println("New name setting failed");
      }
      break;
      case 'p':
      Serial.println("Enter new password");
      serialInput();
      if(Bluetooth.setPass(buffer))
      {
        Serial.println("New password set");
      }
      else
      {
        Serial.println("New password setting failed");
      }
      break;
      case 'c':
      Bluetooth.setMode(MASTER);
      Serial.println("Enter slave address");
      serialInput();
      if(Bluetooth.connect(buffer))
      {
        Serial.println("Connected");
      }
      else
      {
        Serial.println("Connection failed");
        Bluetooth.setMode(SLAVE);
      }
      break;
      case 'd':
      Serial.println("Disconnect");
      if(Bluetooth.disconnect())
      {
        Serial.println("Disconnected");
        Bluetooth.setMode(SLAVE);
      }
      else
      {
        Serial.println("Disconnection failed");
      }
      break;
      case 'v':
      Serial.print("Verbose mode: ");
      if(Bluetooth.verbose == 1)
      {
        Bluetooth.verbose = 0;
        Serial.println("off");
      }
      else
      {
        Bluetooth.verbose = 1;
        Serial.println("on");
      }
      break;
      case 's':
      Serial.println("Serching for devices...");
      Bluetooth.setMode(MASTER);
      Bluetooth.verbose = 1;
      Bluetooth.search();
      Bluetooth.verbose = 0;
      Bluetooth.setMode(SLAVE);
      break;
      case 'b':
      Serial.println("Set new baud rate");
      serialInput();
      Bluetooth.setBaud(buffer);
      break;
      case 'f':
      Serial.println("Searching baud rate...");
      baud = Bluetooth.findBaud();
      if(baud)
      {
        Serial.print("Baud rate at ");
        Serial.println(baud);
      }
      else
      {
        Serial.println("Did not find the baud rate");
      }
      break;
      case 'a':
      Serial.println("Enter AT command:");
      serialInput();
      Bluetooth.command(buffer);
      break;
      default:
      Serial.println(
        "Available actions:\r\n"
        "v: toggle verbose mode\r\n"
        "n: set new name\r\n"
        "p: set new password\r\n"
        "b: set baud rate\r\n"
        "f: find the baud rate and connect\r\n"
        "c: connect to a bluetooth module\r\n"
        "d: disconnect\r\n"
        "s: search bluetooth devices\r\n"
        "a: send AT command"
      );
    }
  }
}

