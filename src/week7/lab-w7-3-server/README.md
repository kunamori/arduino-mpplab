# Lab W7-3-SERVER: ESP32 WiFi UDP RGB LED Control Server

**File**: `lab-w7-3-server.ino`

## Description

This lab demonstrates ESP32 WiFi UDP server for RGB LED control. The ESP32 creates a WiFi Access Point and listens for UDP packets containing color and brightness data. The RGB LED displays the selected color (Red, Green, or Blue) at the specified brightness (0-255). The server acknowledges each RGB update. This is the **server** side that controls a physical RGB LED based on client input (lab-w7-3-client).

## Components Required

- ESP32 DEVKIT × 1
- RGB LED (common cathode) × 1
- 220Ω resistor × 1 (shared for all colors)
- Breadboard
- Jumper wires

## Pin Configuration

| ESP32 Pin | Component | Connection                           | Notes           |
| --------- | --------- | ------------------------------------ | --------------- |
| GPIO 4    | RGB LED   | Red anode                            | PWM capable     |
| GPIO 2    | RGB LED   | Green anode                          | PWM capable     |
| GPIO 15   | RGB LED   | Blue anode                           | PWM capable     |
| GND       | RGB LED   | Common cathode → 220Ω resistor → GND | Shared resistor |

## ASCII Wiring Diagram

```
        ESP32 DEVKIT         RGB LED (Common Cathode)
      ┌─────────────┐
      │             │              ┌───────┐
      │   GPIO 4 ●──┼──────────────┤ R (+) │
      │             │              │       │
      │   GPIO 2 ●──┼──────────────┤ G (+) │
      │             │              │ RGB   │
      │  GPIO 15 ●──┼──────────────┤ B (+) │
      │             │              │       │
      │             │              │ COM(-)│───┐
      │             │              └───────┘   │
      │   GND    ●──┼──────────────────────────┴── [220Ω] ── GND
      └─────────────┘                              (Single resistor)

      WiFi: Creates AP "ESP32-Nihahaha"
      UDP Server: Listens on port 6969
      IP Address: 192.168.4.1
      Server responds to: 192.168.4.5

      Receives: "color_brightness&"
      Examples: "0_255&" (Red, full brightness)
                "1_128&" (Green, half brightness)
                "2_0&"   (Blue, off)
      Sends: "A" (acknowledgment)

      Color Codes:
        0 = Red (GPIO 4)
        1 = Green (GPIO 2)
        2 = Blue (GPIO 15)
```

## Breadboard Layout

1. Place ESP32 on breadboard
2. Place RGB LED on breadboard (identify pins)
3. Connect RGB LED anodes (long legs):
   - Red anode → GPIO 4
   - Green anode → GPIO 2
   - Blue anode → GPIO 15
4. Connect RGB LED common cathode (longest leg or marked):
   - Common cathode → 220Ω resistor → GND rail
5. **Note**: Single 220Ω resistor to common ground (new requirement)

## Component-Specific Details

### ESP32 DEVKIT

- **WiFi**: 802.11 b/g/n
- **Operating Voltage**: 3.3V logic, 5V power
- **PWM**: LEDC (LED Control) peripheral for brightness control
- **WiFi Mode**: Access Point (AP) mode
- **GPIO 4, 2, 15**: All PWM capable

### RGB LED (Common Cathode)

- **Type**: 4-pin RGB LED with common cathode
- **Pin Identification**:
  - **Longest pin**: Common cathode (-)
  - Three shorter pins: R, G, B anodes (+)
  - Typically: R, Common(-), G, B (but verify with datasheet)
- **Forward Voltage**:
  - Red: ~2.0V
  - Green: ~3.0V
  - Blue: ~3.0V
- **Forward Current**: 20mA per color (typ)
- **Common Cathode**: All cathodes connected internally
- **PWM Control**: Brightness via duty cycle (0-255)

### Resistor Configuration

- **Value**: 220Ω (red-red-brown)
- **Configuration**: Single resistor on common cathode to GND
- **Purpose**: Current limiting for all three colors
- **Power Rating**: 1/4W sufficient

## Network Configuration

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│(lab-w7-3-client) │   "ESP32-Nihahaha"    │  (This Device)   │
│  192.168.4.5     │                       │  192.168.4.1     │
└──────────────────┘                       └──────────────────┘
       │                                           │
   Button + Pot                                    │
   (Color + Brightness)                     RGB LED (GPIO 4,2,15)
       │                                           │
       │  RGB Data: "1_180&"                      │
       └─────────────────────────────────────────►│
       ◄──────────────────────────────────────────┘
                Response: "A"
            192.168.4.1:6969 ◄──► 192.168.4.5:6969
```

### Network Details

- **SSID**: "ESP32-Nihahaha"
- **Password**: "12345678"
- **Server IP**: 192.168.4.1 (Access Point IP)
- **Server Port**: 6969
- **Protocol**: UDP (WiFiUDP)
- **WiFi Mode**: AP+STA (Access Point + Station)
- **Client IP**: 192.168.4.5 (static on client)
- **Response Target**: Hardcoded to 192.168.4.5

## Special Notes

- **WiFi Mode**: Access Point (AP) mode - creates its own WiFi network
- **Protocol**: UDP (connectionless, bidirectional)
- **Data Format**: Receives "color_brightness&" (e.g., "0_180&", "1_255&", "2_100&")
- **Parsing**: Splits string by "\_" and "&" delimiters
- **Color Codes**: 0=Red, 1=Green, 2=Blue
- **Brightness**: 0-255 (PWM duty cycle)
- **Acknowledgment**: Sends "A" back to 192.168.4.5 after processing
- **State Tracking**: `led_data[3]` array stores R, G, B brightness
- **Shared Resistor**: Single 220Ω resistor on common cathode
- **Hardcoded Client**: Responds only to 192.168.4.5
- Serial baud rate: 9600

## How It Works

1. **WiFi Setup**:
   - ESP32 creates Access Point "ESP32-Nihahaha"
   - Sets mode to WIFI_AP_STA
   - Obtains IP address 192.168.4.1
   - Prints "AP IP: 192.168.4.1"

2. **UDP Initialization**:
   - `udp.begin(port)` starts listening on port 6969
   - Server ready to receive UDP packets

3. **RGB LED Initialization**:
   - GPIO 4, 2, 15 configured for PWM
   - `led_data[]` array initialized to {0, 0, 0} (all off)

4. **Packet Reception** (loop):
   - Parse incoming UDP packet
   - Read packet data into buffer
   - Convert to string

5. **Data Parsing**:
   - `splitString()` function processes input
   - Splits by "\_" and "&" delimiters
   - Extracts two values:
     - `data[0]`: Color selection (0, 1, or 2)
     - `data[1]`: Brightness (0-255)

6. **RGB Update**:
   - Update array: `led_data[data[0]] = data[1]`
   - Write PWM values to all three colors:
     - `analogWrite(GPIO_4, led_data[0])` - Red
     - `analogWrite(GPIO_2, led_data[1])` - Green
     - `analogWrite(GPIO_15, led_data[2])` - Blue
   - 50ms delay for stability

7. **Acknowledgment**:
   - Begin UDP packet to 192.168.4.5:6969
   - Send "A"
   - End packet

8. **Serial Output**:
   - Print color index (data[0])
   - Print brightness value (data[1])

## Testing Instructions

1. **Upload Server Sketch**:
   - Upload this sketch to ESP32 server
   - Open Serial Monitor (9600 baud)

2. **Verify Access Point**:
   - Check Serial Monitor for "AP IP: 192.168.4.1"
   - On your phone/computer, you should see "ESP32-Nihahaha" WiFi network

3. **Check Initial RGB State**:
   - RGB LED should be off (all colors at 0)

4. **Setup Client**:
   - Upload lab-w7-3-client.ino to another ESP32
   - Connect button to GPIO 4
   - Connect potentiometer to GPIO 34
   - Client should connect to "ESP32-Nihahaha" AP

5. **Test Red Channel**:
   - Client button cycles to 0 (Red)
   - Adjust client potentiometer
   - Server Serial Monitor shows:
     ```
     0
     180
     ```
   - RGB LED red component changes brightness

6. **Test Green Channel**:
   - Client button cycles to 1 (Green)
   - Adjust potentiometer
   - Server shows:
     ```
     1
     200
     ```
   - RGB LED green component changes brightness

7. **Test Blue Channel**:
   - Client button cycles to 2 (Blue)
   - Adjust potentiometer
   - Server shows:
     ```
     2
     150
     ```
   - RGB LED blue component changes brightness

8. **Test Color Mixing**:
   - Set Red to 255 (max)
   - Set Green to 255 (max)
   - Result: Yellow (R+G)
   - Set Blue to 255 (max)
   - Result: White (R+G+B)

## Serial Monitor Output Examples

```
AP IP: 192.168.4.1
0
180
0
200
0
255
1
100
1
150
2
50
2
255
```

(First line: color index, Second line: brightness)

## Data Protocol

**Request Format** (Client → Server):

```
"color_brightness&"

Examples:
  "0_255&"  → Red channel, brightness 255 (max)
  "1_128&"  → Green channel, brightness 128 (half)
  "2_0&"    → Blue channel, brightness 0 (off)

Color codes:
  0 = Red (GPIO 4)
  1 = Green (GPIO 2)
  2 = Blue (GPIO 15)

Brightness: 0-255 (PWM duty cycle)
  0 = Off
  128 = Half brightness
  255 = Full brightness
```

**Response Format** (Server → Client):

```
"A"

Single character acknowledgment
Sent to 192.168.4.5:6969
```

**UDP Transaction**:

```
1. Client sends UDP "1_180&" to 192.168.4.1:6969
2. Server receives packet
3. Server parses: data[0]=1 (Green), data[1]=180
4. Server updates: led_data[1] = 180
5. Server writes PWM:
   - analogWrite(4, led_data[0])   // Red
   - analogWrite(2, led_data[1])   // Green (180)
   - analogWrite(15, led_data[2])  // Blue
6. Server sends UDP "A" to 192.168.4.5:6969
7. Client receives "A"
8. Client sends next update
```

## RGB Color Mixing

```
Primary Colors (single channel):
  Red:   (255, 0,   0)
  Green: (0,   255, 0)
  Blue:  (0,   0,   255)

Secondary Colors (two channels):
  Yellow:  (255, 255, 0)   → Red + Green
  Cyan:    (0,   255, 255) → Green + Blue
  Magenta: (255, 0,   255) → Red + Blue

White (all channels):
  White: (255, 255, 255)

Off:
  Black: (0, 0, 0)

Examples with this lab:
1. Set R=255, G=0, B=0   → Red
2. Set R=255, G=255, B=0 → Yellow
3. Set R=255, G=255, B=255 → White
```

## Troubleshooting

| Issue                     | Possible Cause      | Solution                                     |
| ------------------------- | ------------------- | -------------------------------------------- |
| RGB LED not lighting      | Wrong polarity      | Verify common cathode to GND, anodes to GPIO |
| Only one color works      | Wrong pin mapping   | Check R→4, G→2, B→15                         |
| Wrong colors displayed    | RGB pins swapped    | Verify RGB LED pinout with datasheet         |
| Dim RGB LED               | Resistor too high   | Verify 220Ω, not 2.2kΩ                       |
| RGB LED too bright        | No resistor         | Add 220Ω resistor to common cathode          |
| Colors don't mix          | Wrong LED type      | Must be common cathode, not common anode     |
| No WiFi AP visible        | Code not running    | Check Serial Monitor, verify upload          |
| Client no response        | Wrong client IP     | Server hardcoded to 192.168.4.5              |
| No acknowledgment sent    | UDP response failed | Check client IP is 192.168.4.5               |
| Serial shows wrong values | Parsing error       | Check string format "X_Y&"                   |

## Code Reference

```cpp
#include <WiFi.h>
#include <WiFiUdp.h>

#define LED_RED_PIN 4
#define LED_GREEN_PIN 2
#define LED_BLUE_PIN 15

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.5";
WiFiUDP udp;

char packetBuffer[255];

int data[2] = {0};
int led_data[3] = {0,0,0};

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  udp.begin(port);
}

void loop() {
  int numbuffer = udp.parsePacket();
  if (numbuffer > 0) {
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String s(packetBuffer);

      splitString(s);
      led_data[data[0]] = data[1];
      analogWrite(LED_RED_PIN, led_data[0]);
      analogWrite(LED_GREEN_PIN, led_data[1]);
      analogWrite(LED_BLUE_PIN, led_data[2]);
      delay(50);

      Serial.println(data[0]);
      Serial.println(data[1]);
    }

    udp.beginPacket(host, port);
    udp.print("A");
    udp.flush();
    udp.endPacket();
  }
}

void splitString(String str) {
  int index = 0;
  String s = "";
  for (int i = 0; i < str.length(); i++) {
    if (str[i] != '_' && str[i] != '&') {
      s += str[i];
    } else {
      data[index] = s.toInt();
      index++;
      s = "";
    }
  }
  data[index] = s.toInt();
}
```

## PWM Brightness Control

```
ESP32 PWM (LEDC):

analogWrite(pin, value);
  - value: 0-255
  - 0 = 0% duty cycle (OFF)
  - 128 = 50% duty cycle (half brightness)
  - 255 = 100% duty cycle (full brightness)

PWM Waveform (50% duty cycle):
     ┌────┐    ┌────┐    ┌────┐
     │    │    │    │    │    │
  ───┘    └────┘    └────┘    └────

  ON: 50% of time
  OFF: 50% of time
  Average current: 50% of max
  Brightness: 50%

LED perceives average light intensity
```

## RGB LED Types

⚠️ **Important**: This lab uses COMMON CATHODE RGB LED

**Common Cathode** (this lab):

- Common pin: Cathode (-) to GND
- R, G, B pins: Anodes (+) to GPIO
- GPIO HIGH = LED ON
- GPIO LOW = LED OFF
- PWM HIGH % = Brightness

**Common Anode** (NOT this lab):

- Common pin: Anode (+) to VCC
- R, G, B pins: Cathodes (-) to GPIO
- GPIO LOW = LED ON
- GPIO HIGH = LED OFF
- PWM LOW % = Brightness
- Requires code change (invert PWM)

## Circuit Calculation

```
Single 220Ω Resistor on Common Cathode:

Assume Red LED at full brightness (255):
  ESP32 3.3V → Red LED (2V drop) → 220Ω → GND

  Voltage across resistor: 3.3V - 2V = 1.3V
  Current: I = V/R = 1.3V / 220Ω ≈ 6mA

  ✓ Safe: 6mA < 20mA max LED current
  ✓ GPIO safe: 6mA < 40mA max per pin

If multiple colors ON simultaneously:
  Each color has its own GPIO pin
  Each color current limited by shared resistor
  Total GND current = sum of active color currents
  Max (all 3 colors full): ~18mA total
  ✓ Safe for ESP32 GND pin
```

## Network Architecture

This lab demonstrates **bidirectional UDP RGB control**:

- **Server**: Creates WiFi Access Point, controls RGB LED via PWM
- **Client**: Sends color/brightness commands, receives acknowledgments
- **Protocol**: UDP with custom acknowledgment
- **Hardcoded Response**: Server always responds to 192.168.4.5
- **Real-time**: Low-latency color/brightness updates
- **State Persistence**: RGB LED maintains last color until changed

## Applications

- RGB mood lighting
- Smart LED bulbs
- Color-changing decorations
- Visual notifications (status indicators)
- Stage/event lighting
- Gaming ambient lighting
- Music visualization
- Art installations
- Wearable LED accessories
- Color therapy devices

## Enhancements

- Support multiple clients (different IPs)
- Add smooth color transitions (fade)
- Implement HSV color space
- Add color presets/scenes
- Store favorite colors in EEPROM
- Add strobe/pulse effects
- Implement rainbow cycle
- Add web interface
- Support multiple RGB LEDs
- Add addressable LED strips (WS2812B)
- Implement music sync
- Add scheduling (timed color changes)
