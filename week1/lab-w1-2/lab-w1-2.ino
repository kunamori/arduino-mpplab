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

// Define array for storing LED status.
int ledStatus[] = {0,0,0,0,0,0,0};

// Array for Registor into Function "PinMode".
int ledPins[] = { LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};

// Define global for selected led variable.
int selLED;

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
  if (Serial.available() > 0) {
      // Reading Input
      String input = Serial.readStringUntil('\n');

      //  Send buzzer signal when get input
      digitalWrite(Buzzer, HIGH);
      delay(100);
      digitalWrite(Buzzer, LOW);
      delay(100);

      // String -> int
      selLED = input.toInt();

      // checking status array, turn on/off then update status in array.
      if (selLED >= 1 and selLED <= NUMBER_LED){
        if (ledStatus[(selLED - 1)] == 0){
          digitalWrite(ledPins[(selLED - 1)], HIGH);
          ledStatus[(selLED - 1)] = 1;
          Serial.print("LED: ");
          Serial.print(input);
          Serial.println(" ON");
        } else if (ledStatus[selLED - 1] == 1){
          digitalWrite(ledPins[(selLED - 1)], LOW);
          ledStatus[(selLED - 1)] = 0;
          Serial.print("LED: ");
          Serial.print(input);
          Serial.println(" OFF");
        }
        // Send status on serial monitor
      }else{
        // Print error when input string or number greater than NUMBER_LED
        Serial.println("ERROR: INPUT_USER");
      }
  } 
}
