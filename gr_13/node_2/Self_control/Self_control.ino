
/*********************************************************************************************************
  INCLUDES
*********************************************************************************************************/

#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <TimerThree.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

Servo myservo;

int x_axis;
int score;
int flag = 1;
int state = 0;

uint16_t ball_x = 0;
uint16_t ball_y = 0;

MCP_CAN CAN0(53); // Set CS to pin 53

#define MAX 80
#define CAMERA_OFFSET 10;

#define OE A2
#define RST A3
#define SEL A4
#define EN A5
#define DIR A6

#define SERVO_PIN 4
#define DIODE_PIN A7
#define SOLENOID_PIN 13

#define CENTER 3700 //Needs to be changed...

/*********************************************************************************************************
  SETUP
*********************************************************************************************************/

void setup(void) {  
  /*===============UART=================*/
  
  Serial.begin(9600);
  Serial.println("MCP2515 CAN Receive init...");

  /*==============CAN INIT==============*/
  
  CAN0.begin(CAN_500KBPS);
  pinMode(2, INPUT); 
  
  /*==============MOTORBOX==============*/

  pinMode(DIR, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(DIR,LOW);
  digitalWrite(EN,HIGH);
  
  Wire.begin(); 

  carriage_init();

  /*==============DECODER==============*/

  pinMode(OE, OUTPUT);
  pinMode(SEL, OUTPUT);
  pinMode(RST, OUTPUT); 
  
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);
  pinMode(A15, INPUT);

  /*==============SERVO==============*/

  myservo.attach(SERVO_PIN);
  myservo.write(90);
    
  /*=============DIODE==============*/
  pinMode(A7,INPUT);
  
  /*==============SOLENOID==============*/

  pinMode(SOLENOID_PIN,OUTPUT);
  digitalWrite(SOLENOID_PIN,LOW);

  /*==============INTERRUPTS==============*/
  Timer3.initialize(5000);         // initialize timer1, and set a 1/2 second period
  Timer3.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt

  //carriage_center();

}

uint8_t reverse_byte(uint8_t x) {
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
	x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
	return x;
}

int read_decoder(void) { 
  uint8_t MSB;
  uint8_t LSB;
  
  digitalWrite(SEL,LOW); //Sel
  digitalWrite(OE,LOW); //!OE

  delayMicroseconds(25);
  
  MSB = reverse_byte(PINK);
  
  digitalWrite(SEL,HIGH); //Sel
  delayMicroseconds(25);
  
  LSB = reverse_byte(PINK);
  
  digitalWrite(RST,LOW); //Rst
  delayMicroseconds(5);
  digitalWrite(RST,HIGH); //Rst
  
  digitalWrite(OE,HIGH); //!OE
  
  return ((MSB << 8) | LSB);
}

void carriage_speed(int speed) {
  Wire.beginTransmission(byte(0b0101000)); //DAC addr
  Wire.write(0); 
  Wire.write(speed); 
  Wire.endTransmission();
}

static inline void carriage_right(void) {
   digitalWrite(DIR,HIGH);
}

static inline void carriage_left(void) {
   digitalWrite(DIR,LOW);
}

void carriage_init(void) {
  carriage_speed(155);
  carriage_left();
  delay(600);
  carriage_speed(0);
  delay(200);
  x_axis = 0;
}

uint8_t carriage_goto_pos(uint8_t x_percent) {
    
  uint8_t x = x_axis/MAX;
  
  if(x_percent < 3) {
    x_percent = 4;
  }
  
  if ((x < (x_percent + 3)) && (x > (x_percent - 3))) {
    carriage_speed(0);
    return 0; 
  }
  
  if (x > x_percent) { 
	do {
          x = x_axis/MAX;
          carriage_speed(110);
          carriage_left();
	} while (x >= x_percent);
  } else {
	do {
           x = x_axis/MAX;
           carriage_speed(110);
	   carriage_right();
	} while (x <= x_percent);
  }
  carriage_speed(0);
  return 1;
}

void callback() {
  x_axis -= read_decoder();
}

void print_can_msg(void) {
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
  Serial.println();
}

uint8_t get_diode_status(void) {
  uint8_t temp;
  temp += analogRead(DIODE_PIN);
  temp += analogRead(DIODE_PIN);
  return (temp/2);  
}

void read_pi(void) {
  if (Serial.available() > 0) {
    char temp = Serial.read();         
    if(temp == 'X') {
      while(!(Serial.available() > 0));
      ball_x = Serial.read();
      //ball_x = (Serial.read() * 100) / 255;
    } else if(temp == 'Y'){
      while(!(Serial.available() > 0));
      ball_y = Serial.read();
      //ball_y = (Serial.read() * 100) / 255;   
    }
  }
}

uint8_t calculate_vector(void) {
 
 uint8_t x0,y0,x1,y1,y;
 
 int dx, dy, a;
 
 read_pi();
 x0 = ball_x;
 y0 = ball_y;
 delay(5);
 read_pi();
 x1 = ball_x;
 y1 = ball_y;
 
 if(y1 > y0) {
  //carriage_init();
  return 0;
 }
 
 dy = y0-y1;
 dx = x0-x1;
 
 if(dx == 0) {
   return 0;
 }
 
 a = dy/dx;
 
 y = a*y0 + x0;
 /*
 Serial.print("Vector dy: ");
 Serial.print(dy);
 Serial.print(" dx: ");
 Serial.print(dx);
 Serial.print(" a: ");
 Serial.print(a);
 Serial.print(" y: ");
 Serial.println(y);
 */
 return ((y * 100) / 255); 
}

void score_counter(void) {
 if((get_diode_status() < 25) && flag) {
   score++;
   flag = 0;
   state = 0; 
   unsigned char stmp[1] = {1};
   CAN0.sendMsgBuf(0x05, 0, 1, stmp); 
 }
}

/*********************************************************************************************************
  LOOP
*********************************************************************************************************/

void loop() {
    
    //read_pi();
    //carriage_goto_pos(((ball_x*100)/255));
    //Serial.println(ball_x);
    
    
    
    uint8_t goto_x = calculate_vector();
    
    
    //Serial.println(goto_x);
    
    
    if(goto_x != 0) {
      carriage_goto_pos(goto_x);
    }
    
    read_pi();
    if((ball_y > 40) && (ball_y < 60)) {
        digitalWrite(SOLENOID_PIN,HIGH);
        delay(150);
        digitalWrite(SOLENOID_PIN,LOW);  
    }
    
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
