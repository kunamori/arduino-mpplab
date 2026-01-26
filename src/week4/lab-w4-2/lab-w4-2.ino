#include "DHT.h"

#define PIN_DHT 2
#define DHTTYPE DHT22

DHT dht(PIN_DHT, DHTTYPE);


void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // DHT22 sensors require a minimum interval between reads (typically 1–2 seconds).
  // Respecting this interval reduces the chance of getting invalid readings.
  delay(2000);

  // Read humidity (percentage), Celsius temperature, and Fahrenheit temperature.
  // Each read returns a float and may return NAN on failure — always validate the results.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true); // pass `true` to get Fahrenheit

  // Error handling: check for NaN results which indicate a failed sensor read.
  // Note: be sure to check all three values (h, t, f). Some implementations mistakenly repeat checks.
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("ERROR: Failed to Read from DHT Sensors.");
    // Early return avoids printing invalid values and allows the next loop to attempt another read.
    return;
  }

  // Print validated readings to the Serial monitor.
  Serial.print("Humidity: ");
  Serial.println(h);
  Serial.print("Temp (C): ");
  Serial.println(t);
  Serial.print("Temp (F): ");
  Serial.println(f);
  Serial.println("===================");
}
