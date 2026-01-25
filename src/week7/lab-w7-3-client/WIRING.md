# Lab W7-3-CLIENT: ESP32 WiFi UDP RGB LED Control Client with Button and Potentiometer

**File**: `lab-w7-3-client.ino`

## Description
This lab demonstrates ESP32 WiFi UDP client for controlling an RGB LED remotely using a button and potentiometer. The button cycles through RGB colors (R/G/B), and the potentiometer adjusts brightness (0-255). The client sends periodic heartbeat packets and responds to server acknowledgments. This is the **client** side that provides user input to control the RGB LED on the server (lab-w7-3-server).

## Components Required
- ESP32 DEVKIT × 1
- Push button × 1
- Potentiometer (10kΩ) × 1
- Breadboard
- Jumper wires

## Pin Configuration

| ESP32 Pin | Component | Connection | Notes |
|-----------|-----------|------------|-------|
| GPIO 4 | Button | Color selector | INPUT_PULLUP |
| GPIO 34 | Potentiometer | Brightness control (wiper) | ADC1_CH6, input only |
| 3.3V | Potentiometer | One outer pin | Power |
| GND | Button + Pot | Common ground | Shared GND |

## ASCII Wiring Diagram

```
        ESP32 DEVKIT        Button    Potentiometer
      ┌─────────────┐       
      │             │       ┌───┐         ┌─────┐
      │   GPIO 4 ●──┼───────┤ ● │         │  1  │  3.3V
      │             │       └─┬─┘         │     │
      │             │         │           │  ╱  │
      │  GPIO 34 ●──┼─────────┼───────────┤ 2   │  Wiper
      │  (ADC)      │         │           │     │
      │             │         │           │  ╲  │
      │  3.3V    ●──┼─────────┼───────────┤  3  │  Outer
      │             │         │           └─────┘
      │   GND    ●──┼─────────┴──────────────────── GND
      └─────────────┘       

      WiFi: Connects to "ESP32-Nihahaha" (server's AP)
      UDP: Sends to 192.168.4.1:6969
      Client IP: 192.168.4.5 (static)
      
      Button: Cycles through R(0), G(1), B(2)
      Potentiometer: Sets brightness 0-255
      
      Data Sent:
        Heartbeat: "10_20&" (every 1 second)
        RGB Data:  "0_180&" (color_brightness)
                   "1_255&"
                   "2_100&"
```

## Breadboard Layout

1. Place ESP32 on breadboard
2. Connect Button:
   - One side → GPIO 4
   - Other side → GND
   - Uses INPUT_PULLUP (no external resistor)
3. Connect Potentiometer:
   - One outer pin → 3.3V
   - Middle pin (wiper) → GPIO 34
   - Other outer pin → GND
4. **Note**: Swap pot outer pins to reverse rotation direction

## Component-Specific Details

### ESP32 DEVKIT
- **WiFi**: 802.11 b/g/n
- **Operating Voltage**: 3.3V logic
- **ADC**: 12-bit resolution (0-4095) on GPIO 34
- **WiFi Mode**: Station (STA) mode with static IP
- **GPIO 34**: ADC1_CH6, input only (no output capability)
- **GPIO 4**: INPUT_PULLUP for button

### Push Button
- **Configuration**: INPUT_PULLUP mode (internal pull-up enabled)
- **Active State**: LOW (0) when pressed
- **Debouncing**: 20ms software delay
- **Function**: Cycles through RGB color selection (0→1→2→0...)

### Potentiometer
- **Type**: Rotary potentiometer
- **Resistance**: 10kΩ (1kΩ to 100kΩ works)
- **Rotation**: ~270° (varies by model)
- **Output**: Variable voltage 0V to 3.3V
- **Mapping**: ADC 0-4095 → PWM 0-255

## Network Configuration

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│  (This Device)   │   "ESP32-Nihahaha"    │ (lab-w7-3-server)│
│  192.168.4.5     │                       │  192.168.4.1     │
│  (Static IP)     │                       │   (Gateway)      │
└──────────────────┘                       └──────────────────┘
       │                                           │
   Button + Pot                                RGB LED
   (Color + Brightness)                     (GPIO 4,2,15)
       │                                           │
       │  Heartbeat: "10_20&" (1 sec)            │
       │  ──────────────────────────────────────► │
       │                                           │
       │  ◄─────────────────────────────────────  │
       │          Response: "A"                    │
       │                                           │
       │  RGB Data: "0_255&" (R, full bright)    │
       │  ──────────────────────────────────────► │
       │                                           │
       │  ◄─────────────────────────────────────  │
       │          Response: "A"                    │
            192.168.4.1:6969
```

### Network Details
- **SSID**: "ESP32-Nihahaha"
- **Password**: "12345678"
- **Client IP**: 192.168.4.5 (static configuration)
- **Gateway**: 192.168.4.1 (server IP)
- **Subnet**: 255.255.255.0
- **Server IP**: 192.168.4.1 (Access Point)
- **Server Port**: 6969
- **Protocol**: UDP (WiFiUDP)
- **Client UDP Port**: 6969 (same as server)

## Special Notes

- **WiFi Mode**: Station (STA) mode - connects to server's Access Point
- **IP Configuration**: STATIC IP (192.168.4.5) - not DHCP
- **Protocol**: UDP (connectionless, bidirectional)
- **Data Format**: "color_brightness&" (e.g., "0_180&", "1_255&", "2_100&")
- **Heartbeat**: Sends "10_20&" every 1 second (dummy data)
- **Acknowledgment**: Server responds with "A" after receiving RGB data
- **Color Selection**: Button cycles: 0 (Red) → 1 (Green) → 2 (Blue) → 0...
- **Brightness**: Potentiometer mapped 0-4095 → 0-255
- **Timing**: Heartbeat overridden when server sends "A" acknowledgment
- Serial baud rate: 9600

## How It Works

1. **WiFi Connection**:
   - Configure static IP (192.168.4.5, gateway 192.168.4.1, subnet 255.255.255.0)
   - ESP32 connects to "ESP32-Nihahaha" AP
   - Waits until connected
   - Prints local IP (192.168.4.5)

2. **UDP Initialization**:
   - `udp.begin(port)` starts listening on port 6969
   - Client can now send and receive UDP packets

3. **Button Monitoring** (loop):
   - Read GPIO 4 button state
   - Detect button press (edge detection)
   - 20ms debounce delay
   - Increment `selRGB_BTN` counter (0→1→2→0)
   - Convert to string for transmission

4. **Potentiometer Reading**:
   - Read analog value from GPIO 34 (0-4095)
   - Map to PWM range: 0-4095 → 0-255
   - Convert to string

5. **Heartbeat Transmission**:
   - Every 1 second (if no server response)
   - Send dummy packet "10_20&"
   - Keeps connection active

6. **Server Acknowledgment**:
   - Check for incoming UDP packet
   - Read packet data
   - If packet contains "A"...
   - Send RGB data: `selRGB + "_" + brightness + "&"`
   - Reset 1-second timer

7. **Serial Output**:
   - Print current RGB selection (0, 1, or 2)

## Testing Instructions

1. **Setup Server First**:
   - Upload lab-w7-3-server.ino to another ESP32
   - Server creates AP "ESP32-Nihahaha"
   - Server connects RGB LED to GPIO 4 (R), 2 (G), 15 (B)

2. **Upload Client Sketch**:
   - Upload this sketch to ESP32 client

3. **Open Serial Monitor** (9600 baud):
   - Should see "Connecting to WiFi..."
   - When connected: IP "192.168.4.5"

4. **Test Color Selection**:
   - Press button (GPIO 4)
   - Serial Monitor shows: 0 (Red selected)
   - Press button again: 1 (Green selected)
   - Press button again: 2 (Blue selected)
   - Press button again: 0 (wraps around to Red)

5. **Test Brightness Control**:
   - Rotate potentiometer fully counter-clockwise
   - Brightness = 0 (LED off or very dim)
   - Rotate potentiometer fully clockwise
   - Brightness = 255 (LED full brightness)

6. **Observe RGB LED**:
   - Select Red (button press to 0)
   - Adjust potentiometer
   - Red LED brightness should change
   - Select Green (button press to 1)
   - Green LED brightness should change
   - Select Blue (button press to 2)
   - Blue LED brightness should change

7. **Verify Server Communication**:
   - Server Serial Monitor should show:
     ```
     0
     180
     ```
   - First line: color selection
   - Second line: brightness value

## Serial Monitor Output Examples

```
Connecting to WiFi...
Connecting to WiFi...
192.168.4.5
0
0
1
1
2
2
0
0
```

(Numbers represent RGB color selection: 0=R, 1=G, 2=B)

## Data Protocol

**Heartbeat Format** (Client → Server):
```
"10_20&"

Sent every 1 second to keep connection active
Dummy data, not processed by server for RGB control
```

**RGB Data Format** (Client → Server):
```
"color_brightness&"

Examples:
  "0_0&"    → Red channel, brightness 0 (off)
  "0_180&"  → Red channel, brightness 180
  "1_255&"  → Green channel, brightness 255 (max)
  "2_100&"  → Blue channel, brightness 100

Color codes:
  0 = Red (GPIO 4)
  1 = Green (GPIO 2)
  2 = Blue (GPIO 15)
  
Brightness: 0-255 (PWM duty cycle)
```

**Acknowledgment Format** (Server → Client):
```
"A"

Server sends acknowledgment after processing RGB data
Triggers client to send updated RGB values
```

**UDP Transaction**:
```
1. Every 1 second: Client sends "10_20&" (heartbeat)
2. Button pressed: selRGB_BTN increments
3. Pot adjusted: brightness value changes
4. Server responds: "A"
5. Client receives "A"
6. Client sends: "1_180&" (current color_brightness)
7. Server processes RGB data
8. Server sends: "A"
9. Timer resets, repeat from step 4
```

## State Machine

```
Color Selection State Machine:

  selRGB_BTN = 0 (RED)
       │
       │ Button Press
       ▼
  selRGB_BTN = 1 (GREEN)
       │
       │ Button Press
       ▼
  selRGB_BTN = 2 (BLUE)
       │
       │ Button Press
       ▼
  selRGB_BTN = 0 (RED)  [wraps around]
       │
      ...
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Won't connect to WiFi | Server not running | Start server first (lab-w7-3-server) |
| Wrong IP address | Static IP not set | Verify WiFi.config() before WiFi.begin() |
| Button not responding | Wrong GPIO pin | Check GPIO 4 connection |
| Multiple color changes | Bouncing | Increase debounce delay (20ms→50ms) |
| Pot not responding | Wrong GPIO | GPIO 34 is input-only, check connection |
| Erratic pot readings | Floating pin | Check pot wiring to 3.3V and GND |
| RGB LED not changing | UDP packet lost | Check server running and responsive |
| No brightness control | ADC not reading | Verify GPIO 34 ADC connection |
| WiFi disconnects | Weak signal | Move ESP32s closer together |
| Wrong color displayed | Color index wrong | Verify selRGB_BTN: 0=R, 1=G, 2=B |

## Code Reference

```cpp
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
  // Button handling
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

  // Potentiometer reading
  int a = analogRead(34);
  a = map(a, 0, 4095, 0, 255);
  String ab = String(a);

  // Heartbeat (every 1 second)
  if(millis()-time_out >= 1000){
    time_out = millis();
    send_data("10_20&");
  }

  // Receive acknowledgment
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
```

## Network Architecture

This lab demonstrates **bidirectional UDP communication with acknowledgments**:
- **Server**: Creates WiFi Access Point, controls RGB LED
- **Client**: Sends color/brightness data, receives acknowledgments
- **Protocol**: UDP with custom acknowledgment mechanism
- **Static IP**: Client uses fixed IP for predictable routing
- **Heartbeat**: Keeps connection active, server knows client is alive
- **Responsive**: Server acknowledges each RGB update

## Applications

- Remote RGB lighting control
- Color-adjustable mood lighting
- Stage lighting systems
- Smart home RGB bulbs
- Visual notifications
- Ambient lighting control
- Gaming peripherals
- Art installations

## Enhancements

- Add OLED to display current color/brightness
- Implement smooth color transitions
- Add preset color patterns
- Store favorite colors in EEPROM
- Implement color mixing (multiple channels)
- Add fade effects
- Implement auto-cycle mode
- Add multiple RGB LEDs
- Web interface for color picker
- Implement HSV color space
- Add strobe/pulse effects
