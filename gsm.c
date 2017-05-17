#include "Wire.h"
#include "MPU6050.h"
#include "I2Cdev.h"
#include <SD.h>
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial gprsSerial(7, 8);
int keyPin = 2;
int vibroState = 0;
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t ax1, ay1, az1;
int16_t gx1, gy1, gz1;
int16_t tmp, tmp1;

Servo servo;
File sensorDataFile; 
char* SENSOR_DATA_FILE_NAME = "log.txt";

void setup() {
  gprsSerial.begin(19200);
  delay(1000);
  servo.attach(4);
  Serial.begin(9600);
  Wire.begin();
  pinMode(10, OUTPUT);
  if( !SD.begin(10))
  {
        Serial.println("Initialization failed!");
        return;
  }
  SD.remove("log.txt");
  accelgyro.initialize();
  delay(100);
}

void loop() {
 int flag1=0, flag2=0, flag3=0, flag4=0;
 vibroState = digitalRead(keyPin);
 accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
 tmp = accelgyro.getTemperature();
 Serial.print("a/g:\t");
 Serial.print(ax); Serial.print("\t");
 Serial.print(ay); Serial.print("\t");
 Serial.print(az); Serial.print("\t");
 Serial.print(gx); Serial.print("\t");
 Serial.print(gy); Serial.print("\t");
 Serial.print(gz); Serial.print("\t");
 Serial.println(tmp);
 delay(1000);
 accelgyro.getMotion6(&ax1, &ay1, &az1, &gx1, &gy1, &gz1);
 tmp1 = accelgyro.getTemperature();
 Serial.print("a/g:\t");
 Serial.print(ax1); Serial.print("\t");
 Serial.print(ay1); Serial.print("\t");
 Serial.print(az1); Serial.print("\t");
 Serial.print(gx1); Serial.print("\t");
 Serial.print(gy1); Serial.print("\t");
 Serial.print(gz1); Serial.print("\t");
 Serial.println(tmp1);
 
int s1=ax-ax1;
int s2=ay-ay1;
int s3=az-az1;
int s4=gx-gx1;
int s5=gy-gy1;
int s6=gz-gz1;
int s7=tmp-tmp1;

servo.write(90);
//servo.writeMicroseconds(1500);
sensorDataFile = SD.open(SENSOR_DATA_FILE_NAME, FILE_WRITE);
if((abs(s1)>5000)||(abs(s2)>5000)||(abs(s3)>5000))
{
  flag1=1;
  char msg[128];
  sprintf(msg, "Accelerometer. Change the level of the car. %d %d %d",s1,s2,s3);
  Serial.println(msg);
  sensorDataFile.println(msg);
}
if((abs(s4)>500)||(abs(s5)>500)||(abs(s6)>500))
{
  flag2=1;
  char msg[128];
  sprintf(msg, "Gyroscope. Change the level of the car. %d %d %d",s4,s5,s6);
  Serial.println(msg);
  sensorDataFile.println(msg);
}
if(abs(s7)>100)
{
  flag3=1;
  char msg[128];
  sprintf(msg, "Temperature sensor. Sudden changes in temperature. %d",s7);
  Serial.println(msg);
  sensorDataFile.println(msg);
}
if(vibroState == HIGH)
{
  flag4=1;
  char msg[128];
  sprintf(msg, "Vibro Sensor. Blow on the car. %d",flag4);
  Serial.println(msg);
  sensorDataFile.println(msg);
}
if((flag1==1)||(flag2==1)||(flag3==1)||(flag4==1))
 {
   servo.write(180);
   delay(1000);
   servo.write(0);
   gprsSerial.println("ATD + +37544111111;");
   delay(10000);
 }
 sensorDataFile.close();
 flag1=0;
 flag2=0;
 flag3=0;
 flag4=0;
 delay(1000);
}
