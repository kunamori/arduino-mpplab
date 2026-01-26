#include <WiFi.h>

bool state1=1,lastState1=1;
bool state2=1,lastState2=1;
int x=0;

// WiFi credentials (SSID and password used to connect to the access point)
char *ssid = "ESP32-Nihahaha";
char *passwd = "12345678";
const uint16_t port = 6969; // server port to send messages to
char *host = "192.168.4.1"; // server hostname/IP (target for the TCP client)
// WiFiClient is used to create a TCP client connection to the server.
// Typical usage pattern: client.connect(host, port) -> client.print(...) -> client.flush() -> client.stop()
WiFiClient client;

void setup() {
  Serial.begin(9600);

  // Begin WiFi connection. This will block until the device joins the configured AP.
  // In production, consider a non-blocking approach or a connection timeout to avoid hanging indefinitely.
  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

void loop() {

  // Read button connected to pin D5 (active-low with INPUT_PULLUP).
  // Use edge-detection by comparing the current and last states, then debounce.
  state1 = digitalRead(5); // read button state
  if (state1 != lastState1) {
      // Simple debounce: wait a short time and confirm the button is still pressed.
      delay(20); // debounce
      if (digitalRead(5) == 0) { // LOW == pressed with INPUT_PULLUP
        x++;
        // Wrap counter if it exceeds 99 to maintain a two-digit value.
        if (x > 99) { // if exceeds 2 digits, reset to 0
          x = 0;
        }
        // Open a TCP connection to the server and send the current value.
        // Message format: a numeric value followed by '&' as a delimiter, e.g., "42&"
        if (client.connect(host, port)) {

          client.print(String(x) + "&");
          client.flush(); // ensure the data is sent before closing the connection
          client.stop();  // close the connection gracefully
        }
      }
  }
  lastState1 = state1;

  // Read button connected to pin D4 (active-low with INPUT_PULLUP).
  state2 = digitalRead(4); // read button state
  if (state2 != lastState2) {
      // Debounce input to avoid false triggers from mechanical bounce.
      delay(20); // debounce
      if (digitalRead(4) == 0) { // LOW == pressed
        x--;
        // Wrap-around if the count goes below zero to keep it within 0..99.
        if (x < 0) { // if below 0, reset to 99
          x = 99;
        }
        // Send the updated counter using the same message format ("value&").
        if (client.connect(host, port)) {
          client.print(String(x) + "&");
          client.flush();
          client.stop();
        }
      }
  }
  lastState2 = state2;

  Serial.println(x); // for debugging (prints the locally tracked counter value)

}
