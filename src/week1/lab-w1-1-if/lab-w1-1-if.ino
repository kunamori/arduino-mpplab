// Define LED Pinout
#define LED_1 10
#define LED_2 9
#define LED_3 8
#define LED_4 7
#define LED_5 6
#define LED_6 5

// define Buzzer Pinout.
#define Buzzer 4

// define Number of LED.
int NUMBER_LED = 6;

// array for Registor into Function "PinMode".
int ledPins[] = { LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};

// define global variable
int i = 1;

void setup() {
  // start serial daemon
  Serial.begin(9600);

  // for loop for registor pinMode.
  for (int i = 0; i < NUMBER_LED; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Register Buzzer Pinout
  pinMode(Buzzer, OUTPUT);

}

void loop() {
  if (i == 1){
    digitalWrite(LED_1,HIGH);
  }
  if (i == 2){
    digitalWrite(LED_2,HIGH);
  }
  if (i == 3){
    digitalWrite(LED_3,HIGH);
  }
  if (i == 4){
    digitalWrite(LED_4,HIGH);
  }
  if (i == 5){
    digitalWrite(LED_5,HIGH);
  }
  if (i == 6){
    digitalWrite(LED_6,HIGH);
  }
  if (i == 7){
    // turn off all LEDs
    digitalWrite(LED_1,LOW);
    digitalWrite(LED_2,LOW);
    digitalWrite(LED_3,LOW);
    digitalWrite(LED_4,LOW);
    digitalWrite(LED_5,LOW);
    digitalWrite(LED_6,LOW);
    i = 0; // reset to 0
  }

  // buzzer working after do action
  digitalWrite(Buzzer, HIGH);
  delay(500);
  digitalWrite(Buzzer, LOW);
  delay(500);

  i++;
}
