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
  Serial.begin(9600); // corrected capitalization: `Serial` is the built-in serial interface

  // Set AP mode and start the soft AP (capitalization corrected below)
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());

  udp.begin(port); // start UDP listener on configured port
  dht.begin();     // initialize DHT sensor
}

void loop() {
  // Respect DHT sensor timing: wait ~2 seconds between reads to avoid requesting data too frequently
  delay(2000);

  // Read humidity (%) and temperature (C and F)
  float h = dht.readHumidity();           // relative humidity in percent
  float t = dht.readTemperature();         // temperature in Celsius
  float f = dht.readTemperature(true);     // temperature in Fahrenheit

  // Validate sensor readings: DHT library returns NaN when a read fails
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("ERROR: Failed to read from DHT sensor");
    // Skip handling network requests when the sensor reading is invalid
    return;
  }

  // Check for incoming UDP packets (non-blocking)
  int packetSize = udp.parsePacket();
  if (packetSize) {
    // Read the incoming packet into `packetBuffer` and null-terminate it
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String input(packetBuffer);

      // Respond to known requests: "TEMP" -> temperature (C), "HUMI" -> humidity (%)
      if (input == "TEMP") {
        // Reply to the sender's IP and port with the most recent temperature
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.print(String(t));
        udp.endPacket();
      } else if (input == "HUMI") {
        // Reply with the most recent humidity reading
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.print(String(h));
        udp.endPacket();
      } else {
        // Unknown request type: log it for debugging
        Serial.print("Received unknown request: ");
        Serial.println(input);
      }
    }
  }
}
