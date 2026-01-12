#include <WiFi.h>

// Wifi credentials
char *ssid = "esp32-test";
char *passwd = "123456";

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, passwd);
  Serial.println("Access Point Started");
  Serial.print("AP IP Address: " + WiFi.softAPIP().toString());
}

void loop() {
}
