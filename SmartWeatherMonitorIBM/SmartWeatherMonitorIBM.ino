//2 libraries
#include <ESP8266WiFi.h>--------
#include <PubSubClient.h>
 
//-------- Customise these values -----------
//connecting to wifi
const char* ssid = "HeLanhl";
const char* password = "345345345";
//including dht library
#include "DHT.h"
#define DHTPIN D2    // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);//enabling dht
 
#define ORG "hrqgde"
#define DEVICE_TYPE "internship"
#define DEVICE_ID "345312"
#define TOKEN "345312345312"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";//ibm topic
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;//to publish client
PubSubClient client(server, 1883,wifiClient);//port of mqtt protocol

void setup() {
 Serial.begin(115200);
 Serial.println();
 dht.begin();//reads sensor values
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
}
 
void loop() {
float h = dht.readHumidity();
float t = dht.readTemperature();

if (isnan(h) || isnan(t))//is not a no. checking
{
Serial.println("Failed to read from DHT sensor!");
delay(1000);
return;
}
PublishData(t,h);
delay(100);
}

void PublishData(float temp, float humid){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += temp;
  payload+="," "\"humidity\":";
  payload += humid;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
