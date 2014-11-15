
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
uint8_t car_init = 1; 
uint8_t init_right = 1;

MCP_CAN CAN0(53); // Set CS to pin 53

#define MAX 95
#define CAMERA_OFFSET 10;
#define BALL_OFFSET 3

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
  /*===============UART INIT=============*/
  
  Serial.begin(38400);
  Serial.println("MCP2515 CAN Receive init...");

  /*==============CAN INIT==============*/
  
  CAN0.begin(CAN_500KBPS);
  pinMode(2, INPUT); 
  
  /*===========MOTORBOX INIT============*/

  pinMode(DIR, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(DIR,LOW);
  digitalWrite(EN,HIGH);
  
  Wire.begin(); 

  carriage_init_left();

  /*===========DECODER INIT============*/

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

  /*===========SERVO INIT===========*/

  myservo.attach(SERVO_PIN);
  myservo.write(90);
    
  /*===========DIODE INIT===========*/
  pinMode(A7,INPUT);
  
  /*=========SOLENOID INIT==========*/

  pinMode(SOLENOID_PIN,OUTPUT);
  digitalWrite(SOLENOID_PIN,LOW);

  /*========INTERRUPT INIT==========*/
  Timer3.initialize(3000);         // initialize timer1, and set a 3ms period
  Timer3.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt

}

static inline uint8_t reverse_byte(uint8_t x) {
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
	x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
	return x;
}

/*********************************************************************************************************
  DECODER
*********************************************************************************************************/

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

void callback() {
  x_axis -= read_decoder();
}

/*********************************************************************************************************
  CARRIAGE
*********************************************************************************************************/

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

void carriage_init_left(void) {
  carriage_speed(255);
  carriage_left();
  delay(300);
  carriage_speed(0);
  delay(300);
  x_axis = 0;
}

void carriage_init_right(void) {
  carriage_speed(255);
  carriage_right();
  delay(300);
  carriage_speed(0);
  delay(300);
  x_axis = MAX*100;
}

uint8_t carriage_goto_pos(int x_percent) {
  int x = x_axis/MAX;
  
  //Guards
  if(x > 100) {
     x = 100;
  }
  if (x < 0) {
     x = 0;
  }
  
  if(x_percent < BALL_OFFSET) {
    x_percent = BALL_OFFSET;
  }
  if(x_percent > (100-BALL_OFFSET)) {
    x_percent = 100-BALL_OFFSET;
  }
  
  if ((x <= (x_percent + BALL_OFFSET)) && (x >= (x_percent - BALL_OFFSET))) {
    carriage_speed(0);
    return 0; 
  }
  
  if (x > x_percent) { 
  	do {
            x = x_axis/MAX;
            carriage_speed(150);
            carriage_left();
  	} while (x > x_percent);
  } else {
  	do {
             x = x_axis/MAX;
             carriage_speed(150);
  	     carriage_right();
  	} while (x < x_percent);
  }
  carriage_speed(0);
  return 1;
}

void calibrate_carriage(void) {
  carriage_goto_pos(0);
  Serial.print("Goto: 0 x_axis: ");
  Serial.println(x_axis);
  delay(1000);
  carriage_goto_pos(100);
  Serial.print("Goto: 100 x_axis: ");
  Serial.println(x_axis);
  delay(1000); 
  carriage_goto_pos(50);
  Serial.print("Goto: 50 x_axis: ");
  Serial.println(x_axis);
  delay(1000); 
}

/*********************************************************************************************************
  CAN
*********************************************************************************************************/

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

/*********************************************************************************************************
  DIODE
*********************************************************************************************************/

uint8_t get_diode_status(void) {
  uint8_t temp;
  temp += analogRead(DIODE_PIN);
  temp += analogRead(DIODE_PIN);
  return (temp/2);  
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
  Raspberry PI - UART
*********************************************************************************************************/

static inline uint8_t get_x(void) {
    while(!(Serial.available() > 0));
    while(Serial.read() != 'X');
    while(!(Serial.available() > 0));
    return Serial.read();
}

static inline uint8_t get_y(void) {
    while(!(Serial.available() > 0));
    while(Serial.read() != 'Y');
    while(!(Serial.available() > 0));
    return Serial.read();
}

/*********************************************************************************************************
  Self Control - Algorithms
*********************************************************************************************************/

static inline uint8_t calculate_vector(void) {
  uint8_t x0, x1, y0, y1;
  int dx, dy, x;
  float a;

  x0 = get_x();
  y0 = get_y();

  delay(10);

  x1 = get_x();
  y1 = get_y();
  
  if(y1 > y0) {
    return 0;
  }

  dy = y0-y1;
  dx = x1-x0;

  a = (float(dy))/(float(dx)); //A way to avoid float may be to multiply with 100 (percentage conv)
  //a = dy/dx; Check if this works 

  x = y0*a + x0;

  if (x > 255) {
   return 90; //Its bouncing of the wall
  }
  if (x < 0) {
   return 10;
  }
  
  return ((x * 100) / 255); 
}

void func() {
 
 uint8_t x0 = 0;
 uint8_t x1 = 0;
 uint8_t y0 = 0;
 uint8_t y1 = 0; 
 
 //Ball is going upwards
 while(y1 >= y0) { 
   //Serial.println("Going up!");
   y0 = get_y();
   delay(20);
   y1 = get_y();
 }
 
 //Calculate vector when threshold is passed
 //while(get_y() > 170);
 delay(50);
 
 uint8_t goto_x = calculate_vector();
 carriage_goto_pos(goto_x);
 
 //delay(100);
 
 while(1) {
   y0 = get_y();
   x0 = get_x();
   
   //carriage_goto_pos(x0);
   
   if(y0 < 50 && x0 < 40) {
        myservo.write(140);
    } else if(y0 < 50 && x0 > 230) {
        myservo.write(40);
    } else {
        myservo.write(90); 
    }
  
    if((y0 > 5) && (y0 < 30)) {
        digitalWrite(SOLENOID_PIN,HIGH);
        delay(100);
        digitalWrite(SOLENOID_PIN,LOW);
        delay(100);
        digitalWrite(SOLENOID_PIN,HIGH);
        delay(100);
        digitalWrite(SOLENOID_PIN,LOW);
        if(x0 > 50) {
          //init_right = 0;
          carriage_init_right();  
        } else {
          //init_right = 1;
          carriage_init_left();  
        }
        break;
    }  
 }
}

static inline void plan_b(void) {
   
   uint8_t y0 = (get_y()*100)/255;
   uint8_t x0 = (get_x()*100)/255;
   
   carriage_goto_pos(x0);
   
   if(y0 < 50 && x0 < 40) {
        myservo.write(150);
    } else if(y0 < 50 && x0 > 230) {
        myservo.write(50);
    } else {
        myservo.write(100); 
    }
  
    if((y0 > 5) && (y0 < 40)) {
        digitalWrite(SOLENOID_PIN,HIGH);
        delay(100);
        digitalWrite(SOLENOID_PIN,LOW);
        delay(100);
        digitalWrite(SOLENOID_PIN,HIGH);
        delay(100);
        digitalWrite(SOLENOID_PIN,LOW);
        if(x0 > 50) {
          carriage_init_right();  
        } else {
          carriage_init_left();  
        }
    }  
}

/*********************************************************************************************************
  LOOP
*********************************************************************************************************/

void loop() {
   plan_b();  
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
