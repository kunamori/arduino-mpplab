#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.1";

IPAddress ip(192,168,4,5);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

String selLED = "0";

void setup() {
  Serial.begin(9600);

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  udp.begin(port);
}

void send_data(String message) {
  udp.beginPacket(host, port);
  udp.print(message);
  udp.endPacket();
}

void loop() {
  int a = analogRead(34);
  a = map(a, 0, 4095, 0, 255);

  String a_str = String(a);
    Serial.print(a);

  int numbuffer = udp.parsePacket();
  if (numbuffer > 0) {
    char packetBuffer[255];
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String s(packetBuffer);

      if (s == "A") {
        send_data(selLED + "_" + a_str + "&");
        Serial.println("Sent: " + selLED + "_" + a_str);
      }
    }
  }
  delay(50);
}
