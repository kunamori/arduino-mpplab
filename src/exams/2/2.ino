#include <WiFi.h>

int n1 = 0;
int n2 = 0;

bool lastState1 = HIGH;
bool lastState2 = HIGH;

// Wifi credentials
char *ssid = "wifi07";
char *passwd = "12345678";
const uint16_t port = 2000;
char *host = "192.168.4.1";
WiFiClient client;  // create wifi client object

void sendData() {
  if (client.connect(host, port)) {
    // "N1_N2&"
    String payload = String(n1) + "_" + String(n2) + "&";
    Serial.print("Sending: ");
    Serial.println(payload);
    client.print(payload);
    client.flush();
    client.stop();
  } else {
    Serial.println("Connection to host failed");
  }
}

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected.");

  // Set up the button pins with pull-up resistors
  pinMode(16, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  // Read the current state of the buttons
  bool state1 = digitalRead(16);
  bool state2 = digitalRead(4);

  // Check for SW1 press (falling edge)
  if (state1 == LOW && lastState1 == HIGH) {
    delay(20);  // Debounce delay
    // Re-check the state to confirm the press
    if (digitalRead(16) == LOW) {
      n1++;
      if (n1 > 99) {  // Reset to 0 if it exceeds 99
        n1 = 0;
      }
      sendData();  // Send the updated values
    }
  }

  // Check for SW2 press (falling edge)
  if (state2 == LOW && lastState2 == HIGH) {
    delay(20);  // Debounce delay
    // Re-check the state to confirm the press
    if (digitalRead(4) == LOW) {
      n2++;
      if (n2 > 99) {  // Reset to 0 if it exceeds 99
        n2 = 0;
      }
      sendData();  // Send the updated values
    }
  }

  // Update the last state for both buttons
  lastState1 = state1;
  lastState2 = state2;

  delay(10);
}
