#include <WiFi.h>

// Hardware Configuration - Potentiometer
constexpr uint8_t POTENTIOMETER_PIN = 34;

// ADC Configuration (ESP32 12-bit ADC)
constexpr uint16_t ADC_MAX_VALUE = 4095;
constexpr uint8_t SERVO_MIN_ANGLE = 0;
constexpr uint8_t SERVO_MAX_ANGLE = 180;
constexpr uint16_t UPDATE_INTERVAL_MS = 50;

// WiFi Configuration
const char* WIFI_SSID = "ESP32-Nihahaha";
const char* WIFI_PASSWORD = "12345678";
const char* SERVER_HOST = "192.168.4.1";
constexpr uint16_t SERVER_PORT = 6969;
constexpr uint16_t WIFI_CONNECT_DELAY_MS = 500;

WiFiClient client;

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_CONNECT_DELAY_MS);
    Serial.print(F("."));
  }
  
  Serial.println(F("\nWiFi connected"));
}

uint8_t readServoAngle() {
  const uint16_t adcValue = analogRead(POTENTIOMETER_PIN);
  const uint16_t mappedValue = map(adcValue, 0, ADC_MAX_VALUE, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  return constrain(mappedValue, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
}

void sendAngleToServer(const uint8_t angle) {
  if (client.connect(SERVER_HOST, SERVER_PORT)) {
    client.print(String(angle) + "&");
    client.flush();
    
    // Wait for server response
    while (client.available() == 0);
    
    const String response = client.readStringUntil('&');
    Serial.print(F("Response from server: "));
    Serial.println(response);
    
    client.stop();
  }
}

void setup() {
  Serial.begin(9600);
  connectToWiFi();
}

void loop() {
  const uint8_t angle = readServoAngle();
  
  sendAngleToServer(angle);
  
  Serial.println(angle);
  delay(UPDATE_INTERVAL_MS);
}
