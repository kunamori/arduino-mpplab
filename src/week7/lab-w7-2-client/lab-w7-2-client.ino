#include <WiFi.h>
#include <WiFiUdp.h>

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.1";
WiFiUDP udp;

// IP Configuration
IPAddress ip(192,168,4,5);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
unsigned long timeout = 0;

String SENSOR_DATA;

void setup() {
  Serial.begin(9600);
  WiFi.config(ip, gateway, subnet);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print(WiFi.localIP());
  udp.begin(port);
}

void loop() {

  if (Serial.available() > 0) {
    SENSOR_DATA = Serial.readStringUntil('\n');
    SENSOR_DATA.trim();

    udp.beginPacket(host, port);
    udp.print(SENSOR_DATA);
    udp.endPacket();

  }

  int packetSize = udp.parsePacket();
  if (packetSize) {
    char packetBuffer[255];
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String input(packetBuffer);
      if(SENSOR_DATA == "TEMP"){
        Serial.println("Received (Temperature) : " + input);
      }
      if(SENSOR_DATA == "HUMI"){
        Serial.println("Received (Humidity) : " + input);
      }
    }
  }
}
