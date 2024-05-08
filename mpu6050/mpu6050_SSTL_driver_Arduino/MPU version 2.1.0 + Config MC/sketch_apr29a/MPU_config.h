#ifndef MPU_CONFIG_H_
#define MPU_CONFIG_H_

#include <Arduino.h>
#include <Wire.h>

//define the hardware connection of the AD0 Pin for the MPU_60X0
#define AD0   LOW 


//define the required I2C functions "I2C_Macro_XXXX" according to the used MC 
#define I2C_Macro_Begin()                             Wire.begin()
#define I2C_Macro_BeginTransmission(SLV_ADD)          Wire.beginTransmission(SLV_ADD)   
#define I2C_Macro_Write(VALUE)                        Wire.write(VALUE)
#define I2C_Macro_EndTransmission(STOP_STATE)         Wire.endTransmission(STOP_STATE)   
#define I2C_Macro_RequestFrom(SLV_ADD, LENGTH)        Wire.requestFrom(SLV_ADD, LENGTH)   
#define I2C_Macro_Available()                         Wire.available()   
#define I2C_Macro_Read()                              Wire.read()   


#endif /* MPU_CONFIG_H_ */