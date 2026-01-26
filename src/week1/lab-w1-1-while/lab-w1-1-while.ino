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
  // Local counter for iterating through LED indices (0-based)
  int i = 0;

  // Phase 1: Turn LEDs ON one-by-one using a while-loop
  // - Condition: loop runs while i is less than NUMBER_LED (i goes from 0 to NUMBER_LED-1)
  // - Each iteration turns the LED at ledPins[i] ON and emits a short buzzer beep as feedback.
  while (i < NUMBER_LED) {
    // NOTE: `if(i >= i)` is always true (any number is >= itself) and therefore redundant.
    // It's left here intentionally as a placeholder in case you want to replace it with
    // a meaningful condition later (e.g., a threshold or additional sensor check).
    if (i >= i) {
      // Turn the current LED on
      digitalWrite(ledPins[i], HIGH);
    }

    // Buzzer feedback to indicate the LED was activated:
    // - Turn buzzer ON, wait 500ms, then turn OFF and wait another 500ms.
    digitalWrite(Buzzer, HIGH);
    delay(500);
    digitalWrite(Buzzer, LOW);
    delay(500);

    // Advance to the next LED index
    i++;
  }

  // Phase 2: Reset index to 0 so the next phase starts from the first LED again
  i = 0;

  // Phase 3: Turn all LEDs OFF sequentially using another while-loop
  // - Mirrors Phase 1 but sets each LED to LOW
  while (i < NUMBER_LED) {
    digitalWrite(ledPins[i], LOW);
    i++;
  }

  // Final buzzer beep to signal completion of the on/off cycle
  digitalWrite(Buzzer, HIGH);
  delay(500);
  digitalWrite(Buzzer, LOW);
  delay(500);
}
