/*
  Home.h - Library for wraping the functions controling the Return to Home 
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Home.h" 
#include "Context.h"

Home::Home(int pin){}

void Home::setup(Context &_context){
  context = &_context;
}

void Home::apply(){
  //if(remote.isSwitchD()){
  //  context->target[0] = context->target[1];
  //}
}
