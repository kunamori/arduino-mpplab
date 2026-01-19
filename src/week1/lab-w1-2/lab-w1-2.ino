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
constexpr uint16_t BUZZER_BEEP_MS = 100;

// LED pin array
const uint8_t LED_PINS[NUMBER_LED] = {LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};

// State arrays
bool ledStatus[NUMBER_LED] = {false};

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
  delay(BUZZER_BEEP_MS);
  digitalWrite(BUZZER_PIN, LOW);
  delay(BUZZER_BEEP_MS);
}

void toggleLED(const uint8_t ledIndex) {
  if (ledStatus[ledIndex]) {
    digitalWrite(LED_PINS[ledIndex], LOW);
    ledStatus[ledIndex] = false;
    Serial.print(F("LED: "));
    Serial.print(ledIndex + 1);
    Serial.println(F(" OFF"));
  } else {
    digitalWrite(LED_PINS[ledIndex], HIGH);
    ledStatus[ledIndex] = true;
    Serial.print(F("LED: "));
    Serial.print(ledIndex + 1);
    Serial.println(F(" ON"));
  }
}

void processSerialInput() {
  const String input = Serial.readStringUntil('\n');
  const uint8_t selectedLED = input.toInt();
  
  beepBuzzer();
  
  if (selectedLED >= 1 && selectedLED <= NUMBER_LED) {
    toggleLED(selectedLED - 1);
  } else {
    Serial.println(F("ERROR: INPUT_USER"));
  }
}

void setup() {
  Serial.begin(9600);
  initializePins();
}

void loop() {
  if (Serial.available() > 0) {
    processSerialInput();
  }
}
