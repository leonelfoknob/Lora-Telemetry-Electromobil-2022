/*****************************************can bus deneme*************************/
#include <SPI.h>

#include "stdio.h"
#include "string.h"

#include "mcp2515_can.h"

const int SPI_CS_PIN = 10;

mcp2515_can CAN(SPI_CS_PIN); // Set CS pin

uint8_t Flag_Recv = 0;
uint8_t len = 0;
uint8_t buf[8];
uint32_t canId = 0x000;

String str;
int a=0;

float power,energy, volts,currents,max_temp;


void setup(){
  // Open serial communications and wait for port to open:

  Serial.begin(9600);  
  SPI.begin();                       //Begins SPI communication

  pinMode(2,INPUT);
  
  if(CAN.begin(CAN_1000KBPS) == CAN_OK) 
     Serial.print("can init ok!!\r\n");
  else 
     Serial.print("Can init fail!!\r\n");
  delay(2000);

}

void loop() { // run over and over 
   //if(!digitalRead(2))                   // check if data was recieved
   //{             
     CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
     canId = CAN.getCanId(); 
   //} 
      if(canId == 54){
          byte hexArray[] = {buf[0],buf[1],buf[2],buf[3]};
          byte hexArray_1[] = {buf[4],buf[5],buf[6],buf[7]};          
          memcpy(&volts, hexArray,sizeof(hexArray) );
          memcpy(&currents, hexArray_1, sizeof(hexArray_1));
         CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
         canId = CAN.getCanId();
      }
      else if(canId == 55){
          byte hexArray_2[] = {buf[0],buf[1],buf[2],buf[3]};
          byte hexArray_3[] = {buf[4],buf[5],buf[6],buf[7]};          
          memcpy(&power, hexArray_2,sizeof(hexArray_2) );
          memcpy(&energy, hexArray_3, sizeof(hexArray_3)); 
          CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
          canId = CAN.getCanId();      
      }
      else if(canId == 56){
          max_temp = buf[0]; 
          CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
          canId = CAN.getCanId(); 
      }     

     a++;
     if(a==20){
       Serial.print(String(volts)+String(",")+String(currents)+String(",")+String(power)+String(",")+String(energy));
       Serial.print(String(",")+String(max_temp)+String(",")+ String()+String(",")+ String());
       Serial.println(String(",") + String()+ String(",") + String()+ String(">"));
       a=0;
     }
    delay(50);
}
