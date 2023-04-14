#include <Servo.h>//Servo Control
#include <Wire.h> // For i2c (receive values from RPi
//#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// define Const/Serial Addresses
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
unsigned char buffer[8]; // IEEE-754

// Global Vars
Servo servo_az; // create servo object for azimuth
Servo servo_el; // create servo object for elevation
unsigned int ADC_read[5]; // int for test pin ADC
float PD_v[5] = {0.0, 0.0, 0.0, 0.0, 0.0}; // float of ADC conversion
float curr_V; //float for min reading of current ADC conversion
int az = 0; // Azimuth Angle
int el = 0; // Elevation Angle (this value - 15 = actual angle)
float min_v = 3.0; // Initialize min test pin reading at value above upper bound
float rcvd_flt = 0.0;
float SNR_MAX = -10.0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  
  Serial.begin(9600); //For printing to Terminal
  while (!Serial) continue; // wait for serial port to connect
  Serial.write((const char *)&rcvd_flt, sizeof(float)/*4*/); // IEEE-754 pointer
  
  servo_az.attach(9);   //Azimuth control pin = 9
  servo_el.attach(10);  //Elevation control pin = 10

  // Initialize the I2C display
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // LCD Display Test
  lcd.setCursor(1,0);
  lcd.print("hello everyone");
  lcd.setCursor(1,1);
  lcd.print("konichiwaa");
  
  delay(2000);
  lcd.clear();
}

void loop() {
  
  // Tell User that the Alignment is beginning
  Serial.println("!! Starting 1st Alignment Sweep !!");
  Serial.println(" ");

  // Debug
  Serial.print("Starting AZ Angle = ");
  Serial.println(az);
  Serial.print("Starting EL Angle = ");
  Serial.println(el - 15);
  Serial.println(" ");

  // LCD Print Redundancy          
  lcd.setCursor(0, 0);
  lcd.print("Start of Align!");
  delay(2000);
  lcd.clear();

  // Debug
  //int sweep_count = 1; //Sweep Counter, Reference & Debug Only

  //First Initial Sweep, Using a range of 15° increments
  PD_Sweep(45, 135, 120, 90, 15, 1);
    
    // Start of Secondary Alignment Sweep, Fine Tuning using 5° increments
    Serial.println("Ending Initial Sweep...");
    Serial.println(" ");
    Serial.println("!! Starting 2nd Alignment Sweep !!");
    Serial.println(" ");

    // Debug
    Serial.print("2nd Start, AZ Angle = ");
    Serial.println(az);
    Serial.print("2nd Start, EL Angle = ");
    Serial.println(el - 15);
    Serial.println(" ");
    
    // Print Sweep Status to I2C Display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ending 1st Sweep");
    lcd.setCursor(0, 1);
    lcd.print("Begin 2nd Sweep!");
    delay(2000);
    lcd.clear();
  
    // Set El/Az angles for 2nd Sweep
    //int az2 = az - 10;
    //int el2 = el + 10;
    //sweep_count = 1;
    
    // 2nd Sweep Begin
    PD_Sweep((az - 10), (az + 10), (el + 10), (el - 10) , 5, 1);

    //Exits sweep pattern and currently holds angle where the minimum test pin value was recorded
    //Can use i2c to get values from RPi to sweep in smaller increments
    delay(1000);
    
    // Set EL/AZ for 3rd Sweep
    int az3 = az - 5;
    int el3 = el - 5;
    int sweep_count = 1;
                  
                  //Final Alignment Loop, will use Extra Fine Tuning using 1° increments
                  //Note: Will align using SNR values from the Pi, and not Voltage from ADC/PD
                  while(1){
                          // Setup to receieve Floats of SNR data from the Rpi_4 over UART, IEEE-754
                    if (Serial.readBytes(buffer, sizeof(float)) == sizeof(float)) {
                          memcpy(&rcvd_flt, buffer, sizeof(float));
                          Serial.print("Initial SNR: ");
                          Serial.println(rcvd_flt, 3);
                          Serial.println(" ");
                          delay(1000);
                          
                    } else {
                          // I/O error - no data, not enough bytes, etc.
                          rcvd_flt = 0.0;
                          Serial.print("No Data Rcvd!");
                          Serial.println(rcvd_flt, 3);
                          delay(1000);
                    } 

    // Start of Last Alignment Sweep, Super Fine Tuning using 1° increments
    Serial.println(" ");
    Serial.println("Ending Secondary Sweep...");
    Serial.println(" ");
    Serial.println("!! Starting Final Alignment Sweep !!");
    Serial.println(" ");

    // Debug
    Serial.print("3rd Start, AZ Angle = ");
    Serial.println(az);
    Serial.print("3rd Start, EL Angle = ");
    Serial.println(el - 15);
    Serial.println(" ");
                    
    // Print Sweep Status to I2C Display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ending 2nd Sweep");
    lcd.setCursor(0, 1);
    lcd.print("Begin Last Sweep");
    delay(2000);
    lcd.clear();

                    // Start of Last Sweep
                    for(int i = az3; i <= (az3 + 5); i += 1){
                      for(int j = el3; j <= (el3 + 5); j += 1){
                        delay(1000);
                        servo_az.write(map(i, 0, 180, 0, 180)); // 3rd azimuth servo position
                        delay(1000);
                        servo_el.write(map(j, 0, 180, 0, 180)); // 3rd elevation servo position
                        delay(1000);
                        
                        Serial.print("Current SNR = ");
                        Serial.println(rcvd_flt, 3);
                  
                        Serial.print("Maximum SNR = ");
                        Serial.println(SNR_MAX, 3);
                        Serial.println(" ");
                  
                        // Print Current SNR to I2C Display, Line 1
                        lcd.setCursor(0, 0);
                        lcd.print("Cur SNR: ");
                        lcd.print(rcvd_flt, 3);
                  
                        // Print Max SNR to I2C Display, Line 2
                        lcd.setCursor(0, 1);
                        lcd.print("Max SNR: ");
                        lcd.print(SNR_MAX, 3);
                        
                            //Checks if current test pin value is lower than the current min value
                            //If new min is found, that value and the angles it was found are recorded
                            if(rcvd_flt > SNR_MAX){
                              delay(1000);
                              SNR_MAX = rcvd_flt;
                              az = i;
                              el = j;
                              Serial.print("Updated Maximum SNR = ");
                              Serial.println(SNR_MAX, 3);
                              Serial.print("AZ angle of Max SNR = ");
                              Serial.println(az);
                              Serial.print("EL angle of Max SNR = ");
                              Serial.println(el - 15);
                              Serial.println(" ");
                              }
                              
                      delay(1000);
                      Serial.print("3rd Sweep #:  ");
                      Serial.println(sweep_count);
                      Serial.println(" ");
                      sweep_count = sweep_count + 1;
                     }
                   } //End of 3rd & Final Alignment Sweep

       // Begin Infinite Loop to Hold Final Gimbal Position
       delay(1000);
       lcd.clear();
      
       // Hold Final Angle
       while(1) {
         delay(1000);
         servo_az.write(map(az, 0, 180, 0, 180)); // final azimuth servo position
         delay(1000);
         servo_el.write(map(el, 0, 180, 0, 180)); // final elevation servo position
         delay(1000);
         
         Serial.print("Highest recorded SNR = ");
         Serial.println(SNR_MAX, 3);      
         Serial.print("Current AZ angle = ");
         Serial.println(az);   
         Serial.print("Current EL angle = ");
         Serial.println(el - 15);
         Serial.println(" ");
               
         // Print t3h end Loop
         lcd.setCursor(0, 0);
         lcd.print(" *END OF ALIGN* ");
         lcd.setCursor(0, 1);
         lcd.print("    U WIN!!?   ");
               
         delay(100000);
       }
       
  }
} // End of Main

//Sweeping function using ADC readings
void PD_Sweep(int az_Start, int az_End, int el_Start, int el_End, int stepSize, int sweep_count){
  for(int i = az_Start; i <= az_End; i += stepSize){
    for(int j = el_Start; j >= el_End; j-= stepSize){
      
      // Delays to minimize servo jittering
      delay(1000);
      servo_az.write(map(i, 0, 180, 0, 180)); // 1st azimuth servo position
      delay(1000);
      servo_el.write(map(j, 0, 180, 0, 180)); // 1st elevation servo position
      delay(1000);

      // Read the voltage from the RF Power Detector (PD) to the Analog to Digital Converter (ADC)
      ADC_read[0] = analogRead(A0);
      ADC_read[1] = analogRead(A0);
      ADC_read[2] = analogRead(A0);
      ADC_read[3] = analogRead(A0);
      ADC_read[4] = analogRead(A0);
      delay(250);
      //PD_v = ADC_read * 5.0 / 1023.0; // Convert raw value to usable data
      PD_v[0] = ADC_read[0] * 5.0 / 1023.0;
      PD_v[1] = ADC_read[1] * 5.0 / 1023.0;
      PD_v[2] = ADC_read[2] * 5.0 / 1023.0;
      PD_v[3] = ADC_read[3] * 5.0 / 1023.0;
      PD_v[4] = ADC_read[4] * 5.0 / 1023.0;

      curr_V = PD_v[0];
      for(int p = 1; p < 5; p++){
        if(PD_v[p] < curr_V){
          curr_V = PD_v[p];
          }
        }
      
      // Print PD Voltages to Serial Monitor
      Serial.print("Current  Input  Voltage = ");
      Serial.println(curr_V, 6);
      Serial.print("Current Minimum Voltage = ");
      Serial.println(min_v, 6);
      Serial.println(" ");

      // Print Current Voltage to Line 1 of I2C Display, Sweep 1
      lcd.setCursor(0, 0);
      lcd.print("Cur V1: ");
      lcd.print(curr_V, 6);

      // Print Minimum Voltage to Line 2 of I2C Display, Sweep 1
      lcd.setCursor(0, 1);
      lcd.print("Min V1: ");
      lcd.print(min_v, 6);
     
          //Checks if current test pin value is lower than the current min value...
          //If new min is found, that value and the associated angles are saved as a reference, Sweep 1 only
          if(curr_V < min_v){
            min_v = curr_V;
            az = i;
            el = j;
            Serial.print("Updated Minimum Voltage = ");
            Serial.println(min_v, 6);
            Serial.print("AZ angle of Min V = ");
            Serial.println(az);
            Serial.print("EL angle of Min V = ");
            Serial.println(el - 15);
            Serial.println(" ");
            }
            
      //delay(1000);
      Serial.print("1st Sweep #:  ");
      Serial.println(sweep_count);
      Serial.println(" ");
      sweep_count = sweep_count + 1;
      }
    }
}//End PD_Sweep function
