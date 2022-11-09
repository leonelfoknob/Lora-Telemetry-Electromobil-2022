//verici
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


int pot = A0;
int pot_val;

SoftwareSerial ESerial(PIN_RX, PIN_TX);
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.begin (20, 4);

  Serial.begin(9600);
  ESerial.begin(9600);
  Serial.println("Starting Sender");
  Transceiver.init();
  Transceiver.PrintParameters();
  lcd.setCursor(4, 0);
  lcd.print("Lora Verici");
  //lcd.setCursor(7, 1);
  //lcd.print("basliyor...");
  
}

void loop() {
  MyData.Count++;
  MyData.value = analogRead(pot);
  MyData.map_value = map(MyData.value,0,1023,0,180);
  Transceiver.SendStruct(&MyData, sizeof(MyData));
  Serial.print("Sending: "); 
  Serial.println(MyData.Count);
  lcd.setCursor(3, 0);
  lcd.print("Lora Verici Aktif");
  lcd.setCursor(1, 1);
  lcd.print( MyData.Count++);
  lcd.setCursor(1, 2);
  lcd.print(MyData.value);
  lcd.setCursor(1, 3);
  lcd.print(MyData.map_value);
  delay(500);

}
