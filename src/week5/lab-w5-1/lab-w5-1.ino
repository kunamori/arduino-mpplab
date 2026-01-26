// Week 5 — Lab w5-1: Keypad-controlled PIN verification with Servo and Buzzer
// Purpose: read a 4-digit PIN from a 4x4 keypad. If the entered PIN matches `setPIN`,
// actuate the servo to 180 degrees (unlock) and provide buzzer and serial feedback.
// HW: 4x4 Keypad (rows: 9,8,7,6; cols: 5,4,3,2), Servo on A0, Buzzer on A1

#include <Keypad.h>
#include <Servo.h>

#define Buzzer A1 // Buzzer pin (active HIGH to beep)

// Setup Servo
#define ServoPin A0
Servo myServo; // servo object attached to ServoPin

// Metrix Table Configuration.
const byte ROWS = 4;
const byte COLS = 4;

// PIN Configuration.
const int maxPIN = 4; // Maximum of PIN Numbers
const char setPIN[maxPIN] = {'3', '6', '2', '9'}; 
char inputPIN[maxPIN];


// Keypad Mapping Matrix
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Keypad Pin Configuration
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

// Create Keypad Function. 
Keypad keypad = Keypad(makeKeymap(keys), rowPins,colPins,ROWS,COLS);

void setup() {
  Serial.begin(9600);

  // Configure buzzer pin as an output for audible feedback on incorrect PIN
  pinMode(Buzzer, OUTPUT);

  // Attach the servo to the configured pin so we can control its position
  myServo.attach(ServoPin);

  // Ensure the servo starts at the closed/locked position (0 degrees)
  myServo.write(0);
}

int i; // index into inputPIN[] (0..maxPIN-1) used while collecting keypad digits

void loop() {
  // `state` reserved for future use (e.g., a state machine); not used in current sketch
  int state = 0;

  // Ensure the servo starts the loop in the closed/locked position
  myServo.write(0);

  // Prompt for PIN input on the Serial Monitor
  Serial.print("PIN INPUT: ");

  // Collect exactly `maxPIN` key presses from the keypad.
  // `keypad.getKey()` returns:
  // - a character (e.g., '1','2',...,'A','B', etc.) when a key is pressed
  // - NO_KEY when no key is pressed
  for (i = 0; i < maxPIN; ) {
    char key = keypad.getKey();
    if (key != NO_KEY) {
      // Store pressed key into the input buffer and echo to Serial for feedback
      inputPIN[i] = key;
      Serial.print(inputPIN[i]);
      i++;
    }
  }

  Serial.println("");

  // Compare entered PIN with the stored PIN:
  // - strncmp returns 0 when the first `maxPIN` characters are identical
  // Using `maxPIN` ensures exactly the configured number of digits are compared.
  if (strncmp(inputPIN, setPIN, maxPIN) == 0) {
    Serial.println("INFO: PIN Correct!!");

    // Success feedback: move servo to 180 degrees to represent 'unlock'
    myServo.write(180);
    delay(1000);

    // Return servo to locked position after a short delay (optional)
    myServo.write(0);
  } else {
    Serial.println("ERROR: PIN Incorrect.");

    // Error feedback: short buzzer beep (on 500ms, off 500ms)
    digitalWrite(Buzzer, HIGH);
    delay(500);
    digitalWrite(Buzzer, LOW);
    delay(500);
  }
}
