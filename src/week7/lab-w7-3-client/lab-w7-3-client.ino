#include <WiFi.h>
#include <WiFiUdp.h>

// Week 7 — Lab w7-3-client: UDP-based RGB/handshake client
// Purpose:
// - Periodically send a heartbeat message to the server (example: "10_20&").
// - If the server replies with "A", send the current selection and analog value in format "selRGB_value&".
// - Messages use '&' as a delimiter. UDP is connectionless; no delivery guarantee.
// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.1";
WiFiUDP udp;

// IP Configuration
IPAddress ip(192,168,4,5);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
unsigned long time_out = 0;

String selRGB = "0";
int selRGB_BTN = 0;
bool state1=1,lastState1=1;

void setup() {
  Serial.begin(9600);

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());
  udp.begin(port);

  pinMode(4, INPUT_PULLUP);
}

// Helper function: send a short, delimited UDP payload to the configured host and port.
// - `message` should include any protocol delimiter (this sketch uses '&' at the end).
// - The function opens a UDP packet, writes the payload, and finalizes the packet.
void send_data(String message) {
  udp.beginPacket(host, port);
  char char_message[255];
  sprintf(char_message, "%s", message.c_str());
  udp.print(char_message);
  udp.flush();
  udp.endPacket();
}

void loop() {
  // Read the selection button (pin 4) and use edge detection + debounce to detect presses.
  // - Edge detection: compare current reading with last reading to find transitions.
  // - Debounce: after detecting a change, wait briefly and confirm the button is still pressed.
  state1 = digitalRead(4);
  if (state1 != lastState1) {
      delay(20); // debounce: confirm stable state after mechanical contact settles
      if (digitalRead(4) == 0) { // active-low: LOW means pressed
        selRGB_BTN++;
        if (selRGB_BTN > 2) {
          selRGB_BTN = 0; // cycle selection through 0..2
        }
      }
  }
  lastState1 = state1;
  Serial.println(selRGB_BTN);
  selRGB = String(selRGB_BTN);

  // Sample analog input (pin 34). On ESP32 analogRead returns 0..4095 (12-bit ADC).
  // Map that raw value to 0..255 so it can be used as an RGB/brightness value.
  int a = analogRead(34);
  a = map(a, 0, 4095, 0, 255);
  String ab = String(a);

  // Periodic heartbeat / handshake: every 1000 ms send a small message that the server can respond to.
  // The server may reply with 'A' to request the full payload from the client.
  if (millis() - time_out >= 1000) {
    time_out = millis();
    send_data("10_20&"); // heartbeat message; '&' is the delimiter used by this protocol
  }

  int numbuffer = udp.parsePacket();
  if (numbuffer > 0) {
    char packetBuffer[255];
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String s(packetBuffer);

      // When the server sends 'A' it is asking the client to send the current data payload.
      if (s == "A") {
        // Compose and send: "<selRGB>_<value>&" where '&' is the message terminator recognized by the server.
        send_data(selRGB + "_" + ab + "&");
        // Reset the heartbeat timer so we don't immediately send another handshake.
        time_out = millis();
      }
    }
  }
  delay(10);
}
