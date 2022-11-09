/**************************************************************verici***************************************************************/
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
/*********** hall effect sensor****************/
const float circumference = 75.36; //çapimiz 12 Write Wheel Circumference *cm -2*pi*r-
uint8_t hall_Thresh = 3; // "10 value +25rpm" set number of hall trips for RPM reading (higher improves accuracy) ----------> 3şer 3şer artıyoruz
uint8_t hall_Count; // Counter for each spin
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


// Calculation Speed
float calc_Speed(float passed_Time)
{
  // Calculation Wheel Speed 
  float m_Speed = (circumference / 100000) * hall_Count / (passed_Time / 3600);
  return m_Speed;
}

// Calculation RPM
float calc_Rpm(float passed_Time)
{
  // Calculation RPM 
  float rpm_Val =(hall_Count/passed_Time) * 60;
  return rpm_Val;
}
void clear_LCD_RpmSpeed(float start_Time)
{
    // set 0 values on lcd screen     
    float e_Time = millis();
    if((e_Time - start_Time) / 1000 > 12)
    {
      MyData.hiz = 0;
      MyData.rpm = 0;
      Transceiver.SendStruct(&MyData, sizeof(MyData));
    Serial.print("hiz: ");Serial.print(MyData.hiz);Serial.println(" km/h");
    Serial.print("rpm: ");Serial.print(MyData.rpm);Serial.println(" rpm");
    MyData.rpm= 258;
    MyData.gerilim=100;
    MyData.temp=37;
    MyData.sarj_durum=45;
    MyData.power = 12;             
    MyData.akim = 15;

    hiz = MyData.hiz; // km/h
    rpm = MyData.rpm;
    gerilim = MyData.gerilim; //V
    sicaklik= MyData.temp; //DEGRE
    sarj_durum = MyData.sarj_durum;
    power = MyData.power; //W            
    akim = MyData.akim;  //A

  
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
  lcd.createChar(0, pil1);
  lcd.createChar(1, pil2);
  lcd.createChar(2, pil3);
  lcd.begin (20, 4);
  lcd.setCursor(2, 0);
  lcd.print("Tel ve AKS veri");
  pinMode(8, INPUT);
  Serial.begin(9600);
}
/*********************************void loop***********************/
void loop() {
  /********************** rpm and speed calculation**************/
  // Initialize values
  hall_Count = 1;
  float start = millis();
  bool on_state = false;
  // Calculation Spin Time
  while (1) {
    if (digitalRead(8) == 0) 
    {
      if (on_state == false) 
      {
        on_state = true;
        hall_Count++;                   // Increase counter in each spin 
      }
    }
    else 
    {
      on_state = false;
    }
    clear_LCD_RpmSpeed(start);         // Control whether movement
    if (hall_Count >= hall_Thresh)      // Control counter overflow as threshold 
    {
      break;
    }
  } 
  // Calculation Passed Time 
  float end_Time = millis();
  float time_Passed = (end_Time - start) / 1000;
  MyData.hiz = calc_Speed(time_Passed); // speed value
  MyData.rpm = calc_Rpm(time_Passed);// rpm value
  /******************************** lora send*****************/
  Transceiver.SendStruct(&MyData, sizeof(MyData));
  /*********************** print value to serial monitor *****************************/
  Serial.print("hiz: ");Serial.print(MyData.hiz);Serial.println(" km/h");
  Serial.print("rpm: ");Serial.print(MyData.rpm);Serial.println(" rpm");
  /*********************** print value to lcd screen *****************************/
  /***********************değerelerimiz*******************/

    MyData.rpm= 258;
    MyData.gerilim=100;
    MyData.temp=37;
    MyData.sarj_durum=45;
    MyData.power = 12;             
    MyData.akim = 15;
    //MyData.hiz = 50;

    hiz = MyData.hiz; // km/h
    rpm = MyData.rpm;
    gerilim = MyData.gerilim; //V
    sicaklik= MyData.temp; //DEGRE
    sarj_durum = MyData.sarj_durum;
    power = MyData.power; //W            
    akim = MyData.akim;  //A

  
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

  delay(1);// delay in between reads for stability
}
