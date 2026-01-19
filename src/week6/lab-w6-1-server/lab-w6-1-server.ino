#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

// LCD Configuration
constexpr uint8_t LCD_ADDRESS = 0x27;
constexpr uint8_t LCD_COLS = 16;
constexpr uint8_t LCD_ROWS = 2;

// WiFi Configuration
const char* WIFI_SSID = "ESP32-Nihahaha";
const char* WIFI_PASSWORD = "12345678";
constexpr uint16_t SERVER_PORT = 6969;

// Timing Configuration
constexpr uint16_t STARTUP_DELAY_MS = 2000;

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);
WiFiServer server(SERVER_PORT);

void initializeLCD() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("Server Started"));
  lcd.setCursor(0, 1);
  lcd.print(F("IP:"));
  lcd.print(WiFi.softAPIP());
  delay(STARTUP_DELAY_MS);
  lcd.clear();
}

void initializeWiFi() {
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  WiFi.mode(WIFI_AP_STA);
  server.begin();
  
  Serial.print(F("AP IP address: "));
  Serial.println(WiFi.softAPIP());
}

void handleClientConnection(WiFiClient& client) {
  Serial.println(F("New Client connected"));
  
  while (client.connected()) {
    if (client.available()) {
      const String request = client.readStringUntil('&');
      
      Serial.print(F("Received request: "));
      Serial.println(request);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(request);
    }
  }
  
  client.stop();
  Serial.println(F("Client disconnected"));
}

void setup() {
  Serial.begin(9600);
  
  initializeWiFi();
  initializeLCD();
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    handleClientConnection(client);
  }
}
