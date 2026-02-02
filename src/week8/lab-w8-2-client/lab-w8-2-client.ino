#include <WiFi.h>
#include <HTTPClient.h>

bool state1=1,lastState1=1;
bool state2=1,lastState2=1;

int rr_state[2] = {0,0};

// Configuration
// Wifi credentials
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
// App Script
String key = "YOUR_APP_SCRIPT_KEY";

unsigned long time_request = 0;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

String request_google(String url){
  HTTPClient http;
  http.begin(url.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  String s = "";
  if(httpCode > 0){
    s = http.getString();
  }
  http.end();
  Serial.println(s);
  return s;
  
}

void loop() {
  state1=digitalRead(5); // read button state
  if(state1!=lastState1){
      delay(20); //debounce
      if(digitalRead(5)==0){
        Serial.println("BTN Pin 5 Pressed");
        if (rr_state[1] == 0){
          rr_state[1] = 1;
        }else{
          rr_state[1] = 0;
        }
        String url = "https://script.google.com/macros/s/" + key + "/exec?s2=" + rr_state[1];
        String str = request_google(url);
        Serial.println(str);
      }
  }
  lastState1=state1;

  state2=digitalRead(4); // read button state
  if(state2!=lastState2){
      delay(20); //debounce
      if(digitalRead(4)==0){
        Serial.println("BTN Pin 4 Pressed");
        if (rr_state[0] == 0){
          rr_state[0] = 1;
        }else{
          rr_state[0] = 0;
        }
        String url = "https://script.google.com/macros/s/" + key + "/exec?s1=" + rr_state[0];
        String str = request_google(url);
        Serial.println(str);
      }
    }
  lastState2=state2;

}

