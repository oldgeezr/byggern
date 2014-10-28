#include <avr/interrupt.h>
#include <avr/io.h>
#include <TimerThree.h>

volatile uint8_t counter = 0;

void setup()
{
  Serial.begin(9600);
  Timer3.initialize(50000);         // initialize timer1, and set a 1/2 second period
  Timer3.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}
 
void callback()
{
  Serial.println("YOLO");
}

void loop() {
    Serial.print("test value:");
    Serial.println(counter);
    delay(200);
}
