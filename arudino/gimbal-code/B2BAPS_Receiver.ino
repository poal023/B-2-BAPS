//This code was written by Team Taarakian
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <utility/imumaths.h>

// LCD object
LiquidCrystal_I2C lcd(0x27, 20, 4); // i2c address 0x27 and 16x2 display size
//Initializes once and sets up the arduino port being used by the LCD
void setup() {
  Serial.begin(9600);
    // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Test
  lcd.setCursor(0,0);
  lcd.print("hello everyone");
  lcd.setCursor(0,1);
  lcd.print("konichiwaa");
  delay(2000);
  lcd.clear();

}
//Constantly reads the changes that occur to the x, y, z axis of the gimbal and prints it on the lcd.
void loop() {
  while(Serial.available() >= 3) {
    int x = Serial.read();`
    int y = Serial.read();
    int z = Serial.read();

    lcd.setCursor(0,0);
    lcd.print("X: ");
    lcd.print(x);
    
    //lcd.setCursor(0,4);
    lcd.print("   Y: ");
    lcd.print(y);

    lcd.setCursor(0,1);
    lcd.print("Z: ");
    lcd.print(z);
  }
}
