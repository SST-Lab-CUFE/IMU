#ifndef MPU_PRIVATE_H_
#define MPU_PRIVATE_H_

#include "STD_TYPES.h"
#include "MPU_config.h" 

#if AD0==LOW
#define MPU_ADDRESS 			  (*(volatile u8 *)0x68)
#else 
#define MPU_ADDRESS 			  (*(volatile u8 *)0x69)
#endif

//MPU_60X0 configuration and control registers
#define SMPLRT_DIV 			    (*(volatile u8 *)0x19)
#define CONFIG     			    (*(volatile u8 *)0x1A)
#define GYRO_CONFIG 			  (*(volatile u8 *)0x1B)
#define ACCEL_CONFIG 			  (*(volatile u8 *)0x1C)
#define PWR_MGMT_1 			    (*(volatile u8 *)0x6B)

//MPU_60X0 Data registers
#define ACCEL_XOUT    		  (*(volatile u8 *)0x3B)

#define TEMP_OUT    	   	  (*(volatile u8 *)0x41)

#define GYRO_XOUT      		  (*(volatile u8 *)0x43)



#endif /* MPU_PRIVATE_H_ */