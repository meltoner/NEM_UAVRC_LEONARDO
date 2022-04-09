// Konstantinos Papageorgiou - 2022 - kp@rei.gr
// Code utilising MPU6050, GPS, Servo
// Considers four time intervalas - 1ms - 10, 50 , 500 and 5000
// Uses Smotthing with dynamic number of points per sensor value
// Able to derive distance and degrees from a target LAT LOT
// Controls steering throttle to achieve 30 degrees roll
// Computes actual degree of the earth, by checking the gps consequent points - when on straight line - to derive the mpu offset from time of start.
// sds
 
#include "Morse.h"

#include <IBusBM.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#include <Wire.h>
#include <MPU6050_light.h>

#include <Servo.h>

#include <AltSoftSerial.h>
#include <TinyGPS++.h>

MPU6050 mpu(Wire);

Servo steer;
Servo throttle;

int throttleValue = 0;
int steerValue = 0;
int center = 94;

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
sensors_event_t event; 

TinyGPSPlus gps;
AltSoftSerial gpsPort; //5 ,13
IBusBM IBus; // IBus object 

// lat, lng, distance, degrees, x, y, z, mag // where z matches to degrees
#define SENSORS 8
float sensors[] = {0,0,0,0,0,0,0,0};

#define EXT_SENSORS 10
byte ext_sensors[] = {0,0,0,0,0,0,0,0,0,0};

float zoffset=0;

unsigned long now = millis();
int intervals[] = {10,50,100,500,5000};
unsigned long timers[] = {now,now,now,now,now};

boolean gpsLock = false;

//Akropoli
double TARGET_LAT = 37.9584512;
double TARGET_LNG = 23.7502464;


void updateMag(){  
  mag.getEvent(&event);    
  float heading = atan2(event.magnetic.x, event.magnetic.y);  // are inverted
  heading = heading -3.3;
  // Find yours here: http://www.magnetic-declination.com/
  
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
    
  heading = heading * 180/M_PI;

  if(heading<0)
    heading = 360+heading;
    
  sensors[7] = heading;
  
}

void setupMag(){
  while(!mag.begin()) 
    Serial.println("HMC5883 not detected!");
  sensor_t sensor;
  mag.getSensor(&sensor);  
  
  for(int i =0;i<10;i++){
    updateMag();
    zoffset += sensors[7];
    delay(100);
  }

  zoffset = zoffset /10;
  Serial.println("Mag sensor ready"); 
}

void setupMPU(){
  Wire.begin();
  while(mpu.begin() != 0)
    Serial.println("MPU6050 not detected!");
  
  Serial.println("Do not move");
  delay(1000); 
  mpu.calcOffsets();
  Serial.println("MPU ready.");
}

void setSteer(int value){
  if(steerValue != value){
    steerValue = value;
    steer.write(value);  
  }  
}

void setupSTEER(){
  steer.attach(6); 
  setSteer(center);
  Serial.println("Steer ready.");
}

void setupTHROTTLE(){
  throttle.attach(9); 
  throttle.writeMicroseconds(900); // send "stop" signal to ESC. Also necessary to arm the ESC.
  delay(1000); // delay to allow the ESC to recognize the stopped signal.
  Serial.println("Throttle ready.");
}

void setupGPS(){
  gpsPort.begin(9600);
  Serial.println("GPS ready.");
}

void setupIBUS(){
  IBus.begin(Serial1);
  delay(200);
  Serial.println("IBUS ready.");
}



int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue){ 
  //https://www.youtube.com/watch?v=JZQoHmLqZYg
  uint16_t ch = IBus.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

void updateIBUS(){
  for(int i = 0; i < EXT_SENSORS; i++)
   ext_sensors[i] = (byte)readChannel(i, 0, 255, 0);  
}

void setThrottle(int value){
  if(value > ext_sensors[4])
    value = ext_sensors[4];

  if(throttleValue != value){
    throttleValue = value;
    int pwmVal = map(value, 0, 255, 1000, 2000); // maps to PWM value.
    throttle.writeMicroseconds(pwmVal); // Send signal to ESC.  
  }  
}

int degree = 0;
void updateMPU() {  
    sensors[4] = mpu.getAngleX();
    sensors[5] = mpu.getAngleY();
    sensors[6] = mpu.getAngleZ(); 
    degree = -((int)sensors[6]) % 360;    
    if(degree<0)degree+=360;
    
    //degree = degree-zoffset;
}

int target = 0;

int getDegreeDiff(){
     int diff = target-degree;
     if(diff>180)
      diff=diff-360;
     if(diff<-180)
      diff=diff+360;
            
     if(diff>45)
      diff=45;
     if(diff<-45)
      diff=-45;
      
    return diff;
}

void updateSteer(){
   int control = map(ext_sensors[0], 0, 255, -50, 50);
   if(hasTarget())
      setSteer(center - getDegreeDiff());
   else
      setSteer(center - control );
}

int transferFunction(int value, int theshold, int add, int divider){
   float x = (float)value;    
    if(x<theshold){
      x = 0;
    }else{
      x = x+add;
      x = (x * (x/divider))+(10/2);
    }
    return (int)x;
}

void updateThrottle(){
    setThrottle(transferFunction(ext_sensors[2], 15, 20, 300)); 
}

boolean hasTarget(){
  return ext_sensors[6] == 255 ;
}

boolean isSwitchCHalf(){
   return ((int)ext_sensors[8]) == 127;
}

boolean isSwitchCFull(){
  return ext_sensors[8] == 255 ;
}

boolean updateGpsDegreeTarget(){
  if(isSwitchCHalf())
   target = (int)sensors[3];

  if(isSwitchCFull() && gpsLock){
    TARGET_LAT = sensors[0];
    TARGET_LNG = sensors[1];
  }
}

boolean hasNewDegree(){
  if(hasTarget()){
    int control = map(ext_sensors[0], 0, 255, 0, 40)-20;
    if(abs(control)>2 && !(isSwitchCHalf()))
      target += control/2;
  }
  if(ext_sensors[7] == 255)
    target = degree;
}

void updateGPS(){
   while (gpsPort.available())
      gps.encode(gpsPort.read());   

   if(!gpsLock && gps.location.lat() != 0.0)
     gpsLock = true;
   
   if(gpsLock){
     sensors[0] = gps.location.lat();
     sensors[1] = gps.location.lng();
     returnHome();
   }
}

//-----------------------------------------

void returnHome(){    
    sensors[2] = gps.distanceBetween(sensors[0], sensors[1], TARGET_LAT, TARGET_LNG) / 1000.0;
    sensors[3] = gps.courseTo(sensors[0], sensors[1], TARGET_LAT,TARGET_LNG) * 1000;
}

void run_invoker(){
  mpu.update();
  now = millis();
  for(int i = 0; i < 4; i++){
   if ((now - timers[i]) > intervals[i]) {
    timers[i] = now;
    switch(i){
      case 0: apply_very_fast_invoker(); break;
      case 1: apply_fast_invoker(); break;
      case 2: apply_invoker(); break;
      case 3: apply_slow_invoker(); break;
      case 4: apply_very_slow_invoker(); break;
    }
   }
  }
}

void printSensors(){
 // Serial.print(Serial.print(sensors[6], 2));
 // Serial.print(" ");
 // Serial.print(Serial.print(sensors[7], 2));
 // Serial.print(" ");
 // Serial.print(zoffset);
 // Serial.print(" ");
 // Serial.print(Serial.print(sensors[6]+zoffset, 2));
//  Serial.println(" ");

 Serial.print(Serial.print(-sensors[6]+zoffset, 2));
 Serial.print(" ");

  for(int i = 0; i < SENSORS; i++){
    if(i<2)
      Serial.print(sensors[i], 6);
    else
      Serial.print(sensors[i], 2);
   Serial.print(" ");
  }
  for(int i = 0; i < EXT_SENSORS; i++){
   Serial.print(ext_sensors[i]);
   Serial.print(" ");
  }
  Serial.print(throttleValue);
  Serial.print(" ");
  Serial.print(steerValue);
  Serial.print(" ");
  Serial.print(target); 
  Serial.println(""); 
 
}


void setup() {
  setupTHROTTLE();
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("Setting up"); 
  setupIBUS();
  setupMag();
  setupMPU();  
  setupSTEER();
  setupGPS();
  Serial.println("Setup done");
}

void apply_very_fast_invoker(){   
   updateMag();
   updateMPU();   
}

void apply_fast_invoker(){
   updateIBUS();
   updateSteer();
   updateThrottle();
}

void apply_invoker(){
  updateGPS();
  hasNewDegree();
  updateGpsDegreeTarget();
}
void apply_slow_invoker(){
  printSensors();
}

void apply_very_slow_invoker(){
}

void loop(){ 
 run_invoker();
}
