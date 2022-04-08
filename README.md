# NEMA UVRC

Konstantinos Papageorgiou - 2022 - kp@rei.gr

Software utilises an MPU, GPS, Servo, ESC, magnetometer and a remote control input, in an arduino leonardo.
Sensors and accuators are all in classes
Considers five intervals of task execution
Controls steering
Boots esc motor, and applies a non liner transfer function to the speed remote input
Computes degrees heading at boot time, used as an offcet for the mpu degree heading
Derives distance and degree from a target gps LAT LOT

gps : M8n
magmetometer : M8n's HMC5883
remote control: flysky F9-ia10b
mpu : MPU6050



