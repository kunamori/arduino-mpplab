// Week 1 — Lab w1-2: Interactive LED control via Serial
// Purpose: toggle individual LEDs by sending numbers 1–6 over Serial
// HW: 6 LEDs (pins 10..5), Buzzer (pin 4)
// Notes: stores LED states in `ledStatus[]`; input parsed with `toInt()`
//
// Define LED Pinout
// <-- Left    Right -->
//    10 9 8 7 6 5 [4] <- Buzzer
#define LED_1 10
#define LED_2 9
#define LED_3 8
#define LED_4 7
#define LED_5 6
#define LED_6 5

// Define Buzzer Pinout.
#define Buzzer 4

// Define Number of LED.
// `NUMBER_LED` defines how many LEDs from `ledPins[]` are active.
int NUMBER_LED = 6;

// Define array for storing LED status.
// - ledStatus[i] == 0 => LED is OFF
// - ledStatus[i] == 1 => LED is ON
// The array has one slot per LED; the first NUMBER_LED entries are used.
int ledStatus[] = {0,0,0,0,0,0,0};

// Array for Registor into Function "PinMode".
int ledPins[] = { LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};

// Define global for selected led variable.
int selLED;

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
  // Check for incoming serial data. The user is expected to send a line
  // containing a number (1..NUMBER_LED) followed by Enter/newline.
  if (Serial.available() > 0) {
      // Read the incoming line (up to the newline).
      // Typical input example: "3\n"
      String input = Serial.readStringUntil('\n');

      // Acknowledge receipt with a short buzzer pulse so the user knows input was received.
      digitalWrite(Buzzer, HIGH);
      delay(100);
      digitalWrite(Buzzer, LOW);
      delay(100);

      // Convert the input string to an integer.
      // Note: `toInt()` returns 0 if conversion fails (non-numeric input).
      selLED = input.toInt();

      // Validate that the parsed integer corresponds to a valid LED index (1..NUMBER_LED).
      if (selLED >= 1 and selLED <= NUMBER_LED){
        // Convert to 0-based array index for `ledPins` / `ledStatus`.
        int idx = selLED - 1;

        // If the LED is currently off (status == 0) turn it on and update `ledStatus`.
        if (ledStatus[idx] == 0){
          digitalWrite(ledPins[idx], HIGH);
          ledStatus[idx] = 1;
          Serial.print("LED: ");
          Serial.print(input);
          Serial.println(" ON");
        } else if (ledStatus[idx] == 1){
          // If it is on, turn it off and update `ledStatus`.
          digitalWrite(ledPins[idx], LOW);
          ledStatus[idx] = 0;
          Serial.print("LED: ");
          Serial.print(input);
          Serial.println(" OFF");
        }
        // The above Serial prints provide immediate feedback about the new state.
      }else{
        // Input was out of range or invalid; notify the user.
        Serial.println("ERROR: INPUT_USER");
      }
  }
}
