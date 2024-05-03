#include "driver_mpu6050_interface.h"

void setup() {
  Serial.begin(9600);
  mpu6050_interface_debug_print("%s %d %c %f", "Hello", 6,'a',1.6);
}

void loop() {
}
