#include <WiFi.h>

// debounce variables
bool state1=1,lastState1=1;
bool state2=1,lastState2=1;

// relay state variables
char state_relay[2] = {0,0};

// Wifi credentials
char *ssid = "ESP32-Nihahaha";
char *passwd = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.1";
WiFiClient client;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

void loop() {
  state1=digitalRead(5); // read button state
  if(state1!=lastState1){
    delay(20); //debounce
    if(digitalRead(5)==0){
      if (state_relay[0] == 0){
        if (client.connect(host, port)) {

          client.print("ON1&");
          client.flush();
          client.stop();
        }
        state_relay[0] = 1;
      }
      else{
        if (client.connect(host, port)) {
          client.print("OFF1&");
          client.flush();
          client.stop();
        }
        state_relay[0] = 0;
      }
    }
  }
  lastState1=state1;

  state2=digitalRead(4); // read button state
  if(state2!=lastState2){
    delay(20); //debounce
    if(digitalRead(4)==0){
      if (state_relay[1] == 0){
        if (client.connect(host, port)) {

          client.print("ON2&");
          client.flush();
          client.stop();
        }
        state_relay[1] = 1;
      }
      else{
        if (client.connect(host, port)) {
          client.print("OFF2&");
          client.flush();
          client.stop();
        }
        state_relay[1] = 0;
      }
    }
  }
  lastState2=state2;
}
