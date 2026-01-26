#include <WiFi.h>
#include <WiFiUdp.h>

// Wifi credentials
// Protocol: send a short sensor request string (e.g. "TEMP" or "HUMI") as a UDP datagram to host:port.
// The server is expected to reply with the requested value as a UDP datagram. This sketch prints the reply.
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.1";
WiFiUDP udp;

// IP Configuration (static IP)
// - `ip` is the static address assigned to this device (192.168.4.5).
// - `gateway` is the AP/router address (192.168.4.1).
// - `subnet` defines the subnet mask.
// Note: calling `WiFi.config(...)` before `WiFi.begin(...)` applies these static settings.
IPAddress ip(192,168,4,5);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
unsigned long timeout = 0;

String SENSOR_DATA;

void setup() {
  Serial.begin(9600);
  // Apply a static IP configuration; call WiFi.config(...) before WiFi.begin(...) so the chosen
  // static address (ip) is used instead of DHCP. This makes the device address predictable when
  // used in direct AP + client setups.
  WiFi.config(ip, gateway, subnet);

  // Start WiFi and wait for a connection to the AP (blocking here for simplicity).
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print(WiFi.localIP()); // prints the effective IP (static or DHCP)
  // Initialize the UDP socket on the chosen local port for both sending and receiving datagrams.
  udp.begin(port);
}

void loop() {

  if (Serial.available() > 0) {
    SENSOR_DATA = Serial.readStringUntil('\n');
    SENSOR_DATA.trim();

    // Send the sensor request (plain ASCII string) as a UDP datagram to the server.
    // Examples:
    //  - "TEMP" -> request a temperature reading
    //  - "HUMI" -> request a humidity reading
    // The server should respond with the requested value as a UDP datagram.
    udp.beginPacket(host, port);
    udp.print(SENSOR_DATA); // payload = SENSOR_DATA
    udp.endPacket();        // send the UDP datagram

  }

  int packetSize = udp.parsePacket();
  if (packetSize) {
    char packetBuffer[255];
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String input(packetBuffer);

      // Interpret the server response: this client assumes the incoming UDP payload
      // corresponds to the most recent `SENSOR_DATA` request. For example, if the last
      // sent request was "TEMP" then the incoming payload is treated as a temperature.
      // Note: This simple approach assumes responses arrive in-order and match the last request.
      // For improved robustness, consider including an ID/type in the server reply or a request ID.
      if (SENSOR_DATA == "TEMP") {
        Serial.println("Received (Temperature) : " + input);
      } else if (SENSOR_DATA == "HUMI") {
        Serial.println("Received (Humidity) : " + input);
      } else {
        Serial.println("Received (Unknown) : " + input);
      }
    }
  }
}
