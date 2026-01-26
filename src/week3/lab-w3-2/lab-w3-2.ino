// Week 3 — Lab w3-2: 7-segment display with multiplexing
// Purpose: Display a two-digit number using a multiplexed 7-segment display.
// - `display_num[]` stores segment encodings for digits 0–9 using a gfedcba bit order,
//   where a bit value of 0 turns a segment ON (active-low), which is typical for common-anode displays.
//   Example: 0x40 (0b01000000) for '0' has the g bit set (1 = OFF) and bits 0..5 cleared (0 = ON)
//   to light segments a..f and render the digit '0'.
// - Bits 0..5 (a..f) are written directly to PORTB (pins D8..D13) for fast updates.
// - Bit 6 (segment g) is handled separately on `S_G` (D6) via digitalWrite.
// Multiplexing approach:
// - The two digits are selected using `S_D1` and `S_D2`. To show a number, the code writes
//   the segment pattern for the tens digit, enables its digit select, waits briefly, then
//   writes the pattern for the ones digit and enables that select. Rapid repetition gives the
//   illusion both digits are lit simultaneously (persistence of vision).
// Pin mapping (summary):
// - Digit selects: S_D1 = 5, S_D2 = 4
// - Segment G pin: S_G  = 6
// - Segment bits a..f -> PORTB (D8..D13)
//
// Define Push Button
#define PUSH_SW_1 2
#define PUSH_SW_2 3

// Define 7-Segment Pin
#define S_D1 5
#define S_D2 4
#define S_G 6

// 7 Segment Display (active-low encoding for a common-anode display)
int display_num[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
int x = 0;

// Counter & Deboucing
int counter = 0;
int o_counter = 0;
unsigned long t = 0;

void setup() {
  Serial.begin(9600);
  // Configure PORTB (D8..D13) as outputs so all segment lines (a..f) can be written with a single assignment.
  // Using `PORTB = pattern` is significantly faster than individual digitalWrite() calls, which helps
  // keep the multiplexing refresh smooth and flicker-free.
  DDRB = 0xFF; // Set D8..D13 (PORTB) as outputs

  // Segment G is driven on pin D6; digit select lines are on D5 and D4.
  pinMode(S_G, OUTPUT);
  pinMode(S_D1, OUTPUT);
  pinMode(S_D2, OUTPUT);

  // Configure push buttons (active-low wiring using internal pull-ups).
  pinMode(PUSH_SW_1, INPUT_PULLUP);
  pinMode(PUSH_SW_2, INPUT_PULLUP);

  // Use hardware interrupts for responsive counter increments/decrements on button presses.
  attachInterrupt(digitalPinToInterrupt(PUSH_SW_1), count_func_i, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_SW_2), count_func_d, FALLING);
}

void count_func_i(){
  if(millis()-t>200){
    t=millis();
    if(counter == 99){
      counter = 0;
    }
    else{
      counter++;
    }
  }
}

void count_func_d(){
  if(millis()-t>200){
    t=millis();
    if(counter == 0){
      counter = 99;
    }
    else{
      counter--;
    }
  }
}

void loop() {
  if(counter != o_counter){
    Serial.println(counter);
    o_counter=counter;
    x = counter;
  }
  // --- Multiplexing: update tens digit then ones digit rapidly ---
  // Tens digit:
  // - Write the a..f segment bits (bits 0..5 of the pattern) directly to PORTB for speed.
  // - Extract the g segment (bit 6) by shifting right 6 and masking with 0x01, then output to S_G.
  // - Enable the tens digit (S_D1 HIGH, S_D2 LOW) for a short time so it is visible.
  PORTB = display_num[x/10];
  digitalWrite(S_G, 0x01 & (display_num[x/10] >> 6));
  digitalWrite(S_D1, HIGH);
  digitalWrite(S_D2, LOW);
  delay(7); // short dwell to maintain brightness and refresh rate

  // Ones digit:
  // - Repeat the same steps for the ones place.
  PORTB = display_num[x%10];
  digitalWrite(S_G, 0x01 & (display_num[x%10] >> 6));
  digitalWrite(S_D1, LOW);
  digitalWrite(S_D2, HIGH);
  delay(7); // combined with the previous delay, this forms the full refresh period
}
