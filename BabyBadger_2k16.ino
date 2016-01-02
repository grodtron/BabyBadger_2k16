const int ledPin = 13; // the pin that the LED is attached to
int incomingByte;      // a variable to read incoming serial data into


// If defined, then all Debug.<whatever> will go through to Serial
#define SERIAL_DEBUG
#include "serial.h"
#include "Gyro.h"
#include "motors.h"

Gyro gyro;
Motors badger;// This is the object for the 2x Motor setup
int heading=0;
bool tracking=0;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  Debug.println("START");
  // initialize the LED pin as an output:
   MOVE_SIDE_FWD(LEFT,0);
   MOVE_SIDE_FWD(RIGHT,0);
   gyro.callibrate();
   heading=gyro.value_cor();
  Debug.println("DONE SETUP");
}

void loop() {
  
gyro.update();


 
  if(tracking==1){
    heading=heading-90;
  }

  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'w') {
      Debug.println("FORWARD");
      MOVE_SIDE_FWD(LEFT,255);
      MOVE_SIDE_FWD(RIGHT,255);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 's') {
      Debug.println("BACKWARD");
      MOVE_SIDE_BAK(LEFT,255);
      MOVE_SIDE_BAK(RIGHT,255);
    }
    if (incomingByte == 'a') {
    Debug.println("LEFT");
      MOVE_SIDE_BAK(LEFT,255);
      MOVE_SIDE_FWD(RIGHT,255);
    }    
    if (incomingByte == 'd') {
    Debug.println("RIGHT");
      MOVE_SIDE_FWD(LEFT,255);
      MOVE_SIDE_BAK(RIGHT,255);
    }
    if (incomingByte == ' ') {
    Debug.println("STOP");
      MOVE_SIDE_FWD(LEFT,0);
      MOVE_SIDE_FWD(RIGHT,0);
    }
    if (incomingByte == 'T') {
    Debug.println("TRACK");
    tracking=1;
    }
    if (incomingByte == 'G') {
    Debug.print(gyro.value());
    Debug.print("  ");
    Debug.print("Corrected:");
    Debug.println(gyro.value_cor());
    }
    if (incomingByte == 'q') {
     heading--;
    }
    if (incomingByte == 'e') {
     heading++;
    }
  }
  else{
    //Debug.println(gyro.value());   
       }
}


void track(float input){
  Debug.println("TRACKING***********************");
  const int delta = 5;
  if(gyro.value_cor()<(input-delta)){
      MOVE_SIDE_FWD(LEFT,150);
      MOVE_SIDE_FWD(RIGHT,255);
  }
  else if(gyro.value_cor()>(input+delta)){
      MOVE_SIDE_FWD(LEFT,255);
      MOVE_SIDE_FWD(RIGHT,150);
  }
  else {
      MOVE_SIDE_FWD(LEFT,255);
      MOVE_SIDE_FWD(RIGHT,255);
    tracking=0;
  }
}


