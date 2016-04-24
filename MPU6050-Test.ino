/*
    MPU6050 Triple Axis Gyroscope & Accelerometer. Pitch & Roll Accelerometer Example.
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-zyroskop-i-akcelerometr-mpu6050.html
    GIT: https://github.com/jarzebski/Arduino-MPU6050
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() 
{
  Serial.begin(115200);

  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  //Calibrate
  Serial.print("Calibration...");
  mpu.calibrateGyro(1000);
  Serial.print("complete!");
  Serial.println();
  delay(1000);
}

void loop()
{
  // Read normalized values 
  Vector normGyro = mpu.readNormalizeGyro();
  float temp = mpu.readTemperature();

  // Output
  Serial.print(" DR = ");
  Serial.print(normGyro.XAxis);
  Serial.print(" DP = ");
  Serial.print(normGyro.YAxis);
  Serial.print(" DY = ");
  Serial.print(normGyro.ZAxis);
  Serial.print(" T = ");
  Serial.print(temp);
  
  Serial.println();
  
  delay(10);
}


