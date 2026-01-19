#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// Hardware Configuration
constexpr uint8_t PIN_DHT = 2;
constexpr uint8_t DHTTYPE = DHT22;

// LCD Configuration
constexpr uint8_t LCD_ADDRESS = 0x27;
constexpr uint8_t LCD_COLS = 16;
constexpr uint8_t LCD_ROWS = 2;

constexpr uint16_t STARTUP_DELAY_MS = 2000;
constexpr uint16_t SENSOR_READ_INTERVAL_MS = 2000;

DHT dht(PIN_DHT, DHTTYPE);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

unsigned long lastReadTime = 0;

void initializeLCD() {
  lcd.init();
  lcd.backlight();
  lcd.home();
  lcd.print(F("Power On."));
  delay(STARTUP_DELAY_MS);
  lcd.clear();
}

void displaySensorData(const float temperature, const float humidity) {
  lcd.setCursor(0, 0);
  lcd.print(F("Temp : "));
  lcd.print(temperature, 1);
  lcd.print(F("C"));
  
  lcd.setCursor(0, 1);
  lcd.print(F("Humidity: "));
  lcd.print(humidity, 1);
  lcd.print(F("%"));
}

void readAndDisplaySensors() {
  const float h = dht.readHumidity();
  const float t = dht.readTemperature();
  
  if(isnan(h) || isnan(t)){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Sensor Error!"));
    return;
  }
  
  displaySensorData(t, h);
}

void setup() {
  Serial.begin(9600);
  initializeLCD();
  dht.begin();
}

void loop() {
  const unsigned long currentTime = millis();
  
  if (currentTime - lastReadTime >= SENSOR_READ_INTERVAL_MS) {
    lastReadTime = currentTime;
    readAndDisplaySensors();
  }
}
