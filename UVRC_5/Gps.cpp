/*
  Gps.h - Library for wraping the functions controling the Gps 
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Gps.h" 
#include <AltSoftSerial.h>
#include <TinyGPS++.h>
#include "Context.h"

Gps::Gps(int pin){}

void Gps::setup(Context &_context){
  context = &_context;

  isLocked = false;
  gpsPort.begin(9600);

  TARGET_LAT = 37.9584512;   //Akropoli
  TARGET_LNG = 23.7502464;
  Serial.println("Gps ready.");
}

void Gps::apply(){
  while (gpsPort.available())
    gps.encode(gpsPort.read());   

  if(!isLocked && gps.location.lat() != 0.0)
   isLocked = true;

  if(isLocked){
   context->latlng[0] = gps.location.lat();
   context->latlng[1] = gps.location.lng();
   returnHome();
  }
}

void Gps::setTarget(double LAT, double LNG){
  TARGET_LAT = LAT;
  TARGET_LNG = LNG;
}

void Gps::returnHome(){    
  context->sensors[2] = gps.distanceBetween(context->latlng[0], context->latlng[1], TARGET_LAT, TARGET_LNG) / 1000.0;
  context->sensors[3] = gps.courseTo(context->latlng[0], context->latlng[1], TARGET_LAT,TARGET_LNG) * 1000;
}