#include <WiFi.h>
#include <HTTPClient.h>

// Configuration
// Wifi credentials
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
// App Script
String key1 = "APP_SCRIPT_KEY_1";
String key2 = "APP_SCRIPT_KEY_2";
unsigned long time_request = 0;
int data[2] = {0};

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  if(millis() - time_request >= 500){
    time_request = millis();
    String str1 = request_google(key1);
    String str2 = request_google(key2);
    if(str1!="" or str2!=""){
      Serial.println("---------");
      Serial.print("Relay 0: ");
      Serial.println(str1);
      Serial.print("Relay 1: ");
      Serial.println(str2);
      Serial.println("---------");
    }
    if(str1 == "1"){
      digitalWrite(12, LOW);
    }
    if(str1 == "0"){
      digitalWrite(12, HIGH);
    }
    if(str2 == "1"){
      digitalWrite(13, LOW);
    }
    if(str2 == "0"){
      digitalWrite(13, HIGH);
    }
  }
}

String request_google(String key){
  HTTPClient http;
  String url = "https://script.google.com/macros/s/" + key + "/exec";
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
