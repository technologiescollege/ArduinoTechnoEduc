// Unit tests for Adafruit_VEML6070 library
// By C. Matlack

// These unit tests require the AUnit unit testing library,
// available within the Arduino IDE. Note that old versions of
// the Arduino IDE, including 1.6.5, may not use a new enough
// C++ compiler for nullptr to be defined.


#include <Wire.h>
#include <Adafruit_VEML6070.h>
#include <AUnitVerbose.h>

// Pin assignments
#define POWER_PIN (13)
#define ACK_PIN   (11)

// Globals
Adafruit_VEML6070 uv = Adafruit_VEML6070();


test(0_hello) {
  assertEqual(1, 1);
}


test(1_initialize_and_read) {

  uv.begin(VEML6070_1_T);  // pass in the integration time constant

  for (uint8_t i = 0; i < 10; i++) {
    uint16_t value = uv.readUV();
    Serial.print("UV: ");
    Serial.println(value);
    assertNotEqual(value, (uint16_t) 0xFFFF);
    assertNotEqual(value, (uint16_t) 0);
  }
}


test(2_interrupt) {
  Serial.println("\nUV measurement must *cross* from <102  to >102 for this test to be informative.");
  Serial.println("Successful interrupt trigger will result in values of 65535 (I2C read failure).");
  
  // Check that interrupt is not triggered 
  assertTrue(digitalRead(ACK_PIN));
  assertFalse(uv.clearAck());
  
  uv.begin(VEML6070_4_T);
  uv.setInterrupt(true, 0);

  // Note that once the interrupt is tripped,
  // values will read as 65535 because the
  // device becomes unresponsive to I2C reads
  for (uint16_t i = 0; i < 30; i++) {
    uint16_t val = uv.readUV();
    Serial.print("UV: "); 
    Serial.println(val);
    
    if (!digitalRead(ACK_PIN)) { break; }
  }  

  bool pin = digitalRead(ACK_PIN);
  bool state = uv.clearAck();
  
  Serial.print("ACK ");
  Serial.println(state ? "was set (as desired)." : "not set, skipping");

  assertTrue(pin ^ state);  // If ACK is in triggered state, pin should be low

  uv.clearAck();          // Redundant, but thorough
  uv.setInterrupt(false);
  uv.clearAck();          // Redundant, but thorough
  
  if (!state) { skip(); } // Don't mark as success if we didn't trigger interrupt
}


void setup() {
  delay(1000); // wait for stability on some boards to prevent garbage Serial
  Serial.begin(115200);
  while(!Serial); // for the Arduino Leonardo/Micro only

  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
  delay(100);
}


void loop() {
  aunit::TestRunner::run();
}