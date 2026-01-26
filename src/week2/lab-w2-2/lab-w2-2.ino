// Define Potentiometer
#define POTEN_1 A0

float POTEN_VALUE;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read the raw ADC value from the potentiometer pin.
  // On Arduinos with a 10-bit ADC, analogRead returns values in the range 0..1023.
  POTEN_VALUE = analogRead(POTEN_1);

  // Convert the raw ADC reading to a voltage (in volts).
  // - Divide by 1023.0 to normalize the ADC value to the range [0.0, 1.0].
  //   Using 1023.0 (floating point) ensures floating-point division and preserves precision.
  // - Multiply by the reference voltage (commonly 5.0V on many Arduino boards)
  //   to map the normalized value to an actual voltage reading.
  POTEN_VALUE = (POTEN_VALUE / 1023.0) * 5.0;

  // Print the resulting measured voltage to the serial monitor (units: volts).
  Serial.println(POTEN_VALUE);

  // Small delay to make serial output readable and limit the sampling rate.
  delay(100);
}
