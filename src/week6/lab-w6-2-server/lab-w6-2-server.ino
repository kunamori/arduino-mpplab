#include <WiFi.h>

// Hardware Configuration - Relay Pins (Active LOW)
constexpr uint8_t RELAY_1_PIN = 12;
constexpr uint8_t RELAY_2_PIN = 13;

// WiFi Configuration
const char* WIFI_SSID = "ESP32-Nihahaha";
const char* WIFI_PASSWORD = "12345678";
constexpr uint16_t SERVER_PORT = 6969;

WiFiServer server(SERVER_PORT);

void initializeRelays() {
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  
  // Turn off both relays initially (HIGH = OFF for active-low relays)
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
}

void initializeWiFi() {
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  WiFi.mode(WIFI_AP_STA);
  
  Serial.print(F("AP IP address: "));
  Serial.println(WiFi.softAPIP());
  
  server.begin();
}

void processRelayCommand(const String& command) {
  Serial.print(F("Received request: "));
  Serial.println(command);
  
  if (command == "ON1") {
    digitalWrite(RELAY_1_PIN, LOW);
  } else if (command == "OFF1") {
    digitalWrite(RELAY_1_PIN, HIGH);
  } else if (command == "ON2") {
    digitalWrite(RELAY_2_PIN, LOW);
  } else if (command == "OFF2") {
    digitalWrite(RELAY_2_PIN, HIGH);
  }
}

void handleClient(WiFiClient& client) {
  Serial.println(F("New Client connected"));
  
  while (client.connected()) {
    if (client.available()) {
      const String request = client.readStringUntil('&');
      processRelayCommand(request);
    }
  }
  
  client.stop();
  Serial.println(F("Client disconnected"));
}

void setup() {
  Serial.begin(9600);
  
  initializeRelays();
  initializeWiFi();
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    handleClient(client);
  }
}
