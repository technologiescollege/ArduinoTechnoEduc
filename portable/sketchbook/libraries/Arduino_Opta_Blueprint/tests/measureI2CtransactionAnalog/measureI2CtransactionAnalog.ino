#include "OptaBlue.h"

#define OPTA_CONTROLLER_DETECT_PIN PG_8

#define EXPANSION_INDEX 0

//#define MEASURE_RTD
//#define MEASURE_ADC_SINGLE
//#define MEASURE_ADC_SINGLE_WITH_CONVERSION
//#define MEASURE_ADC_ALL_AT_ONCE
//#define MEASURE_ALL_DIGITAL_INPUT
//#define MEASURE_ALL_DIGITAL_INPUT_AT_ONCE
//#define MEASURE_WRITE_SINGLE_DAC_CHANNEL
//#define MEASURE_WRITE_ALL_DAC
#define MEASURE_WRITE_PWM 

AnalogExpansion aexp;

void setup() {
  Serial.begin(115200);
  while(!Serial) {
    
  }
  Serial.println("TEST SETUP for measuring I2C timing");
  OptaController.begin();
  
  pinMode(OPTA_CONTROLLER_DETECT_PIN, INPUT_PULLUP);
  pinMode(LED_USER, OUTPUT);
  pinMode(BTN_USER, INPUT);

  aexp = OptaController.getExpansion(EXPANSION_INDEX);
  if(aexp) {
     #ifdef MEASURE_RTD
     aexp.beginChannelAsRtd(0, false, 1.2);
     #endif

     #ifdef MEASURE_ADC_SINGLE
     aexp.beginChannelAsVoltageAdc(0);
     #endif

     #ifdef MEASURE_ADC_SINGLE_WITH_CONVERSION
     aexp.beginChannelAsVoltageAdc(0);
     #endif

     #ifdef MEASURE_ADC_ALL_AT_ONCE
     aexp.beginChannelAsVoltageAdc(0);
     aexp.beginChannelAsVoltageAdc(1);
     aexp.beginChannelAsVoltageAdc(2);
     aexp.beginChannelAsVoltageAdc(3);
     aexp.beginChannelAsVoltageAdc(4);
     aexp.beginChannelAsVoltageAdc(5);
     aexp.beginChannelAsVoltageAdc(6);
     aexp.beginChannelAsVoltageAdc(7);
     #endif

     #ifdef MEASURE_ALL_DIGITAL_INPUT
     aexp.beginChannelAsDigitalInput(0);
     aexp.beginChannelAsDigitalInput(1);
     aexp.beginChannelAsDigitalInput(2);
     aexp.beginChannelAsDigitalInput(3);
     aexp.beginChannelAsDigitalInput(4);
     aexp.beginChannelAsDigitalInput(5);
     aexp.beginChannelAsDigitalInput(6);
     aexp.beginChannelAsDigitalInput(7);
     #endif

     #ifdef MEASURE_ALL_DIGITAL_INPUT_AT_ONCE
     aexp.beginChannelAsDigitalInput(0);
     aexp.beginChannelAsDigitalInput(1);
     aexp.beginChannelAsDigitalInput(2);
     aexp.beginChannelAsDigitalInput(3);
     aexp.beginChannelAsDigitalInput(4);
     aexp.beginChannelAsDigitalInput(5);
     aexp.beginChannelAsDigitalInput(6);
     aexp.beginChannelAsDigitalInput(7);
     #endif

     #ifdef MEASURE_WRITE_SINGLE_DAC_CHANNEL
     aexp.beginChannelAsVoltageDac(0);
     #endif

     #ifdef MEASURE_WRITE_ALL_DAC
     aexp.beginChannelAsVoltageDac(0);
     aexp.beginChannelAsVoltageDac(1);
     aexp.beginChannelAsVoltageDac(2);
     aexp.beginChannelAsVoltageDac(3);
     aexp.beginChannelAsVoltageDac(4);
     aexp.beginChannelAsVoltageDac(5);
     aexp.beginChannelAsVoltageDac(6);
     aexp.beginChannelAsVoltageDac(7);
     #endif

     
  }
  else {
    while(1) {
      Serial.println("Analog expansion not found");
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
    #ifdef MEASURE_RTD
    Serial.println("executing aexp.getRtd(0);");
    v = aexp.getRtd(0);
    #endif
    #ifdef MEASURE_ADC_SINGLE
    Serial.println("executing aexp.getAdc(0, true);");
    v = aexp.getAdc(0, true);
    #endif

    #ifdef MEASURE_ADC_SINGLE_WITH_CONVERSION
    Serial.println("executing aexp.pinVoltage(0, true);");
    v = aexp.pinVoltage(0, true);
    #endif

    #ifdef MEASURE_ADC_ALL_AT_ONCE
    Serial.println("executing aexp.updateAnalogInputs();");
    aexp.updateAnalogInputs();
    v = aexp.getAdc(0, false);
    #endif

    #ifdef MEASURE_ALL_DIGITAL_INPUT
    Serial.println("executing aexp.digitalRead(0, true);");
    v = aexp.digitalRead(0, true);
    #endif

    #ifdef MEASURE_ALL_DIGITAL_INPUT_AT_ONCE
    Serial.println("executing aexp.updateDigitalInputs();");
    aexp.updateDigitalInputs();
    v = aexp.digitalRead(0, false);
    #endif

    #ifdef MEASURE_WRITE_SINGLE_DAC_CHANNEL
    Serial.println("executing aexp.setDac(0, 5050, true);");
    aexp.setDac(0, 5050, true);
    #endif

    #ifdef MEASURE_WRITE_ALL_DAC
    Serial.println("executing aexp.updateAnalogOutputs();");
    aexp.setDac(0, 5050, false);
    aexp.updateAnalogOutputs();
    #endif

    #ifdef MEASURE_WRITE_PWM
    Serial.println("executing aexp.etPwm(OA_PWM_CH_0, 500000, 250000)");
    static uint32_t period = 499999;
    aexp.setPwm(OA_PWM_CH_0, period, 250000);
    period++;
    #endif
    
    digitalWrite(LED_USER,LOW);
    Serial.println("Measured " + String(v));
  }
  

}
