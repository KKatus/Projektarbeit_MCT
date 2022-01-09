#include <ESP8266HTTPClient.h>

#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

const char* ssid="ban-31525";
const char* password = "zNLz-vhnF-BEhg-Qr4b";

const char* endpoint = "http://192.168.1.110/api/mJhxGganHivoTvo1cgzouwxFudmCuQuYgqwi7Cf6/lights/6/state";

int Pin0 = 0;
int Pin1 = 1;
int Pin2 = 2;
int val = 0;
int pirState = LOW;

void setup() {
  //pinMode(Pin2, INPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)           // Connect with the wifi
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");
}
void loop()
{ 
  val = analogRead(A0); // read input value
   Serial.println(analogRead(A0));
  if (val >= 17) 
  { // check if the input is HIGH
    if (pirState == LOW) 
    {
      // we have just turned on
      Serial.println("Motion detected!");
      TrunOffLight();
      delay(2000);
      TrunOnLight();
      delay(2000);
      for (int i = 0; i<10; i++)
      {
        ChangeColor(random(0,65535));                  // Generate the random number and change the colour accordingly
        delay(1000);
      }
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
      if (pirState == HIGH)
      {
        // we have just turned of
        Serial.println("Motion ended!");
        // We only want to print on the output change, not state
        pirState = LOW;
       }
  }
}
void TrunOnLight()
{
  sendHTTPMessage("{\"on\":true}");                 // HTTP message to turn the light on
}
void TrunOffLight()
{ 
  sendHTTPMessage("{\"on\":false}");               // HTTP message to turn the light off
}
void ChangeColor (unsigned int colorCode)
{
  String message = "{\"on\":true, \"sat\":254, \"bri\":150,\"hue\":";
  message = message + String(colorCode) + "}";
  sendHTTPMessage(message);
}
void sendHTTPMessage(String message)                // Function to do HTTP PUT request
{
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClient wifiClient;
    HTTPClient http;
    http.begin(wifiClient, endpoint);
    http.addHeader("Content-Type", "text/plain;charset=UTF-8");
    http.PUT(message);
    http.end();
  } 
  else
  {
    Serial.println("Request not sent: Not connected");
  }
}
