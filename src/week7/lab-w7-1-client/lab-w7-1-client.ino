#include <WiFi.h>
#include <WiFiUdp.h>

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.1";
WiFiUDP udp;

void setup() {
  Serial.begin(9600);

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
  udp.beginPacket(host, port);

  if(Serial.available() > 0 ){
    String selLED = Serial.readStringUntil('\n');
    char char_message[255];
    sprintf(char_message, "%s", selLED.c_str());
    udp.print(char_message);
    udp.flush();
    udp.endPacket();
    delay(1000);
  }
}
