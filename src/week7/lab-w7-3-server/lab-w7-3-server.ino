#include <WiFi.h>
#include <WiFiUdp.h>

#define LED_RED_PIN 4
#define LED_GREEN_PIN 2
#define LED_BLUE_PIN 15

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.5";
WiFiUDP udp;

char packetBuffer[255];

int data[2] = {0};
int led_data[3] = {0,0,0};

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  udp.begin(port);
}

void loop() {
  int numbuffer = udp.parsePacket();
  if (numbuffer > 0) {
    // Read the incoming UDP datagram into our buffer (up to 255 bytes).
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      // Null-terminate so we can safely treat packetBuffer as a C-string.
      packetBuffer[len] = '\0';
      String s(packetBuffer);

      // Expected payload format: "<channel>_<value>&" (for example: "1_128&").
      // splitString() parses the payload and places two integers into the global `data[]`:
      // - data[0] = channel index (which LED channel to update)
      // - data[1] = PWM value (0..255)
      splitString(s);

      // Update the cached LED values array at the requested channel.
      led_data[data[0]] = data[1];

      // Apply the PWM values to the respective LED pins.
      // analogWrite typically accepts values in the 0..255 range on many boards.
      analogWrite(LED_RED_PIN, led_data[0]);
      analogWrite(LED_GREEN_PIN, led_data[1]);
      analogWrite(LED_BLUE_PIN, led_data[2]);

      // Small delay to allow the hardware to update and to avoid extremely rapid successive writes.
      delay(50);

      // Debug output: print which channel and value were received.
      Serial.println(data[0]);
      Serial.println(data[1]);
    }

    // Simple handshake / acknowledgement:
    // Send a short 'A' message back to the configured host to acknowledge receipt and processing.
    // This is a lightweight way for the server to tell the client it handled the packet.
    udp.beginPacket(host, port);
    udp.print("A");
    udp.flush();
    udp.endPacket();
  }
}

void splitString(String str) {
  // Parse a payload in the format "<index>_<value>&" into integer parts stored in `data[]`.
  // Example: "2_128&" -> data[0] = 2, data[1] = 128
  // The function splits the string by the delimiters '_' and '&'.
  int index = 0;
  String s = "";
  for (int i = 0; i < str.length(); i++) {
    if (str[i] != '_' && str[i] != '&') {
      s += str[i];
    } else {
      data[index] = s.toInt();
      index++;
      s = "";
    }
  }
  data[index] = s.toInt();
}
