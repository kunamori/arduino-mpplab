// Define Push Button
#define PUSH_SW_1 2
#define PUSH_SW_2 3

// Define Counter, State
// state_sw [0] current state, [1] new state;
int state_sw[4] = {0,0,0,0};
int counter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PUSH_SW_1, INPUT_PULLUP);
  pinMode(PUSH_SW_2, INPUT_PULLUP);
}

void loop() {
  // Implement for Switch 1 / increment value
  state_sw[0] = digitalRead(PUSH_SW_1);
  if(state_sw[1] != state_sw[0]){
    // delay for deboucing signal
    delay(50);
    if(digitalRead(PUSH_SW_1) == 0){
      // when push button at counter = 99 then reset to 0
      if(counter == 99){
        counter = 0;
      }else{
        counter++;
      }
      Serial.println(counter);
    }
  }
  // set new state
  state_sw[1] = state_sw[0];

  // Implement for Switch 2 / decrement value
  state_sw[2] = digitalRead(PUSH_SW_2);
  if(state_sw[3] != state_sw[2]){
    // delay for deboucing signal
    delay(50);
    if(digitalRead(PUSH_SW_2) == 0){
      // when push button at counter = 0 then return to 99
      if(counter == 0){
        counter = 99;
      }
      else{
        counter--;
      }
      Serial.println(counter);
    }
  }
  // set new state
  state_sw[3] = state_sw[2];
}
