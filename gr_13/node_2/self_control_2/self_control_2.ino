
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
uint8_t car_init = 1; 
uint8_t init_right = 1;
uint8_t going_right = 1;

MCP_CAN CAN0(53); // Set CS to pin 53

#define MAX 92
#define BALL_OFFSET 3

#define AUTOMATIC_SPEED 150
#define MANUAL_SPEED 150

#define OE A2
#define RST A3
#define SEL A4
#define EN A5
#define DIR A6

#define SERVO_PIN 10
#define DIODE_PIN A7
#define SOLENOID_PIN 8

#define DIODE_THRESHOLD 100

enum state_t {
  STANDBY,
  MANUAL,
  AUTO,
  LOOSE
};

state_t previous_state = STANDBY;
state_t state = STANDBY;

/*********************************************************************************************************
  SETUP
*********************************************************************************************************/

void setup(void) {  
  /*===============UART INIT=============*/
  
  Serial.begin(38400);
  Serial.println("MCP2515 CAN Receive init...");
  
  /*===============RASPI INIT=============*/
  
  Serial3.begin(38400);
  Serial.println("Ras PI camera receive init...");

  /*==============CAN INIT==============*/
  
  CAN0.begin(CAN_500KBPS);
  pinMode(2, INPUT); 
  
  /*===========MOTORBOX INIT============*/

  pinMode(DIR, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(DIR,LOW);
  digitalWrite(EN,HIGH);
  
  Wire.begin(); 

  carriage_init_left_slow();

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
  myservo.write(100);
    
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

void carriage_init_left_slow(void) {
  myservo.write(100);
  carriage_speed(110);
  carriage_left();
  delay(1200);
  carriage_speed(0);
  delay(300);
  x_axis = 0;
  myservo.write(100);
}

void carriage_init_left(void) {
  carriage_speed(255);
  carriage_left();
  delay(600);
  carriage_speed(0);
  delay(300);
  x_axis = 0;
}

void carriage_init_right(void) {
  carriage_speed(255);
  carriage_right();
  delay(600);
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
            going_right = 0;
            x = x_axis/MAX;
            carriage_speed(AUTOMATIC_SPEED);
            carriage_left();
  	} while (x > x_percent);
  } else {
  	do {
             going_right = 1;
             x = x_axis/MAX;
             carriage_speed(AUTOMATIC_SPEED);
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
  carriage_goto_pos(25);
  Serial.print("Goto: 50 x_axis: ");
  Serial.println(x_axis);
  delay(1000); 
  carriage_goto_pos(50);
  Serial.print("Goto: 50 x_axis: ");
  Serial.println(x_axis);
  delay(1000); 
  carriage_goto_pos(75);
  Serial.print("Goto: 50 x_axis: ");
  Serial.println(x_axis);
  delay(1000); 
  carriage_goto_pos(100);
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

int get_diode_status(void) {
  int temp = 0;
  temp = analogRead(DIODE_PIN);
  temp += analogRead(DIODE_PIN);
  return (temp/2);  
}

void score_counter(void) {
   if((get_diode_status() < DIODE_THRESHOLD) && flag) {
     score++;
     flag = 0;
     state = LOOSE; 
     unsigned char stmp[1] = {1};
     CAN0.sendMsgBuf('L', 0, 1, stmp);
     CAN0.sendMsgBuf('L', 0, 1, stmp);
     CAN0.sendMsgBuf('L', 0, 1, stmp);
     
     carriage_init_left_slow();  
     
   }
}

/*********************************************************************************************************
  Raspberry PI - UART
*********************************************************************************************************/

static inline uint8_t get_x(void) {
    while(!(Serial3.available() > 0));
    while(Serial3.read() != 'X');
    while(!(Serial3.available() > 0));
    return Serial3.read();
}

static inline uint8_t get_y(void) {
    while(!(Serial3.available() > 0));
    while(Serial3.read() != 'Y');
    while(!(Serial3.available() > 0));
    return Serial3.read();
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

  a = (float(dy))/(float(dx));

  x = y0*a + x0;

  if (x > 255) {
   return 90; //Its bouncing of the wall
  }
  if (x < 0) {
   return 10;
  }
  
  return ((x * 100) / 255); 
}

void self_predictor() {
 
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
          carriage_init_right();  
        } else {
          carriage_init_left();  
        }
        break;
    }  
 }
}

static inline void self_control(void) {
   
   static uint8_t y0;
   static uint8_t x0;
   
   x0 = (get_x()*100)/255;
   y0 = (get_y()*100)/191;
   
   carriage_goto_pos(x0);
   
   if(y0 < 50 && x0 < 10) {
        myservo.write(140);
   } else if(y0 < 50 && x0 > 90) {
        myservo.write(60);
   } else {
        myservo.write(90); 
   }
   
   y0 = (get_y()*100)/191;
   
   if(y0 != 0 && y0 < 20) {
        digitalWrite(SOLENOID_PIN,HIGH);
        delay(100);
        digitalWrite(SOLENOID_PIN,LOW);
   } 
}

/*********************************************************************************************************
  Manual Control
*********************************************************************************************************/

static inline void manual_control(char command) {
    
    char carriage_control = rxBuf[1];
    char servo_control = rxBuf[2];
    char solenoid_control = rxBuf[3];
    
    /*===========CARRIAGE CONTROL==========*/
    switch(carriage_control) {
      case 'L': //left
        carriage_speed(MANUAL_SPEED);
        carriage_left();
        break;
      case 'R': //right
        carriage_speed(MANUAL_SPEED);
        carriage_right();
        break;
      default:
        carriage_speed(0);
        break;
     }

     /*================SERVO===============*/
     uint8_t pos = 150-servo_control;
     if((pos > 50) && (pos < 140)) {
       myservo.write(pos);
     }
     
     /*==============SOLENOID==============*/
     switch(solenoid_control) {
      case 1: //left
        digitalWrite(SOLENOID_PIN,HIGH);
        delay(250);
        digitalWrite(SOLENOID_PIN,LOW);
        break;
      case 2: //right
        digitalWrite(SOLENOID_PIN,HIGH);
        delay(250);
        digitalWrite(SOLENOID_PIN,LOW);
        break;
      case 3: //Both
        digitalWrite(SOLENOID_PIN,HIGH);
        delay(250);
        digitalWrite(SOLENOID_PIN,LOW);
        break;
     }
}

/*********************************************************************************************************
  State Machine 
*********************************************************************************************************/

void fsm(char command) {

  switch(state) {
    case STANDBY:
      if(command == 'M') {
        state = MANUAL;
      } else if(command == 'A') {
        state = AUTO;
      }
      break;  
    case AUTO:
      previous_state = state;
      self_control();
      score_counter(); 
      break;
    case MANUAL:
       previous_state = state;
       manual_control(command); 
       score_counter(); 
       break;
    case LOOSE:
       if(command == 'N') {
           //New game
            score = 0;
            flag = 1;
            state = previous_state; 
        } else if(command == 'C') {
            //Continue
            flag = 1;
            state = previous_state;
        }
       break;
  }  
}

/*********************************************************************************************************
  LOOP
*********************************************************************************************************/

void loop() {
  //calibrate_carriage();
  
  char command = 0;
  if(!digitalRead(2)) {
      CAN0.readMsgBuf(&len, rxBuf);              // Read data: len = data length, buf = data byte(s)
      rxId = CAN0.getCanId();                    // Get message ID
      //print_can_msg();
      command = rxBuf[0];
      
      if(command == 'S') {
        state = STANDBY;
        previous_state = STANDBY;
        flag = 1;
        carriage_init_left_slow();  
      }
      fsm(command);
  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
