#include <WiFi.h>

// Week 6 — Lab w6-2-client
// Purpose: read two local buttons and toggle two remote relays by sending short TCP commands
// to a server. This file documents debounce logic, relay state tracking, and WiFi client usage.
//
// Notes:
// - Buttons use INPUT_PULLUP (active-low). A pressed button reads LOW (0).
// - A short debounce delay is used to filter mechanical bounce before confirming a press.
// - Messages sent to the server are brief commands terminated with '&', e.g. "ON1&", "OFF2&".
// - `state_relay[]` keeps the locally-known relay states to avoid redundant messages.

// Debounce and edge-detection variables for the two buttons.
// - stateX: the current raw reading (HIGH/LOW) for button X
// - lastStateX: the previous loop's reading, used to detect transitions
bool state1 = 1, lastState1 = 1;
bool state2 = 1, lastState2 = 1;

// Relay state tracking: 0 == OFF, 1 == ON
// - state_relay[0] corresponds to relay 1
// - state_relay[1] corresponds to relay 2
char state_relay[2] = {0, 0};

// WiFi and server configuration
char *ssid = "ESP32-Nihahaha";
char *passwd = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.1"; // server IP (AP mode) or hostname

// WiFi client object used for short-lived TCP connections to send commands
WiFiClient client;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

void loop() {
  // Read and debounce button on pin 5 (active-low with INPUT_PULLUP).
  // Edge-detection: act only when the current reading differs from the previous one.
  state1 = digitalRead(5); // current raw reading (HIGH when released, LOW when pressed)
  if (state1 != lastState1) {
    // Simple debounce: wait briefly and confirm the button is still pressed.
    delay(20); // debounce (ms)
    if (digitalRead(5) == 0) { // confirmed press (LOW)
      // Toggle remote relay 1:
      // - If the locally stored state is 0 (OFF), send the ON command and set local state to 1.
      // - Otherwise send the OFF command and set local state to 0.
      if (state_relay[0] == 0) {
        // Open a short TCP connection to the server and send the ON command for relay 1.
        if (client.connect(host, port)) {
          client.print("ON1&"); // message format: "ON1&" to indicate Relay 1 -> ON
          client.flush();       // ensure bytes are transmitted
          client.stop();        // close the connection
        }
        state_relay[0] = 1; // update local state
      } else {
        // Send the OFF command for relay 1.
        if (client.connect(host, port)) {
          client.print("OFF1&"); // message: "OFF1&" to indicate Relay 1 -> OFF
          client.flush();
          client.stop();
        }
        state_relay[0] = 0; // update local state
      }
    }
  }
  // Save the current raw reading for the next loop iteration (edge detection).
  lastState1 = state1;

  // Read and debounce button on pin 4 (active-low). Implementation mirrors button 1 logic.
  state2 = digitalRead(4); // current raw reading (HIGH when released, LOW when pressed)
  if (state2 != lastState2) {
    // Debounce to filter contact bounce reliably
    delay(20); // debounce (ms)
    if (digitalRead(4) == 0) { // confirmed press
      // Toggle remote relay 2: send ON2& or OFF2& depending on the current known state.
      if (state_relay[1] == 0) {
        if (client.connect(host, port)) {
          client.print("ON2&"); // command to server to turn relay 2 on
          client.flush();
          client.stop();
        }
        state_relay[1] = 1; // update local state
      } else {
        if (client.connect(host, port)) {
          client.print("OFF2&"); // command to server to turn relay 2 off
          client.flush();
          client.stop();
        }
        state_relay[1] = 0; // update local state
      }
    }
  }
  // Update previous reading for next cycle
  lastState2 = state2;
}
