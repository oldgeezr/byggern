// demo: CAN-BUS Shield, receive data
#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

MCP_CAN CAN0(53);                               // Set CS to pin 10


void setup()
{
  Serial.begin(9600);
  CAN0.begin(CAN_500KBPS);                       // init can bus : baudrate = 500k 
  pinMode(2, INPUT);                            // Setting pin 2 for /INT input
  Serial.println("MCP2515 Library Receive Example...");
  
  pinMode(A6, OUTPUT); //Direction
  pinMode(A5, OUTPUT); //Enable
  analogWrite(A6,0);
  analogWrite(A5,255);
  Wire.begin(); // join i2c bus (address optional for master)
  Wire.beginTransmission(byte(0b0101000)); // transmit to device #44 (0x2c)
  Wire.write(0);            // sends instruction byte  
  Wire.write(255);             // sends potentiometer value byte  
  Wire.endTransmission();     // stop transmitting
  
}

void set_speed(int fart) {
 
  Wire.beginTransmission(byte(0b0101000)); // transmit to device #44 (0x2c)
  Wire.write(0);            // sends instruction byte  
  Wire.write(fart);             // sends speed value byte  
  Wire.endTransmission();     // stop transmitting
} 

void loop()
{
    rxBuf[0] = 4;
    if(!digitalRead(2))                         // If pin 2 is low, read receive buffer
    {
      CAN0.readMsgBuf(&len, rxBuf);              // Read data: len = data length, buf = data byte(s)
      rxId = CAN0.getCanId();                    // Get message ID
      Serial.print("ID: ");
      Serial.print(rxId, HEX);
      Serial.print("  Data: ");
      for(int i = 0; i<len; i++)                // Print each byte of the data
      {
        if(rxBuf[i] < 0x10)                     // If data byte is less than 0x10, add a leading zero
        {
          Serial.print("0");
        }
        Serial.print(rxBuf[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
    switch(rxBuf[0]) {
       case 0: //left
         set_speed(100);
         analogWrite(A6,0);
       case 1: //right
         set_speed(100);
         analogWrite(A6,255);
       default:
         set_speed(0);  
    }
    delay(100);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
