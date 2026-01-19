// Define RGB LED Pinout
// <-- Left     Right -->
// Blue[9] Green [10] Red[11]
#define LED_BLUE 9
#define LED_GREEN 10
#define LED_RED 11

// Define RGB Variable
int ERR_CODE = 0;
int VAL_RGB[3] = {0,0,0};
int i = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop() {
  if(Serial.available() > 0){
    // Spilt string by use `&`
    String s = Serial.readStringUntil('N');
    VAL_RGB[0] = s.substring(0, s.indexOf('&')).toInt();
    VAL_RGB[1] = s.substring(s.indexOf('&') + 1, s.lastIndexOf('&')).toInt();
    VAL_RGB[2] = s.substring(s.lastIndexOf('&') + 1).toInt();
    // Reset Error State
    ERR_CODE = 0;
    // Detect Input Error
    for(i = 0; i < 3; i++){
      if(VAL_RGB[i] > 255 || VAL_RGB[i] < 0){
        Serial.println("ERROR: Input more than 255 or negative number.");
        ERR_CODE = 1;
      }
    }
    // if there is no error then run program.
    if(ERR_CODE == 0){
      analogWrite(LED_RED,VAL_RGB[0]);
      analogWrite(LED_GREEN,VAL_RGB[1]);
      analogWrite(LED_BLUE,VAL_RGB[2]);
    }

  }
}
