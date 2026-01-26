#include <WiFi.h>
#include <ESP32Servo.h>

// Week 6 — Lab w6-3-server: TCP server driving a servo via angle commands
// Purpose: create an ESP32 WiFi access point and a small TCP server that accepts angle values
// sent by a client (terminated with '&'), sets the connected servo to that angle, and sends back
// a short acknowledgement ("E&"). This sketch uses the Servo library for servo control and
// WiFiServer for simple TCP connections.
//
// WiFi AP credentials used by the ESP32 softAP
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
WiFiServer server(port);
 
// Servo instance: a single servo attached to a hardware pin (26)
Servo Servo1;

void setup() {
  // Initialize serial console for debug and status messages
  Serial.begin(9600);
 
  // Configure the ESP32 as a WiFi Access Point (softAP).
  // This creates a local network with the configured SSID/password so clients can connect directly.
  WiFi.softAP(ssid, password);
  // Use AP+STA mode to allow AP features while keeping station capabilities if needed.
  WiFi.mode(WIFI_AP_STA);
 
  // Print the AP IP address to the serial console for diagnostics.
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
 
  // Start the TCP server to listen for incoming client connections on the configured port.
  server.begin(port);
 
  // Attach the servo to GPIO26 (choose an appropriate pin for your board).
  Servo1.attach(26);
}

void loop() {
  // Check for an incoming client connection. `server.available()` returns a WiFiClient
  // object that evaluates to true when a client is connected.
  WiFiClient client = server.available();
 
  if (client) {
    Serial.println("New Client connected");
 
    // While the client remains connected, process any incoming data.
    while (client.connected()) {
      if (client.available()) {
        // Read a message up to the '&' delimiter that the client uses to mark message end.
        String request = client.readStringUntil('&');
        Serial.print("Received request: ");
        Serial.println(request);
 
        // Expect the request to contain an ASCII integer representing the desired servo angle.
        // Convert the string to an integer and write it to the servo.
        // Note: servo angles are typically in the 0..180 range; consider validating/clamping on either side.
        int angle = request.toInt();
        Servo1.write(angle);
 
        // Send a short acknowledgement back to the client, using '&' as the delimiter.
        client.print("E&");
        client.flush();
      }
    }
    // Clean up the client connection after it disconnects to free resources.
    client.stop();
    Serial.println("Client disconnected");
  }
}
