#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.5";

char packetBuffer[255];
int data[2] = {0};

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  udp.begin(port);
}

void loop() {
  int numbuffer = udp.parsePacket();
  if (numbuffer > 0) {
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String s(packetBuffer);
      Serial.println("Received: " + s);

      splitString(s);

      int c = data[0] + data[1];
      Serial.println(c);
    }

    udp.beginPacket(host, port);
    udp.print("A");
    udp.endPacket();
  }
}

void splitString(String str) {
  int index = 0;
  String s = "";

  for (int i = 0; i < str.length(); i++) {
    if (str[i] != '_' && str[i] != '&') {
      s += str[i];
    } else {
      data[index] = s.toInt();
      index++;
      s = "";
    }
  }
  // เก็บค่าตัวสุดท้าย
  data[index] = s.toInt();
}
