int incomingByte = 0;   // for incoming serial data
uint8_t x = 0;
uint8_t y = 0;
int option = 0;

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}

void loop() {

        /* send data only when you receive data:
        Serial.print("x:");
        Serial.print(x);
        Serial.print("y:");
        Serial.print(y);
        Serial.println();
        delay(300);

        Serial.print("x1:");
        Serial.print(x);
        Serial.print("y1:");
        Serial.print(y);
        Serial.println();
        delay(300);
        */
        
        //int coords[2];
        //get_coordinates(coords);
        
        int x_temp = -1;
        int y_temp = -1;
        
        while(x_temp == -1) {
          x_temp = get_x();
        }
        
        while(y_temp == -1) {
          y_temp = get_y();
        }
        
        Serial.print("x_coord: ");
        Serial.print(x_temp);
        Serial.print(", y_coord: ");
        Serial.println(y_temp);
        
        x_temp = -1;
        y_temp = -1;
        
        while(x_temp == -1) {
          x_temp = get_x();
        }
        
        while(y_temp == -1) {
          y_temp = get_y();
        }
        
        Serial.print("x_coord2: ");
        Serial.print(x_temp);
        Serial.print(", y_coord2: ");
        Serial.println(y_temp);
        
        
}

int get_x(void) {
  if (Serial.available() > 0) {
    char temp = Serial.read();
    if(temp == 'X') {
    while(!(Serial.available() > 0));
      return Serial.read();
    } 
  }
  return -1;
}

int get_y(void) {
  if (Serial.available() > 0) {
    char temp = Serial.read();
    if(temp == 'Y') {
    while(!(Serial.available() > 0));
      return Serial.read();
    } 
  }
  return -1;
}
