/*
  Mag.h - Library for wraping the functions controling the Magnometer
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Context.h"
#include "Mag.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

Mag::Mag(int pin){  
  _pin = pin;
  mag = Adafruit_HMC5883_Unified(12345);
}

void Mag::setup(Context &_context){ 
  context = &_context;

  while(!mag.begin()) 
    Serial.println("HMC5883 not detected!");

  sensor_t sensor;
  mag.getSensor(&sensor);  
  
  for(int i = 0; i < 10; i++){
    apply();    
    delay(200);
  }

  initMagOffset();  
  Serial.println("Mag sensor ready"); 
}

void Mag::apply(){
  mag.getEvent(&event);
  
  float heading = atan2(event.magnetic.x, event.magnetic.y);
  heading = heading -3.3;
  
  if(heading < 0)
    heading += 2 * PI;

  if(heading > 2 * PI)
    heading -= 2 * PI;
    
  heading = heading * 180 / M_PI;

  if(heading < 0)
    heading = 360 + heading;

  context->positional[3] = heading;

  context->derivatives[1] = context->positional[2] + context->derivatives[0];

  if(context->derivatives[1] < 0)
    context->derivatives[1] = 360 + context->derivatives[1];

  if(context->derivatives[1] > 360)
    context->derivatives[1] = context->derivatives[1] - 360;

  //========

  int diff = context->targets[0] - context->derivatives[1];

  if(diff > 180)
    diff = diff -360;

   if(diff < -180)
    diff = diff + 360;

  context->derivatives[2] = diff;
} 

float Mag::getOffset(){
  float mag = context->positional[3];
  float mpu = context->positional[2];
  return  mag - mpu;
}

void Mag::initMagOffset(){  
    context->derivatives[0] = getOffset();
}

void Mag::updateMagOffset(){
  if(abs(context->positional[0]) < 7) // is horizotal
    initMagOffset();
    //context->derivatives[0] = context->derivatives[0]*0.8 + getOffset()*0.2;
}

