/*
  Mag.h - Library for wraping the functions controling the Magnometer
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Mag.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

Mag::Mag(int pin){  
  _pin = pin;
  mag = Adafruit_HMC5883_Unified(12345)
}

void Mag::setup(){
  while(!mag.begin()) 
    Serial.println("HMC5883 not detected!");

  sensor_t sensor;
  mag.getSensor(&sensor);  
  
  for(int i = 0;i < 10;i++){
    updateMag();
    zoffset += sensors[7];
    delay(100);
  }

  zoffset = zoffset / 10;
  Serial.println("Mag sensor ready"); 
}

void Mag::apply(float sensors[]){
  mag.getEvent(&event);    
  float heading = atan2(event.magnetic.x, event.magnetic.y);  // are inverted
  heading = heading -3.3;

  // Find yours here: http://www.magnetic-declination.com/
  
  if(heading < 0)
    heading += 2 * PI;
  if(heading > 2 * PI)
    heading -= 2 * PI;
    
  heading = heading * 180 / M_PI;

  if(heading < 0)
    heading = 360 + heading;
    
  sensors[7] = heading;
}
