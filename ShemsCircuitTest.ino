#include <PWmon.h>
#include <SoftwareSerial.h>
#include <Wire.h>

// Changes value to 1 to enable debuging using serial monitor
#define DEBUG 1                                       

// Creates an instance for Power calculations
PowerMonitor csun1;                                             

void setup()
{
  if(DEBUG){
    // Setting hardware serial baud rate to 9600
    Serial.begin(9600);                             
  } 
                      
  // Voltage: (input pin, calibration, phase_shift)
  csun1.voltage(2,183.155,4.675);

  // Current: (input pin, calibration
  csun1.current(1,6.866);
  
  delay(1000);
}

void loop() {

  // Calculates all values: (No.of half wavelengths (crossings, time-out) 
  csun1.calcVI(20,2000);
  
  //extract calculations into variables in sketch
  float realPower       = csun1.realPower;      
  float apparentPower   = csun1.apparentPower;
  float powerFactor     = csun1.powerFactor;
  float Irms            = csun1.Irms;   
  float supplyVoltage   = csun1.Vrms;           
          
  // print the values in order (Real Power, Apparent Power, PF, Irms,Vrms)
  csun1.serialprint();
  delay(1000);
}

