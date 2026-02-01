#include <WiFi.h>

bool state1=1,lastState1=1;
bool state2=1,lastState2=1;

char *ssid = "wifi02";
char *passwd = "12345678";
const uint16_t port = 2000;
char *host = "192.168.4.1";
WiFiClient client; // create wifi client object

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  pinMode(26, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
}

void loop() {
  int a = analogRead(34);
  a = map(a, 0, 4095, 0, 180);
  a = constrain(a, 0, 180);

  state1=digitalRead(26); // read button state
  if(state1!=lastState1){
      delay(20); //debounce
      if(digitalRead(26)==0){
        }
        if (client.connect(host, port)) {

          client.print("ON_"+String(a)+"&");
          Serial.println("ON_"+String(a)+"&");
          client.flush();
          client.stop();
        }
      }
  lastState1=state1;

  state2=digitalRead(25); // read button state
  if(state2!=lastState2){
      delay(20); //debounce
      if(digitalRead(25)==0){
        if (client.connect(host, port)) {
          client.print("OFF_"+String(0)+"&");
          Serial.println("OFF_"+String(0)+"&");
          client.flush();
          client.stop();
        }
      }
    }
  lastState2=state2;

  Serial.println(a); // for debugging
  delay(50);
}
