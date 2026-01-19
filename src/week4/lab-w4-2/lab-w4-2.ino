#include "DHT.h"

// Hardware Configuration
constexpr uint8_t PIN_DHT = 2;
constexpr uint8_t DHTTYPE = DHT22;

// Sensor timing
constexpr uint16_t SENSOR_READ_INTERVAL_MS = 2000;

DHT dht(PIN_DHT, DHTTYPE);

// Timing variables for non-blocking operation
unsigned long lastReadTime = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void readAndDisplaySensorData() {
  const float h = dht.readHumidity();
  const float t = dht.readTemperature();
  const float f = dht.readTemperature(true);
  
  if(isnan(h) || isnan(t) || isnan(f)){
    Serial.println(F("ERROR: Failed to Read from DHT Sensors."));
    return;
  }
  
  Serial.print(F("Humidity: "));
  Serial.println(h);
  Serial.print(F("Temp (C): "));
  Serial.println(t);
  Serial.print(F("Temp (F): "));
  Serial.println(f);
  Serial.println(F("==================="));
}

void loop() {
  const unsigned long currentTime = millis();
  
  // Non-blocking delay using millis()
  if (currentTime - lastReadTime >= SENSOR_READ_INTERVAL_MS) {
    lastReadTime = currentTime;
    readAndDisplaySensorData();
  }
}
