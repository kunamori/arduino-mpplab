// Hardware Configuration - Potentiometer
constexpr uint8_t POTEN_PIN = A0;

// ADC Configuration
constexpr uint16_t ADC_MAX_VALUE = 1023;
constexpr uint8_t REFERENCE_VOLTAGE = 5;
constexpr uint16_t READ_INTERVAL_MS = 100;

void setup() {
  Serial.begin(9600);
}

float readPotentiometerVoltage() {
  const uint16_t adcValue = analogRead(POTEN_PIN);
  return (static_cast<float>(adcValue) / ADC_MAX_VALUE) * REFERENCE_VOLTAGE;
}

void loop() {
  const float voltage = readPotentiometerVoltage();
  Serial.println(voltage);
  delay(READ_INTERVAL_MS);
}
