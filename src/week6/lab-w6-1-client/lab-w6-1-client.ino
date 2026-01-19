#include <WiFi.h>

// Hardware Configuration - Button Pins
constexpr uint8_t BUTTON_UP = 5;
constexpr uint8_t BUTTON_DOWN = 4;

// Counter Configuration
constexpr uint8_t COUNTER_MIN = 0;
constexpr uint8_t COUNTER_MAX = 99;
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
uint8_t counter = 0;

WiFiClient client;

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_CONNECT_DELAY_MS);
    Serial.println(F("Connecting to WiFi..."));
  }
  
  Serial.println(F("Connected to WiFi"));
}

void sendCounterToServer(const uint8_t value) {
  if (client.connect(SERVER_HOST, SERVER_PORT)) {
    client.print(String(value) + "&");
    client.flush();
    client.stop();
  }
}

void handleButton(const uint8_t pin, ButtonState& btnState, const bool increment) {
  btnState.current = digitalRead(pin);
  
  if (btnState.current != btnState.last) {
    delay(DEBOUNCE_DELAY_MS);
    
    if (digitalRead(pin) == LOW) {
      if (increment) {
        counter = (counter >= COUNTER_MAX) ? COUNTER_MIN : counter + 1;
      } else {
        counter = (counter <= COUNTER_MIN) ? COUNTER_MAX : counter - 1;
      }
      
      sendCounterToServer(counter);
    }
  }
  
  btnState.last = btnState.current;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  
  connectToWiFi();
}

void loop() {
  handleButton(BUTTON_UP, button1, true);    // Increment
  handleButton(BUTTON_DOWN, button2, false); // Decrement
  
  Serial.println(counter);
}
