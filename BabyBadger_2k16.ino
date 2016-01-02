const int ledPin = 13; // the pin that the LED is attached to
int incomingByte;      // a variable to read incoming serial data into


// If defined, then all Debug.<whatever> will go through to Serial
#define SERIAL_DEBUG
#include "serial.h"
#include "Gyro.h"
#include "motor.h"
Gyro gyro;
Motor badger;// This is the object for the 2x Motor setup

bool tracking=0;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  Debug.println("START");
  // initialize the LED pin as an output:
   badger.setup();
   gyro.callibrate();
  Debug.println("DONE SETUP");
}

void loop() {
  
gyro.update();

Debug.print(gyro.value());
Debug.print("/t");
Debug.print("Corrected:");
Debug.println(gyro.value_cor());
  
  if(tracking==1){
    track(1200);
  }

  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'w') {
      Debug.println("FORWARD");
      badger.FWD(200);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 's') {
      Debug.println("BACKWARD");
      badger.BAK(200);
    }
    if (incomingByte == 'a') {
    Debug.println("LEFT");
    badger.SPIN_LEFT(200);
    }    
    if (incomingByte == 'd') {
    Debug.println("RIGHT");
    badger.SPIN_RIGHT(200);
    }
    if (incomingByte == ' ') {
    Debug.println("STOP");
    badger.STOP();
    }
    if (incomingByte == 'T') {
    Debug.println("TRACK");
    tracking=1;
    }
  }
  else{
    //Debug.println(gyro.value());   
       }
}


void track(float input){
  Debug.println("TRACKING***********************");
  const int delta = 200;
  if(gyro.value_cor()<(input-delta)){
    badger.SPIN_RIGHT(200);
  }
  else if(gyro.value_cor()>(input+delta)){
    badger.SPIN_LEFT(200);
  }
  else {
    badger.FWD(200);
    tracking=0;
  }
}


