#include <Servo.h>
#include <Smoothed.h>

#include <AltSoftSerial.h>
#include <TinyGPS++.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#include <MPU6050_light.h>
Servo steer;

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
sensors_event_t event; 

TinyGPSPlus gps;
AltSoftSerial sc; //RX pin 8 Tx pin 8
#define gpsPort sc

MPU6050 mpu(Wire);
unsigned long timer = 0;

boolean gpsLock = false;

unsigned long lastInvocationTime = 0;
unsigned long invocationDelay = 500;

unsigned long lastFastInvocationTime = 0;
unsigned long fastInvocationDelay = 50;

// lat, lng, alt-meters, x, y, z
//float sensors[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
Smoothed <float> sensors[6];
 

 

void setupMpu(){
  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){
    Serial.println("MPU6050 not detected ... Check your wiring!");
    delay(500);
  } 
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("MPU ready.");
}
void setupMag(){
 while(!mag.begin()){    
    Serial.println("HMC5883 not detected ... Check your wiring!");
    delay(500);
  }
  sensor_t sensor;
  mag.getSensor(&sensor);  
  delay(500);
  Serial.println("Mag sensor ready"); 
}

void setupSteer(){
  steer.attach(5); 
  steer.write(90);
}

void setupGPS(){
  gpsPort.begin(9600);   
}

void setup() {
  for(int i = 0; i < 6; i++){
    sensors[i].begin(SMOOTHED_AVERAGE, 10);
    sensors[i].add(0.0);
  }
  Serial.begin(9600);
  delay(400);
  setupMpu();
  //setupSteer();
  setupGPS();
  //setupMag();
 Serial.println("Setup done");
}

void updateMag(){
  Serial.println("1");
  mag.getEvent(&event);  
  Serial.println("2");
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  // Find yours here: http://www.magnetic-declination.com/
  float declinationAngle = 0.079587;
  heading += declinationAngle;
  
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
    
  float headingDegrees = heading * 180/M_PI; 
  sensors[3].add(headingDegrees);
}

void updateGPS(){
   while (gpsPort.available()){
     Serial.print("..");
      gps.encode(gpsPort.read());
   }

   if(!gpsLock && gps.location.lat() != 0.0)
     gpsLock = true;
   
   if(gpsLock){
     sensors[0].add(gps.location.lat());
     sensors[1].add(gps.location.lng());
     sensors[2].add(gps.altitude.meters());
   }
   
}

//-----------------------------------------

double TARGET_LAT = 37.9584512;
double TARGET_LNG = 23.7502464;
  
void returnHome(){    
    double distanceKm =
      gps.distanceBetween(
        sensors[0].get(),
        sensors[1].get(),
        TARGET_LAT,
        TARGET_LNG) / 1000.0;
    
    double courseTo =
      gps.courseTo(
        sensors[0].get(),
        sensors[1].get(),
        TARGET_LAT,
        TARGET_LNG);

    Serial.print("Distance (km) to Target: ");
    Serial.println(distanceKm);
    Serial.print("Course to Target : ");
    Serial.println(courseTo);
    Serial.print("Directions: ");
    Serial.println(gps.cardinal(courseTo));
}

long now = millis();
void run_invoker(){
  updateMPU();
  now = millis();
  
  if ((now - lastFastInvocationTime) > fastInvocationDelay) {
    lastFastInvocationTime = now;
    apply_fast_invoker();    
  }
  
  if ((now - lastInvocationTime) > invocationDelay) {
    lastInvocationTime = now;
    apply_slow_invoker();    
  }
  

}

void printGPS(){  
  Serial.print("LAT=");  Serial.println(sensors[0].get(), 6);
  Serial.print("LONG="); Serial.println(sensors[1].get(), 6);
  Serial.print("ALT=");  Serial.println(sensors[3].get());   
}

void printSensors(){
  for(int i = 0; i < 6; i++){
   Serial.print(sensors[i].get());
   Serial.print(" ");
  }
  Serial.println(" ");
}


void updateMPU() {
  mpu.update();
  if((millis()-timer)>10){
    sensors[3].add(mpu.getAngleX());
    sensors[4].add(mpu.getAngleY());
    sensors[5].add(mpu.getAngleZ());
    timer = millis();  
  }
}

void updateSteer(){
   float current = sensors[3].get()/2;
   steer.write(current);
}

void apply_fast_invoker(){
  Serial.print(".");
  //if(mag.begin()){
    //updateMag();
    //updateSteer();
  //}
}

void apply_slow_invoker(){
  //updateGPS();
  printSensors();
  //returnHome();
}

void loop(){ 
 run_invoker();
}
