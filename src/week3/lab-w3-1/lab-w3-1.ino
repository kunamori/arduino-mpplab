// Week 3 — Lab w3-1: Interrupt-driven counting with debounce
// Purpose: Use hardware interrupts to increment/decrement a shared counter when buttons are pressed.
// The buttons are wired using INPUT_PULLUP (active-low). To prevent multiple triggers due
// to mechanical bounce, the ISRs perform a simple software debounce using `millis()` --
// only accepting interrupts at least 200ms after the previous accepted press.
// HW: 2 push-buttons on pins 2 and 3
#define PUSH_SW_1 2
#define PUSH_SW_2 3

int counter = 0;            // current counter value (0..99)
int o_counter = 0;          // last-printed value to avoid repeated Serial prints
unsigned long t = 0;        // timestamp (ms) of the last accepted button event; used for debounce

void setup() {
  // Start Serial for debugging and to show counter updates
  Serial.begin(9600);

  // Configure push buttons with internal pull-ups:
  // With INPUT_PULLUP, the pin reads HIGH when the button is not pressed and LOW when pressed.
  pinMode(PUSH_SW_1, INPUT_PULLUP);
  pinMode(PUSH_SW_2, INPUT_PULLUP);

  // Attach interrupt service routines for the two buttons.
  // - digitalPinToInterrupt maps a digital pin to the MCU interrupt number
  // - FALLING edge is used because the buttons are active-low (press => HIGH -> LOW)
  attachInterrupt(digitalPinToInterrupt(PUSH_SW_1), count_func_i, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_SW_2), count_func_d, FALLING);
}

void count_func_i(){
  // ISR for increment button:
  // - Uses a simple software debounce strategy: accept input only if at least 200ms
  //   have passed since the last accepted press (`millis() - t > 200`).
  // - Keep ISRs minimal: update `counter` and record the current time in `t`.
  if (millis() - t > 200) {
    t = millis();
    if (counter == 99) {
      counter = 0; // wrap-around at the top
    } else {
      counter++;   // regular increment
    }
  }
}

void count_func_d(){
  // ISR for decrement button:
  // - Uses the same 200ms debounce rule to avoid multiple triggers from bounce.
  // - Update `t` to the current time and decrement (with wrap-around).
  if (millis() - t > 200) {
    t = millis();
    if (counter == 0) {
      counter = 99; // wrap-around at the bottom
    } else {
      counter--;    // regular decrement
    }
  }
}

void loop() {
  // Print the counter only when it changes to reduce Serial output volume.
  // The ISRs handle quick, time-sensitive work (updating `counter` and `t`), while the
  // main loop performs slower operations such as Serial I/O.
  if (counter != o_counter) {
    Serial.println(counter);
    o_counter = counter;
  }
  // Note: avoid long work here if you need the system to be responsive to frequent interrupts.
}
