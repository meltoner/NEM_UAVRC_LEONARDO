// Konstantinos Papageorgiou - 2022 - kp@rei.gr
//
// # NEM Unmanned Vehicle Remote controlled - UVRC
// 
// ## Software Features :
// 
// - Software utilises an MPU, GPS, Servo, ESC, magnetometer and a remote control input, in an arduino leonardo.
// - Sensors and accuators are all in classes
// - Considers seven interval / frequencies of task execution
// - Controls steering based on target 
// - Boots esc motor, and applies a non liner transfer function to the speed remote input
// - Computes degrees heading at boot time, used as an offcet for the mpu degree heading
// - Derives distance and degree from a target gps LAT LOT
// - Return to home on signal loss
// - pending-Detection of low battery and return to home
// 
// ## Hardware
// 
// Sensors :
// 
// - Gps : M8n
// - Magmetometer : M8n's HMC5883
// - Receiver : flysky F9-ia10b
// - MPU : MPU6050
// - Voltage sensor 0-25v MH-electronic
// 
// Other: 
// - Transmitter : flysky FS-I6X
// - Arduino Leonardo
// - Sparkfun Logic Level Converter Bi-Directional

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

  Serial.println(F("Powering up"));
  remote.setup(context);
  mag.setup(context);
  steer.setup(context);
  gps.setup(context);
  battery.setup(context);
  home.setup(context);
  invoker.setup(context);
  Serial.println(F("Setup done"));

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
  //every 0.5 second printout allenviromental variables
  //context.apply();
  remote.telemetry();
}

void heartBeat(){
  //dynamic periodicity 
  // Every : 2s when no gps, 1sec when gps lock, 0.5 seconds when returing to home is active, 0.1 second when power is less than 30%.
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
      case 7: home.bursts();break;
      case 100: break;
    }
}

void loop(){
  mpu.update(); 
  run_invoker(invoker.apply());
}
