#include <WiFi.h>
#include <WiFiUdp.h>

// Hardware Configuration - RGB LED Pins
constexpr uint8_t LED_RED_PIN = 4;
constexpr uint8_t LED_GREEN_PIN = 2;
constexpr uint8_t LED_BLUE_PIN = 15;

// WiFi Configuration
const char* WIFI_SSID = "ESP32-Nihahaha";
const char* WIFI_PASSWORD = "12345678";
const char* CLIENT_HOST = "192.168.4.5";
constexpr uint16_t UDP_PORT = 6969;

// Buffer Configuration
constexpr uint16_t PACKET_BUFFER_SIZE = 255;
constexpr uint8_t RGB_CHANNELS = 3;
constexpr uint8_t PARSE_DATA_SIZE = 2;
constexpr uint16_t UPDATE_DELAY_MS = 50;

WiFiUDP udp;
char packetBuffer[PACKET_BUFFER_SIZE];

uint8_t rgbValues[RGB_CHANNELS] = {0, 0, 0};
int parsedData[PARSE_DATA_SIZE] = {0, 0};

void parseRGBCommand(const String& input) {
  uint8_t index = 0;
  String valueStr = "";
  
  for (uint16_t i = 0; i < input.length(); i++) {
    if (input[i] != '_' && input[i] != '&') {
      valueStr += input[i];
    } else {
      parsedData[index] = valueStr.toInt();
      index++;
      valueStr = "";
      
      if (index >= PARSE_DATA_SIZE) {
        break;
      }
    }
  }
  
  if (!valueStr.isEmpty() && index < PARSE_DATA_SIZE) {
    parsedData[index] = valueStr.toInt();
  }
}

void updateRGBLED() {
  analogWrite(LED_RED_PIN, rgbValues[0]);
  analogWrite(LED_GREEN_PIN, rgbValues[1]);
  analogWrite(LED_BLUE_PIN, rgbValues[2]);
}

void sendAcknowledgment() {
  udp.beginPacket(CLIENT_HOST, UDP_PORT);
  udp.print(F("A"));
  udp.flush();
  udp.endPacket();
}

void processUDPPacket() {
  const int numBytes = udp.parsePacket();
  
  if (numBytes > 0) {
    const int len = udp.read(packetBuffer, PACKET_BUFFER_SIZE - 1);
    
    if (len > 0) {
      packetBuffer[len] = '\0';
      const String input(packetBuffer);
      
      parseRGBCommand(input);
      
      // Update RGB value for the selected channel
      if (parsedData[0] >= 0 && parsedData[0] < RGB_CHANNELS) {
        rgbValues[parsedData[0]] = constrain(parsedData[1], 0, 255);
      }
      
      updateRGBLED();
      
      Serial.print(F("Channel: "));
      Serial.println(parsedData[0]);
      Serial.print(F("Value: "));
      Serial.println(parsedData[1]);
      
      delay(UPDATE_DELAY_MS);
      sendAcknowledgment();
    }
  }
}

void initializeWiFi() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.print(F("AP IP: "));
  Serial.println(WiFi.softAPIP());
}

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  
  initializeWiFi();
  udp.begin(UDP_PORT);
}

void loop() {
  processUDPPacket();
}
