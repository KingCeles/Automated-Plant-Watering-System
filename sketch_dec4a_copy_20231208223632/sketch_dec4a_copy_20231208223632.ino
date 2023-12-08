#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>

String ssid = "fcco";
String pass = "fcco7120";
const int httpPort = 80;
const char* apiKey = "ZM1ZESL03K8TTELR";

const int soilsensorpin = A0;
const int pirsensor = D4;
const int blue_led = D7;
const int red_led = D8;
const int trigPin = D3;
const int echoPin = D2;
long duration;
long distance;


WiFiClient client;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid);
  Serial.println("");

  //Initialize ThingSpeak
  ThingSpeak.begin(client);

  pinMode(soilsensorpin,INPUT);
  pinMode(D2,OUTPUT);
  pinMode(pirsensor,INPUT);
  pinMode(blue_led,OUTPUT);
  pinMode(red_led,OUTPUT);
  pinMode(trigPin, OUTPUT); // Set Trig pin as an output
  pinMode(echoPin, INPUT); // Set Echo pin as an input
}

 



void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status()!=WL_CONNECTED){
    Serial.print("Attempting to connect");
    while(WiFi.status()!= WL_CONNECTED){
      WiFi.begin(ssid,pass);
      delay(5000);
      
    }
    Serial.println("\nConnected.");

  }
int pirval = digitalRead(pirsensor);
int soilMoisture = analogRead(soilsensorpin);
int Moistpercent = map(soilMoisture,0,1023,100,0);
digitalWrite(trigPin,LOW);
delayMicroseconds(2);

digitalWrite(trigPin,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin,LOW);
duration = pulseIn(echoPin,HIGH);
distance = duration*0.034/2;


//Send soil moisture to Thingspeak and also PIR
ThingSpeak.setField(1, Moistpercent);
ThingSpeak.setField(2,pirval);
ThingSpeak.setField(3,distance);
Serial.print("PIR Sensor: ");
Serial.print(pirval);
Serial.print("Soil Moisture: ");
Serial.println(soilMoisture);
Serial.print("Distance: ");
Serial.println(distance);
ThingSpeak.writeFields(2033371,apiKey);

// Wait for 15 seconds before sending the next reading ]
delay(2000);

// Activate relay under condition that soil moisture
if(soilMoisture > 768){
  digitalWrite(D2,HIGH);
  digitalWrite(blue_led,HIGH);

}

else{
  digitalWrite(D2,LOW);
  digitalWrite(blue_led,LOW);
}

if(pirval == 1)
{
  digitalWrite(red_led,HIGH);
}

else{
  digitalWrite(red_led,LOW);
}
}
