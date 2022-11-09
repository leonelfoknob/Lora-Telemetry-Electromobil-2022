//verici_1
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
  float rpm_val;
  float speed_val;
};

int Chan;
DATA MyData;


const float circumference = 38.82; // Write Wheel Circumference *cm -2*pi*r-
uint8_t hall_Thresh = 10; // "10 value +25rpm" set number of hall trips for RPM reading (higher improves accuracy) ----------> 5şer 5şer artıyoruz
uint8_t hall_Count; // Counter for each spin


SoftwareSerial ESerial(PIN_RX, PIN_TX);
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.begin (20, 4);

  Serial.begin(9600);
  ESerial.begin(9600);
  //Serial.println("Starting Sender");
  Transceiver.init();
  //Transceiver.PrintParameters();
  lcd.setCursor(4, 0);
  lcd.print("Lora Verici");
  //lcd.setCursor(7, 1);
  //lcd.print("basliyor...");
  
}

void loop() {
/************************************* hız *********************************************************/

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

  MyData.speed_val = calc_Speed(time_Passed);
  MyData.rpm_val = calc_Rpm(time_Passed);
  count.count

  delay(1);// delay in between reads for stability

/************************************* hız *********************************************************/
  
  //MyData.Count++;
  Transceiver.SendStruct(&MyData, sizeof(MyData));
  Serial.print("Sending: "); 
  //Serial.println(MyData.Count);
  //Serial.println(MyData.rpm_val);
  Serial.println(MyData.speed_val);
  lcd.setCursor(0, 0);
  lcd.print("Hiz:");
  lcd.setCursor(5, 0);
  lcd.print(MyData.speed_val);
  /*lcd.setCursor(1, 2);
  lcd.print(MyData.rpm_val);
  lcd.setCursor(7, 2);
  lcd.print("rpm");
  lcd.setCursor(1, 3);
  lcd.print(MyData.speed_val);
  lcd.setCursor(6, 3);
  lcd.print("km/h");*/
  delay(1);

}

// Calculation Speed
int calc_Speed(float passed_Time)
{

  // Calculation Wheel Speed 
  float m_Speed = (circumference / 100000) * hall_Count / (passed_Time / 3600);
  return m_Speed;
  //Serial.print((int)m_Speed);
  //Serial.println(" km/h");
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
   /* Serial.println("0  rpm");
    Serial.println("0  km/h");*/
    lcd.setCursor(1, 2);
    lcd.print("0  rpm");
    lcd.setCursor(1, 3);
    lcd.print("0  km/h");  
    }
}
