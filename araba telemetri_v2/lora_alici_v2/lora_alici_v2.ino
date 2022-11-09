
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

#include <SoftwareSerial.h>
#include "EBYTE.h"

#define PIN_RX 2
#define PIN_TX 3
#define PIN_M0 4
#define PIN_M1 5
#define PIN_AX 6



struct DATA {
  unsigned long Count;
  int Bits;
  float Volts;
  float Amps;

};


int Chan;
DATA MyData;
unsigned long Last;

// connect to any digital pin to connect to the serial port
// don't use pin 01 and 1 as they are reserved for USB communications
SoftwareSerial ESerial(PIN_RX, PIN_TX);

// create the transceiver object, passing in the serial and pins
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

void setup() {
  // lcd ekran
  lcd.init();
  lcd.backlight();
  lcd.begin (20, 4);
  lcd.setCursor(2, 0);
  lcd.print("Tel ve AKS veri");
  Serial.begin(9600);


  Serial.begin(9600);

  ESerial.begin(9600);
  Serial.println("Starting Reader");

  // this init will set the pinModes for you
  Transceiver.init(); 
  Transceiver.PrintParameters();
}

void loop() {
  if (ESerial.available()) {

    Transceiver.GetStruct(&MyData, sizeof(MyData));
    Serial.print("Count: "); Serial.println(MyData.Count);
    Serial.print("Bits: "); Serial.println(MyData.Bits);
    Serial.print("Volts: "); Serial.println(MyData.Volts);

    lcd.setCursor(0, 1);
    lcd.print("hiz: ");
    lcd.setCursor(4, 1);
    lcd.print(MyData.Count);
    lcd.print("km/h");

    lcd.setCursor(12, 1);
    lcd.print("Temp:");
    lcd.setCursor(17, 1);
    lcd.print(MyData.Bits);
    //lcd.print("__");

    lcd.setCursor(0, 2);
    lcd.print("ger: ");
    lcd.setCursor(4, 2);
    lcd.print(MyData.Volts);
    lcd.print("V");
/*
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
    lcd.print(sarj_durum);*/

    /*lcd.setCursor(12, 3);
    lcd.print("pwr: ");
    lcd.setCursor(16, 3);
    lcd.print(power );
    lcd.print("W");*/
   /* hiz =0;
    rpm = MyData.rpm;
    gerilim = MyData.gerilim; //V
    sicaklik= MyData.temp; //DEGRE
    sarj_durum = MyData.sarj_durum;
    power = MyData.power; //W            
    akim = MyData.akim;  //A*/
   /* Serial.print(hiz);
    Serial.print(",");
    Serial.print(gerilim);
    Serial.print(",");
    Serial.print(sicaklik);
    Serial.print(",");
    Serial.print(sarj_durum);
    Serial.print(",");
    Serial.print(akim );
    Serial.print(",");
    delay(1000);*/



    
    Last = millis();

  }
  else {
    if ((millis() - Last) > 1000) {
      Serial.println("Searching: ");
      Last = millis();
    }

  }
}
