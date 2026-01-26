#include <WiFi.h>

// WiFi access point and TCP server configuration
// - This device runs as an Access Point (softAP) and hosts a simple TCP server on the configured port.
// - Clients connect to this server and send short command strings to toggle relays.
// - The server expects commands terminated with '&' (e.g., "ON1&", "OFF2&").
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
WiFiServer server(port);

void setup() {
  Serial.begin(9600);

  WiFi.softAP(ssid, password);
  WiFi.mode(WIFI_AP_STA);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  // Relay control notes:
  // - Relays are wired as active-LOW on this board: driving the pin LOW energizes (turns ON) the relay,
  //   while HIGH turns the relay off.
  // Initialize both relays to OFF (HIGH) to ensure a safe default state on startup.
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client connected");
    while (client.connected()) {
      if (client.available()) {
        // Read a command string from the client up to the delimiter '&'.
        // Expected commands (examples): "ON1&", "OFF1&", "ON2&", "OFF2&"
        // The `readStringUntil('&')` call returns the command portion before '&'.
        String request = client.readStringUntil('&');
        Serial.print("Received request: ");
        Serial.println(request);

        // Interpret the request and update the relay outputs accordingly.
        // NOTE: relays are active-LOW on this setup: writing LOW activates the relay (turns it ON).
        if (request == "ON1") {
          // Turn Relay 1 ON (drive active-low relay pin LOW)
          digitalWrite(12, LOW);
        } else if (request == "OFF1") {
          // Turn Relay 1 OFF (drive pin HIGH)
          digitalWrite(12, HIGH);
        } else if (request == "ON2") {
          // Turn Relay 2 ON
          digitalWrite(13, LOW);
        } else if (request == "OFF2") {
          // Turn Relay 2 OFF
          digitalWrite(13, HIGH);
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
