// Hardware Configuration - Push Buttons
constexpr uint8_t PUSH_SW_1 = 2;
constexpr uint8_t PUSH_SW_2 = 3;

// Counter Configuration
constexpr uint8_t COUNTER_MIN = 0;
constexpr uint8_t COUNTER_MAX = 99;
constexpr uint16_t DEBOUNCE_DELAY_MS = 200;

// State variables
volatile uint8_t counter = 0;
uint8_t oldCounter = 0;
volatile unsigned long lastInterruptTime = 0;

void incrementCounter() {
  const unsigned long currentTime = millis();
  
  // Debouncing
  if (currentTime - lastInterruptTime > DEBOUNCE_DELAY_MS) {
    lastInterruptTime = currentTime;
    counter = (counter == COUNTER_MAX) ? COUNTER_MIN : counter + 1;
  }
}

void decrementCounter() {
  const unsigned long currentTime = millis();
  
  // Debouncing
  if (currentTime - lastInterruptTime > DEBOUNCE_DELAY_MS) {
    lastInterruptTime = currentTime;
    counter = (counter == COUNTER_MIN) ? COUNTER_MAX : counter - 1;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PUSH_SW_1, INPUT_PULLUP);
  pinMode(PUSH_SW_2, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(PUSH_SW_1), incrementCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_SW_2), decrementCounter, FALLING);
}

void loop() {
  if (counter != oldCounter) {
    Serial.println(counter);
    oldCounter = counter;
  }
}
