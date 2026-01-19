#include <WiFi.h>
#include <WiFiUdp.h>
#include "DHT.h"

// Hardware Configuration
constexpr uint8_t PIN_DHT = 33;
constexpr uint8_t DHTTYPE = DHT22;

// WiFi Configuration
const char* WIFI_SSID = "ESP32-Nihahaha";
const char* WIFI_PASSWORD = "12345678";
constexpr uint16_t UDP_PORT = 6969;

// Timing Configuration
constexpr uint16_t SENSOR_READ_INTERVAL_MS = 2000;
constexpr uint16_t PACKET_BUFFER_SIZE = 255;

DHT dht(PIN_DHT, DHTTYPE);
WiFiUDP udp;
char packetBuffer[PACKET_BUFFER_SIZE];

unsigned long lastReadTime = 0;
float temperature = 0.0f;
float humidity = 0.0f;

void initializeWiFi() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.print(F("AP IP: "));
  Serial.println(WiFi.softAPIP());
}

void readSensorData() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("ERROR: Failed to read from DHT sensor"));
  }
}

void processUDPRequest() {
  const int packetSize = udp.parsePacket();
  
  if (packetSize > 0) {
    const int len = udp.read(packetBuffer, PACKET_BUFFER_SIZE - 1);
    
    if (len > 0) {
      packetBuffer[len] = '\0';
      const String input(packetBuffer);
      
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      
      if (input == "TEMP") {
        udp.print(String(temperature, 1));
      } else if (input == "HUMI") {
        udp.print(String(humidity, 1));
      }
      
      udp.endPacket();
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  initializeWiFi();
  udp.begin(UDP_PORT);
  dht.begin();
}

void loop() {
  const unsigned long currentTime = millis();
  
  // Non-blocking sensor reading
  if (currentTime - lastReadTime >= SENSOR_READ_INTERVAL_MS) {
    lastReadTime = currentTime;
    readSensorData();
  }
  
  processUDPRequest();
}
