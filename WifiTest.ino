#include <SoftwareSerial.h>
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

String Device = "Device2" ;
int power = 500;
double powerfactor = 0.90;
SoftwareSerial esp8266(RX,TX); 
 
  
void setup() {
  Serial.begin(9600);
  esp8266.begin(9600);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ WIFINAME +"\",\""+ PASS +"\"",10,"OK");
}
void loop() {
  power = power + 2;
  String getData = "GET /write_data.php?device="+Device+"&&power="+String(power)+"&&powerfactor="+String(powerfactor)+" HTTP/1.1\r\nHost: 54.164.68.96\r\n\r\n";
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()),4,">");
 esp8266.println(getData);
 delay(1500);
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 //delay(5000);
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

