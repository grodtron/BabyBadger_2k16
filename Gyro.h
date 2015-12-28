/*
 * Auther: Ryan Cooke
 * Subject: Concordia Eng Games Machine Gyroscope header
 * save date:11/29/2015
 */
#include <Wire.h>
#include "ITG3200.h"

class Gyro {
  private:
    ITG3200 gyro1;

    float x,y,z;// values of the gyro 
    float ax,ay,az;
    float previous_t;
    float  current_t;
    float sample;
    float sample_with_error;
    float error_per_sec;
    
    float area (float t1, float y1, float t2, float y2)
    {
      float deltaT;
      float deltaY;
      float sum, sum2;
      deltaT=t2-t1;
      deltaY=y2-y1;
      
      sum=deltaT*deltaY;
      sum=sum/2;
      
      sum2=deltaT*y1;
      return sum+sum2;
    }

  public:
    Gyro(){
    }

    void callibrate(){
      gyro1.init();
      gyro1.zeroCalibrate(200,20);//sample 200 times to calibrate and it will take 200*20ms
      // our calibration
    
     
      x=0;
      ax=0;
      y=0;
      ay=0;
      z=0;
      az=0;
      sample=0;
      error_per_sec=0;
      float cali_start=(micros()/1000000.0);
      previous_t=cali_start;// changed from taking its own micros measurement
      
      for(int i=0; i<100;i++)
      {
        update();
      }
      float cali_end=(micros()/1000000.0);
      error_per_sec=(sample/(cali_end-cali_start));
      x=0;
      ax=0;
      y=0;
      ay=0;
      z=0;
      az=0;
      sample=0;
      Debug.print("Error Per second: ");
      Debug.println(error_per_sec);
    }

    void update(){
      gyro1.getAngularVelocity(&x,&y,&z);// Pass by reference store the values in the x,y,z floats
      float current_t=(micros()/1000000.0);
      sample=sample+area(previous_t,z,current_t,az);
      sample_with_error=sample_with_error+area(previous_t,z,current_t,az)-(error_per_sec*(current_t-previous_t));
      previous_t=current_t;
      az=z;
    }

    float value(){
      /*
       * Return Normal heading for the selected axis
       * Sensor has some drifting
       * Perliminary testing seems to suggest a 30 degree drift over a perido of 8 minutes
       */
      return sample;
    }

    float value_cor(){
      /*
       * Return Corrected heading for the selected axis
       * Need to get more conclusive testing with this correction method
       */
      return sample_with_error;
    }
   
};

