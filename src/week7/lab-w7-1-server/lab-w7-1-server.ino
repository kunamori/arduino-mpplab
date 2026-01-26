// Week 7 — Lab w7-1-server: UDP server for remote LED control
// Purpose: Run an ESP32 access point and a UDP server that listens for packets with
// a number (1..6) indicating which LED to toggle. Example payload: "3"
// Notes:
// - This sketch uses `udp.parsePacket()` to check for incoming datagrams and `udp.read(...)`
//   to copy the payload into `packetBuffer`.
// - The payload is parsed into an integer and validated to be within 1..NUMBER_LED.
// - LED states are tracked in `ledStatus[]` to allow toggling behavior.
#include <WiFi.h>
#include <WiFiUdp.h>

// LED pin mapping (defines the pins the six LEDs are attached to)
#define LED_1 13
#define LED_2 12
#define LED_3 14
#define LED_4 27
#define LED_5 26
#define LED_6 25

int NUMBER_LED = 6;
int ledStatus[] = {0,0,0,0,0,0};
int ledPins[] = { LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};
int selLED;

// WiFi and UDP configuration:
// - This sketch creates an Access Point (softAP) so clients can connect directly to this device.
// - UDP is used for lightweight unconnected messages; a single datagram containing the LED number is expected.
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
// Buffer to receive incoming UDP packet payloads. Make sure it's large enough for expected messages.
char packetBuffer[255];
// UDP object used to receive datagrams
WiFiUDP udp;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUMBER_LED; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Start the ESP32 in Access Point mode so clients can connect directly to it.
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Start UDP listener on the configured port. This prepares the UDP stack to receive packets.
  udp.begin(port);
}

void loop() {
  // Check if a UDP datagram has arrived. parsePacket() returns the byte-length of the next packet.
  int numBytes = udp.parsePacket();

  if (numBytes > 0) {
    // Read the packet into packetBuffer; len is number of bytes read (up to buffer size).
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      // Null-terminate the received bytes so we can safely treat packetBuffer as a C-string.
      packetBuffer[len] = '\0';

      // Convert the received bytes into an Arduino String for convenient parsing.
      String input(packetBuffer);

      // Convert the string payload to an integer. Expected payload: a single integer such as "3"
      selLED = input.toInt();

      // Validate the parsed integer is in the allowable LED range (1..NUMBER_LED).
      if (selLED >= 1 && selLED <= NUMBER_LED) {
        int index = selLED - 1; // Convert from 1-based LED number to 0-based array index.

        // Toggle the LED state: if currently OFF (0), turn it ON (HIGH) and update status.
        if (ledStatus[index] == 0) {
          digitalWrite(ledPins[index], HIGH);
          ledStatus[index] = 1;
          Serial.println("LED " + input + " ON");
        } else {
          // Otherwise, turn the LED OFF and update status to reflect the change.
          digitalWrite(ledPins[index], LOW);
          ledStatus[index] = 0;
          Serial.println("LED " + input + " OFF");
        }
      } else {
        // Input was not a valid LED number; report an error for debugging.
        Serial.println("ERROR: INPUT_USER");
      }
    }
  }
}
