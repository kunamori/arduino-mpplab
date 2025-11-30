// Define LED Pinout
// <-- Left    Right -->
//    10 9 8 7 6 5 [4] <- Buzzer
#define LED_1 10
#define LED_2 9
#define LED_3 8
#define LED_4 7
#define LED_5 6
#define LED_6 5

// Define Buzzer Pinout.
#define Buzzer 4

// Define Number of LED.
int NUMBER_LED = 6;

// Array for Registor into Function "PinMode".
int ledPins[] = { LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};

void setup() {
  Serial.begin(9600);
  // for loop for registor pinMode.
  for (int i = 0; i < NUMBER_LED; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  // Register Buzzer Pinout
  pinMode(Buzzer, OUTPUT);
}

void loop() {

  for (int i = 0; i < NUMBER_LED; i++){
    if(i >= i){
      digitalWrite(ledPins[i], HIGH);
    }
    digitalWrite(Buzzer, HIGH);
    delay(500);
    digitalWrite(Buzzer, LOW);
    delay(500);
  }
  for (int i = 0; i < NUMBER_LED; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(Buzzer, HIGH);
  delay(500);
  digitalWrite(Buzzer, LOW);
  delay(500);
}
