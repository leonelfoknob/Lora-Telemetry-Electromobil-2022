
#include <SoftwareSerial.h>
#include "EBYTE.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);
/******************************* lora *************/
#define PIN_RX 2
#define PIN_TX 3
#define PIN_M0 4
#define PIN_M1 5
#define PIN_AX 6

struct DATA {
  int hiz;
  int power; 
  int energy;
  int volts;
  int currents;
  int max_temp;
};

int Chan;
DATA MyData;
unsigned long Last;
SoftwareSerial ESerial(PIN_RX, PIN_TX);
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

void setup() {
  // lcd ekran
  lcd.init();
  lcd.backlight();
  lcd.begin (20, 4);
  lcd.setCursor(2, 0);
  lcd.print("Tel ve AKS veri");
  Serial.begin(9600);
  //******************* lora
  ESerial.begin(9600);
  Transceiver.init(); 
  //Transceiver.PrintParameters();
}

void loop() {
  if (ESerial.available()) {

    Transceiver.GetStruct(&MyData, sizeof(MyData));
    //**********************************************lcd screen write
    lcd.setCursor(0, 1);
    lcd.print("hiz: ");
    lcd.setCursor(4, 1);
    lcd.print(MyData.hiz);
    lcd.print("km/h");

    /*lcd.setCursor(12, 1);
    lcd.print("Temp:");
    lcd.setCursor(17, 1);
    lcd.print(MyData.max_temp);

    lcd.setCursor(0, 2);
    lcd.print("ger: ");
    lcd.setCursor(4, 2);
    lcd.print(MyData.volts);
    lcd.print("V");

    lcd.setCursor(12, 2);
    lcd.print("Amp: ");
    lcd.setCursor(16, 2);
    lcd.print(MyData.currents);
    lcd.print("A");

    lcd.setCursor(0, 3);
    lcd.print("pwr: ");
    lcd.print(":");
    lcd.setCursor(4, 3);
    lcd.print("%");
    lcd.print(MyData.power);

    lcd.setCursor(12, 3);
    lcd.print("Eng: ");
    lcd.setCursor(16, 3);
    lcd.print(MyData.energy);
    lcd.print("W");*/

//----------------------------------> send to serial
    Serial.print(MyData.hiz);
    Serial.print(",");
    Serial.print(MyData.max_temp);
    Serial.print(",");
    Serial.print(MyData.volts);
    Serial.print(",");
    Serial.print(MyData.currents);
    Serial.print(",");
    Serial.print(MyData.power);
    Serial.print(",");
    Serial.print(MyData.energy);
    //"delay(200);  
    Last = millis();

  }
  else {
    if ((millis() - Last) > 1000) {
      //----------------------------------> send to serial
    Serial.print(0);
    Serial.print(",");
    Serial.print(0);
    Serial.print(",");
    Serial.print(0);
    Serial.print(",");
    Serial.print(0);
    Serial.print(",");
    Serial.print(0);
    Serial.print(",");
    Serial.print(0);
    lcd.setCursor(0, 1);
    lcd.print("hiz: ");
    lcd.setCursor(4, 1);
    lcd.print(0);
    lcd.print("km/h");

    lcd.setCursor(12, 1);
    lcd.print("Temp:");
    lcd.setCursor(17, 1);
    lcd.print(0);

    lcd.setCursor(0, 2);
    lcd.print("ger: ");
    lcd.setCursor(4, 2);
    lcd.print(MyData.volts);
    lcd.print("V");

    lcd.setCursor(12, 2);
    lcd.print("Amp: ");
    lcd.setCursor(16, 2);
    lcd.print(0);
    lcd.print("A");

    lcd.setCursor(0, 3);
    lcd.print("pwr: ");
    lcd.print(":");
    lcd.setCursor(4, 3);
    lcd.print("%");
    lcd.print(0);

    lcd.setCursor(12, 3);
    lcd.print("Eng: ");
    lcd.setCursor(16, 3);
    lcd.print(0);
    lcd.print("W");
    delay(200);
   // Last = millis();
    }

  }
}
