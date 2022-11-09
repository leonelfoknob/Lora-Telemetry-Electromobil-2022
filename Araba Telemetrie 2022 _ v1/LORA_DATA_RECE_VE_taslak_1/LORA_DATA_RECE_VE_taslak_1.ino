/**************************************************************alici***************************************************************/
/**************lora modul******************/
#include <SoftwareSerial.h>
#include "EBYTE.h"
/**************lcd******************/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);
/**************************Lora module***************************/
#define PIN_RX 2
#define PIN_TX 3
#define PIN_M0 4
#define PIN_M1 5
#define PIN_AX 6
SoftwareSerial ESerial(PIN_RX, PIN_TX);
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);
unsigned long Last;

struct DATA{
  int hiz;
  float rpm;
};

int hiz;
float rpm;

DATA MyData;

/*********************************void setup***********************/
void setup() {
  //lora
  ESerial.begin(9600);
  Transceiver.init();
  Transceiver.PrintParameters();
  // lcd ekran
  lcd.init();
  lcd.backlight();
  lcd.begin (20, 4);
  Serial.begin(9600);
}
/*********************************void loop***********************/
void loop() {
  if (ESerial.available()) {
    Transceiver.GetStruct(&MyData, sizeof(MyData));
    hiz = MyData.hiz;
    rpm = MyData.rpm;
    /*********************** print value to serial monitor *****************************/
    Serial.print("hiz: ");Serial.print(hiz);Serial.println(" km/h");
    Serial.print("rpm: ");Serial.print(rpm);Serial.println(" rpm");
    /*********************** print value to lcd screen *****************************/
    lcd.setCursor(0, 0);
    lcd.print("hiz:");
    lcd.setCursor(4, 0);
    lcd.print(MyData.hiz);
    lcd.print("km/h");

    lcd.setCursor(10, 0);
    lcd.print("rpm:");
    lcd.setCursor(14, 0);
    lcd.print(MyData.rpm);

    lcd.setCursor(0, 2);
    lcd.print("                    ");
    Last = millis();
  }
  else {
    if ((millis() - Last) > 1000) {
      Serial.println("Searching: ");
      lcd.setCursor(0, 0);
      lcd.print("                    ");
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      lcd.setCursor(0, 2);
      lcd.print(" veri gelmiyor ...  ");
      lcd.setCursor(0, 3);
      lcd.print("                    ");
      Last = millis();
    }
  }

}
