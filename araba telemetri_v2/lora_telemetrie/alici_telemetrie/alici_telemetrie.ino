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

/********************************************dataları****************/
struct DATA{
  int hiz;
  int rpm;               //gosterilmeyecek
  int gerilim;           //Voltage
  int temp;             //max_temp
  int sarj_durum ;       //energy
  int power;             //guç
  int akim;             //current
};

DATA MyData;

int hiz;
int rpm;      //gosterilmeyecek
int gerilim;
int sicaklik;
int sarj_durum; //
int power;             
int akim;             

/*********************************void setup***********************/
void setup() {
  //lora
  ESerial.begin(9600);
  Transceiver.init();
  Transceiver.PrintParameters();
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
  if (ESerial.available()) {
    Transceiver.GetStruct(&MyData, sizeof(MyData));
    hiz = MyData.hiz; // km/h
    rpm = MyData.rpm;
    gerilim = MyData.gerilim; //V
    sicaklik= MyData.temp; //DEGRE
    sarj_durum = MyData.sarj_durum;
    power = MyData.power; //W            
    akim = MyData.akim;  //A
    /*********************** print value to serial monitor *****************************/
   // Serial.print("hiz: ");Serial.print(hiz);Serial.println(" km/h");
   // Serial.print("rpm: ");Serial.print(rpm);Serial.println(" rpm");
    /*********************** print value to lcd screen *****************************/
        lcd.setCursor(0, 1);
    lcd.print("hiz: ");
    lcd.setCursor(4, 1);
    lcd.print(MyData.hiz);
    lcd.print("km/h");

    lcd.setCursor(12, 1);
    lcd.print("Temp:");
    lcd.setCursor(17, 1);
    //lcd.print(MyData.temp);
    lcd.print("__");

    lcd.setCursor(0, 2);
    lcd.print("ger: ");
    lcd.setCursor(4, 2);
    lcd.print(gerilim);
    lcd.print("V");

    lcd.setCursor(12, 2);
    lcd.print("Amp: ");
    lcd.setCursor(16, 2);
    //lcd.print(akim);
    lcd.print("__");
    lcd.print("A");

    lcd.setCursor(0, 3);//pwr
    lcd.write(0);
    lcd.write(1);
    lcd.write(2);
    lcd.print(":");
    lcd.setCursor(4, 3);
    lcd.print("%");
    lcd.print(sarj_durum);

    /*lcd.setCursor(12, 3);
    lcd.print("pwr: ");
    lcd.setCursor(16, 3);
    lcd.print(power );
    lcd.print("W");*/
    //hiz = MyData.hiz; // km/h
    hiz =0;
    rpm = MyData.rpm;
    gerilim = MyData.gerilim; //V
    sicaklik= MyData.temp; //DEGRE
    sarj_durum = MyData.sarj_durum;
    power = MyData.power; //W            
    akim = MyData.akim;  //A
    Serial.print(hiz);
    Serial.print(",");
    Serial.print(gerilim);
    Serial.print(",");
    Serial.print(sicaklik);
    Serial.print(",");
    Serial.print(sarj_durum);
    Serial.print(",");
    Serial.print(akim );
    Serial.print(",");
    delay(1000);
  }
  else {
    if ((millis() - Last) > 1000) {
    lcd.setCursor(0, 1);
    lcd.print("hiz: ");
    lcd.setCursor(4, 1);
    lcd.print(MyData.hiz);
    lcd.print("km/h");

    lcd.setCursor(12, 1);
    lcd.print("Temp:");
    lcd.setCursor(17, 1);
    //lcd.print(MyData.temp);
    lcd.print("__");

    lcd.setCursor(0, 2);
    lcd.print("ger: ");
    lcd.setCursor(4, 2);
    lcd.print(gerilim);
    lcd.print("V");

    lcd.setCursor(12, 2);
    lcd.print("Amp: ");
    lcd.setCursor(16, 2);
    //lcd.print(akim);
    lcd.print("__");
    lcd.print("A");

    lcd.setCursor(0, 3);//pwr
    lcd.write(0);
    lcd.write(1);
    lcd.write(2);
    lcd.print(":");
    lcd.setCursor(4, 3);
    lcd.print("%");
    lcd.print(sarj_durum);

    /*lcd.setCursor(12, 3);
    lcd.print("pwr: ");
    lcd.setCursor(16, 3);
    lcd.print(power );
    lcd.print("W");*/
    }
  }

}
