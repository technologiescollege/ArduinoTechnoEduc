#include "OptaBlue.h"

#define OPTA_CONTROLLER_DETECT_PIN PG_8

#define EXPANSION_INDEX 0

//#define MEASURE_ADC_SINGLE
//#define MEASURE_ADC_ALL_AT_ONCE
#define MEASURE_ALL_DIGITAL_INPUT
//#define MEASURE_ALL_DIGITAL_INPUT_AT_ONCE

DigitalExpansion dexp;

void setup() {
  Serial.begin(115200);
  while(!Serial) {
    
  }
  Serial.println("TEST SETUP for measuring I2C timing on DIGITAL");
  OptaController.begin();
  
  pinMode(OPTA_CONTROLLER_DETECT_PIN, INPUT_PULLUP);
  pinMode(LED_USER, OUTPUT);

  dexp = OptaController.getExpansion(EXPANSION_INDEX);
  if(!dexp) {
    while(1) {
      Serial.println("Digital expansion not found");
      delay(1000);
    }
  }

}

void loop() {
  OptaController.update();
  static int counter = 0;
  static int measure = 0;
  digitalWrite(LED_USER,LOW);
  if(digitalRead(BTN_USER) == LOW) {
    counter++;
    if(counter > 10000) {
      measure++;
    }
  }
  else {
    counter=0;
    measure = 0;
  }
  if(measure == 1) {
    digitalWrite(LED_USER,HIGH);
    float v = 0;
   
    #ifdef MEASURE_ADC_SINGLE
    Serial.println("executing dexp.analogRead(0, true);");
    v = dexp.analogRead(0, true);
    #endif

    #ifdef MEASURE_ADC_ALL_AT_ONCE
    Serial.println("executing aexp.updateAnalogInputs();");
    dexp.updateAnalogInputs();
    v = dexp.analogRead(0, false);
    #endif

    #ifdef MEASURE_ALL_DIGITAL_INPUT
    Serial.println("executing dexp.digitalRead(0, true);");
    v = dexp.digitalRead(0, true);
    #endif

    #ifdef MEASURE_ALL_DIGITAL_INPUT_AT_ONCE
    Serial.println("executing dexp.updateDigitalInputs();");
    dexp.updateDigitalInputs();
    v = dexp.digitalRead(0, false);
    #endif
    
    digitalWrite(LED_USER,LOW);
    Serial.println("Measured " + String(v));
  }

}
