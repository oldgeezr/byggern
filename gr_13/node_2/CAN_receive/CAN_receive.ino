
#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

MCP_CAN CAN0(53); // Set CS to pin 10

#define OE A2
#define RST A3
#define SEL A4
#define EN A5
#define DIR A6

void setup() {
  
  Serial.begin(9600);
  Serial.println("MCP2515 CAN Receive init...");
  
  //CAN init////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  
  CAN0.begin(CAN_500KBPS);
  pinMode(2, INPUT); 
  
  //Carriage control////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  
  pinMode(DIR, OUTPUT); //Direction
  pinMode(EN, OUTPUT); //Enable
  digitalWrite(DIR,LOW);
  digitalWrite(EN,HIGH);
  
  Wire.begin(); 

  //////////////////////////////////////////////////////////////////////////
  
  pinMode(OE, OUTPUT); //!OE
  pinMode(SEL, OUTPUT); //SEL
  pinMode(RST, OUTPUT); //RST
  
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);
  pinMode(A15, INPUT);
  
}

int read_decoder() {
  
  uint8_t MSB[8];
  uint8_t LSB[8];
  
  digitalWrite(SEL,LOW); //Sel
  digitalWrite(OE,LOW); //!OE
  delayMicroseconds(25);
  
  //MSB = PINK;
  MSB[0] = digitalRead(A8);
  MSB[1] = digitalRead(A9);
  MSB[2] = digitalRead(A10);
  MSB[3] = digitalRead(A11);
  MSB[4] = digitalRead(A12);
  MSB[5] = digitalRead(A13);
  MSB[6] = digitalRead(A14);
  MSB[7] = digitalRead(A15);
  
  digitalWrite(SEL,HIGH); //Sel
  delayMicroseconds(25);
  
  //LSB = PINK;
  LSB[0] = digitalRead(A8);
  LSB[1] = digitalRead(A9);
  LSB[2] = digitalRead(A10);
  LSB[3] = digitalRead(A11);
  LSB[4] = digitalRead(A12);
  LSB[5] = digitalRead(A13);
  LSB[6] = digitalRead(A14);
  LSB[7] = digitalRead(A15);
  
  digitalWrite(RST,LOW); //Rst
  delayMicroseconds(5);
  digitalWrite(RST,HIGH); //Rst
  
  digitalWrite(OE,HIGH); //!OE
  
  //Serial.print(MSB);
  //Serial.print(LSB);
  
  for(int i = 0; i < 8; i++) {
     Serial.print(MSB[i]); 
  }
  for(int i = 0; i < 8; i++) {
     Serial.print(LSB[i]); 
  }
  Serial.println();
  
  return 0;
  //return (MSB << 4) + LSB);
  
}

void carriage_speed(int v) {
  Wire.beginTransmission(byte(0b0101000));
  Wire.write(0); 
  Wire.write(v); 
  Wire.endTransmission();
}

void carriage_right() {
   digitalWrite(DIR,HIGH);
}

void carriage_left() {
   digitalWrite(DIR,LOW);
}

void loop()
{    
    /*
    if(!digitalRead(2))                         // If pin 2 is low, read receive buffer
    {
      CAN0.readMsgBuf(&len, rxBuf);              // Read data: len = data length, buf = data byte(s)
      rxId = CAN0.getCanId();                    // Get message ID
      
      Serial.print("ID: ");
      Serial.print(rxId, HEX);
      Serial.print("  Data: ");
      for(int i = 0; i<len; i++)                // Print each byte of the data
      {        if(rxBuf[i] < 0x10)                     // If data byte is less than 0x10, add a leading zero
        {
          Serial.print("0");
        }
        Serial.print(rxBuf[i], HEX);
        Serial.print(" ");
      }
      Serial.println(rxBuf[0]);
   
      switch(rxBuf[0]) {
        case 'L': //left
          carriage_speed(110);
          carriage_left();
          break;
        case 'R': //right
          carriage_speed(110);
          carriage_right();
          break;
        default:
          carriage_speed(0);
          break;
       }
    }
    */
    
    read_decoder();
    delay(100);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
