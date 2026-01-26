// Define Push Button
#define PUSH_SW_1 2
#define PUSH_SW_2 3

// Define Counter and state-array used for edge detection/debouncing
// `state_sw` layout:
//   [0] = current reading for PUSH_SW_1
//   [1] = previous reading for PUSH_SW_1
//   [2] = current reading for PUSH_SW_2
//   [3] = previous reading for PUSH_SW_2
int state_sw[4] = {0,0,0,0};
int counter = 0;

void setup() {
  Serial.begin(9600);
  // Configure push-button pins to use the internal pull-up resistors.
  // With `INPUT_PULLUP`, the pin reads HIGH when the button is not pressed,
  // and reads LOW when the button is pressed (button pulls pin to GND).
  pinMode(PUSH_SW_1, INPUT_PULLUP);
  pinMode(PUSH_SW_2, INPUT_PULLUP);
}

void loop() {
  // Implement for Switch 1 / increment value
  // Read current raw state of switch 1; with INPUT_PULLUP, LOW means 'pressed'
  state_sw[0] = digitalRead(PUSH_SW_1);
  // If the current reading differs from the previous reading => a transition occurred
  if (state_sw[1] != state_sw[0]) {
    // Debounce: wait briefly to allow the mechanical contact to settle.
    // During bouncing the reading may fluctuate; this delay helps avoid false triggers.
    delay(50);
    // Confirm the stable state after the debounce delay; check if the button is pressed.
    if (digitalRead(PUSH_SW_1) == 0) { // LOW == pressed when using INPUT_PULLUP
      // Wrap counter to 0 when reaching 99, otherwise increment.
      if (counter == 99) {
        counter = 0;
      } else {
        counter++;
      }
      Serial.println(counter); // print the updated counter for debugging/user feedback
    }
  }
  // Store the current reading as 'previous' for edge detection on the next loop
  state_sw[1] = state_sw[0];

  // Implement for Switch 2 / decrement value
  // Read current raw state of switch 2 (LOW when pressed due to INPUT_PULLUP)
  state_sw[2] = digitalRead(PUSH_SW_2);
  // Edge detection: if current reading differs from previous, handle transition
  if (state_sw[3] != state_sw[2]) {
    // Debounce delay to allow contact bounce to settle
    delay(50);
    // Confirm the stable press; LOW => pressed
    if (digitalRead(PUSH_SW_2) == 0) {
      // If counter is at lower bound (0), wrap to 99; otherwise decrement normally
      if (counter == 0) {
        counter = 99;
      } else {
        counter--;
      }
      Serial.println(counter);
    }
  }
  // Update previous reading for the next cycle
  state_sw[3] = state_sw[2];
}
