// Hardware Configuration - Push Buttons
constexpr uint8_t PUSH_SW_1 = 2;
constexpr uint8_t PUSH_SW_2 = 3;

// Counter Configuration
constexpr uint8_t COUNTER_MIN = 0;
constexpr uint8_t COUNTER_MAX = 99;
constexpr uint8_t DEBOUNCE_DELAY_MS = 50;

// State variables
struct ButtonState {
  uint8_t currentState;
  uint8_t lastState;
};

ButtonState button1 = {0, 0};
ButtonState button2 = {0, 0};
uint8_t counter = 0;

void handleButtonPress(const uint8_t pin, ButtonState& btnState, const bool increment) {
  btnState.currentState = digitalRead(pin);
  
  if (btnState.lastState != btnState.currentState) {
    delay(DEBOUNCE_DELAY_MS);
    
    if (digitalRead(pin) == LOW) {
      if (increment) {
        counter = (counter == COUNTER_MAX) ? COUNTER_MIN : counter + 1;
      } else {
        counter = (counter == COUNTER_MIN) ? COUNTER_MAX : counter - 1;
      }
      Serial.println(counter);
    }
  }
  
  btnState.lastState = btnState.currentState;
}

void setup() {
  Serial.begin(9600);
  pinMode(PUSH_SW_1, INPUT_PULLUP);
  pinMode(PUSH_SW_2, INPUT_PULLUP);
}

void loop() {
  handleButtonPress(PUSH_SW_1, button1, true);   // Increment
  handleButtonPress(PUSH_SW_2, button2, false);  // Decrement
}
