#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define Buzzer A0

MFRC522 mfrc522(SS_PIN, RST_PIN);
String rfidnum = "";


// LCD Module Config
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.home();
  lcd.print("Power On.");
  delay(2000);
  lcd.setCursor(10,1);
  lcd.print("   ");
  lcd.clear();

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(Buzzer, OUTPUT);
}

void loop() {
  lcd.setCursor(0,0);
  if (mfrc522.PICC_IsNewCardPresent()){
    if (mfrc522.PICC_ReadCardSerial()){
      rfidnum = "";
      for(byte i = 0; i < mfrc522.uid.size; i++){
        rfidnum += toHexStr(mfrc522.uid.uidByte[i])+":";
      }
      rfidnum.toUpperCase();
      Serial.println(rfidnum);

      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();

      lcd.print(rfidnum);
      digitalWrite(Buzzer, HIGH);
      delay(500);
      digitalWrite(Buzzer, LOW);
      delay(500);
    }
  }

}

String toHexStr(byte n){
  if(n<0x10) return ("0"+String(n,HEX));
  else return String(n,HEX);
}
