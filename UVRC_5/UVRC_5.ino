// Konstantinos Papageorgiou - 2022 - kp@rei.gr
// NEMA UVRC 
// ------------
// Software utilises an MPU, GPS, Servo, ESC, magnetometer and a remote control input, in an arduino leonardo.
// Sensors and accuators are all in classes
// Considers five intervals of task execution
// Controls steering
// Boots esc motor, and applies a non liner transfer function to the speed remote input
// Computes degrees heading at boot time, used as an offcet for the mpu degree heading
// Derives distance and degree from a target gps LAT LOT
// ------------
// gps : M8n
// magmetometer : M8n's HMC5883
// remote control: flysky F9-ia10b
// mpu : MPU6050


#include "Context.h"
#include "Invoker.h"

#include "Mag.h"
#include "Gps.h" 
#include "Mpu.h" 
#include "Steer.h"
#include "Throttle.h"
#include "Remote.h"
#include "Home.h"
#include "Blink.h"
#include "Battery.h"
#include <EEPROM.h>

Context context(0);
Invoker invoker(0);

Mag mag(0);
Gps gps(0);
Mpu mpu(4);
Steer steer(6);
Throttle throttle(9);
Remote remote(0);
Home home(0);
Blink blink(0);
Battery battery(0);

//-----------------------------------------

void setup() {

  context.setup();
  blink.setup(context);
  throttle.setup(context);
  mpu.setup(context);
  delay(2000);
  Serial.println("Powering up");
  remote.setup(context);
  mag.setup(context);
  steer.setup(context);
  gps.setup(context);
  battery.setup(context);
  home.setup(context);
  invoker.setup(context);
  Serial.println("Setup done");

}

void apply_very_fast_invoker(){
   mag.apply();
   mpu.apply();
}

void apply_fast_invoker(){
   remote.apply();
   steer.apply();
   throttle.apply();
}

void apply_invoker(){
  gps.apply();
  steer.hasNewDegree();
  home.apply();
}

void apply_slow_invoker(){ 
  //every 0.5 second
  //printout all context enviromental variables
  context.apply();
}

void heartBeat(){
  //dynamic periodicity 2s no gps, 1sec gps lock, 0.5 seconds returing to home active, 0.1 second when power less than 30%.
  blink.apply();
}

void apply_slower_invoker(){
  //every 1 second
  home.apply();
  battery.apply();
}

void updateMagOffset(){
  //10 seconds period
  mag.updateMagOffset();
}

void run_invoker(int i){
    switch(i){
      case 0: apply_very_fast_invoker(); break;
      case 1: apply_fast_invoker(); break;
      case 2: apply_invoker(); break;
      case 3: apply_slow_invoker(); break;
      case 4: apply_slower_invoker(); break;
      case 5: heartBeat(); break;
      case 6: updateMagOffset();break;
      case 100: break;
    }
}

void loop(){
     mpu.update(); 
     run_invoker(invoker.apply());
}
