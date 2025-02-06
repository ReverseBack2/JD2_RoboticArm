#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);       // called this way, it uses the default address 0x40

#define SERVOMIN  125                                                 // this is the 'minimum' pulse length count (out of 4096) 125
#define SERVOMAX  655                                                 // this is the 'maximum' pulse length count (out of 4096) 625

/*
ESP32:
Connect GND's
3V3 to VCC on driver
GPIO 21(P21) to SDA
GPIO 22(P22) to SCL

Driver:
V+ to power supply

*/


int incomingByte = 0; // for incoming serial data
char letter = 'a';
char buf [256];
int len = 0;
float var1;
float var2;
char intBufA [3];
int angleA = 0;
char intBufB [3];
int angleB = 0;

void setup() {
  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps
  
  for(int i = 0; i < 256; i++){
    buf[i] = '\0';
  }

  board1.begin();
  board1.setPWMFreq(60);                  // Analog servos run at ~60 Hz updatesc
}



void loop() {


    board1.setPWM(0, 0, angleToPulse(angleA) );
    board1.setPWM(5, 0, angleToPulse(angleB) );

    // board1.setPWM(0, 0, angleToPulse(0) );
    // delay(800);
    // board1.setPWM(0, 0, angleToPulse(180) );
    // delay(800);
    
   
    // delay(100);

  // send data only when you receive data:
  if (Serial.available() > 0) {
    len = 0;
    // read the incoming byte:
    for(int i = 0; i < 256; i++){
      incomingByte = Serial.read();
      letter = incomingByte;
      buf[i] = letter;
      len += 1;
      if(incomingByte == 10){
        break;
      }
    }

    switch (buf[0]) {
      case 'a':
        //Gcode command 1
        Serial.println("motor 1 control");
        intBufA[0] = buf[1];
        intBufA[1] = buf[2];
        intBufA[2] = buf[3];
        angleA = atoi(intBufA);
        break;
      case 'b':
        //Gcode command 2
        Serial.println("motor 2 control");
        intBufB[0] = buf[1];
        intBufB[1] = buf[2];
        intBufB[2] = buf[3];
        angleB = atoi(intBufB);
        break;
      case '*':
        //reading empty buffer
        Serial.println("*");
        break;
      default:
        //Gcode command 5
        Serial.print(buf);
        break;
    }

    // say what you got:
    // Serial.println(buf);

    for(int i = 0; i < len; i++){
      buf[i] = '\0';
    }
  }
}

int angleToPulse(int ang)                             //gets angle in degree and returns the pulse width
  {  int pulse = map(ang,0, 180, SERVOMIN,SERVOMAX);  // map angle of 0 to 180 to Servo min and Servo max 
     //Serial.print("Angle: ");Serial.print(ang);
     //Serial.print(" pulse: ");Serial.println(pulse);
     return pulse;
  }