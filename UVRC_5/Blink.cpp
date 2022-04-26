/*
  Blink.h - Library for wraping the functions controling the Return to Blink 
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Blink.h" 
#include "Context.h"

Blink::Blink(int pin){}

void Blink::setup(Context &_context){
  context = &_context;
  pinMode(12, OUTPUT);
  apply();
}

void Blink::apply(){
  digitalWrite(12, !digitalRead(12));
}
