#include <SoftwareSerial.h>
#include "secrets.h"
#include "ThingSpeak.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

String convertToString(char* a);
//void sendCommand(String command, int maxTime, char readReplay[], bool printOut);

SoftwareSerial ESPserial(2, 3); // RX | TX
RF24 radio(7, 8); // CE, CSN

String SSID_=convertToString(SECRET_SSID);  
String PASS_=convertToString(SECRET_PASS); 
 
String HOST = "api.thingspeak.com";
String PORT = "80";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 

const byte address[6] = "00001";
float Data_ = 999.99;
bool flag_data = false;

int period = 1000;
unsigned long time_now = 0;

void setup()
{

Serial.begin(9600); // communication with the host computer
while (!Serial) { ; }

  // Start the software serial for communication with the ESP8266
  ESPserial.begin(9600);
  
  Serial.println("Mini Project EE524");
  Serial.println("Title : Water Level Monitoring for in-door plant ");
  sendCommand("AT",5,"OK");
  
//  sendCommand("AT+UART_DEF=9600,8,1,0,0",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+SSID_+"\",\""+PASS_+"\"",15,"OK");
  //ESPserial.println("AT+CIFSR");
  Serial.println("");
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

}

void loop()

{
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println("recieve data");
    Data_= atof(text);
    Serial.println(Data_);
    if (Data_ != 999.99)
      flag_data = true;
    else
      flag_data = false;

  }
  if (flag_data == true){
    String sendData_ = "GET /update?api_key=XPGCIJESJ2B6LETV&field1="+String(Data_);
//    sendCommand("AT",5,"OK");
//    sendCommand("AT+RST",10,"OK");
    sendCommand("AT+CIPMUX=1",5,"OK");
    sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
    sendCommand("AT+CIPSEND=0," +String(sendData_.length()+4),4,">");
    Serial.println(sendData_);
    ESPserial.println(sendData_);delay(1500);countTrueCommand++;
    sendCommand("AT+CIPCLOSE=0",5,"OK");
    Serial.println("");}
    flag_data = false;
    Data_ = 999.99;
  time_now = millis();
  while(millis() < time_now + period){
  }

//// listen for communication from the ESP8266 and then write it to the serial monitor
//
//if ( ESPserial.available() ) { Serial.write( ESPserial.read() ); }
//
//// listen for user input and send it to the ESP8266
//
//if ( Serial.available() ) { ESPserial.write( Serial.read() ); }
}

String convertToString(char* a)
{
    String s(a);
    return s;
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    ESPserial.println(command);//at+cipsend
    if(ESPserial.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
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
