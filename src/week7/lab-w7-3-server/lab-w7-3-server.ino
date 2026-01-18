#include <WiFi.h>
#include <WiFiUdp.h>

#define LED_RED_PIN 4
#define LED_GREEN_PIN 2
#define LED_BLUE_PIN 15

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.5";
WiFiUDP udp;

char packetBuffer[255];

int data[2] = {0};
int led_data[3] = {0,0,0};

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

      splitString(s);
      led_data[data[0]] = data[1];
      analogWrite(LED_RED_PIN, led_data[0]);
      analogWrite(LED_GREEN_PIN, led_data[1]);
      analogWrite(LED_BLUE_PIN, led_data[2]);
      delay(50);

      Serial.println(data[0]);
      Serial.println(data[1]);
    }

    udp.beginPacket(host, port);
    udp.print("A");
    udp.flush();
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
  data[index] = s.toInt();
}
