#include <Servo.h>//Servo Control
#include <Wire.h> // For i2c (receive values from RPi
#include <SoftwareSerial.h>

// Global Vars
Servo servo_az; // create servo object for azimuth
Servo servo_el; // create servo object for elevation
unsigned int ADC_read[16]; // int for test pin ADC
float PD_v[16]; // float of ADC conversion
float curr_V; //float for min reading of current ADC conversion
int az = 0; // Azimuth Angle
int el = 0; // Elevation Angle (this value - 15 = actual angle)
float min_v = 3.0; // Initialize min test pin reading at value above upper bound
float rcvd_flt[16]; //Receives magnitude samples from RPi
float avg_Pwr;
float SNR_MAX = -10.0; // Initialize max pwr reading at value below lower bound
float read_sum; //Used to average pwr detector readings
float sample_Sum; //Used to average Pi mag readings

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  
  Serial.begin(9600); //For printing to Terminal
  while (!Serial) continue; // wait for serial port to connect
  Serial.println("Arduino is ready.");  // Print a message to indicate that the Arduino is ready
  
  servo_az.attach(9);   //Azimuth control pin = 9
  servo_el.attach(10);  //Elevation control pin = 10
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
  delay(1000);
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
  Serial.println("Ready!");
  while(1){
    //Checks if data is received prior to sweep
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

        // Start of Last Sweep
        for(int i = az3; i <= (az3 + 10); i += 1){
          for(int j = el3; j <= (el3 + 10); j += 1){
            delay(250);
            servo_az.write(map(i, 0, 180, 0, 180)); // 3rd azimuth servo position
            delay(250);
            servo_el.write(map(j, 0, 180, 0, 180)); // 3rd elevation servo position
            delay(500);
            //samples and updates rcvd_flt during sweep
sample:      
             // Setup to receieve Floats of SNR data from the Rpi_4 over UART, IEEE-754
              if(Serial.available() > 0){
                for(int s = 0; s < 16; s++){
                  String data = Serial.readStringUntil('\n');
                  rcvd_flt[s] = data.toFloat();
                  if(rcvd_flt[s] > 100.0){
                    rcvd_flt[s] = rcvd_flt[s] / 10.0;
                    }
                    delay(1000);
                }
                sample_Sum = 0;
                for(int s = 0; s < 16; s++){
                  sample_Sum += rcvd_flt[s];
                  }
                avg_Pwr = sample_Sum / 16.0;
                  Serial.print("Initial SNR: ");
                  Serial.println(avg_Pwr, 6);
                  Serial.println(" ");
                  delay(1000);
              }
              else {
                  // I/O error - no data, not enough bytes, etc.
                  avg_Pwr = 0.0;
                  Serial.print("No Data Rcvd!  ");
                  Serial.println(avg_Pwr, 6);
                  delay(1000);
                  goto sample;
              }
              
              Serial.print("Current Magnitude = ");
              Serial.println(avg_Pwr, 6);
        
              Serial.print("Maximum Magnitude = ");
              Serial.println(SNR_MAX, 6);
              Serial.println(" ");
              
                  //Checks if current test pin value is lower than the current min value
                  //If new min is found, that value and the angles it was found are recorded
                  if(avg_Pwr > SNR_MAX){
                    delay(1000);
                    SNR_MAX = avg_Pwr;
                    az = i;
                    el = j;
                    Serial.print("Updated Maximum Magnitude = ");
                    Serial.println(SNR_MAX, 6);
                    Serial.print("AZ angle of Max Magnitude = ");
                    Serial.println(az);
                    Serial.print("EL angle of Max Magnitude = ");
                    Serial.println(el - 15);
                    Serial.println(" ");
                    }
                    
            delay(100);
            Serial.print("3rd Sweep #:  ");
            Serial.println(sweep_count);
            Serial.println(" ");
            sweep_count = sweep_count + 1;
           }
         } //End of 3rd & Final Alignment Sweep

       // Begin Infinite Loop to Hold Final Gimbal Position
       delay(1000);
             
       // Hold Final Angle
       while(1) {
         delay(250);
         servo_az.write(map(az, 0, 180, 0, 180)); // final azimuth servo position
         delay(250);
         servo_el.write(map(el, 0, 180, 0, 180)); // final elevation servo position
         delay(500);
         
         Serial.print("Highest recorded Magnitude = ");
         Serial.println(SNR_MAX, 6);      
         Serial.print("Current AZ angle = ");
         Serial.println(az);   
         Serial.print("Current EL angle = ");
         Serial.println(el - 15);
         Serial.println(" ");
         
         delay(100000);
       }    
  }
} // End of Main

//Sweeping function using ADC readings
void PD_Sweep(int az_Start, int az_End, int el_Start, int el_End, int stepSize, int sweep_count){
  for(int i = az_Start; i <= az_End; i += stepSize){
    for(int j = el_Start; j >= el_End; j-= stepSize){
      
      // Delays to minimize servo jittering
      delay(250);
      servo_az.write(map(i, 0, 180, 0, 180)); // 1st azimuth servo position
      delay(250);
      servo_el.write(map(j, 0, 180, 0, 180)); // 1st elevation servo position
      delay(500);

      //Take average of ADC readings
      for(int p = 0; p < 16; p++){
        ADC_read[p] = analogRead(A0);
        delay(1);
        PD_v[p] = ADC_read[p] * 5.0 / 1023.0;
        delay(1);
        }

      read_sum = 0;
      for(int p = 0; p < 16; p++){
        read_sum += PD_v[p];
        }

      curr_V = read_sum / 16;
            
      // Print PD Voltages to Serial Monitor
      Serial.print("Current  Input  Voltage = ");
      Serial.println(curr_V, 6);
      Serial.print("Current Minimum Voltage = ");
      Serial.println(min_v, 6);
      Serial.println(" ");

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
            
      delay(100);
      Serial.print("Current Sweep #:  ");
      Serial.println(sweep_count);
      Serial.println(" ");
      sweep_count = sweep_count + 1;
      }
    }
}//End PD_Sweep function
