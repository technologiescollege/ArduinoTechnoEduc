#include <IRremote.h>

IRsend irsend;

void setup()
{

}

void loop() {
      irsend.sendSony(0xa90, 12); 
}
