#include <WiFi.h>
#include <WiFiUdp.h>

// Hardware Configuration - LED Pins
constexpr uint8_t LED_1 = 13;
constexpr uint8_t LED_2 = 12;
constexpr uint8_t LED_3 = 14;
constexpr uint8_t LED_4 = 27;
constexpr uint8_t LED_5 = 26;
constexpr uint8_t LED_6 = 25;

// LED Configuration
constexpr uint8_t NUMBER_LED = 6;
constexpr uint8_t PACKET_BUFFER_SIZE = 255;

const uint8_t LED_PINS[NUMBER_LED] = {LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};
bool ledStatus[NUMBER_LED] = {false};

// WiFi Configuration
const char* WIFI_SSID = "ESP32-Nihahaha";
const char* WIFI_PASSWORD = "12345678";
constexpr uint16_t UDP_PORT = 6969;

char packetBuffer[PACKET_BUFFER_SIZE];
WiFiUDP udp;

void initializeLEDs() {
  for (uint8_t i = 0; i < NUMBER_LED; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
}

void initializeWiFi() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.print(F("AP IP: "));
  Serial.println(WiFi.softAPIP());
  
  udp.begin(UDP_PORT);
}

void toggleLED(const uint8_t ledNumber) {
  if (ledNumber < 1 || ledNumber > NUMBER_LED) {
    Serial.println(F("ERROR: INPUT_USER"));
    return;
  }
  
  const uint8_t index = ledNumber - 1;
  
  if (ledStatus[index]) {
    digitalWrite(LED_PINS[index], LOW);
    ledStatus[index] = false;
    Serial.print(F("LED "));
    Serial.print(ledNumber);
    Serial.println(F(" OFF"));
  } else {
    digitalWrite(LED_PINS[index], HIGH);
    ledStatus[index] = true;
    Serial.print(F("LED "));
    Serial.print(ledNumber);
    Serial.println(F(" ON"));
  }
}

void processUDPPacket() {
  const int numBytes = udp.parsePacket();
  
  if (numBytes > 0) {
    const int len = udp.read(packetBuffer, PACKET_BUFFER_SIZE - 1);
    
    if (len > 0) {
      packetBuffer[len] = '\0';
      const String input(packetBuffer);
      const uint8_t selectedLED = input.toInt();
      
      toggleLED(selectedLED);
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  initializeLEDs();
  initializeWiFi();
}

void loop() {
  processUDPPacket();
}
