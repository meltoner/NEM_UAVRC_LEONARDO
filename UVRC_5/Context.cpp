/*
  Context.h - Library for wraping the functions controling the Enviroment context
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Context.h"


Context::Context(int pin){  
  _pin = pin;
}

void Context::setup(){
    Serial.begin(9600);
}

void Context::reflectSensor(float value, int precission){
  Serial.print(value, value == 0?0:precission);
  Serial.print(" ");
}

void Context::apply(){


//  reflectSensor(derivatives[0], 0); //offset
//  reflectSensor(derivatives[1], 0); //heading
//  reflectSensor(derivatives[2], 0); // diff from target heading
////  reflectSensor(positional[2], 1); // mpu heading
////  reflectSensor(positional[3], 1); // mag heading
//
//  for(int i = 0; i < 4; i++)
//    reflectSensor(positional[i], 1);
//
  reflectSensor(targets[0], 1); // target heading
  reflectSensor(targets[1], 1); // gps target heading
  reflectSensor(targets[2], 1); // target distance
//
//  reflectSensor(actuators[0], 0); // steer
//  reflectSensor(actuators[1], 0); // throttle
//  
//  reflectSensor(latlng[0], 6);
//  reflectSensor(latlng[1], 6);
//
//  for(int i = 0; i < 10; i++)
//    reflectSensor(ext_sensors[i], 0);
//  
  Serial.println(""); 

}

int Context::pruneDegrees(float value){
  if(value < 0)
    return 360 + (((int)value) % 360);
  return ((int)value) % 360;
} 