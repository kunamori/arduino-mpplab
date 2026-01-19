#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi Configuration
const char* WIFI_SSID = "ESP32-Nihahaha";
const char* WIFI_PASSWORD = "12345678";
const char* SERVER_HOST = "192.168.4.1";
constexpr uint16_t UDP_PORT = 6969;
constexpr uint16_t WIFI_CONNECT_DELAY_MS = 1000;

// Buffer Configuration
constexpr uint16_t MESSAGE_BUFFER_SIZE = 255;
constexpr uint16_t SEND_DELAY_MS = 1000;

WiFiUDP udp;

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_CONNECT_DELAY_MS);
    Serial.println(F("Connecting to WiFi..."));
  }
  
  Serial.println(F("Connected to WiFi"));
  Serial.print(F("Local IP: "));
  Serial.println(WiFi.localIP());
}

void sendUDPMessage(const String& message) {
  char messageBuffer[MESSAGE_BUFFER_SIZE];
  snprintf(messageBuffer, MESSAGE_BUFFER_SIZE, "%s", message.c_str());
  
  udp.beginPacket(SERVER_HOST, UDP_PORT);
  udp.print(messageBuffer);
  udp.flush();
  udp.endPacket();
}

void processSerialInput() {
  const String input = Serial.readStringUntil('\n');
  sendUDPMessage(input);
  delay(SEND_DELAY_MS);
}

void setup() {
  Serial.begin(9600);
  
  connectToWiFi();
  udp.begin(UDP_PORT);
}

void loop() {
  if (Serial.available() > 0) {
    processSerialInput();
  }
}
