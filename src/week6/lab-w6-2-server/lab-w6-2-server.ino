#include <WiFi.h>

const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
WiFiServer server(port);
void setup() {
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
  WiFi.mode(WIFI_AP_STA);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
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
        if (request == "ON1") {
          digitalWrite(12, LOW);
        } else if (request == "OFF1") {
          digitalWrite(12, HIGH);
        } else if (request == "ON2") {
          digitalWrite(13, LOW);
        } else if (request == "OFF2") {
          digitalWrite(13, HIGH);
        }
      }

    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
