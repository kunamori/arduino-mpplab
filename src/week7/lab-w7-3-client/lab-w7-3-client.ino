#include <WiFi.h>
#include <WiFiUdp.h>

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

void send_data(String message) {
  udp.beginPacket(host, port);
  char char_message[255];
  sprintf(char_message, "%s", message.c_str());
  udp.print(char_message);
  udp.flush();
  udp.endPacket();
}

void loop() {
  state1=digitalRead(4);
  if(state1!=lastState1){
      delay(20); //debounce
      if(digitalRead(4)==0){
        selRGB_BTN++;
        if(selRGB_BTN>2){
          selRGB_BTN=0;
        }
      }
  }
  lastState1=state1;
  Serial.println(selRGB_BTN);
  selRGB = String(selRGB_BTN);

  int a = analogRead(34);
  a = map(a, 0, 4095, 0, 255);
  String ab = String(a);

  if(millis()-time_out >= 1000){
    time_out = millis();
    send_data("10_20&");
  }

  int numbuffer = udp.parsePacket();
  if (numbuffer > 0) {
    char packetBuffer[255];
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String s(packetBuffer);

      if (s == "A") {
        send_data(selRGB+"_"+ab+"&");
        time_out = millis();
      }
    }
  }
  delay(10);
}
