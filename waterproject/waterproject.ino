#include <WiFiNINA.h>
#include "secrets.h"
#include <DHT.h>
#include <PubSubClient.h>

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;   
char mqtt[] = MQTT_SERVER; //using mosquitto
int mqtt_port = 1883;
const char* mqtt_user = MQTT_USERNAME;
const char* mqtt_password = MQTT_PASS;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

#define DHTPIN 2
#define DHTTYPE DHT22
#define WATERPIN A0
#define LEDPIN 3
#define LEDPIN2 6
DHT dht(DHTPIN, DHTTYPE);

float hum;
float temp;
int waterLevel;

void setup() {
  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  digitalWrite(LEDPIN, LOW); //make sure led is off
  digitalWrite(LEDPIN2, LOW); //make sure led is off
  connectWifi();

  dht.begin();
  //setting MQTT broker ip and port, and what to do if it recieves data
  client.setServer(mqtt, mqtt_port);
  client.setCallback(callback);

}

void loop() {
  //connecting to MQTT broker
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();
  //getting data using the sensors
  hum = dht.readHumidity();
  temp= dht.readTemperature();
  waterLevel = analogRead(WATERPIN);
  //send data to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.println(hum);
  Serial.print("Water Level: ");
  Serial.println(waterLevel);
  //publish data to respective topics  
  publishSensorData("home/sensors/temperature", temp);
  publishSensorData("home/sensors/humidity", hum);
  publishSensorData("home/sensors/waterLevel", waterLevel);

  delay(5000);
}
//connecting to MQTT broker with user and pw, and subscribing to a topic to read data from
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");

    if (client.connect("Nano33", mqtt_user, mqtt_password)) {
      Serial.println("Connected");
      client.subscribe("home/system/LED"); //recieve data from this topic
      client.subscribe("home/system/FAN"); //recieve data from this topic aswell
    }
    else {
      Serial.println("failed trying again in 5 seconds");
      delay(5000);
    }
  }
}
//connecting to wifi with ssid and password
void connectWifi() {
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(10000);     
    } 
    Serial.println("\nConnected.");
  }
}
//publishing data to the topic
void publishSensorData(const char* topic, float value) {
  char msgBuffer[50];
  sprintf(msgBuffer, "%.2f", value); //convert float value to string before sending to topic
  client.publish(topic, msgBuffer); //string in msg buffer sent to topic
}
//what to do when a message is recieved
void callback (char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; //make sure it is in a valid string
  String message = String((char*)payload); //convert payload to string
  //allows for button on node-red to turn on led for time to simulate pump being on and then off
  if (String(topic) == "home/system/LED") { //make sure message is from this topic
    if (message == "ON") { //simulate water pump, press "on" in node-red and pumps water for 2 seconds
      digitalWrite(LEDPIN, HIGH);
      delay(2000);
      digitalWrite(LEDPIN, LOW);
    } 
  }
  if (String(topic) == "home/system/FAN") { //checks if message is coming from FAN
    if (message == "ON") {
      digitalWrite(LEDPIN2, HIGH); //turn led on to simulate turning a fan on when temps are too high
    }
    else if (message == "OFF") {
      digitalWrite(LEDPIN2, LOW); //turn off when its temp goes back to normal
    }
  }
}
