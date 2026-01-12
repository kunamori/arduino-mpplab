#include <WiFi.h>

bool state1=1,lastState1=1;
bool state2=1,lastState2=1;
char state_relay[2] = {0,0};

// Wifi credentials
char *ssid = "ESP32-Nihahaha";
char *passwd = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.1";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}
int oldvalue = 0;
void loop() {
  WiFiClient client;
    int a = analogRead(34);
  a = map(a, 0, 4095, 0, 180);
  delay(50);
  Serial.println(a);
    if (client.connect(host, port)) {
      client.print(String(a)+"&");
      client.flush();
      client.stop();
  }
}
