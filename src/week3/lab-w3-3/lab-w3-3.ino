// Week 3 — Lab w3-3: Serial-controlled 7-segment display and multiplexing
// Purpose: read a numeric value (0..99) from Serial and display it on a two-digit 7-segment module.
// Input format: send an integer followed by newline (e.g., "42\n").
// Notes:
// - `display_num[]` stores segment bit patterns (active-low encoding typically used with common-anode displays).
// - Bits 0..5 correspond to segments a..f and are written directly to PORTB (fast writes for flicker-free multiplexing).
// - Bit 6 corresponds to the 'g' segment and is output separately on `S_G`.
// - `S_D1` and `S_D2` are the digit-select lines used to multiplex the tens and ones digits in rapid succession.
#define S_D1 5
#define S_D2 4
#define S_G 6

// 7 Segment Display mapping (digits 0..9)
int display_num[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

// `x` holds the displayed value (0..99); `lx` is a temporary parsed value from Serial input
int x = 0;
int lx = 0;

void setup() {
  // Start Serial to accept numeric input from the serial monitor
  Serial.begin(9600);

  // Configure PORTB (pins D8..D13) as outputs so we can write segment bitmasks quickly
  // A single assignment to PORTB is much faster than multiple digitalWrite() calls,
  // which is important for stable multiplexing refresh rates.
  DDRB = 0xFF; // Set 8-13 pinout directly (D8..D13 -> PORTB)

  // Configure segment/digit control pins as outputs
  pinMode(S_G, OUTPUT);  // segment 'g' (bit 6)
  pinMode(S_D1, OUTPUT); // digit select for tens
  pinMode(S_D2, OUTPUT); // digit select for ones

  // Initialize digit selects to a safe state (both off)
  digitalWrite(S_D1, LOW);
  digitalWrite(S_D2, LOW);
}

void loop() {
  // If data available on Serial, read a line and parse it to an integer.
  // Example input: "57\n" -> displays 57
  if (Serial.available() > 0) {
    // Read until newline; `input` contains the ASCII digits sent by the user
    String input = Serial.readStringUntil('\n');

    // Convert to integer once and store in `lx` for validation
    lx = input.toInt();

    // Accept only values within the displayable range (0..99)
    if (lx <= 99 && lx >= 0) {
      x = lx; // update the displayed value only when input is valid
    }
    // If invalid, we silently ignore the input; you could add an error print here if desired.
  }

  // Multiplexing scheme:
  // 1) Write the tens digit segment pattern to PORTB (fast)
  // 2) Output the 'g' segment separately from the bitmask's bit 6
  // 3) Enable the tens digit select and delay briefly to make it visible
  PORTB = display_num[x / 10];
  digitalWrite(S_G, 0x01 & (display_num[x / 10] >> 6)); // extract bit 6 (segment g)
  digitalWrite(S_D1, HIGH); // enable tens digit
  digitalWrite(S_D2, LOW);  // ensure ones digit is disabled
  delay(7); // short dwell time for this digit

  // 4) Repeat for the ones digit
  PORTB = display_num[x % 10];
  digitalWrite(S_G, 0x01 & (display_num[x % 10] >> 6)); // extract bit 6 for ones digit
  digitalWrite(S_D1, LOW);  // disable tens digit
  digitalWrite(S_D2, HIGH); // enable ones digit
  delay(7); // dwell for ones digit
}
