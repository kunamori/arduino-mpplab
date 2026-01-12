#include <WiFi.h>

// Wifi credentials
char *ssid = "KitthapobiPhone";
char *passwd = "muhahaha1234";

// IP Address configuration 
IPAddress local_IP(192, 168, 22, 100);
IPAddress gateway(192, 168, 22, 78);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(9600);
  // WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: "+WiFi.localIP().toString());
  Serial.print("Subnet Mask: "+WiFi.subnetMask().toString());
  Serial.print("Default Gateway: "+WiFi.gatewayIP().toString());
}

void loop() {
}
