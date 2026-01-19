#include <Keypad.h>
#include <Servo.h>

// Hardware Configuration
constexpr uint8_t BUZZER_PIN = A1;
constexpr uint8_t SERVO_PIN = A0;

// Keypad Configuration
constexpr uint8_t KEYPAD_ROWS = 4;
constexpr uint8_t KEYPAD_COLS = 4;
constexpr uint8_t PIN_LENGTH = 4;

// Servo Configuration
constexpr uint8_t SERVO_LOCKED = 0;
constexpr uint8_t SERVO_UNLOCKED = 180;
constexpr uint16_t UNLOCK_DURATION_MS = 1000;
constexpr uint16_t BUZZER_BEEP_MS = 500;

// PIN Configuration
const char CORRECT_PIN[PIN_LENGTH] = {'3', '6', '2', '9'};
char inputPIN[PIN_LENGTH];

// Keypad Mapping Matrix
const char KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Keypad Pin Configuration
const uint8_t ROW_PINS[KEYPAD_ROWS] = {9, 8, 7, 6};
const uint8_t COL_PINS[KEYPAD_COLS] = {5, 4, 3, 2};

// Create Keypad and Servo objects
Keypad keypad = Keypad(makeKeymap(KEYS), ROW_PINS, COL_PINS, KEYPAD_ROWS, KEYPAD_COLS);
Servo doorServo;

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  doorServo.attach(SERVO_PIN);
  doorServo.write(SERVO_LOCKED);
}

void unlockDoor() {
  Serial.println(F("INFO: PIN Correct!!"));
  doorServo.write(SERVO_UNLOCKED);
  delay(UNLOCK_DURATION_MS);
  doorServo.write(SERVO_LOCKED);
}

void soundAlarm() {
  Serial.println(F("ERROR: PIN Incorrect."));
  digitalWrite(BUZZER_PIN, HIGH);
  delay(BUZZER_BEEP_MS);
  digitalWrite(BUZZER_PIN, LOW);
  delay(BUZZER_BEEP_MS);
}

void readPINFromKeypad() {
  Serial.print(F("PIN INPUT: "));
  
  for (uint8_t i = 0; i < PIN_LENGTH; ) {
    const char key = keypad.getKey();
    
    if (key != NO_KEY) {
      inputPIN[i] = key;
      Serial.print(inputPIN[i]);
      i++;
    }
  }
  
  Serial.println();
}

bool verifyPIN() {
  return (strncmp(inputPIN, CORRECT_PIN, PIN_LENGTH) == 0);
}

void loop() {
  doorServo.write(SERVO_LOCKED);
  
  readPINFromKeypad();
  
  if (verifyPIN()) {
    unlockDoor();
  } else {
    soundAlarm();
  }
}
