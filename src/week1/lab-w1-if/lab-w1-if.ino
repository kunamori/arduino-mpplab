// Hardware Configuration - LED Pins
constexpr uint8_t LED_1 = 10;
constexpr uint8_t LED_2 = 9;
constexpr uint8_t LED_3 = 8;
constexpr uint8_t LED_4 = 7;
constexpr uint8_t LED_5 = 6;
constexpr uint8_t LED_6 = 5;
constexpr uint8_t BUZZER_PIN = 4;

// LED Configuration
constexpr uint8_t NUMBER_LED = 6;
constexpr uint16_t DELAY_MS = 500;

const uint8_t LED_PINS[NUMBER_LED] = {LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};

// State variable for sequencing
uint8_t currentStep = 1;

void initializePins() {
  for (uint8_t i = 0; i < NUMBER_LED; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void beepBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(DELAY_MS);
  digitalWrite(BUZZER_PIN, LOW);
  delay(DELAY_MS);
}

void turnOffAllLEDs() {
  for (uint8_t i = 0; i < NUMBER_LED; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }
}

void setup() {
  Serial.begin(9600);
  initializePins();
}

void loop() {
  // Turn on LEDs sequentially using if statements
  if (currentStep == 1) {
    digitalWrite(LED_1, HIGH);
  }
  if (currentStep == 2) {
    digitalWrite(LED_2, HIGH);
  }
  if (currentStep == 3) {
    digitalWrite(LED_3, HIGH);
  }
  if (currentStep == 4) {
    digitalWrite(LED_4, HIGH);
  }
  if (currentStep == 5) {
    digitalWrite(LED_5, HIGH);
  }
  if (currentStep == 6) {
    digitalWrite(LED_6, HIGH);
  }
  if (currentStep == 7) {
    // Turn off all LEDs and reset
    turnOffAllLEDs();
    currentStep = 0;
  }
  
  beepBuzzer();
  currentStep++;
}
