#include <Keypad.h>
#include <Servo.h>

#define Buzzer A1

// Setup Servo
#define ServoPin A0
Servo myServo; 

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
  pinMode(Buzzer, OUTPUT);
  myServo.attach(ServoPin);
}

int i;

void loop() {
  int state = 0;
  myServo.write(0);
  Serial.print("PIN INPUT: ");
  for (i=0; i < maxPIN;){
    char key = keypad.getKey();
    if (key != NO_KEY){
      inputPIN[i] = key;
      Serial.print(inputPIN[i]);
      i++;
    }
  }
  Serial.println("");
  if(strncmp(inputPIN, setPIN, maxPIN) == 0){
    Serial.println("INFO: PIN Correct!!");
    myServo.write(180);
    delay(1000);
  }else{
    Serial.println("ERROR: PIN Incorrect.");
    digitalWrite(Buzzer, HIGH);
    delay(500);
    digitalWrite(Buzzer, LOW);
    delay(500);

  }
}
