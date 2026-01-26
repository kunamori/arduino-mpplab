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
// `i` is the current selection index (1–6 correspond to LEDs 1..6).
// When `i` reaches 7 we treat it as a signal to reset/turn all LEDs off and wrap back to 0.
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
  // LED selection logic:
  // - `i` acts as a simple state machine selecting which LED to turn ON.
  // - Values 1..6 correspond to LED_1..LED_6 respectively.
  // - When `i` == 7 we interpret it as the 'reset' step: turn all LEDs OFF and wrap back to 0.
  // The code below uses a series of `if` checks to light the selected LED.
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
    // Turn off all LEDs as part of the reset/wrap step
    digitalWrite(LED_1,LOW);
    digitalWrite(LED_2,LOW);
    digitalWrite(LED_3,LOW);
    digitalWrite(LED_4,LOW);
    digitalWrite(LED_5,LOW);
    digitalWrite(LED_6,LOW);
    i = 0; // reset to 0 so next iteration becomes i==1 after increment below
  }

  // Buzzer notification:
  // - The buzzer is toggled ON for 500ms and OFF for 500ms to indicate an action has occurred.
  // - This happens after the LED selection above, so it serves as feedback for each step.
  digitalWrite(Buzzer, HIGH);
  delay(500);
  digitalWrite(Buzzer, LOW);
  delay(500);

  // Move to the next selection. Because we reset `i` to 0 in the `i==7` block,
  // incrementing here causes the cycle to continue with 1..6 then reset again.
  i++;
}
