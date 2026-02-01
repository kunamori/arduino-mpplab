#include <WiFi.h>
#include <HTTPClient.h>

// Configuration
// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
// App Script
String key = "YOUR_APP_SCRIPT_KEY";


void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if(Serial.available() > 0 ){
    String data = Serial.readStringUntil('\n');
    String str = request_google(data);
    Serial.println(str);
  }
  String url = "https://script.google.com/macros/s/" + key + "/exec?data=" + data;
  http.begin(url.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  String s = "";
  if(httpCode > 0){
    s = http.getString();
  }
  http.end();
  return s;
}
