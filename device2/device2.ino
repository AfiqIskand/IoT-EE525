#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
unsigned long period = 1800000;
unsigned long time_now = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(A0);
  double percentage = value * (100 / 1023.0);
  const char text[32] = "";
  dtostrf (percentage,6,2,text);
  Serial.println(value);
  Serial.println(percentage);
  Serial.println(text);
//  Serial.println(sizeof(String(value)));
  
  radio.write(&text, sizeof(String(text)));
  time_now = millis();
  while(millis() < time_now + period){
//    Serial.println(time_now);
//    Serial.println(period);
//    Serial.println(time_now + period);
//    Serial.println(millis());
  }
}
