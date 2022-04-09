/*
  Mpu.h - Library for wraping the functions controling the Mpunometer
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Mpu.h"

#include <Wire.h>
#include <MPU6050_light.h>

Mpu::Mpu(int pin){  
  _pin = pin; 
}

MPU6050 sensor(Wire);

void Mpu::setup(float sensors[]){
  Wire.begin();
  while(sensor.begin() != 0)
    Serial.println("MPU6050 not detected!");

  Serial.println("Do not move");
  delay(1000); 
  sensor.calcOffsets();
  Serial.println("MPU ready.");
}
void Mpu::update(){
  sensor.update();  
}
void Mpu::apply(float sensors[]){
    sensors[4] = sensor.getAngleX();
    sensors[5] = sensor.getAngleY();
    sensors[6] = sensor.getAngleZ(); 
    
    degree = -((int)sensors[6]) % 360;    
    if(degree<0)
      degree+=360;
}
