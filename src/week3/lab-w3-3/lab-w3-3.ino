// Define 7-Segment Pin
#define S_D1 5
#define S_D2 4
#define S_G 6

// 7 Segment Display
// Anode Type
int display_num[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
int x = 0;
int lx = 0;

void setup() {
  Serial.begin(9600);
  DDRB = 0xFF; // Set 8-13 pinout directly
  pinMode(S_G, OUTPUT);
  pinMode(S_D1, OUTPUT);
  pinMode(S_D2, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    // Reading Input
    String input = Serial.readStringUntil('\n');
    lx = input.toInt();
    if (lx<=99 && lx>=0){
      x = input.toInt();
    }
  }
  PORTB = display_num[x/10];
  digitalWrite(S_G, 0x01 & display_num[x/10]>>6);
  digitalWrite(S_D1, 1);
  digitalWrite(S_D2, 0);
  delay(7);

  PORTB = display_num[x%10];
  digitalWrite(S_G, 0x01 & display_num[x%10]>>6);
  digitalWrite(S_D1, 0);
  digitalWrite(S_D2, 1);
  delay(7);
}
