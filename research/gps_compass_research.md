
-------
notes on incorporating also the magnetometer of the gps ublox m8n
==
https://www.youtube.com/watch?v=BVP0ny55STQ

HMC5883L is used in ublox 
community also replied that a ist8310 is used 

https://github.com/kriswiner/MPU6050/wiki/Affordable-9-DoF-Sensor-Fusion
https://github.com/kriswiner/MPU6050HMC5883AHRS

https://forum.arduino.cc/t/arduino-mpu6050-hmc5883l/631657/5

https://howtomechatronics.com/tutorials/arduino/how-i2c-communication-works-and-how-to-use-it-with-arduino/

magnetometer address 0x1E
mpu 0x68

either one is scanned not at the same time - when both only the 0x68 is udentified

https://www.ti.com/lit/an/slva704/slva704.pdf
https://www.circuitbasics.com/basics-of-the-i2c-communication-protocol/

https://learn.sparkfun.com/tutorials/i2c/all
https://www.sparkfun.com/products/12009?_ga=2.167635646.1082899826.1648417824-1013542653.1638983343


https://electropeak.com/learn/interfacing-gy-87-10dof-imu-mpu6050-hmc5883l-bmp085-module-with-arduino/

https://www.elecrow.com/imu-10dof-mpu6050hmc5883lbmp180-p-1238.html
