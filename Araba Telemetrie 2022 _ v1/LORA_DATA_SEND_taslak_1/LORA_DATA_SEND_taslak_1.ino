/**************************************************************verici***************************************************************/
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
/*********** hall effect sensor****************/
const float circumference = 38.82; // Write Wheel Circumference *cm -2*pi*r-
uint8_t hall_Thresh = 3; // "10 value +25rpm" set number of hall trips for RPM reading (higher improves accuracy) ----------> 3şer 3şer artıyoruz
uint8_t hall_Count; // Counter for each spin

struct DATA{
  int hiz;
  float rpm;
};

DATA MyData;

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
    MyData.hiz=0;
    MyData.rpm = 0;
    lcd.setCursor(0, 0);
    lcd.print("hiz:");
    lcd.setCursor(4, 0);
    lcd.print(MyData.hiz);
    lcd.print("km/h");
    lcd.print("             ");

    lcd.setCursor(10, 0);
    lcd.print("rpm:");
    lcd.setCursor(14, 0);
    lcd.print(MyData.rpm);
    lcd.print("              ");
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
    clear_LCD_RpmSpeed(start);          // Control whether movement
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
  lcd.setCursor(0, 0);
  lcd.print("hiz:");
  lcd.setCursor(4, 0);
  lcd.print(MyData.hiz);
  lcd.print("km/h");

  lcd.setCursor(10, 0);
  lcd.print("rpm:");
  lcd.setCursor(14, 0);
  lcd.print(MyData.rpm);

  delay(1);// delay in between reads for stability
}
