/*
 * Auther: Ryan Cooke
 * Subject: Concordia Eng Games Machine Gyroscope header
 * save date:11/29/2015
 */
#include <Wire.h>
#include "ITG3200.h"

class Gyro {
  private:
    
	//This is a list of registers in the ITG-3200. Registers are parameters that determine how the sensor will behave, or they can hold data that represent the
	//sensors current status.
	//To learn more about the registers on the ITG-3200, download and read the datasheet.
	char WHO_AM_I = 0x00;
	char SMPLRT_DIV= 0x15;
	char DLPF_FS = 0x16;
	char GYRO_XOUT_H = 0x1D;
	char GYRO_XOUT_L = 0x1E;
	char GYRO_YOUT_H = 0x1F;
	char GYRO_YOUT_L = 0x20;
	char GYRO_ZOUT_H = 0x21;
	char GYRO_ZOUT_L = 0x22;

	//This is a list of settings that can be loaded into the registers.
	//DLPF, Full Scale Register Bits
	//FS_SEL must be set to 3 for proper operation
	//Set DLPF_CFG to 3 for 1kHz Fint and 42 Hz Low Pass Filter
	char DLPF_CFG_0 = (1<<0);
	char DLPF_CFG_1 = (1<<1);
	char DLPF_CFG_2 = (1<<2);
	char DLPF_FS_SEL_0 = (1<<3);
	char DLPF_FS_SEL_1 = (1<<4);

    float x,y,z;// values of the gyro 
    float ax,ay,az;
    float previous_t;
    float  current_t;
    float sample;
    float sample_with_error;
    float error_per_sec;
	//I2C devices each have an address. The address is defined in the datasheet for the device. The ITG-3200 breakout board can have different address depending on how
	//the jumper on top of the board is configured. By default, the jumper is connected to the VDD pin. When the jumper is connected to the VDD pin the I2C address
	//is 0x69.
	char itgAddress = 0x69;// used to be ox69
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
	
	//Added for ITG3200 Sparkfun chip
		
	//This function will write a value to a register on the itg-3200.
	//Parameters:
	//  char address: The I2C address of the sensor. For the ITG-3200 breakout the address is 0x69.
	//  char registerAddress: The address of the register on the sensor that should be written to.
	//  char data: The value to be written to the specified register.
	void itgWrite(char address, char registerAddress, char data)
	{
	  //Initiate a communication sequence with the desired i2c device
	  Wire.beginTransmission(address);
	  //Tell the I2C address which register we are writing to
	  Wire.write(registerAddress);
	  //Send the value to write to the specified register
	  Wire.write(data);
	  //End the communication sequence
	  Wire.endTransmission();
	}

	//This function will read the data from a specified register on the ITG-3200 and return the value.
	//Parameters:
	//  char address: The I2C address of the sensor. For the ITG-3200 breakout the address is 0x69.
	//  char registerAddress: The address of the register on the sensor that should be read
	//Return:
	//  unsigned char: The value currently residing in the specified register
	unsigned char itgRead(char address, char registerAddress)
	{
    //Debug.println("ITG READ FUNCTION");
	  //This variable will hold the contents read from the i2c device.
	  unsigned char data=0;
	  //Send the register address to be read.
	  Wire.beginTransmission(address);
	  //Send the Register Address
	  Wire.write(registerAddress);
	  //End the communication sequence.
   
	  int error = 0;
	  error=Wire.endTransmission();
	  Debug.println(error);
    if(error==0){// Added error checking If not 0 then there is an error
      
	  //Ask the I2C device for data
	  Wire.beginTransmission(address);
	  Wire.requestFrom(address, 1);// Failling on this line **********************************
    }
	  //Wait for a response from the I2C device
	  if(Wire.available()){
		//Save the data sent from the I2C device
		data = Wire.read();
	  }
   else
	  
	  //End the communication sequence.
	  Wire.endTransmission();
	  
	  //Return the data read during the operation
	  return data;
	}

	//This function is used to read the X-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
	//NOTE: This value is NOT in degrees per second. 
	//Usage: int xRate = readX();
	int readX(void)
	{
	  int data=0;
	  data = itgRead(itgAddress, GYRO_XOUT_H)<<8;
	  data |= itgRead(itgAddress, GYRO_XOUT_L);  
	  
	  return data;
	}

	//This function is used to read the Y-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
	//NOTE: This value is NOT in degrees per second. 
	//Usage: int yRate = readY();
	int readY(void)
	{
	  int data=0;
	  data = itgRead(itgAddress, GYRO_YOUT_H)<<8;
	  data |= itgRead(itgAddress, GYRO_YOUT_L);  
	  
	  return data;
	}

	//This function is used to read the Z-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
	//NOTE: This value is NOT in degrees per second. 
	//Usage: int zRate = readZ();
	int readZ(void)
	{
	  int data=0;
	  data = itgRead(itgAddress, GYRO_ZOUT_H)<<8;
	  data |= itgRead(itgAddress, GYRO_ZOUT_L); 
	  return data;
	} 
	// End of Sparkfun inport

  public:
    Gyro(){
    }

    void callibrate(){
        //Initialize the I2C communication. This will set the Arduino up as the 'Master' device.
  	  Wire.begin();
  	  
  	  //Read the WHO_AM_I register and print the result
  	  char id=0; 
  	  id = itgRead(itgAddress, 0x00);  
  	  Debug.print("ID: ");
  	  Debug.println(id, HEX);
  	  
  	  //Configure the gyroscope
  	  //Set the gyroscope scale for the outputs to +/-2000 degrees per second
  	  itgWrite(itgAddress, DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0));
  	  //Set the sample rate to 100 hz
  	  itgWrite(itgAddress, SMPLRT_DIV, 9);
     
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
  	  //Read the x,y and z output rates from the gyroscope.
  	  //x = readX();
  	  //y = readY();
  	  z = readZ();
	  
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

