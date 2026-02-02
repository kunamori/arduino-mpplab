#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define PIN_DHT 33
#define DHTTYPE DHT22

DHT dht(PIN_DHT, DHTTYPE);

// Configuration
// Wifi credentials
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
// App Script
String key = "YOUR_APP_SCRIPT_KEY";

unsigned long time_request = 0;
void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("ERROR: Failed to read from DHT sensor");
    return;
  }
  if(millis() - time_request >= 5000){
    String str = request_google(h, t);
    Serial.println(str);
  }
}
String request_google(float h, float t){
  HTTPClient http;
  String url = "https://script.google.com/macros/s/" + key + "/exec?temp=" + String(t) + "&humi=" + String(h);
  http.begin(url.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  String s = "";
  if(httpCode > 0){
    s = http.getString();
  }
  http.end();
  Serial.println(s);
  return s;
  
}
