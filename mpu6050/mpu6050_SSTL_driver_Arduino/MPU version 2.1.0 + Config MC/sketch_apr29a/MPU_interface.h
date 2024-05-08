#ifndef MPU_INTERFACE_H_
#define MPU_INTERFACE_H_

#include "STD_TYPES.h"
#include "MPU_private.h"

typedef enum{
  AFS_2g = 0,
  AFS_4g,
  AFS_8g,
  AFS_16g
}Accel_Range;

typedef enum{
  GFS_250 = 0,
  GFS_500,
  GFS_1000,
  GFS_2000
}Gyro_Range;

// Digital low pass filter bandwidth for Accelerometer and Gyroscope respectivly => F(Accelerometer)_(Gyroscope)
typedef enum{
  disable_DLPF=0,   //F260_256
  F184_188,
  F94_98,
  F44_42,
  F21_20,
  F10_10,
  F_5_5
}DLPF_BW;



void MPU_Init         (Accel_Range A_range, Gyro_Range Rot_range, DLPF_BW DLPF_CFG);
void MPU_Get_Accel    (float* Ax, float* Ay, float* Az);
void MPU_Get_Temp     (float* Temp);
void MPU_Get_AngRate  (float* Rotx, float* Roty, float* Rotz);
void MPU_Get_Angles   (float* Roll_X, float* Pitch_Y);


#endif /* MPU_INTERFACE_H_ */