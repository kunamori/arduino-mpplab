#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define PIN_DHT 2
#define DHTTYPE DHT22

DHT dht(PIN_DHT, DHTTYPE);

// Module Config
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize Serial for debugging and error reporting over the serial monitor.
  // The sketch prints errors to Serial when DHT reads fail; enabling Serial helps
  // to diagnose sensor issues during development.
  Serial.begin(9600);

  // Initialize the I2C LCD module:
  // - `lcd.init()` initializes the LCD and prepares it for communication on the configured I2C address (0x27).
  // - `lcd.backlight()` turns the backlight ON so text is visible (use `lcd.noBacklight()` to turn it off).
  lcd.init();
  // lcd.noBacklight();
  lcd.backlight();

  // Show a brief startup message so the user sees that the system powered on,
  // then clear the display to prepare for sensor readings.
  lcd.home();          // place cursor at address 0 (top-left)
  lcd.print("Power On.");
  delay(2000);         // keep message visible for 2 seconds

  // Clear any residual characters on the second line, then clear the whole display.
  // This ensures old content doesn't overlap with subsequent sensor output.
  lcd.setCursor(10,1);
  lcd.print("   ");
  lcd.clear();

  // Initialize the DHT sensor library so subsequent read calls are ready.
  dht.begin();
}

void loop() {
  // Respect the DHT sensor's recommended read interval (~1-2 seconds).
  // A short delay at the top of the loop ensures we do not poll the sensor too quickly.
  delay(2000);

  // Read humidity and temperature (C and F)
  float h = dht.readHumidity();          // Relative humidity in percent
  float t = dht.readTemperature();       // Temperature in Celsius
  float f = dht.readTemperature(true);   // Temperature in Fahrenheit

  // Validate readings: the DHT library returns NaN on failed reads.
  // We explicitly check all three values (h, t, f) to detect errors reliably.
  if (isnan(h) || isnan(t) || isnan(f)) {
    // Report the failure to the serial console for debugging.
    Serial.println("ERROR: Failed to Read from DHT Sensors.");

    // Provide clear feedback on the LCD as well: display an error message,
    // then pause briefly so the message is visible before the next retry.
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DHT Sensor Error");
    lcd.setCursor(0,1);
    lcd.print("Retrying...");
    delay(1000);
    return; // Try again on the next loop iteration
  }

  // If readings are valid, update the LCD display lines:
  // - Top line shows temperature in Celsius.
  // - Bottom line shows humidity as a percentage.
  lcd.setCursor(0,0);
  lcd.print("Temp : ");
  lcd.print(t);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");
}
