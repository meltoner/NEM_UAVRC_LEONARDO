/*
  Context.h - Library for wraping the functions controling the Enviroment context
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Context.h"
#include <EEPROM.h>

Context::Context(int pin){  
  _pin = pin;
}

void Context::setup(){
    Serial.begin(9600);
    EEPROM.get(0, TARGET_LAT);
    EEPROM.get(sizeof(long), TARGET_LNG);
}

void Context::reflectSensor(float value, int precission){
  Serial.print(value, value == 0?0:precission);
  Serial.print(F(" "));
}

void Context::apply(){
  // todo this consumes a lot of memory is there any other way to do it

  // Battery voltage
  reflectSensor(capacity, 1); 
  reflectSensor(voltage, 1); 

  // mpu: x, y, z, mag: z
  for(int i = 0; i < 4; i++)
    reflectSensor(positional[i], 1);

  // 0 {mag offset}, 1 {true heading (mpu z + mag offset)}, 2 {difference of true heading and target heading}
  for(int i = 0; i < 3; i++)
    reflectSensor(derivatives[i], 1);// offset, heading

  // Target heading
  reflectSensor(targets[0], 1); 

  // GPS return to home target heading
  reflectSensor(targets[1], 1);

 // Gps return to home target distance in meters
  reflectSensor(targets[2], 1); 

 // Servo Steer value
 reflectSensor(actuators[0], 0);

  // Throttle
  reflectSensor(actuators[1], 0); 

  // GPS latitute  
  reflectSensor(latlng[0], 6);
  // GPS longditude
  reflectSensor(latlng[1], 6);

  // Flysky remote control values
  for(int i = 0; i < 10; i++)
    reflectSensor(ext_sensors[i], 0);
  
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