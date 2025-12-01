#define LDR A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int LDR_OUT = analogRead(LDR);
  int LDR_DIS = map(LDR_OUT,0,950,0,100);
  Serial.print(LDR_DIS);
  Serial.println("%");
}
