/*
  Context.h - Library for wraping the functions controling the Enviroment context
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Context.h"
#include <EEPROM.h>

Context::Context(byte pin){  
  _pin = pin;
}

void Context::setup(){
    Serial.begin(9600);
    EEPROM.get(0, TARGET_LAT);
    EEPROM.get(sizeof(long), TARGET_LNG);
}

void Context::reflectSensor(float value, byte precission){
  Serial.print(value, value == 0?0:precission);
  Serial.print(" ");
}

void Context::apply(){ // takes 4% of memory

  reflectSensor(capacity, 1); 
  reflectSensor(voltage, 1); 

  for(int i = 0; i < 4; i++)
    reflectSensor(positional[i], 1); // mpu: x, y, z, mag: z

  // 0 {mag offset}, 1 {true heading (mpu z + mag offset)}, 2 {difference of true heading and target heading}
  for(int i = 0; i < 3; i++)
    reflectSensor(derivatives[i], 1); // offset, heading

  for(int i = 0; i < 2; i++)
    reflectSensor(actuators[i], 1); // Servo Steer value, Throttle    

  // Flysky remote control values
  for(int i = 0; i < 10; i++)
    reflectSensor(ext_sensors[i], 0);

  for(int i = 0; i < 2; i++)
    reflectSensor(latlng[i], 6); // current gps

  for(int i = 0; i < 3; i++)
    reflectSensor(targets[i], 1); // Target heading, GPS return to home target heading, Gps return to home target distance in meters
  
  Serial.println(F("")); 

}

int Context::pruneDegrees(float value){
  if(value < 0)
    return 360 + (((int)value) % 360);
  return ((int)value) % 360;
}

boolean Context::isSwitchA(){
  return ext_sensors[6] == 255 ;
}

boolean Context::isSwitchB(){
  return ext_sensors[7] == 255 ;
}

boolean Context::isSwitchCHalf(){
   return ext_sensors[8] == 127;
}

boolean Context::isSwitchC(){
  return ext_sensors[8] == 255 ;
}

boolean Context::isSwitchD(){
  return ext_sensors[9] == 255 ;
}

void Context::setGPSTarget(double LAT, double LNG){
  TARGET_LAT = LAT;
  TARGET_LNG = LNG;
  EEPROM.put(0, LAT);
  EEPROM.put(sizeof(long), LNG);
}

int Context::transferFunction(int value, int theshold, int add, int divider){
  //gates and transforms a linear to a non linear curve, accelerating faster as we reach the max value 

  float x = (float)value;
  float sign = 1;

  if(value < 0)
    sign = -1;

  if(abs(x) < theshold){
    x = 0;
  }else{
    x = x + add * sign;
    x = (sign * x * (x / divider )) + (theshold * sign) / 3;
  }
  return (int)x;
}