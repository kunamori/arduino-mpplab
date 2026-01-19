// Hardware Configuration - RGB LED Pins
constexpr uint8_t LED_BLUE = 9;
constexpr uint8_t LED_GREEN = 10;
constexpr uint8_t LED_RED = 11;

// RGB value constraints
constexpr uint8_t RGB_MIN_VALUE = 0;
constexpr uint8_t RGB_MAX_VALUE = 255;
constexpr uint8_t RGB_COMPONENTS = 3;

// RGB value storage
uint8_t valRGB[RGB_COMPONENTS] = {0, 0, 0};

void setup() {
  Serial.begin(9600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

bool parseRGBValues(const String& input) {
  // Parse RGB values from format: R&G&B\n
  const int firstSep = input.indexOf('&');
  const int lastSep = input.lastIndexOf('&');
  
  if (firstSep == -1 || lastSep == -1 || firstSep == lastSep) {
    return false;
  }
  
  valRGB[0] = input.substring(0, firstSep).toInt();
  valRGB[1] = input.substring(firstSep + 1, lastSep).toInt();
  valRGB[2] = input.substring(lastSep + 1).toInt();
  
  // Validate RGB values
  for(uint8_t i = 0; i < RGB_COMPONENTS; i++){
    if(valRGB[i] > RGB_MAX_VALUE){
      Serial.println(F("ERROR: Input more than 255."));
      return false;
    } 
  }
  
  return true;
}

void setRGBLED(const uint8_t red, const uint8_t green, const uint8_t blue) {
  analogWrite(LED_RED, red);
  analogWrite(LED_GREEN, green);
  analogWrite(LED_BLUE, blue);
}

void loop() {
  if(Serial.available() > 0){
    const String input = Serial.readStringUntil('N');
    
    if(parseRGBValues(input)){
      setRGBLED(valRGB[0], valRGB[1], valRGB[2]);
    }
  }
}
