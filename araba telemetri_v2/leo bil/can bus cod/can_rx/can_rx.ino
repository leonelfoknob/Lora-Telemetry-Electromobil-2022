
#include <SPI.h>              //Library for using SPI Communication 
#include <mcp2515.h>
struct can_frame canMsg; 
MCP2515 mcp2515(10);                 // SPI CS Pin 10 
 
void setup() { 
  SPI.begin();                       //Begins SPI communication
  Serial.begin(9600);                //Begins Serial Communication at 9600 baudrate 
  mcp2515.reset();  
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz 
  mcp2515.setNormalMode();                  //Sets CAN at normal mode
}

void loop() 
{
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) // To receive data (Poll Read)
  {
    if(canMsg.can_id==0x036)
    {
     int guc = canMsg.data[0];  
     int power = canMsg.data[0]; 
     int voltage = canMsg.data[0]; 
     int akim = canMsg.data[0]; 
     int temp =  canMsg.data[0];       
     Serial.print("guc :");Serial.println(guc);
     Serial.print("power :");Serial.println(power);
     Serial.print("voltage :");Serial.println(voltage);
     Serial.print("akim :");Serial.println(akim);
     Serial.print("temp :");Serial.println(temp);
     delay(500);
     }
    }
   
}                    
