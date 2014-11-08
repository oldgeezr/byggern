int incomingByte = 0;   // for incoming serial data
int x = 0;
int y = 0;
int option = 0;

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}

void loop() {

        // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                char temp = Serial.read();
                
                if(temp == 'X') {
                    while(!(Serial.available() > 0));
                    x = Serial.read();
                    Serial.print("X: ");
                    Serial.println(x, DEC);
                } else if(temp == 'Y'){
                    while(!(Serial.available() > 0));
                    y = Serial.read();
                    Serial.print("Y: ");
                    Serial.println(y, DEC);
                }
        }
}
