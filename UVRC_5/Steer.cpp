/*
  Steer.h - Library for wraping the functions controling the Steer
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Steer.h"
#include <Servo.h>

Steer::Steer(int pin){  
  _pin = pin;
}

void Steer::setup(){
  steer.attach(_pin); 
  setSteer(center);
  Serial.println("Steer ready.");
}

void Steer::setSteer(int value){
  if(steerValue != value){
    steerValue = value;
    steer.write(value);  
  }  
}

boolean Steer::hasTarget(byte ext_sensors[]){
  return ext_sensors[6] == 255 ;
}

int Steer::getDegreeDiff(){
     int diff = target - degree;

     if(diff > 180)
      diff = diff -360;

     if(diff < -180)
      diff = diff + 360;
            
     if(diff > 45)
      diff = 45;

     if(diff < -45)
      diff=-45;
    return diff;
}

boolean Steer::isSwitchCHalf(byte ext_sensors[]){
   return ((int)ext_sensors[8]) == 127;
}

void Steer::apply(float sensors[], byte ext_sensors[], int current_degree){
   degree = current_degree;
   if(hasTarget( ext_sensors )){
      setSteer( center - getDegreeDiff() );
   }else{
      int control = map(ext_sensors[0], 0, 255, -50, 50);
      setSteer( center - control );
   }
}

boolean Steer::hasNewDegree(byte ext_sensors[]){
  if(hasTarget(ext_sensors)){
    int control = map(ext_sensors[0], 0, 255, 0, 40) - 20;
    if(abs(control) > 2 && !(isSwitchCHalf(ext_sensors)))
      target += control / 2;
  }
  if(ext_sensors[7] == 255)
    target = degree;
}