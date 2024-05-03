#include "MPU_interface.h"

u8 ranges_config[2] = {0, 0};

void MPU_Init(Accel_Range A_Range, Gyro_Range Rot_Range, DLPF_BW DLPF_CFG)
{
  //store the ranges globally to be used in other functions
  ranges_config[0] = A_Range;
  ranges_config[1] = Rot_Range;

  //initiate I2C communication
  Wire.begin();
    
  //begin a session with MPU_60X0 
  Wire.beginTransmission(MPU_ADDRESS);
    //Configuration register  
    Wire.write(CONFIG);
    Wire.write(DLPF_CFG);   // configure Digital low pass filter (DLPF) bits[0:2] and disable external frame synchronization (FSYNC)
    // End transmission to start a new one
    Wire.endTransmission(true);


  //begin a session with MPU_60X0
  Wire.beginTransmission(MPU_ADDRESS);
    //Configure the Sample rate to 10kHZ
    Wire.write(SMPLRT_DIV);
    if(DLPF_CFG == disable_DLPF)   //Gyroscope Output Rate = 8kHZ
    {
      Wire.write(7);
    }
    else
    {
      Wire.write(0);    //Gyroscope Output Rate = 1kHZ
    }
    // End transmission to start a new one
    Wire.endTransmission(true);


  //begin a session with MPU_60X0
  Wire.beginTransmission(MPU_ADDRESS);
    //Configure the accelerometer
    Wire.write(ACCEL_CONFIG);
    switch(A_Range)
    {
      case AFS_2g:
      Wire.write(0x00);   // Set a full scale of +/- 2g
      break;
      case AFS_4g:
      Wire.write(0b00001000);   // Set a full scale of +/- 4g
      break;
      case AFS_8g:
      Wire.write(0b00010000);   // Set a full scale of +/- 8g
      break;
      case AFS_16g:
      Wire.write(0b00011000);   // Set a full scale of +/- 16g
      break;
    }
    // End transmission to start a new one
    Wire.endTransmission(true);


    //begin a session with MPU_60X0
  Wire.beginTransmission(MPU_ADDRESS);
    //Configure the Gyroscope
    Wire.write(GYRO_CONFIG);
    switch(Rot_Range)
    {
      case GFS_250:
      Wire.write(0x00);   // Set a full scale of +/- 250 DEG/sec
      break;
      case GFS_500:
      Wire.write(0b00001000);   // Set a full scale of +/- 500 DEG/sec
      break;
      case GFS_1000:
      Wire.write(0b00010000);   // Set a full scale of +/- 1000 DEG/sec
      break;
      case GFS_2000:
      Wire.write(0b00011000);   // Set a full scale of +/- 2000 DEG/sec
      break; 
    }     
    // End transmission to start a new one
    Wire.endTransmission(true);   
  

  //begin a session with MPU_60X0
  Wire.beginTransmission(MPU_ADDRESS);
    //Configure the power manegment
    Wire.write(PWR_MGMT_1);
    Wire.write(0X00);
    // End transmission to start a new one
    Wire.endTransmission(true);  
}





void MPU_Get_Accel(float* Ax, float* Ay, float* Az)
{
  u8 A_Range = ranges_config[0];
  u16 Ax_Raw, Ay_Raw, Az_Raw = 0;
  //begin a session with MPU_60X0
  Wire.beginTransmission(MPU_ADDRESS);
    // Access The ACCEL__X register
    Wire.write(ACCEL_XOUT);
    Wire.endTransmission(false);
    //Access 2 byte of data to be read
    Wire.requestFrom(MPU_ADDRESS, 6);
    while(Wire.available() < 6);

    Ax_Raw = Wire.read()<<8 | Wire.read();
    Ay_Raw = Wire.read()<<8 | Wire.read();
    Az_Raw = Wire.read()<<8 | Wire.read();

    switch(A_Range)
    {
      case AFS_2g:
      *Ax = Ax_Raw/16384.0;   
      *Ay = Ay_Raw/16384.0;   
      *Az = Az_Raw/16384.0;   
      break;
      case AFS_4g:
      *Ax = Ax_Raw/8192.0;   
      *Ay = Ay_Raw/8192.0;   
      *Az = Az_Raw/8192.0;
      break;
      case AFS_8g:
      *Ax = Ax_Raw/4096.0;   
      *Ay = Ay_Raw/4096.0;   
      *Az = Az_Raw/4096.0;   
      break;
      case AFS_16g:
      *Ax = Ax_Raw/2048.0;   
      *Ay = Ay_Raw/2048.0;   
      *Az = Az_Raw/2048.0;   
      break;
    }
}





void MPU_Get_Temp(float* Temp)
{
  u16 Temp_Raw = 0;
  //begin a session with MPU_60X0
  Wire.beginTransmission(MPU_ADDRESS);
    // Access The ACCEL__X register
    Wire.write(TEMP_OUT);
    Wire.endTransmission(false);
    //Access 2 byte of data to be read
    Wire.requestFrom(MPU_ADDRESS, 2);
    while(Wire.available() < 2);

    //Temp_Raw = Wire.read()<<8 | Wire.read();
    //*Temp = Temp_Raw/340.0 + 36.53;   // Temprature in celsius
    *Temp = Wire.read()<<8 | Wire.read();   //test
}





void MPU_Get_AngRate(float* Rotx, float* Roty, float* Rotz)
{
  u8 Rot_Range = ranges_config[1];
  u16 Rotx_Raw, Roty_Raw, Rotz_Raw = 0;
  //begin a session with MPU_60X0
  Wire.beginTransmission(MPU_ADDRESS);
    // Access The GYRO_X register
    Wire.write(GYRO_XOUT);
    Wire.endTransmission(false);
    //Access 2 byte of data to be read
    Wire.requestFrom(MPU_ADDRESS, 6);
    while(Wire.available() < 6);
    
    Rotx_Raw = Wire.read()<<8 | Wire.read();
    Roty_Raw = Wire.read()<<8 | Wire.read();
    Rotz_Raw = Wire.read()<<8 | Wire.read();

    switch(Rot_Range)
    {
      case GFS_250:
      *Rotx = Rotx_Raw/131.0;   
      *Roty = Roty_Raw/131.0;   
      *Rotz = Rotz_Raw/131.0;   
      break;
      case GFS_500:
      *Rotx = Rotx_Raw/65.5;   
      *Roty = Roty_Raw/65.5;   
      *Rotz = Rotz_Raw/65.5;
      break;
      case GFS_1000:
      *Rotx = Rotx_Raw/32.8;   
      *Roty = Roty_Raw/32.8;   
      *Rotz = Rotz_Raw/32.8;   
      break;
      case GFS_2000:
      *Rotx = Rotx_Raw/16.4;   
      *Roty = Roty_Raw/16.4;   
      *Rotz = Rotz_Raw/16.4;   
      break;
    }
}





void MPU_Get_Angles(float* Roll_X, float* Pitch_Y)
{
  float Ax, Ay, Az = 0.0;
  MPU_Get_Accel(&Ax, &Ay, &Az);
  // Calculating Roll and Pitch from the accelerometer data
  *Roll_X = (atan(Ay / sqrt(pow(Ax, 2) + pow(Az, 2))) * 180 / PI) ; 
  *Pitch_Y = (atan(-1 * Ax / sqrt(pow(Ay, 2) + pow(Az, 2))) * 180 / PI) ; 
}