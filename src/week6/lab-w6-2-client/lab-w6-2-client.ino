#include <WiFi.h>

// Hardware Configuration - Button Pins
constexpr uint8_t BUTTON_1 = 5;
constexpr uint8_t BUTTON_2 = 4;

// Debounce Configuration
constexpr uint8_t DEBOUNCE_DELAY_MS = 20;

// WiFi Configuration
const char* WIFI_SSID = "ESP32-Nihahaha";
const char* WIFI_PASSWORD = "12345678";
const char* SERVER_HOST = "192.168.4.1";
constexpr uint16_t SERVER_PORT = 6969;
constexpr uint16_t WIFI_CONNECT_DELAY_MS = 1000;

// State variables
struct ButtonState {
  bool current;
  bool last;
};

ButtonState button1 = {true, true};
ButtonState button2 = {true, true};
bool relayState[2] = {false, false};

WiFiClient client;

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_CONNECT_DELAY_MS);
    Serial.println(F("Connecting to WiFi..."));
  }
  
  Serial.println(F("Connected to WiFi"));
}

void sendRelayCommand(const char* command) {
  if (client.connect(SERVER_HOST, SERVER_PORT)) {
    client.print(command);
    client.flush();
    client.stop();
  }
}

void handleButton(const uint8_t pin, ButtonState& btnState, const uint8_t relayIndex) {
  btnState.current = digitalRead(pin);
  
  if (btnState.current != btnState.last) {
    delay(DEBOUNCE_DELAY_MS);
    
    if (digitalRead(pin) == LOW) {
      if (relayState[relayIndex]) {
        sendRelayCommand(relayIndex == 0 ? "OFF1&" : "OFF2&");
        relayState[relayIndex] = false;
      } else {
        sendRelayCommand(relayIndex == 0 ? "ON1&" : "ON2&");
        relayState[relayIndex] = true;
      }
    }
  }
  
  btnState.last = btnState.current;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  
  connectToWiFi();
}

void loop() {
  handleButton(BUTTON_1, button1, 0);  // Control Relay 1
  handleButton(BUTTON_2, button2, 1);  // Control Relay 2
}
