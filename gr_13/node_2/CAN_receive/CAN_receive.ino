
#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <avr/interrupt.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

Servo myservo;

int x_axis;

MCP_CAN CAN0(53); // Set CS to pin 10

#define OE A2
#define RST A3
#define SEL A4
#define EN A5
#define DIR A6

void setup() {
  
  /*===============USART=================*/
  
  Serial.begin(9600);
  Serial.println("MCP2515 CAN Receive init...");
  
  /*==============INTERRUPTS==============*/
  // initialize Timer1
  noInterrupts(); // disable all interrupts
  TCCR3A = 0;// set entire TCCR2A register to 0
  TCCR3B = 0;// same for TCCR2B
  TCNT3  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR3A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR3A |= (1 << WGM31);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR3B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK3 |= (1 << OCIE3A);
  interrupts(); // enable all interrupts
  /*==============CAN INIT==============*/
  
  CAN0.begin(CAN_500KBPS);
  pinMode(2, INPUT); 
  
  /*==============MOTORBOX==============*/
  
  pinMode(DIR, OUTPUT); //Direction
  pinMode(EN, OUTPUT); //Enable
  digitalWrite(DIR,LOW);
  digitalWrite(EN,HIGH);
  
  Wire.begin(); 
  
  /*==============DECODER==============*/
  
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
  
  ///Servo///////////////////////////////////////////////////////////////////
  myservo.attach(4);
  
  pinMode(A7,INPUT);
  
  //Solenoid/////////////////////////////////////////////////////////////////
  pinMode(13,OUTPUT);
  
  carriage_init();
  
}

int read_decoder() {
 
  uint8_t MSB_LSB[16];
  int temp = 0;
  
  digitalWrite(SEL,LOW); //Sel
  digitalWrite(OE,LOW); //!OE
  delayMicroseconds(25);
  
  MSB_LSB[0] = digitalRead(A8);
  MSB_LSB[1] = digitalRead(A9);
  MSB_LSB[2] = digitalRead(A10);
  MSB_LSB[3] = digitalRead(A11);
  MSB_LSB[4] = digitalRead(A12);
  MSB_LSB[5] = digitalRead(A13);
  MSB_LSB[6] = digitalRead(A14);
  MSB_LSB[7] = digitalRead(A15);
  
  digitalWrite(SEL,HIGH); //Sel
  delayMicroseconds(25);
  
  
  
  MSB_LSB[8] = digitalRead(A8);
  MSB_LSB[9] = digitalRead(A9);
  MSB_LSB[10] = digitalRead(A10);
  MSB_LSB[11] = digitalRead(A11);
  MSB_LSB[12] = digitalRead(A12);
  MSB_LSB[13] = digitalRead(A13);
  MSB_LSB[14] = digitalRead(A14);
  MSB_LSB[15] = digitalRead(A15);
  
  digitalWrite(RST,LOW); //Rst
  delayMicroseconds(5);
  digitalWrite(RST,HIGH); //Rst
  
  digitalWrite(OE,HIGH); //!OE
  
  for(int i = 0; i < 15; i++) {
    if(MSB_LSB[0]) {
      temp += (!MSB_LSB[15-i])*(2^(i)+1);
    } else {
      temp += (MSB_LSB[15-i])*(2^(i));      
    }
  }
  if(MSB_LSB[0]) {
    temp = temp * (-1);
  }
  
  return temp;
  
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

void carriage_init() {
  carriage_speed(255);
  carriage_left();
  int decoder = 10;
  delay(300);
  carriage_speed(0);
  x_axis = 0;
}

void get_position() {
  
}

ISR(Timer0_COMPA_vect) // timer compare interrupt service routine
{
  Serial.println("Interrupt!");
}

void loop()
{    
    
    if(!digitalRead(2))                         // If pin 2 is low, read receive buffer
    {
      CAN0.readMsgBuf(&len, rxBuf);              // Read data: len = data length, buf = data byte(s)
      rxId = CAN0.getCanId();                    // Get message ID
      /*
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
      */
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
       
       uint8_t pos = 150-rxBuf[1];
       if((pos > 50) && (pos < 140)) {
         myservo.write(pos);
       }
       
       switch(rxBuf[2]) {
        case 1: //left
          digitalWrite(13,HIGH);
          delay(250);
          digitalWrite(13,LOW);
          break;
        case 2: //right
          digitalWrite(13,HIGH);
          delay(250);
          digitalWrite(13,LOW);
          break;
        case 3: //Both
          digitalWrite(13,HIGH);
          delay(250);
          digitalWrite(13,LOW);
          break;
       }
    }
    x_axis -= read_decoder();
    //Serial.println(x_axis);
}



/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
