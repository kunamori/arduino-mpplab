// Hardware Configuration - 7-Segment Display Pins
constexpr uint8_t S_D1 = 5;
constexpr uint8_t S_D2 = 4;
constexpr uint8_t S_G = 6;

// 7 Segment Display Patterns (Common Anode)
const uint8_t DISPLAY_NUM[10] PROGMEM = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

// Display Configuration
constexpr uint8_t DISPLAY_MIN = 0;
constexpr uint8_t DISPLAY_MAX = 99;
constexpr uint8_t DISPLAY_REFRESH_MS = 7;

// State variables
uint8_t displayValue = 0;

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

void processSerialInput() {
  const String input = Serial.readStringUntil('\n');
  const uint8_t value = input.toInt();
  
  if (value <= DISPLAY_MAX && value >= DISPLAY_MIN) {
    displayValue = value;
  }
}

void setup() {
  Serial.begin(9600);
  
  // Set pins 8-13 as output
  DDRB = 0xFF;
  pinMode(S_G, OUTPUT);
  pinMode(S_D1, OUTPUT);
  pinMode(S_D2, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    processSerialInput();
  }
  
  // Multiplex 7-segment display
  displayDigit(displayValue / 10, 0);
  delay(DISPLAY_REFRESH_MS);
  
  displayDigit(displayValue % 10, 1);
  delay(DISPLAY_REFRESH_MS);
}
