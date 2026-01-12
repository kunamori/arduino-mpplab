#include <WiFi.h> 
#include <ESP32Servo.h>

const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;

WiFiServer server(port);
Servo Servo1;

void setup() {
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
  WiFi.mode(WIFI_AP_STA);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();

  Servo1.attach(26);
}

void loop() {
  // Your code here
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client connected");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('&');
        Serial.print("Received request: ");
        Serial.println(request);
        Servo1.write(request.toInt());
    }
    client.stop();
    Serial.println("Client disconnected");
  }
  }
}
