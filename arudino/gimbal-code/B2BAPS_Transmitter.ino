#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  Serial.begin(9600);
  if(!bno.begin()) {
    Serial.println("Error initializing BNO055");
    while(1);
  }
  //bno.setMode(Adafruit_BNO055::OPERATION_MODE_NDOF);
}

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
