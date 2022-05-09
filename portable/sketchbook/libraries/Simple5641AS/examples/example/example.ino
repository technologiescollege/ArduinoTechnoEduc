#include <Simple5641AS.h>

const uint8_t segmentPins[] = { 4, 6, 3, 10, 9, 2, 5, 13 };
const uint8_t digitSelectionPins[] = { 12, 11, 8, 7 };
Simple5641AS component( segmentPins, digitSelectionPins );

void setup() {
  Serial.begin(9600);
  Serial.println("Program started!");
}

void loop() {
  delay(300);

  // countDown() + zeroAnimation() example
  Serial.println("countDown() + zeroAnimation() example");
  component.countDown(5);
  component.zeroAnimation();
  delay(1000);

  // Speed & brightness display with countDown()
  Serial.println("countDown() examples with different parameters (different speed & brightness)");
  component.countDown(50, 80, 400, 2, 25);
  component.countDown(50, 40, 800, 2, 25);
  component.countDown(25, 40, 1600, 2, 25);
  component.countDown(12, 40, 2200, 2, 25);
  component.countDown(2, 40, 2000, 2, 25);

  // slowCountDown() example
  Serial.println("slowCountDown() example");
  component.slowCountDown(5);

  // countUp() example
  Serial.println("countUp() example");
  component.countUp(10);

  // displayNumber() examples
  Serial.println("static number using displayNumber(seconds, number, comma_position)");
  component.displayNumber(2, 321, 5);
  Serial.println("static number using displayNumber(int seconds, float number)");
  component.displayNumber(2, 3.14f);
  component.displayNumber(2, 0.123f);
  component.displayNumber(2, 0.4321f);
  component.displayNumber(2, 1.4321f);
  component.displayNumber(2, 3.432122f);
  Serial.println("static number using displayNumber(int seconds, int number)");
  component.displayNumber(2, 2001);

  // static number example with cycle()
  Serial.println("static number example with cycle()");
  int counter = 1000;
  while (counter > 0) {
    counter--;
    component.cycle(555, 1942, 5);
  }

  // static text example with customDisplayCycle()
  Serial.println("static text example with customDisplayCycle()");
  uint8_t textCode[20] = {
                      B11101110,   // A
                      B01111010,   // d
                      B00001010,   // r
                      B00001000 }; // i
  int counter2 = 2125;
  Serial.println("Mostrando indices:");
  while (counter2 > 0) {
    counter2--;
    component.customDisplayCycle(255, textCode);
  }
  
  // text animation example with customDisplayCycle()
  Serial.println("text animation example with customDisplayCycle()");
  uint8_t animationCode[20] = {
                      B00000000,   // -
                      B00000000,   // -
                      B00000000,   // -
                      B00000000,   // -
                      B11101110,   // A
                      B01111010,   // d
                      B00001010,   // r
                      B00001000,   // i
                      B00111010,   // a
                      B00101010,   // n
                      B11011010,   // 2
                      B11111100,   // 0
                      B11111100,   // 0
                      B11011010,   // 2
                      B11011010,   // 2
                      B11110010,   // 3
                      B00000000,   // -
                      B00000000,   // -
                      B00000000,   // -
                      B00000000 }; // -
  int counter3 = 2125;
  int showChar = 0;
  while (counter3 > 0) {
    counter3--;
    showChar = (2125 - counter3) / 125;
    component.customDisplayCycle(255, animationCode + showChar);
  }

  // visual explanation of how it works:
  Serial.println("visualExplanation()!");
  component.visualExplanation();
  
  // reset
  delay(5000);
}
