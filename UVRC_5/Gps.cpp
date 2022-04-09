/*
  Gps.h - Library for wraping the functions controling the Gps 
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Gps.h" 
#include <AltSoftSerial.h>
#include <TinyGPS++.h>

Gps::Gps(int pin){
}

void Gps::setup(float sensors[]){
  isLocked = false;
  gpsPort.begin(9600);

  TARGET_LAT = 37.9584512;   //Akropoli
  TARGET_LNG = 23.7502464;
  Serial.println("Gps ready.");
}

void Gps::apply(float sensors[]){
  while (gpsPort.available())
    gps.encode(gpsPort.read());   

  if(!isLocked && gps.location.lat() != 0.0)
   isLocked = true;

  if(isLocked){
   sensors[0] = gps.location.lat();
   sensors[1] = gps.location.lng();
   returnHome(sensors);
  }
}

void Gps::setTarget(double LAT, double LNG){
  TARGET_LAT = LAT;
  TARGET_LNG = LNG;
}

void Gps::returnHome(float sensors[]){    
  sensors[2] = gps.distanceBetween(sensors[0], sensors[1], TARGET_LAT, TARGET_LNG) / 1000.0;
  sensors[3] = gps.courseTo(sensors[0], sensors[1], TARGET_LAT,TARGET_LNG) * 1000;
}