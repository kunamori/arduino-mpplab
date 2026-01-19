#include <WiFi.h> 
#include <ESP32Servo.h>

// Hardware Configuration
constexpr uint8_t SERVO_PIN = 26;

// Servo Configuration
constexpr uint8_t SERVO_MIN_ANGLE = 0;
constexpr uint8_t SERVO_MAX_ANGLE = 180;

// WiFi Configuration
const char* WIFI_SSID = "ESP32-Nihahaha";
const char* WIFI_PASSWORD = "12345678";
constexpr uint16_t SERVER_PORT = 6969;

WiFiServer server(SERVER_PORT);
Servo servo1;

void initializeServo() {
  servo1.attach(SERVO_PIN);
  servo1.write(SERVO_MIN_ANGLE);
}

void initializeWiFi() {
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  WiFi.mode(WIFI_AP_STA);
  
  Serial.print(F("AP IP address: "));
  Serial.println(WiFi.softAPIP());
  
  server.begin(SERVER_PORT);
}

void processServoCommand(WiFiClient& client, const String& command) {
  Serial.print(F("Received request: "));
  Serial.println(command);
  
  const uint8_t angle = constrain(command.toInt(), SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  servo1.write(angle);
  
  client.print(F("E&"));
  client.flush();
}

void handleClient(WiFiClient& client) {
  Serial.println(F("New Client connected"));
  
  while (client.connected()) {
    if (client.available()) {
      const String request = client.readStringUntil('&');
      processServoCommand(client, request);
    }
  }
  
  client.stop();
  Serial.println(F("Client disconnected"));
}

void setup() {
  Serial.begin(9600);
  
  initializeServo();
  initializeWiFi();
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    handleClient(client);
  }
}
