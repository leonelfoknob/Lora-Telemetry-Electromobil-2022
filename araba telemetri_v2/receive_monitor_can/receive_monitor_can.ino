#include <SPI.h>

#include "mcp2515_can.h"

const int SPI_CS_PIN = 10;

mcp2515_can CAN(SPI_CS_PIN); // Set CS pin


uint8_t Flag_Recv = 0;
uint8_t len = 0;
uint8_t buf[8];
uint32_t canId = 0x000;


float power,energy, volts,currents;
int door,max_santigrat,mot_temp,speeds,smoke,a=0,b=0;

void setup() {
   

  Serial.begin(115200);

  SPI.begin();                       //Begins SPI communication

  pinMode(2,INPUT);

  
  if(CAN.begin(CAN_1000KBPS) == CAN_OK) 
     Serial.print("can init ok!!\r\n");
  else 
     Serial.print("Can init fail!!\r\n");
     
  delay(2000);
}

void loop() {

CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
     canId = CAN.getCanId(); 
   //} 
      if(canId == 54){
          byte hexArray[] = {buf[0],buf[1],buf[2],buf[3]};
          byte hexArray_1[] = {buf[4],buf[5],buf[6],buf[7]};          
          memcpy(&volts, hexArray,sizeof(hexArray) );//volt = volt = volt_total
          memcpy(&currents, hexArray_1, sizeof(hexArray_1));//curent = final_curent
         CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
         canId = CAN.getCanId();
      }
      else if(canId == 55){
          byte hexArray_2[] = {buf[0],buf[1],buf[2],buf[3]};
          byte hexArray_3[] = {buf[4],buf[5],buf[6],buf[7]};          
          memcpy(&power, hexArray_2,sizeof(hexArray_2) );//power = power_val
          memcpy(&energy, hexArray_3, sizeof(hexArray_3)); //energy = final energy _ value
          CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
          canId = CAN.getCanId();      
      }
      /*else if(canId == 56){
          byte hexArray_4[] = {buf[0],buf[1],buf[2],buf[3]};
          byte hexArray_5[] = {buf[4],buf[5],buf[6],buf[7]};          
          memcpy(&power, hexArray_2,sizeof(hexArray_2) );//power = power_val
          memcpy(&energy, hexArray_3, sizeof(hexArray_3)); //energy = final energy _ value
          CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
          canId = CAN.getCanId();  
      }*/
      Serial.print(String("<")+ String(78)+String(",")+String(volts)+String(",")+String(currents)+String(",")+String(power)+String(",")+String(energy));
       Serial.println(String(",")+String(max_santigrat)+String(",")+ String(mot_temp));
       delay(50);

}
