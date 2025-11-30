// Define Push Button
#define PUSH_SW_1 2
#define PUSH_SW_2 3

// Define 7-Segment Pin
#define S_D1 5
#define S_D2 4
#define S_G 6

// 7 Segment Display
// Anode Type
int display_num[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
int x = 0;

// Counter & Deboucing
int counter = 0;
int o_counter = 0;
unsigned long t = 0;

void setup() {
  Serial.begin(9600);
  DDRB = 0xFF; // Set 8-13 pinout directly
  pinMode(S_G, OUTPUT);
  pinMode(S_D1, OUTPUT);
  pinMode(S_D2, OUTPUT);
  pinMode(PUSH_SW_1, INPUT_PULLUP);
  pinMode(PUSH_SW_2, INPUT_PULLUP);
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
