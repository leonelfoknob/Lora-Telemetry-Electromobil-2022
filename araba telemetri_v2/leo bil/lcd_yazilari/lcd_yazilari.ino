/**************************************************************alici***************************************************************/
/**************lora modul******************/
#include <SoftwareSerial.h>
#include "EBYTE.h"
/**************lcd******************/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);
byte pil1[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte pil2[] = {
  B11111,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B11111
};
byte pil3[] = {
  B00000,
  B00000,
  B11100,
  B11100,
  B11100,
  B11100,
  B00000,
  B00000
};

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
  int rpm;               //gosterilmeyecek
  int gerilim;           //Voltage
  int temp;             //max_temp
  int sarj_durum ;       //energy
  int power;             //guç
  int akim;             //current
};

int hiz;
int rpm;      //gosterilmeyecek
int gerilim;
int sicaklik;
int sarj_durum;
int power;             
int akim;             

// canbus bus data
/*
TEMP1: 34.30
TEMP2: 33.97
TEMP3: 34.52
MAX-TEMP: 34.52

Power(watt)=
Energy=0.01
Voltage=1.54
Current=0.18
*/
DATA MyData;

/*********************************void setup***********************/
void setup() {
  // lcd ekran
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, pil1);
  lcd.createChar(1, pil2);
  lcd.createChar(2, pil3);
  lcd.begin (20, 4);
  lcd.setCursor(2, 0);
  lcd.print("Tel ve AKS veri");
  Serial.begin(9600);
}
/*********************************void loop***********************/
void loop() {
    MyData.hiz=50;
    MyData.rpm=258;
    MyData.gerilim=40;
    MyData.temp=37;
    MyData.sarj_durum=60;
    MyData.power = 12;             
    MyData.akim = 15;             
  
    hiz = MyData.hiz; // km/h
    rpm = MyData.rpm;
    gerilim = MyData.gerilim; //V
    sicaklik= MyData.temp; //DEGRE
    sarj_durum = MyData.sarj_durum;
    power = MyData.power; //W            
    akim = MyData.akim;  //A
    /*********************** print value to serial monitor *****************************/
    Serial.print("hiz: ");Serial.print(hiz);Serial.println(" km/h");
    Serial.print("rpm: ");Serial.print(rpm);Serial.println(" rpm");
    /*********************** print value to lcd screen *****************************/
    lcd.setCursor(0, 1);
    lcd.print("hiz: ");
    lcd.setCursor(5, 1);
    lcd.print(MyData.hiz);
    lcd.print("km/h");

    lcd.setCursor(10, 1);
    lcd.print("Temp:");
    lcd.setCursor(15, 1);
    lcd.print(MyData.temp);

    lcd.setCursor(0, 2);
    lcd.print("ger: ");
    lcd.setCursor(5, 2);
    lcd.print(gerilim);
    lcd.print("V");

    lcd.setCursor(10, 2);
    lcd.print("Amp: ");
    lcd.setCursor(15, 2);
    lcd.print(akim);
    lcd.print("A");

    lcd.setCursor(0, 3);
    lcd.print("sarj: ");
    lcd.setCursor(5, 3);
    lcd.write(0);
    lcd.write(1);
    lcd.write(2);
    lcd.print("%");
    lcd.print(sarj_durum);


}
