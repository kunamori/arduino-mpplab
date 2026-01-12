#include <WiFi.h>
#include <WiFiUdp.h>

#define LED_1 13
#define LED_2 12
#define LED_3 14
#define LED_4 27
#define LED_5 26
#define LED_6 25

int NUMBER_LED = 6;

int ledStatus[] = {0,0,0,0,0,0};
int ledPins[] = { LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};

int selLED;

WiFiUDP udp;
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char packetBuffer[255];

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUMBER_LED; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  udp.begin(port);
}

void loop() {
  int numBytes = udp.parsePacket();
  if (numBytes > 0) {
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';

      String input(packetBuffer);
      selLED = input.toInt();

      if (selLED >= 1 && selLED <= NUMBER_LED) {
        int index = selLED - 1;

        if (ledStatus[index] == 0) {
          digitalWrite(ledPins[index], HIGH);
          ledStatus[index] = 1;
          Serial.println("LED " + input + " ON");
        } else {
          digitalWrite(ledPins[index], LOW);
          ledStatus[index] = 0;
          Serial.println("LED " + input + " OFF");
        }
      } else {
        Serial.println("ERROR: INPUT_USER");
      }
    }
  }
}
