// Define Potentiometer
#define POTEN_1 A0

float POTEN_VALUE;

void setup() {
  Serial.begin(9600);
}

void loop() {
  POTEN_VALUE = analogRead(POTEN_1);
  POTEN_VALUE = (POTEN_VALUE/1023) * 5;
  Serial.println(POTEN_VALUE);
  delay(100);
}
