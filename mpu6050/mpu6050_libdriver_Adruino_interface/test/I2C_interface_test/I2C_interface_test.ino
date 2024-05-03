#include "driver_mpu6050_read_test.h"

void setup() {
  Serial.begin(9600);
  mpu6050_read_test(0x68,2); //test each reading 2 times
}

void loop() {
}
