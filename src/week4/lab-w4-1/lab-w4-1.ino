// Hardware Configuration - LDR (Light Dependent Resistor)
constexpr uint8_t LDR_PIN = A0;

// ADC Configuration
constexpr uint16_t ADC_MIN = 0;
constexpr uint16_t ADC_MAX = 950;
constexpr uint8_t PERCENTAGE_MIN = 0;
constexpr uint8_t PERCENTAGE_MAX = 100;

void setup() {
  Serial.begin(9600);
}

uint8_t readLightPercentage() {
  const uint16_t ldrValue = analogRead(LDR_PIN);
  return map(ldrValue, ADC_MIN, ADC_MAX, PERCENTAGE_MIN, PERCENTAGE_MAX);
}

void loop() {
  const uint8_t lightPercentage = readLightPercentage();
  
  Serial.print(lightPercentage);
  Serial.println(F("%"));
}
