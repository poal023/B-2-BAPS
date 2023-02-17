//This code was written by Team Taarakian
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055(55);

//This function sets up the arduino ports to begin the program operation
void setup() {
  Serial.begin(9600);
  if(!bno.begin()) {
    Serial.println("Error initializing BNO055");
    while(1);
  }
  //bno.setMode(Adafruit_BNO055::OPERATION_MODE_NDOF);
}

//This function constantly detects event changes to the gimbal and writes the calculated x, y, z 
//axis to the receiver so it could write to the lcd display.
void loop() {
  sensors_event_t event;
  bno.getEvent(&event);
  
  int x = event.orientation.x;
  int y = event.orientation.y;
  int z = event.orientation.z;

  Serial.write(x);
  Serial.write(y);
  Serial.write(z);
  
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
