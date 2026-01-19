#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi Configuration
const char* WIFI_SSID = "ESP32-Nihahaha";
const char* WIFI_PASSWORD = "12345678";
const char* SERVER_HOST = "192.168.4.1";
constexpr uint16_t UDP_PORT = 6969;

// Static IP Configuration
const IPAddress STATIC_IP(192, 168, 4, 5);
const IPAddress GATEWAY(192, 168, 4, 1);
const IPAddress SUBNET(255, 255, 255, 0);

// Buffer Configuration
constexpr uint16_t PACKET_BUFFER_SIZE = 255;
constexpr uint16_t WIFI_CONNECT_DELAY_MS = 1000;

WiFiUDP udp;
String sensorDataType = "";

void connectToWiFi() {
  WiFi.config(STATIC_IP, GATEWAY, SUBNET);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_CONNECT_DELAY_MS);
    Serial.println(F("Connecting to WiFi..."));
  }
  
  Serial.println(F("Connected to WiFi"));
  Serial.print(F("Local IP: "));
  Serial.println(WiFi.localIP());
}

void sendUDPRequest(const String& data) {
  udp.beginPacket(SERVER_HOST, UDP_PORT);
  udp.print(data);
  udp.endPacket();
}

void processUDPResponse() {
  const int packetSize = udp.parsePacket();
  
  if (packetSize > 0) {
    char packetBuffer[PACKET_BUFFER_SIZE];
    const int len = udp.read(packetBuffer, PACKET_BUFFER_SIZE - 1);
    
    if (len > 0) {
      packetBuffer[len] = '\0';
      const String response(packetBuffer);
      
      if (sensorDataType == "TEMP") {
        Serial.print(F("Received (Temperature): "));
        Serial.println(response);
      } else if (sensorDataType == "HUMI") {
        Serial.print(F("Received (Humidity): "));
        Serial.println(response);
      }
    }
  }
}

void processSerialInput() {
  sensorDataType = Serial.readStringUntil('\n');
  sensorDataType.trim();
  
  sendUDPRequest(sensorDataType);
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
  
  processUDPResponse();
}
