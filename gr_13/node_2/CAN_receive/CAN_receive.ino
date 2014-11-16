
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
//int state = 0;

MCP_CAN CAN0(53); // Set CS to pin 53

#define OE A2
#define RST A3
#define SEL A4
#define EN A5
#define DIR A6

#define SERVO_PIN 5
#define DIODE_PIN A7
#define SOLENOID_PIN 8

#define CENTER 3700 //Needs to be changed...

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
  carriage_speed(255);
  carriage_left();
  delay(300);
  carriage_speed(0);
  delay(100);
  x_axis = 0;
}

void carriage_center(void) {
	if (x_axis > CENTER) {
		do {
                        carriage_speed(110);
			carriage_left();
		} while (x_axis > CENTER);
	} else {
		do {
                        carriage_speed(110);
			carriage_right();
		} while (x_axis < CENTER);
	}
        carriage_speed(0);
}
//Tenker her at en kan spesifisere x-aksen i prosent eller noe...
void carriage_goto_pos(uint16_t x) {
	if (x_axis > x) {
		do {
                        carriage_speed(110);
			carriage_left();
		} while (x_axis > x);
	} else {
		do {
                        carriage_speed(110);
			carriage_right();
		} while (x_axis < x);
	}
        carriage_speed(0);
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

int get_diode_status(void) {
  int temp;
  temp += analogRead(DIODE_PIN);
  temp += analogRead(DIODE_PIN);
  //Serial.print("Diode:");
  //Serial.println(temp);
  return (temp/2);  
}

void score_counter(void) {
   Serial.println(get_diode_status());
   if((get_diode_status() < 50) && flag) {
     score++;
     flag = 0;
     state = LOOSE; 
     unsigned char stmp[1] = {1};
     CAN0.sendMsgBuf('L', 0, 1, stmp);
     CAN0.sendMsgBuf('L', 0, 1, stmp);
     CAN0.sendMsgBuf('L', 0, 1, stmp);
   }
}

static inline void manual_control(char command) {
      if (command == 'S') {
         state = STANDBY; 
      }
      
      char carriage_control = rxBuf[1];
      char servo_control = rxBuf[2];
      char solenoid_control = rxBuf[3];
      
      /*===========CARRIAGE CONTROL==========*/
      switch(carriage_control) {
        case 'L': //left
          carriage_speed(150);
          carriage_left();
          break;
        case 'R': //right
          carriage_speed(150);
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
  

void fsm(char command) {

  switch(state) {
    case STANDBY:
      Serial.println("STANDBY");
      if(command == 'M') {
        state = MANUAL;
      }
      else if(command == 'A') {
        state = AUTO;
      }
      break;  
    case AUTO:
      Serial.println("AUTO");
      previous_state = state;
      //plan_b();
      //score_counter(); 
      break;
    case MANUAL:
       //Serial.println("MANUAL");
       previous_state = state;
       manual_control(command); 
       score_counter(); 
       break;
    case LOOSE:
       Serial.println("LOOSE");
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

    char command = 0;
  
    if(!digitalRead(2)) {
        CAN0.readMsgBuf(&len, rxBuf);              // Read data: len = data length, buf = data byte(s)
        rxId = CAN0.getCanId();                    // Get message ID
        //print_can_msg();
        command = rxBuf[0];
        fsm(command);
    }
 
    //Serial.println("state: ");
    //int k = state;
    //Serial.print(state);
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

