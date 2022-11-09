
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
};
int Chan;
DATA MyData;
SoftwareSerial ESerial(PIN_RX, PIN_TX);
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

/******************************* hall effect *************/
const float circumference = 75.36; // Write Wheel Circumference *cm -2*pi*r-
uint8_t hall_Thresh = 3; // "10 value +25rpm" set number of hall trips for RPM reading (higher improves accuracy) ----------> 5şer 5şer artıyoruz
uint8_t hall_Count; // Counter for each spin

// Calculation Speed
float calc_Speed(float passed_Time)
{

  // Calculation Wheel Speed 
  float m_Speed = (circumference / 100000) * hall_Count / (passed_Time / 3600);
  return m_Speed;
}

// Calculation RPM
void calc_Rpm(float passed_Time)
{
  // Calculation RPM 
  float rpm_Val =(hall_Count/passed_Time) * 60;
  Serial.print((int)rpm_Val);
  Serial.println("rpm");
}

void clear_LCD_RpmSpeed(float start_Time)
{
    // set 0 values on lcd screen     
    float e_Time = millis();
    if((e_Time - start_Time) / 1000 > 12)
    {
    Serial.println("0  rpm");
    Serial.println("0  km/h");  
    }
}

void setup() {
// lcd ekran
  lcd.init();
  lcd.backlight();
  lcd.begin (20, 4);
  lcd.setCursor(2, 0);
  lcd.print("Tel ve AKS veri");
//lora
  Serial.begin(9600);
  ESerial.begin(9600);
  Transceiver.init();
  Transceiver.PrintParameters();
 //hall effect
  pinMode(8, INPUT);

}

void loop() {
  //hall effect
  hall_Count = 1;
  float start = millis();
  bool on_state = false;
  while (1) {
    if (digitalRead(8) == 0) {
      if (on_state == false) {
        on_state = true;
        hall_Count++;                   // Increase counter in each spin 
      }
    }
    else {
      on_state = false;
    }

    clear_LCD_RpmSpeed(start);          // Control whether movement

    if (hall_Count >= hall_Thresh){
      break;
    }
  } 
  float end_Time = millis();
  float time_Passed = (end_Time - start) / 1000;
  int hiz = calc_Speed(time_Passed);
  //calc_Rpm(time_Passed);
  
  MyData.hiz = hiz;
  Transceiver.SendStruct(&MyData, sizeof(MyData));
  delay(1000);


}
