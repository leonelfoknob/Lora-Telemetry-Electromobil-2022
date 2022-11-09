
#include <SD.h>
#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10); // chip select CS pin 10

File myFile;

int pinCS = 9; // Pin 10 on Arduino Uno
int c;

void setup() {
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();

  canMsg.can_id  = 0x036;           //CAN id as 0x036
  canMsg.can_dlc = 1;
   
  Serial.begin(9600);
  pinMode(pinCS, OUTPUT);
  // SD Card Initialization
  if (SD.begin()){
    Serial.println("SD card is ready to use.");
  } 
  else{
    Serial.println("SD card initialization failed");
    return;
  }
  /****************************** read from sd card *************************************/
    /*myFile = SD.open("data_can.txt");
  if (myFile) {
    Serial.println("Read:");
    // Reading the whole file
    while (myFile.available()) {
      Serial.write(myFile.read());
   }
    myFile.close();
  }
  else {
    Serial.println("error opening test.txt");
  }*/
  /******************************************************************* *****************/
}
void loop() {
  c++;
  canMsg.data[0] = c;
  myFile = SD.open("data_can.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    //Serial.println("Writing to file...");
    // Write to file
    myFile.print("can: ");myFile.println(c);
    myFile.close(); // close the file
    mcp2515.sendMessage(&canMsg);
    Serial.println(c);
    delay(1000);
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }
}
