#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int show=0;

const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
WiFiServer server(port);
void setup() {
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
  WiFi.mode(WIFI_AP_STA);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Server Started");
  lcd.setCursor(0, 1);
  lcd.print("IP:");
  lcd.print(WiFi.softAPIP());
  delay(2000);
  lcd.clear();
}

void loop() {
  // Your code here
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client connected");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('&');
        Serial.print("Received request: ");
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print(request);
        Serial.println(request);
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
