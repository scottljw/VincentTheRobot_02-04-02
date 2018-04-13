#include <SparkFun_MAG3110.h>
// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// BMP280
// This code is designed to work with the BMP280_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Barometer?sku=BMP280_I2CSs#tabs-0-product_tabset-2

#include<Wire.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
// BMP280 I2C address is 0x76(108)
#define Addr 0x76
Adafruit_BMP280 bmp; // I2C
float x,y,z;
MAG3110 mag = MAG3110();
float xf, yf, zf;
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  //Serial.println("hi1");
  mag.initialize();
   if(!mag.error) //You can use this to check if there was an error during initialization.
  {
    mag.setDR_OS(MAG3110_DR_OS_1_25_32);
    mag.start();
  }
  if (!bmp.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  triaxle();
  x = xf; y = yf; z = zf;
  
  //Serial.println("hi2");
}
void loop(){
  Serial.println("hi3");
  triaxle();
  delay(500);
  printPreAndTemp();
  TriAxleCal();
  printTriAxle();
  delay(1500);
}
void printPreAndTemp(){
  Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); // this should be adjusted to your local forcase
    Serial.println(" m");
    
    Serial.println();
    delay(2000);
}

void triaxle(){
  if(mag.error){
    Serial.println("Could not connect to MAG3110 Sensor!");
  }
  if(mag.dataReady()) {
    mag.readMicroTeslas(&xf, &yf, &zf); //This divides the values by 10 to get the reading in microTeslas
  }
}
void TriAxleCal(){
  xf = xf - x;
  yf = yf - y;
  zf = zf - z;
}
void printTriAxle(){
    Serial.println("from this");
    Serial.print("X: ");
    Serial.print(xf);
    Serial.print(", Y: ");
    Serial.print(yf);
    Serial.print(", Z: ");
    Serial.println(zf);
}



