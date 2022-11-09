//alici speed
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include "EBYTE.h"
#define PIN_RX 2
#define PIN_TX 3
#define PIN_M0 4
#define PIN_M1 5
#define PIN_AX 6

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

struct DATA {
  //unsigned long Count;
  //float rpm_val;
  float speed_val;
};
int Chan;
DATA MyData;
unsigned long Last;
SoftwareSerial ESerial(PIN_RX, PIN_TX);
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.begin (20, 4);

  Serial.begin(9600);
  ESerial.begin(9600);
  //Serial.println("Starting Reader");
  Transceiver.init();
  //Transceiver.PrintParameters();
}

void loop() {
if (ESerial.available()) {
    Transceiver.GetStruct(&MyData, sizeof(MyData));
    //Serial.print("Count: "); Serial.println(MyData.Count);
    //Serial.print("rpm: "); Serial.println(MyData.rpm_val);
  Serial.print("speed: ");// Serial.println(MyData.speed_val);
  Serial.println(MyData.speed_val);
  lcd.setCursor(0, 0);
  lcd.print("Hiz:");
  lcd.setCursor(5, 0);
  lcd.print(MyData.speed_val);
  Last = millis();

  }
  else {
    // if the time checker is over some prescribed amount
    // let the user know there is no incoming data
    if ((millis() - Last) > 1000) {
      Serial.println("Searching: ");
      Last = millis();
    }

  }
}
