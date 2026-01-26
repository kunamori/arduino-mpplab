#include <WiFi.h>
#include <WiFiUdp.h>

// Week 7 — Lab w7-1-client: UDP client that sends serial commands to a server
// Purpose: connect to WiFi as a station and send plain ASCII messages via UDP to a host:port.
// - Input format: the sketch reads a line from Serial (terminated by newline) using readStringUntil('\n').
// - The newline is removed by readStringUntil, so the payload contains only the characters typed.
// - Messages are sent as raw ASCII in a single UDP packet (no additional framing). Example payload: "3" or "LED_ON".
// - UDP is connectionless and does not guarantee delivery; if reliability is required, implement acknowledgements/retries in the protocol.
// Implementation notes:
// - `udp.beginPacket(host, port)` starts building a datagram destined for the given host and port.
// - `udp.print(...)` places the payload into the packet buffer.
// - `udp.endPacket()` finalizes and transmits the UDP datagram.
// - `udp.flush()` is commonly a no-op for UDP in many libraries but is safe to call.
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.1";
WiFiUDP udp;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print(WiFi.localIP());
  udp.begin(port);
}

void loop() {
  // Prepare a UDP packet to be sent to the configured host and port.
  // Note: beginPacket() can be called before or after building the payload, but calling it before
  // ensures the destination is set early. If no Serial input is present, the packet will not be sent.
  udp.beginPacket(host, port);

  // If the user has typed a line in the Serial monitor, read and send it as a UDP message.
  if (Serial.available() > 0) {
    // Read one line from Serial (stops at newline). The trailing newline is discarded.
    String selLED = Serial.readStringUntil('\n');

    // Copy the Arduino String into a C-style buffer for transmission
    // (not strictly necessary for udp.print but shown for explicit control)
    char char_message[255];
    sprintf(char_message, "%s", selLED.c_str());

    // Write the message into the UDP packet buffer
    udp.print(char_message);

    // Flush (no-op for some UDP implementations but safe). Then finish and send the packet.
    udp.flush();
    udp.endPacket();

    // Small delay to rate-limit messages and avoid flooding the network;
    // adjust or remove as needed for your application.
    delay(1000);
  }
}
