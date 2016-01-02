#ifndef MOTORS_H_
#define MOTORS_H_

#define MOTOR_TWEENER_DEBUG 1

//#define FORWARD  HIGH
//#define BACKWARD LOW

const int LEFT_PWM  = 10;
const int LEFT_DIR  = A0;

const int RIGHT_PWM   = 5; // IN1 (enable)
const int RIGHT_DIR   = 3; // IN2 (direction)

const int FLIPPER_ENABLE = 4;
const int FLIPPER_DIR    = 6;

#define FLIPPER_ON() do { \
  digitalWrite(FLIPPER_ENABLE, LOW); \
  digitalWrite(FLIPPER_DIR,    HIGH); \
} while(0)

#define FLIPPER_OFF() do { \
  digitalWrite(FLIPPER_ENABLE, HIGH); \
  digitalWrite(FLIPPER_DIR,    LOW); \
} while(0)


#define MOVE_SIDE_FWD(SIDE, AMOUNT) do { \
  analogWrite (SIDE ## _PWM, 255 - (AMOUNT)); \
  digitalWrite(SIDE ## _DIR, HIGH); \
} while(0)

#define MOVE_SIDE_BAK(SIDE, AMOUNT) do { \
  analogWrite (SIDE ## _PWM, 255 - (AMOUNT)); \
  digitalWrite(SIDE ## _DIR, LOW); \
} while(0)

int sign(int x){
  return x >= 0 ? 1 : -1;
}


class Motors {
  public:
    static const int DELAY_TIME = 4;
    static const int MIN_SPEED  = 85;
//    static const int MIN_SPEED  = 110;

  private:
    static void updateValue(int & current, int & target){
      if(current != target){
        // First if we are going into the range [-MIN_SPEED, MIN_SPEED], we skip either across it or to zero
        if(abs(target) > MIN_SPEED && abs(current) < MIN_SPEED){
          // Are we going from fwd to backward or vice versa?
          bool switchDirections = ( 
               (target >= 0 && current >= 0)
            || (target <= 0 && current <= 0))
            ? 1   // If we are not switching directions
            : -1; // If we are switching directions
          
          current = sign(target) * switchDirections * MIN_SPEED;
        }else if(abs(target) < MIN_SPEED && abs(current) < MIN_SPEED){
          current = 0;
          target = 0;
        }else{
          int delta = target - current;
          int dir   = sign(delta);
          delta     = abs(delta);
          
          current += dir * min(8, delta);  
        }
      }
    }
  
  public:  
    Motors() : 
    left(0), leftTarget(0),
    right(0), rightTarget(0),
    lastUpdateTime(millis() - DELAY_TIME)
    { 
      pinMode(RIGHT_PWM, OUTPUT);
      pinMode(RIGHT_DIR, OUTPUT);

      pinMode(LEFT_PWM, OUTPUT);
      pinMode(LEFT_DIR, OUTPUT);

      pinMode(FLIPPER_ENABLE, OUTPUT);
      pinMode(FLIPPER_DIR, OUTPUT);
    }

    void setSpeedImmediate(int newLeft, int newRight){
      leftTarget  = left  = newLeft;
      rightTarget = right = newRight;
      update();
    }
  
    void setTargetSpeed(int newLeftTarget, int newRightTarget){
//      if(leftTarget != newLeftTarget || rightTarget != newRightTarget){
//
//        if(abs(newLeftTarget) < MIN_SPEED && abs(newRightTarget) < MIN_SPEED){
//          left  = leftTarget  = 0;
//          right = rightTarget = 0;
//        }else{
//          leftTarget  = newLeftTarget;
//          rightTarget = newRightTarget;          
//        }
//        
//        Debug.println("New Target: ");
//        Debug.print  ("    Left : "); Debug.println(leftTarget);
//        Debug.print  ("    Right: "); Debug.println(rightTarget);
//      }
      setSpeedImmediate(newLeftTarget, newRightTarget);
    }
    
    void update(){
      long now = millis();
      if(now - DELAY_TIME > lastUpdateTime){
        lastUpdateTime = now;
        
//        updateValue(left,  leftTarget);
//        updateValue(right, rightTarget);
       
        if(left >= 0){
          MOVE_SIDE_FWD(LEFT, left);
        }else{
          MOVE_SIDE_BAK(LEFT, abs(left));
        }
        if(right >= 0){
          MOVE_SIDE_FWD(RIGHT, right);
        }else{
          MOVE_SIDE_BAK(RIGHT, abs(right));
        }
      }
    }

    bool done(){
      return (left == leftTarget) && (right == rightTarget);
    }
  private:
    int left,  leftTarget;
    int right, rightTarget;
    long lastUpdateTime;
  
};

#endif
