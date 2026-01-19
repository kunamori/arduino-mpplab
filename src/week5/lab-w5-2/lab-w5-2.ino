#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

// Hardware Configuration
constexpr uint8_t SS_PIN = 10;
constexpr uint8_t RST_PIN = 9;
constexpr uint8_t BUZZER_PIN = A0;

// LCD Configuration
constexpr uint8_t LCD_ADDRESS = 0x27;
constexpr uint8_t LCD_COLS = 16;
constexpr uint8_t LCD_ROWS = 2;

// Timing Configuration
constexpr uint16_t STARTUP_DELAY_MS = 2000;
constexpr uint16_t BUZZER_BEEP_MS = 500;

MFRC522 rfidReader(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

String rfidUID = "";

String byteToHex(const uint8_t value) {
  if(value < 0x10) {
    return ("0" + String(value, HEX));
  }
  return String(value, HEX);
}

void initializeLCD() {
  lcd.init();
  lcd.backlight();
  lcd.home();
  lcd.print(F("Power On."));
  delay(STARTUP_DELAY_MS);
  lcd.clear();
}

void beepBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(BUZZER_BEEP_MS);
  digitalWrite(BUZZER_PIN, LOW);
  delay(BUZZER_BEEP_MS);
}

void readRFIDCard() {
  rfidUID = "";
  
  for(uint8_t i = 0; i < rfidReader.uid.size; i++){
    rfidUID += byteToHex(rfidReader.uid.uidByte[i]) + ":";
  }
  
  rfidUID.toUpperCase();
  Serial.println(rfidUID);
  
  rfidReader.PICC_HaltA();
  rfidReader.PCD_StopCrypto1();
  
  lcd.setCursor(0, 0);
  lcd.print(rfidUID);
  beepBuzzer();
}

void setup() {
  Serial.begin(9600);
  
  initializeLCD();
  
  SPI.begin();
  rfidReader.PCD_Init();
  
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  if (rfidReader.PICC_IsNewCardPresent()){
    if (rfidReader.PICC_ReadCardSerial()){
      readRFIDCard();
    }
  }
}
