#include <WiFi.h>
#include <WiFiUdp.h>

// Hardware Configuration
constexpr uint8_t BUTTON_PIN = 4;
constexpr uint8_t POTENTIOMETER_PIN = 34;

// ADC Configuration (ESP32 12-bit ADC)
constexpr uint16_t ADC_MAX_VALUE = 4095;
constexpr uint8_t RGB_MIN_VALUE = 0;
constexpr uint8_t RGB_MAX_VALUE = 255;
constexpr uint8_t RGB_CHANNELS = 3;

// WiFi Configuration
const char* WIFI_SSID = "ESP32-Nihahaha";
const char* WIFI_PASSWORD = "12345678";
const char* SERVER_HOST = "192.168.4.1";
constexpr uint16_t UDP_PORT = 6969;

// Static IP Configuration
const IPAddress STATIC_IP(192, 168, 4, 5);
const IPAddress GATEWAY(192, 168, 4, 1);
const IPAddress SUBNET(255, 255, 255, 0);

// Timing Configuration
constexpr uint16_t HEARTBEAT_INTERVAL_MS = 1000;
constexpr uint8_t DEBOUNCE_DELAY_MS = 20;
constexpr uint16_t LOOP_DELAY_MS = 10;
constexpr uint16_t WIFI_CONNECT_DELAY_MS = 1000;
constexpr uint16_t PACKET_BUFFER_SIZE = 255;

WiFiUDP udp;

// State variables
struct ButtonState {
  bool current;
  bool last;
};

ButtonState button = {true, true};
uint8_t selectedRGBChannel = 0;
unsigned long lastHeartbeatTime = 0;

void connectToWiFi() {
  WiFi.config(STATIC_IP, GATEWAY, SUBNET);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_CONNECT_DELAY_MS);
    Serial.println(F("Connecting to WiFi..."));
  }
  
  Serial.println(F("Connected to WiFi"));
  Serial.println(WiFi.localIP());
}

void sendUDPMessage(const String& message) {
  char messageBuffer[PACKET_BUFFER_SIZE];
  snprintf(messageBuffer, PACKET_BUFFER_SIZE, "%s", message.c_str());
  
  udp.beginPacket(SERVER_HOST, UDP_PORT);
  udp.print(messageBuffer);
  udp.flush();
  udp.endPacket();
}

void handleChannelButton() {
  button.current = digitalRead(BUTTON_PIN);
  
  if (button.current != button.last) {
    delay(DEBOUNCE_DELAY_MS);
    
    if (digitalRead(BUTTON_PIN) == LOW) {
      selectedRGBChannel = (selectedRGBChannel + 1) % RGB_CHANNELS;
      Serial.print(F("Selected RGB Channel: "));
      Serial.println(selectedRGBChannel);
    }
  }
  
  button.last = button.current;
}

uint8_t readPotentiometerValue() {
  const uint16_t adcValue = analogRead(POTENTIOMETER_PIN);
  return map(adcValue, 0, ADC_MAX_VALUE, RGB_MIN_VALUE, RGB_MAX_VALUE);
}

void processUDPResponse() {
  const int numBytes = udp.parsePacket();
  
  if (numBytes > 0) {
    char packetBuffer[PACKET_BUFFER_SIZE];
    const int len = udp.read(packetBuffer, PACKET_BUFFER_SIZE - 1);
    
    if (len > 0) {
      packetBuffer[len] = '\0';
      const String response(packetBuffer);
      
      if (response == "A") {
        const uint8_t brightness = readPotentiometerValue();
        const String message = String(selectedRGBChannel) + "_" + String(brightness) + "&";
        sendUDPMessage(message);
        lastHeartbeatTime = millis();
      }
    }
  }
}

void sendHeartbeat() {
  const unsigned long currentTime = millis();
  
  if (currentTime - lastHeartbeatTime >= HEARTBEAT_INTERVAL_MS) {
    lastHeartbeatTime = currentTime;
    sendUDPMessage("10_20&");
  }
}

void setup() {
  Serial.begin(9600);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  connectToWiFi();
  udp.begin(UDP_PORT);
}

void loop() {
  handleChannelButton();
  sendHeartbeat();
  processUDPResponse();
  
  delay(LOOP_DELAY_MS);
}
