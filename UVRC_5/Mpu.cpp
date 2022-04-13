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
    context->sensors[4] = sensor.getAngleX();
    context->sensors[5] = sensor.getAngleY();
    context->sensors[6] = sensor.getAngleZ(); 
    
    degree = -((int)context->sensors[6]) % 360;    
    if(degree < 0)
      degree += 360;

    context->sensors[0] = degree;
}
