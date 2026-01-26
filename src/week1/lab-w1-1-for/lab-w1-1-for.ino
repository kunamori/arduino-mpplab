// Define LED Pinout
#define LED_1 10
#define LED_2 9
#define LED_3 8
#define LED_4 7
#define LED_5 6
#define LED_6 5

// Define Buzzer Pinout.
#define Buzzer 4

// Define Number of LED.
int NUMBER_LED = 6;

// Array for Registor into Function "PinMode".
int ledPins[] = { LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};

void setup() {
  Serial.begin(9600);
  // for loop for registor pinMode.
  for (int i = 0; i < NUMBER_LED; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  // Register Buzzer Pinout
  pinMode(Buzzer, OUTPUT);
}

void loop() {
  // Main sequence:
  // 1) Turn LEDs on one-by-one (left to right as defined in `ledPins[]`).
  // 2) For each LED turned on, issue a short buzzer "beep" (500ms ON, 500ms OFF).
  // 3) After all LEDs are on, turn them all off and issue one final beep to signal completion.
  for (int i = 0; i < NUMBER_LED; i++){
    // NOTE: the condition `if(i >= i)` is always true and therefore redundant.
    // It has been left in place as a placeholder in case you later want to add a meaningful condition.
    if(i >= i){
      // Set the current LED pin to HIGH to turn this LED ON.
      digitalWrite(ledPins[i], HIGH);
    }
    // Buzzer beep to indicate the LED was switched ON:
    // - `digitalWrite(Buzzer, HIGH)` starts the beep.
    // - `delay(500)` keeps the buzzer on for 500ms.
    // - `digitalWrite(Buzzer, LOW)` stops the beep.
    // - `delay(500)` waits 500ms before the next LED is handled.
    digitalWrite(Buzzer, HIGH);
    delay(500);
    digitalWrite(Buzzer, LOW);
    delay(500);
  }
  // After lighting all LEDs, turn them all OFF:
  for (int i = 0; i < NUMBER_LED; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  // Final buzzer beep (same 500ms ON / 500ms OFF pattern) to mark end of the full sequence.
  digitalWrite(Buzzer, HIGH);
  delay(500);
  digitalWrite(Buzzer, LOW);
  delay(500);
}
