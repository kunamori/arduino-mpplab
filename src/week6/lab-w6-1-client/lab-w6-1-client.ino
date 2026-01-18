#include <WiFi.h>

bool state1=1,lastState1=1;
bool state2=1,lastState2=1;
int x=0;

// Wifi credentials
char *ssid = "ESP32-Nihahaha";
char *passwd = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.1";
WiFiClient client; // create wifi client object

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
        x++;
        if(x>99){ // if exceeds 2 digits, reset to 0
          x=0;
        }
        if (client.connect(host, port)) {
          
          client.print(String(x)+"&");
          client.flush();
          client.stop();
        }
      }
  }
  lastState1=state1;

  state2=digitalRead(4); // read button state
  if(state2!=lastState2){
      delay(20); //debounce
      if(digitalRead(4)==0){
        x--;
        if(x<0){ // if below 0, reset to 99
          x=99;
      }
      if (client.connect(host, port)) {
        client.print(String(x)+"&");
        client.flush();
        client.stop();
      }
    }
  }
  lastState2=state2;

  Serial.println(x); // for debugging

}
