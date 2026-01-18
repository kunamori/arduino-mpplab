#include <WiFi.h>

// Wifi credentials
char *ssid = "ESP32-Nihahaha";
char *passwd = "12345678";
char *host = "192.168.4.1";
const uint16_t port = 6969;
WiFiClient client;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  // read analog value from pin
  int a = analogRead(34);
  a = map(a, 0, 4095, 0, 180);
  a = constrain(a, 0, 180);

  if(client.connect(host, port)){
    client.print(String(a) + "&");
    client.flush();
    while (client.available()==0);
    String response = client.readStringUntil('&');
    Serial.print("Response from server: ");
    Serial.println(response);
    client.stop();
  }

  Serial.println(a);
  delay(50);
}
