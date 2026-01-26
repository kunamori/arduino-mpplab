#define LDR A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read raw ADC value from the LDR (photoresistor).
  // On a typical Arduino with a 10-bit ADC this returns values in the range 0..1023.
  // The measured raw value depends on the resistor divider and the ambient light level.
  int LDR_OUT = analogRead(LDR);

  // Map the raw ADC reading to a percentage (0..100).
  // The upper bound (950) is chosen empirically for this circuit — you may need to calibrate it
  // depending on your wiring and expected light range. Note: `map` returns integers; use floating-
  // point math if you need higher precision.
  int LDR_DIS = map(LDR_OUT, 0, 950, 0, 100);
  Serial.print(LDR_DIS);
  Serial.println("%");
}
