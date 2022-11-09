#include <SPI.h>
#include "stdio.h"
#include "string.h"
#include "mcp2515_can.h"

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
SoftwareSerial ESerial(PIN_RX, PIN_TX);
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

/******************************* hall effect *************/
const float circumference = 75.36; // Write Wheel Circumference *cm -2*pi*r-
uint8_t hall_Thresh = 3; // "10 value +25rpm" set number of hall trips for RPM reading (higher improves accuracy) ----------> 5şer 5şer artıyoruz
uint8_t hall_Count; // Counter for each spin

/*******************************can bus********************/
const int SPI_CS_PIN = 10;
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
uint8_t Flag_Recv = 0;
uint8_t len = 0;
uint8_t buf[8];
uint32_t canId = 0x000;
String str;
int a=0;
float power,energy, volts,currents,max_temp;



void setup() {
    MyData.volts = 0;
    MyData.currents = 0;
    MyData.power = 0;
    MyData.energy = 0;
    MyData.max_temp = 0;
//---------------------------------->can  bus
Serial.begin(9600);  
SPI.begin();
pinMode(2,INPUT);//---------------------pin 9 yapariz gerekirse
if(CAN.begin(CAN_1000KBPS) == CAN_OK){ 
     Serial.print("can init ok!!\r\n");}
  else {
     Serial.print("Can init fail!!\r\n");}
  //delay(2000);------------------> açılacak geregirse
// --------------------------------->lcd ekran
  lcd.init();
  lcd.backlight();
  lcd.begin (20, 4);
  lcd.setCursor(2, 0);
  lcd.print("Tel ve AKS veri");
//----------------------------------->lora
  Serial.begin(9600);
  ESerial.begin(9600);
  Transceiver.init();
  Transceiver.PrintParameters();
 //---------------------------->hall effect
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
        hall_Count++;
      }
    }
    else {
      on_state = false;
    }

    clear_LCD_RpmSpeed(start);

    if (hall_Count >= hall_Thresh){
      break;
    }
  } 
  float end_Time = millis();
  float time_Passed = (end_Time - start) / 1000;
  int hiz = calc_Speed(time_Passed);
  //********************************************** can bus            
   /*  CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
     canId = CAN.getCanId();
      if(canId == 54){
          byte hexArray[] = {buf[0],buf[1],buf[2],buf[3]};
          byte hexArray_1[] = {buf[4],buf[5],buf[6],buf[7]};          
          memcpy(&volts, hexArray,sizeof(hexArray) );
          memcpy(&currents, hexArray_1, sizeof(hexArray_1));
         CAN.readMsgBuf(&len, buf);
         canId = CAN.getCanId();
      }
      else if(canId == 55){
          byte hexArray_2[] = {buf[0],buf[1],buf[2],buf[3]};
          byte hexArray_3[] = {buf[4],buf[5],buf[6],buf[7]};          
          memcpy(&power, hexArray_2,sizeof(hexArray_2) );
          memcpy(&energy, hexArray_3, sizeof(hexArray_3)); 
          CAN.readMsgBuf(&len, buf);
          canId = CAN.getCanId();      
      }
      else if(canId == 56){
          max_temp = buf[0]; 
          CAN.readMsgBuf(&len, buf);
          canId = CAN.getCanId(); 
      }    */

     /*a++;
     if(a==20){
       Serial.print(String(volts)+String(",")+String(currents)+String(",")+String(power)+String(",")+String(energy));
       Serial.print(String(",")+String(max_temp)+String(",")+ String()+String(",")+ String());
       Serial.println(String(",") + String()+ String(",") + String()+ String(">"));
       a=0;
     }
    delay(50);*/
  //**********************************************data we will send
      //--------------- gerçek
   MyData.hiz = hiz;
    MyData.volts = volts;
    MyData.currents = currents;
    MyData.power = power;
    MyData.energy = energy;
    MyData.max_temp = max_temp;
        //--------------fake
    
  Transceiver.SendStruct(&MyData, sizeof(MyData));

  //**********************************************lcd screen write
    lcd.setCursor(0, 1);
    lcd.print("hiz: ");
    lcd.setCursor(4, 1);
    lcd.print(MyData.hiz);
    lcd.print("km/h");

    lcd.setCursor(12, 1);
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
    lcd.print("W");
  
  delay(1);


}//end of void function

/*********************************************function *********************************/

// Calculation Speed
float calc_Speed(float passed_Time)
{

  // Calculation Wheel Speed 
  float m_Speed = (circumference / 100000) * hall_Count / (passed_Time / 3600);
  return m_Speed;
}

void clear_LCD_RpmSpeed(float start_Time)
{
    int hiz = 0;
    /*MyData.volts = 0;
    MyData.currents = 0;
    MyData.power = 0;
    MyData.energy = 0;
    MyData.max_temp = 0; */
    //************* fake
    MyData.volts = 0;
    MyData.currents = 0;
    MyData.power = 0;
    MyData.energy = 0;
    MyData.max_temp = 0;
    float e_Time = millis();
    //if((e_Time - start_Time) / 1000 > 12)
    if((e_Time - start_Time) / 1000 > 12)
    {
      lcd.setCursor(0, 1);
      lcd.print("hiz: ");
      lcd.setCursor(4, 1);
      lcd.print(hiz);
      lcd.print("km/h"); 

      lcd.setCursor(12, 1);
    lcd.print("Temp:");
    lcd.setCursor(17, 1);
    lcd.print(max_temp);

    lcd.setCursor(0, 2);
    lcd.print("ger: ");
    lcd.setCursor(4, 2);
    lcd.print(volts);
    lcd.print("V");

    lcd.setCursor(12, 2);
    lcd.print("Amp: ");
    lcd.setCursor(16, 2);
    lcd.print(currents);
    lcd.print("A");

    lcd.setCursor(0, 3);
    lcd.print("pwr: ");
    lcd.print(":");
    lcd.setCursor(4, 3);
    lcd.print("%");
    lcd.print(power);

    lcd.setCursor(12, 3);
    lcd.print("Eng: ");
    lcd.setCursor(16, 3);
    lcd.print(energy);
    lcd.print("W");
  //  Transceiver.SendStruct(&MyData, sizeof(MyData));
    }
}
