// Week 2 — Lab w2-3: Serial-controlled RGB LED using PWM
// Purpose: receive a string from Serial containing three values (R & G & B) and apply them as PWM
// to the RGB LED channels. Expected format: "R&G&BN" where R, G, B are integers 0..255 and 'N' is a terminator.
// Example input: "128&255&64N" => R=128, G=255, B=64
// Pin mapping (typical Arduino PWM pins): BLUE -> 9, GREEN -> 10, RED -> 11
#define LED_BLUE 9
#define LED_GREEN 10
#define LED_RED 11

// Runtime variables:
// - ERR_CODE: error flag (0 == OK, non-zero indicates a validation error)
// - VAL_RGB: parsed R, G, B values (VAL_RGB[0] == R, VAL_RGB[1] == G, VAL_RGB[2] == B)
// - i: generic index used for validation loops
int ERR_CODE = 0;
int VAL_RGB[3] = {0,0,0};
int i = 0;

void setup() {
  // Start Serial for incoming commands and optional debugging output.
  Serial.begin(9600);

  // Configure RGB pins as outputs. These pins support PWM via analogWrite() on standard Arduino boards.
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  // Note: we avoid changing the runtime behavior beyond commenting. If desired,
  // the channels can be initialized to 0 (off) with analogWrite(..., 0) here.
}

void loop() {
  // Check if there is any data available on the serial buffer.
  if (Serial.available() > 0) {
    // Read incoming serial data up to the terminator 'N'.
    // Expected format is a single line containing three numeric values separated by '&'
    // and terminated with 'N'. Example: "255&128&64N".
    // Using a terminator simplifies parsing when multiple messages or partial data are possible.
    String s = Serial.readStringUntil('N');

    // Parse the three numeric fields from the string `s`:
    // - s.substring(0, s.indexOf('&')) -> first value (R)
    // - s.substring(s.indexOf('&') + 1, s.lastIndexOf('&')) -> second value (G)
    // - s.substring(s.lastIndexOf('&') + 1) -> trailing value (B)
    // Each substring is converted to an integer with toInt().
    VAL_RGB[0] = s.substring(0, s.indexOf('&')).toInt();
    VAL_RGB[1] = s.substring(s.indexOf('&') + 1, s.lastIndexOf('&')).toInt();
    VAL_RGB[2] = s.substring(s.lastIndexOf('&') + 1).toInt();

    // Reset error flag before performing validation checks.
    ERR_CODE = 0;

    // Validate each RGB component to ensure it fits in the PWM range (0..255).
    // If any component is out of range, report an error and set ERR_CODE.
    for (i = 0; i < 3; i++) {
      if (VAL_RGB[i] > 255 || VAL_RGB[i] < 0) {
        Serial.println("ERROR: Input more than 255 or negative number.");
        ERR_CODE = 1;
      }
    }

    // On successful validation, apply the values using PWM.
    // analogWrite(pin, value) expects a value in the range 0..255 (0 == off, 255 == full brightness).
    if (ERR_CODE == 0) {
      analogWrite(LED_RED, VAL_RGB[0]);
      analogWrite(LED_GREEN, VAL_RGB[1]);
      analogWrite(LED_BLUE, VAL_RGB[2]);
    }
  }
}
