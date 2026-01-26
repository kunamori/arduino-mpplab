#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

// Initialize the I2C LCD library with the module address (0x27) and dimensions (16x2).
// The `lcd` object is used to show server status and incoming client messages.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi AP configuration and TCP server port
// - This sketch creates an access point (softAP) so clients can connect directly to this device.
// - The TCP server listens on `port` for simple delimited messages from connected clients.
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
WiFiServer server(port);

void setup() {
  // Start serial for debugging and status messages
  Serial.begin(9600);

  // Start the ESP32 in Access Point (softAP) mode so clients can connect directly.
  // This creates a local WiFi network with the given SSID and password.
  WiFi.softAP(ssid, password);

  // Use AP+STA mode so the device can act as an access point while retaining station abilities if needed.
  WiFi.mode(WIFI_AP_STA);

  // Start the TCP server listening on the configured port so clients can connect.
  server.begin();

  // Print the AP IP address to Serial for debugging; useful when you need to connect a client.
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Initialize the I2C LCD and display a brief startup message with the AP IP for local feedback.
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Server Started");
  lcd.setCursor(0, 1);
  lcd.print("IP:");
  lcd.print(WiFi.softAPIP());
  delay(2000);
  // Clear the display after showing the startup info to prepare for incoming messages.
  lcd.clear();
}

void loop() {
  // Check if any TCP client has connected to the server
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client connected");

    // While the client is still connected, process incoming data.
    // This loop will continue until the client disconnects.
    while (client.connected()) {
      // Only act when there is data available to read from the client
      if (client.available()) {
        // Read a message terminated by '&' (this sketch expects that client messages use '&' as a delimiter)
        String request = client.readStringUntil('&');

        // Log the received request to Serial for debugging
        Serial.print("Received request: ");

        // Update the LCD to show the most recent request.
        // Clearing the display before printing ensures old characters aren't left on the screen,
        // but note this can flicker for very frequent updates. Consider writing only when the message changes.
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print(request);

        // Echo the request to Serial as well
        Serial.println(request);
      }
    }

    // When finished with the client, stop the connection to free resources
    client.stop();

    Serial.println("Client disconnected");
  }
}
