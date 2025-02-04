int incomingByte = 0; // for incoming serial data
char letter = 'a';
char buf [256];
int len = 0;

void setup() {
  Serial.begin(115200); // opens serial port, sets data rate to 9600 bps
  for(int i = 0; i < 256; i++){
      buf[i] = '\0';
    }
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    for(int i = 0; i < 256; i++){
      incomingByte = Serial.read();
      letter = incomingByte;
      buf[i] = letter;
      if(incomingByte == 10){
        break;
      }
    }
    

    // say what you got:
    Serial.println("I received: ");
    Serial.println(buf);

    for(int i = 0; i < len; i++){
      buf[i] = '\0';
    }
  }
}