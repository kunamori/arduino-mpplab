#include <WiFi.h>
#include <WiFiUdp.h>
#include "DHT.h"

#define PIN_DHT 33
#define DHTTYPE DHT22

// DHT sensor object
DHT dht(PIN_DHT, DHTTYPE);

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
WiFiUDP udp;

char packetBuffer[255];

void setup() {
  serial.begin(9600);

  wifi.mode(wifi_ap_sta);
  wifi.softap(ssid, password);
  serial.print("ap ip: ");
  serial.println(wifi.softapip());

  udp.begin(port);
  dht.begin();
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("ERROR: Failed to read from DHT sensor");
    return;
  }

  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String input(packetBuffer);

      if (input == "TEMP") {
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.print(String(t));
        udp.endPacket();
      }
      if (input == "HUMI") {
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.print(String(h));
        udp.endPacket();
      }
    }
  }
}
