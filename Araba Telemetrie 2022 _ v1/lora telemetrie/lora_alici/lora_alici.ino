//alici
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
  unsigned long Count;
  int value;
  int map_value;
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
  Serial.println("Starting Reader");
  Transceiver.init();
  Transceiver.PrintParameters();
}

void loop() {
if (ESerial.available()) {
    Transceiver.GetStruct(&MyData, sizeof(MyData));
    Serial.print("Count: "); Serial.println(MyData.Count);
    Serial.print("Bits: "); Serial.println(MyData.value);
    Serial.print("Volts: "); Serial.println(MyData.map_value);
    lcd.setCursor(3, 0);
    lcd.print("Lora Alici Aktif");
    lcd.setCursor(1, 1);
    lcd.print( MyData.Count);
    lcd.setCursor(1, 2);
    lcd.print(MyData.value);
    lcd.setCursor(1, 3);
    lcd.print(MyData.map_value);
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
