#include "DHT.h"

#define PIN_DHT 2
#define DHTTYPE DHT22

DHT dht(PIN_DHT, DHTTYPE);


void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if(isnan(h) || isnan(t) || isnan(t)){
    Serial.println("ERROR: Failed to Read from DHT Sensors.");
    return;
  }
  Serial.print("Humidity: ");
  Serial.println(h);
  Serial.print("Temp (C): ");
  Serial.println(t);
  Serial.print("Temp (F): ");
  Serial.println(f);
  Serial.println("===================");
}
