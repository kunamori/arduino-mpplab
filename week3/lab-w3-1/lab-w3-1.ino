// Define Push Button
#define PUSH_SW_1 2
#define PUSH_SW_2 3

int counter = 0;
int o_counter = 0;
unsigned long t = 0;

void setup() {
  Serial.begin(9600);
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
  }
}
