#include <WiFi.h>

// Wifi credentials
char *ssid = "ESP32-Nihahaha";
char *passwd = "12345678";
char *host = "192.168.4.1";
const uint16_t port = 6969;
WiFiClient client;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  // Read raw analog value from ADC pin (ESP32 ADC range is 0..4095).
  int a_raw = analogRead(34); // raw ADC sample
  // Map the raw ADC reading to a 0..180 range (e.g., a servo angle or similar)
  // - analogRead on ESP32 returns 0..4095 (12-bit ADC typical)
  // - map() performs integer mapping; for higher precision use floating-point math.
  int a = map(a_raw, 0, 4095, 0, 180); // integer mapping from ADC to angle
  // Ensure the final value remains within the expected bounds in case of slight out-of-range readings.
  a = constrain(a, 0, 180);

  // Connect to the server and send the mapped value.
  // Protocol: send "<value>&" and expect the server to reply with a message terminated by '&'.
  // NOTE: the code below blocks while waiting for a response; consider adding a timeout in production code.
  if (client.connect(host, port)) {
    // Send the value followed by '&' as a simple delimiter the server understands.
    client.print(String(a) + "&");
    client.flush(); // flush the outgoing buffer to ensure data is transmitted immediately
    // BLOCKING WAIT: spin until the server sends data back. This will block indefinitely if the server never responds.
    while (client.available() == 0);
    // Read the server response up to the '&' delimiter and print it for debugging/processing.
    String response = client.readStringUntil('&');
    Serial.print("Response from server: ");
    Serial.println(response);
    client.stop(); // close the TCP connection when finished
  }

  Serial.println(a);
  delay(50);
}
