/*
  Mpu.h - Library for wraping the functions controling the Mpunometer
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Mpu.h"

#include <Wire.h>
#include <MPU6050_light.h>
#include "Context.h"

Mpu::Mpu(int pin){  
  _pin = pin; 
}

MPU6050 sensor(Wire);

void Mpu::setup(Context &_context){
  context = &_context;

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

void Mpu::apply(){
    context->positional[0] = sensor.getAngleX(); 
    context->positional[1] = sensor.getAngleY();
    context->positional[2] = -sensor.getAngleZ();
}
