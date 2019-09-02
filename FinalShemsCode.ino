#include <PWmon.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <WiFi.h>  

#define RX 10
#define TX 11
String WIFINAME = "Purgatory";  // NEED TO CHANGE
String PASS     = "RocketMan";   // NEED TO CHANGE

String HOST = "54.164.68.96"; // Host Server
String PORT = "80";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 


SoftwareSerial esp8266(RX,TX);
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
  esp8266.begin(9600);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ WIFINAME +"\",\""+ PASS +"\"",10,"OK");
                  
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
  String Device = "Device1" ;
  int power = csun1.realPower;
  double powerfactor = csun1.powerFactor;
  String getData = "GET /write_data.php?device="+Device+"&&power="+String(power)+"&&powerfactor="+String(powerfactor)+" HTTP/1.1\r\nHost: 54.164.68.96\r\n\r\n";
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()),4,">");
 esp8266.println(getData);
 delay(1500);
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("Pass");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }

