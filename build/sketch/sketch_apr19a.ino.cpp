#include <Arduino.h>
#line 1 "C:\\TEMP\\arduino_modified_sketch_604792\\sketch_apr19a.ino"
#line 1 "C:\\TEMP\\arduino_modified_sketch_604792\\sketch_apr19a.ino"
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

float Vout;
float Vin;
long R1;
int R2;
int Value;
int AnalogInput;

#line 12 "C:\\TEMP\\arduino_modified_sketch_604792\\sketch_apr19a.ino"
void setup();
#line 31 "C:\\TEMP\\arduino_modified_sketch_604792\\sketch_apr19a.ino"
void loop();
#line 12 "C:\\TEMP\\arduino_modified_sketch_604792\\sketch_apr19a.ino"
void setup() {
  lcd.begin(16, 2);

    Vout = 0.01;
  Vin = 0.01;
  R1 = 100000;
  R2 = 10000;
  Value = 0;
  AnalogInput = 0;
  Value = analogRead(AnalogInput);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("VOLTMETRE");
  lcd.setCursor(0,1);
  lcd.print("");
  delay(1);

}

void loop() {
    Value = analogRead(AnalogInput);
    Vout = (Value / 1024) * 5;
    Vin = Vout / (R2 / (R1 + R2));
    if (Vin < 0.09) {
      Vin = 0;

    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("VOLTMETRE");
    lcd.setCursor(0,1);
    lcd.print((String("ENTREE V=") + String(String(Vin))));
    delay(500);

}
