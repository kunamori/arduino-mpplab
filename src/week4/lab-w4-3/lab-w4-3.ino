#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define PIN_DHT 2
#define DHTTYPE DHT22

DHT dht(PIN_DHT, DHTTYPE);

// Module Config
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  // lcd.noBacklight();
  lcd.backlight();

  lcd.home();
  lcd.print("Power On.");
  delay(2000);

  lcd.setCursor(10,1);
  lcd.print("   ");
  lcd.clear();

  dht.begin();

}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if(isnan(h) || isnan(t) || isnan(t)){
    Serial.println("ERROR: Failed to Read from DHT Sensors.");
    return;
  }
  lcd.setCursor(0,0);
  lcd.print("Temp : ");
  lcd.print(t);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");

}
