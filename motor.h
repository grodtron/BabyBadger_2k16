
/*
 * Auther: Ryan Cooke
 * Subject: Concordia Eng Games Machine Motor header
 * save date:12/04/2015
 * save date:12/05/2015
 * I2C Motor shield 
 */
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

class Motor {
private:

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor_0 = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor_1 = AFMS.getMotor(2);

const int PT=0;
const int SB=1;

void MOVE_WHEEL_FWD(bool SIDE, int  AMOUNT) { 
  if(SIDE==0) 
  { 
    myMotor_0->run(FORWARD); 
    myMotor_0->setSpeed(AMOUNT);  
  } 
  if(SIDE==1)
  { 
    myMotor_1->run(FORWARD); 
    myMotor_1->setSpeed(AMOUNT); 
  } 
} 

void MOVE_WHEEL_BAK(bool SIDE,int AMOUNT){ 
  if(SIDE==0) 
  { 
    myMotor_0->run(BACKWARD); 
    myMotor_0->setSpeed(AMOUNT); 
  } 
  if(SIDE==1) 
  { 
    myMotor_1->run(BACKWARD); 
    myMotor_1->setSpeed(AMOUNT); 
  } 
} 





public:

const int N_MOTORS = 2;

void setup(){
  AFMS.begin();  // create with the default frequency 1.6KHz
   // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor_0->setSpeed(150);
  myMotor_0->run(FORWARD);
  // turn on motor
  myMotor_0->run(RELEASE);
  
  myMotor_1->setSpeed(150);
  myMotor_1->run(FORWARD);
  // turn on motor
  myMotor_1->run(RELEASE);
}

void set_PWM(int zero, int one)// 0 is left and 1 is right wheel
{
  myMotor_0->setSpeed(zero);
  myMotor_1->setSpeed(one);
}


void SPIN_LEFT(int AMOUNT) {  
  MOVE_WHEEL_BAK(PT, AMOUNT); 
  MOVE_WHEEL_FWD(SB, AMOUNT);  
} 


void SPIN_RIGHT(int AMOUNT)  {  
  MOVE_WHEEL_FWD(PT, AMOUNT); 
  MOVE_WHEEL_BAK(SB, AMOUNT); 
} 

void FWD(int AMOUNT) {  
  MOVE_WHEEL_FWD(PT, AMOUNT); 
  MOVE_WHEEL_FWD(SB, AMOUNT); 
} 

void BAK(int AMOUNT) {  
  MOVE_WHEEL_BAK(PT, AMOUNT); 
  MOVE_WHEEL_BAK(SB, AMOUNT);  
} 

void TURN_LEFT(int AMOUNT) {  
  MOVE_WHEEL_FWD(PT, 50); 
  MOVE_WHEEL_FWD(SB, AMOUNT);  
} 

void TURN_RIGHT(int AMOUNT) {  
  MOVE_WHEEL_FWD(PT, AMOUNT); 
  MOVE_WHEEL_FWD(SB, 50); 
} 

void  STOP() {
  MOVE_WHEEL_FWD(PT, 0); 
  MOVE_WHEEL_FWD(SB, 0);
}


};

