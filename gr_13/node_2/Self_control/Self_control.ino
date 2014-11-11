
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

uint16_t ball_x = 0;
uint16_t ball_y = 0;

MCP_CAN CAN0(53); // Set CS to pin 53

#define MAX 95
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
  Timer3.initialize(3000);         // initialize timer1, and set a 1/2 second period
  Timer3.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt

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

void callback() {
  x_axis -= read_decoder();
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
  carriage_speed(255);
  carriage_left();
  delay(300);
  carriage_speed(0);
  delay(300);
  x_axis = 0;
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
  
  if(x > 100) {
     x = 100;
  }
  if (x < 0) {
     x = 0;
  }
  
  if(x_percent < 3) {
    x_percent = 3;
  }
  if(x_percent > 97) {
    x_percent = 97;
  }
  
  if ((x <= (x_percent + 3)) && (x >= (x_percent - 3))) {
    carriage_speed(0);
    return 0; 
  }
  
  
  if (x > x_percent) { 
	do {
          /*
          Serial.print("top while ");
          Serial.print("x: ");
          Serial.print(x);
          Serial.print("x_percent: ");
          Serial.println(x_percent);
          */
          x = x_axis/MAX;
          carriage_speed(150);
          carriage_left();
	} while (x > x_percent);
  } else {
	do {
           /*
           Serial.print("bottom while ");
           Serial.print("x: ");
           Serial.print(x);
           Serial.print("x_percent: ");
           Serial.println(x_percent);
           */
           x = x_axis/MAX;
           carriage_speed(150);
	   carriage_right();
	} while (x < x_percent);
  }
  /*
  Serial.print("x: ");
  Serial.print(x);
  Serial.print("x_percent: ");
  Serial.println(x_percent);
  */
  carriage_speed(0);
  return 1;
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

void read_raspi() {
  while(Serial.available()) {
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

static inline int get_x(void) {
  if (Serial.available() > 0) {
    char temp = Serial.read();
    if(temp == 'X') {
    while(!(Serial.available() > 0));
      return Serial.read();
    } 
  }
  return -1;
}

static inline int get_y(void) {
  if (Serial.available() > 0) {
    char temp = Serial.read();
    if(temp == 'Y') {
    while(!(Serial.available() > 0));
      return Serial.read();
    } 
  }
  return -1;
}

uint8_t calculate_vector(void) {
 
 uint8_t x0, x1, y0, y1;
 int dx, dy, x;
 float a;
 
 int x_cor = -1;
 int y_cor = -1;
        
 while(x_cor == -1) {
   x_cor = get_x();
 }
        
 while(y_cor == -1) {
   y_cor = get_y();
 }

 x0 = x_cor;
 y0 = y_cor;
 
 x_cor = -1;
 y_cor = -1;
 
 while(x_cor == -1) {
   x_cor = get_x();
 }
        
 while(y_cor == -1) {
   y_cor = get_y();
 }
 
 x1 = x_cor;
 y1 = y_cor;
 /*
 Serial.print("x0: ");
 Serial.print(x0);
 Serial.print(" x1: ");
 Serial.print(x1);
 Serial.print(" y0: ");
 Serial.print(y0);
 Serial.print(" y1: ");
 Serial.print(y1);
 */
 if(y1 > y0) {
   return 0;
 }
 
 dy = y1-y0;
 dx = x1-x0;
 /*
 Serial.print(" dy: ");
 Serial.print(dy);
 
 Serial.print(" dx: ");
 Serial.print(dx);
 */
 if(dx == 0) {
   return 50;
 }
 
 a = (float(dy))/(float(dx));
 /*
 Serial.print(" a: ");
 Serial.print(a);
 */
 //x = x1 - (y1/a);
 x = x1 + y1*a;
 /*
 Serial.print(" x: ");
 Serial.println(x);
 */
 if (x > 255) {
   return 100;
 }
 if (x < 0) {
   return 1;
 }
 
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
 return ((x * 100) / 255); 
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

void func() {
 
 int y0 = 0;
 int y1 = 0; 
 int y_cor = -1;  
  
 while(y1 >= y0) { 
   //Serial.println("Going up!");
   y_cor = -1;  
   while(y_cor == -1) {
     y_cor = get_y();
   }
   y0 = y_cor;
   y_cor = -1;
   while(y_cor == -1) {
     y_cor = get_y();
   }
   y1 = y_cor;
 }
 /*
 while(y_cor > 200) {
   //Serial.print(y_cor);
   //Serial.println("Checking in to tha motel");
   y_cor = -1;  
   while(y_cor == -1) {
     y_cor = get_y();
   }
 }
 */
 
 uint8_t goto_x = calculate_vector();
 
 
 
 carriage_goto_pos(goto_x);
 
 delay(1000);
 
 carriage_init();
 
}

/*********************************************************************************************************
  LOOP
*********************************************************************************************************/

void loop() {
    
  /*
  carriage_goto_pos(0);
  Serial.print("Goto: 0 x_axis: ");
  Serial.println(x_axis);
  delay(1000);
  carriage_goto_pos(100);
  Serial.print("Goto: 100 x_axis: ");
  Serial.println(x_axis);
  delay(1000); 
  carriage_goto_pos(50);
  Serial.print("Goto: 100 x_axis: ");
  Serial.println(x_axis);
  delay(1000); 
  */
  //Serial.print("x_axis: ");
  //Serial.println(x_axis);
  
    int previous_x = 0;
    int goto_x;
   
    read_raspi();
    
    goto_x = ((ball_x * 100) / 255); 
    carriage_goto_pos(goto_x); 
    
    /*
    if(ball_x > previous_x) { //Going right
      // goto_x = ((ball_x * 100) / 255); 
      carriage_goto_pos(goto_x);  
    } else {
      goto_x = ((ball_x * 100) / 255) - 10;
      carriage_goto_pos(goto_x); 
    }
    */
    if(ball_y < 50 && ball_x < 40) {
        myservo.write(140);
    }
    else if(ball_y < 50 && ball_x > 230) {
        myservo.write(40);
    } else {
        myservo.write(90); 
    }
    
    if((ball_y > 5) && (ball_y < 40)) {
        digitalWrite(SOLENOID_PIN,HIGH);
        delay(100);
        digitalWrite(SOLENOID_PIN,LOW);
        delay(100);
        digitalWrite(SOLENOID_PIN,HIGH);
        delay(100);
        digitalWrite(SOLENOID_PIN,LOW);
        if(goto_x > 50) {
          //init_right = 0;
          carriage_init_right();  
        } else {
          //init_right = 1;
          carriage_init_left();  
        }
        ball_x = 0;
        ball_y = 0;
        
    }
    
    //previous_x = ball_x;
  
     /*
    read_raspi();
    
    static uint8_t goto_x = 0;
    
    if(ball_y > 120 && ball_y < 140) {
      goto_x = calculate_vector();
      carriage_goto_pos(goto_x);
      delay(100);
      //Serial.print("Goto: ");
      //Serial.println(goto_x);
    }
    */
    //func();

}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
