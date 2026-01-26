// Week 5 — Lab w5-2: RFID reader (MFRC522) + I2C LCD + buzzer
// Purpose: detect RFID tags/cards, construct a human-readable UID string,
// display it on an I2C LCD, print it over Serial, and provide buzzer feedback.
// Flow: check for a new card -> read UID bytes -> convert to hex pairs with ":" separators ->
// uppercase & print -> halt card & stop crypto -> display on LCD -> beep buzzer
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

// MFRC522 RC522 SPI connections
#define SS_PIN 10 // Slave Select (SDA) pin for MFRC522
#define RST_PIN 9 // Reset pin for MFRC522 module
#define Buzzer A0 // Buzzer pin (used to give audible feedback on successful reads)

MFRC522 mfrc522(SS_PIN, RST_PIN); // MFRC522 object to interface with the RFID reader
String rfidnum = ""; // Holds the assembled UID string (e.g. "AA:BB:CC:DD:")


// LCD Module Config
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize Serial for debug/log output
  Serial.begin(9600);

  // Initialize I2C LCD and show a brief startup message
  lcd.init();
  lcd.backlight();
  lcd.home();
  lcd.print("Power On.");
  delay(2000);
  // Optional: clear a small area (maintaining original behavior)
  lcd.setCursor(10,1);
  lcd.print("   ");
  lcd.clear();

  // Initialize SPI bus and MFRC522 RFID reader
  SPI.begin();
  mfrc522.PCD_Init();

  // Configure buzzer pin as output
  pinMode(Buzzer, OUTPUT);
}

void loop() {
  // Position LCD cursor to the first row/column so the UID prints from the beginning
  lcd.setCursor(0,0);

  // 1) Check whether a new RFID card is present near the reader
  if (mfrc522.PICC_IsNewCardPresent()) {
    // 2) Attempt to read the card's serial number (UID)
    if (mfrc522.PICC_ReadCardSerial()) {
      // Build a human-readable hex string from the UID bytes, e.g. "AA:BB:CC:DD:"
      rfidnum = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        // Convert each UID byte to a 2-digit hex string and append ':' as a separator
        rfidnum += toHexStr(mfrc522.uid.uidByte[i]) + ":";
      }
      // Ensure letters are uppercase (A-F) for consistent formatting
      rfidnum.toUpperCase();

      // Log the UID to Serial for debugging/recording
      Serial.println(rfidnum);

      // Cleanly stop communication with the card so the reader can accept another card.
      // - PICC_HaltA halts the current card
      // - PCD_StopCrypto1 stops the encryption session (if used)
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();

      // Display the assembled UID on the LCD starting at the cursor (0,0)
      // Note: if the UID is longer than the display width, it will be truncated.
      lcd.print(rfidnum);

      // Short audible beep to indicate a successful read
      digitalWrite(Buzzer, HIGH);
      delay(500);
      digitalWrite(Buzzer, LOW);
      delay(500);
    }
  }
}

// Helper: convert a single byte to a two-character hex string (e.g. 0x0A -> "0A")
// Ensures a leading zero is present for values less than 0x10 to always produce two hex digits.
String toHexStr(byte n) {
  if (n < 0x10) return ("0" + String(n, HEX));
  else return String(n, HEX);
}
