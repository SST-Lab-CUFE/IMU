#include "MPU_interface.h"

u8 ranges_config[2] = {0, 0};

void MPU_Init(Accel_Range A_Range, Gyro_Range Rot_Range, DLPF_BW DLPF_CFG)
{
  //store the ranges globally to be used in other functions
  ranges_config[0] = A_Range;
  ranges_config[1] = Rot_Range;

  //initiate I2C communication
  I2C_Macro_Begin();
    
  //begin a session with MPU_60X0 
  I2C_Macro_BeginTransmission(MPU_ADDRESS);
    //Configuration register  
    I2C_Macro_Write(CONFIG);
    I2C_Macro_Write(DLPF_CFG);   // configure Digital low pass filter (DLPF) bits[0:2] and disable external frame synchronization (FSYNC)
    // End transmission to start a new one
    I2C_Macro_EndTransmission(true);


  //begin a session with MPU_60X0
  I2C_Macro_BeginTransmission(MPU_ADDRESS);
    //Configure the Sample rate to 10kHZ
    I2C_Macro_Write(SMPLRT_DIV);
    if(DLPF_CFG == disable_DLPF)   //Gyroscope Output Rate = 8kHZ
    {
      I2C_Macro_Write(7);
    }
    else
    {
      I2C_Macro_Write(0);    //Gyroscope Output Rate = 1kHZ
    }
    // End transmission to start a new one
    I2C_Macro_EndTransmission(true);


  //begin a session with MPU_60X0
  I2C_Macro_BeginTransmission(MPU_ADDRESS);
    //Configure the accelerometer
    I2C_Macro_Write(ACCEL_CONFIG);
    switch(A_Range)
    {
      case AFS_2g:
      I2C_Macro_Write(0x00);   // Set a full scale of +/- 2g
      break;
      case AFS_4g:
      I2C_Macro_Write(0b00001000);   // Set a full scale of +/- 4g
      break;
      case AFS_8g:
      I2C_Macro_Write(0b00010000);   // Set a full scale of +/- 8g
      break;
      case AFS_16g:
      I2C_Macro_Write(0b00011000);   // Set a full scale of +/- 16g
      break;
    }
    // End transmission to start a new one
    I2C_Macro_EndTransmission(true);


    //begin a session with MPU_60X0
  I2C_Macro_BeginTransmission(MPU_ADDRESS);
    //Configure the Gyroscope
    I2C_Macro_Write(GYRO_CONFIG);
    switch(Rot_Range)
    {
      case GFS_250:
      I2C_Macro_Write(0x00);   // Set a full scale of +/- 250 DEG/sec
      break;
      case GFS_500:
      I2C_Macro_Write(0b00001000);   // Set a full scale of +/- 500 DEG/sec
      break;
      case GFS_1000:
      I2C_Macro_Write(0b00010000);   // Set a full scale of +/- 1000 DEG/sec
      break;
      case GFS_2000:
      I2C_Macro_Write(0b00011000);   // Set a full scale of +/- 2000 DEG/sec
      break; 
    }     
    // End transmission to start a new one
    I2C_Macro_EndTransmission(true);   
  

  //begin a session with MPU_60X0
  I2C_Macro_BeginTransmission(MPU_ADDRESS);
    //Configure the power manegment
    I2C_Macro_Write(PWR_MGMT_1);
    I2C_Macro_Write(0X00);
    // End transmission to start a new one
    I2C_Macro_EndTransmission(true);  
}





void MPU_Get_Accel(float* Ax, float* Ay, float* Az)
{
  u8 A_Range = ranges_config[0];
  u16 Ax_Raw, Ay_Raw, Az_Raw = 0;
  //begin a session with MPU_60X0
  I2C_Macro_BeginTransmission(MPU_ADDRESS);
    // Access The ACCEL__X register
    I2C_Macro_Write(ACCEL_XOUT);
    I2C_Macro_EndTransmission(false);
    //Access 2 byte of data to be read
    I2C_Macro_RequestFrom(MPU_ADDRESS, 6);
    while(I2C_Macro_Available() < 6);

    Ax_Raw = I2C_Macro_Read()<<8 | I2C_Macro_Read();
    Ay_Raw = I2C_Macro_Read()<<8 | I2C_Macro_Read();
    Az_Raw = I2C_Macro_Read()<<8 | I2C_Macro_Read();

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
  I2C_Macro_BeginTransmission(MPU_ADDRESS);
    // Access The ACCEL__X register
    I2C_Macro_Write(TEMP_OUT);
    I2C_Macro_EndTransmission(false);
    //Access 2 byte of data to be read
    I2C_Macro_RequestFrom(MPU_ADDRESS, 2);
    while(I2C_Macro_Available() < 2);

    //Temp_Raw = I2C_Macro_Read()<<8 | I2C_Macro_Read();
    //*Temp = Temp_Raw/340.0 + 36.53;   // Temprature in celsius
    *Temp = I2C_Macro_Read()<<8 | I2C_Macro_Read();   //test
}





void MPU_Get_AngRate(float* Rotx, float* Roty, float* Rotz)
{
  u8 Rot_Range = ranges_config[1];
  u16 Rotx_Raw, Roty_Raw, Rotz_Raw = 0;
  //begin a session with MPU_60X0
  I2C_Macro_BeginTransmission(MPU_ADDRESS);
    // Access The GYRO_X register
    I2C_Macro_Write(GYRO_XOUT);
    I2C_Macro_EndTransmission(false);
    //Access 2 byte of data to be read
    I2C_Macro_RequestFrom(MPU_ADDRESS, 6);
    while(I2C_Macro_Available() < 6);
    
    Rotx_Raw = I2C_Macro_Read()<<8 | I2C_Macro_Read();
    Roty_Raw = I2C_Macro_Read()<<8 | I2C_Macro_Read();
    Rotz_Raw = I2C_Macro_Read()<<8 | I2C_Macro_Read();

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