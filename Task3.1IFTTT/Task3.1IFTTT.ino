#include <WiFiNINA.h>
#include <SPI.h>
#include "secrets.h" // always include thingspeak header file after other header files and custom macros
#include <BH1750FVI.h>
#include <WiFiSSLClient.h> 

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiSSLClient  client;
String httpsRequest = SECRET_REQUEST;
const char * host = "hooks.zapier.com";
const int LIGHT_THRESHOLD = 400;

BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ; 
  }
  LightSensor.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  uint16_t lux = LightSensor.GetLightIntensity();
  Serial.println(lux);
  sendZapier(lux);
  delay(60000); //60 second delay
}

void sendZapier(uint16_t lux){
  String _lux = String(lux);
  if (client.connect(host, 443)) {
    client.println("POST " + httpsRequest + "?light=" + _lux + " HTTP/1.1");
    client.println("Host: " + String(host)); 
    client.println("Connection: close");
    client.println();
   delay(1000); 
   while (client.available()) { //see answer from server to know if success or error
     char c = client.read(); 
     Serial.write(c); 
   } 
   client.stop();
  }
  else {
    Serial.println("Failed to connect to client"); 
  }
}
