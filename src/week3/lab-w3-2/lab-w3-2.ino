// Hardware Configuration - Push Buttons
constexpr uint8_t PUSH_SW_1 = 2;
constexpr uint8_t PUSH_SW_2 = 3;

// 7-Segment Display Pins
constexpr uint8_t S_D1 = 5;
constexpr uint8_t S_D2 = 4;
constexpr uint8_t S_G = 6;

// 7 Segment Display Patterns (Common Anode)
const uint8_t DISPLAY_NUM[10] PROGMEM = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

// Counter Configuration
constexpr uint8_t COUNTER_MIN = 0;
constexpr uint8_t COUNTER_MAX = 99;
constexpr uint16_t DEBOUNCE_DELAY_MS = 200;
constexpr uint8_t DISPLAY_REFRESH_MS = 7;

// State variables
volatile uint8_t counter = 0;
uint8_t displayValue = 0;
uint8_t oldCounter = 0;
volatile unsigned long lastInterruptTime = 0;

void incrementCounter() {
  const unsigned long currentTime = millis();
  
  // Debouncing
  if(currentTime - lastInterruptTime > DEBOUNCE_DELAY_MS){
    lastInterruptTime = currentTime;
    
    if(counter == COUNTER_MAX){
      counter = COUNTER_MIN;
    } else {
      counter++;
    }
  }
}

void decrementCounter() {
  const unsigned long currentTime = millis();
  
  // Debouncing
  if(currentTime - lastInterruptTime > DEBOUNCE_DELAY_MS){
    lastInterruptTime = currentTime;
    
    if(counter == COUNTER_MIN){
      counter = COUNTER_MAX;
    } else {
      counter--;
    }
  }
}

void displayDigit(const uint8_t digit, const uint8_t position) {
  const uint8_t pattern = pgm_read_byte(&DISPLAY_NUM[digit]);
  
  PORTB = pattern;
  digitalWrite(S_G, (pattern >> 6) & 0x01);
  
  if (position == 0) {
    digitalWrite(S_D1, HIGH);
    digitalWrite(S_D2, LOW);
  } else {
    digitalWrite(S_D1, LOW);
    digitalWrite(S_D2, HIGH);
  }
}

void setup() {
  Serial.begin(9600);
  
  // Set pins 8-13 as output
  DDRB = 0xFF;
  pinMode(S_G, OUTPUT);
  pinMode(S_D1, OUTPUT);
  pinMode(S_D2, OUTPUT);
  
  // Configure input buttons with pull-up
  pinMode(PUSH_SW_1, INPUT_PULLUP);
  pinMode(PUSH_SW_2, INPUT_PULLUP);
  
  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(PUSH_SW_1), incrementCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_SW_2), decrementCounter, FALLING);
}

void loop() {
  // Update display value when counter changes
  if(counter != oldCounter){
    Serial.println(counter);
    oldCounter = counter;
    displayValue = counter;
  }
  
  // Multiplex 7-segment display
  displayDigit(displayValue / 10, 0);
  delay(DISPLAY_REFRESH_MS);
  
  displayDigit(displayValue % 10, 1);
  delay(DISPLAY_REFRESH_MS);
}
