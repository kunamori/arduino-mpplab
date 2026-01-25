# Lab W7-1-SERVER: ESP32 WiFi UDP 6-LED Control Server

**File**: `lab-w7-1-server.ino`

## Description

This lab demonstrates ESP32 WiFi UDP server for controlling 6 LEDs. The ESP32 creates a WiFi Access Point and listens for UDP packets containing LED numbers (1-6). Each packet toggles the corresponding LED on/off. Unlike TCP, UDP is connectionless and ideal for real-time control. This is the **server** side that controls 6 physical LEDs.

## Components Required

- ESP32 DEVKIT × 1
- LEDs × 6 (any color)
- 220Ω resistors × 1 (shared for all LEDs)
- Breadboard
- Jumper wires

## Pin Configuration

| ESP32 Pin | Component           | Connection                           | Notes           |
| --------- | ------------------- | ------------------------------------ | --------------- |
| GPIO 13   | LED 1               | Anode (+)                            | Red/Color 1     |
| GPIO 12   | LED 2               | Anode (+)                            | Green/Color 2   |
| GPIO 14   | LED 3               | Anode (+)                            | Blue/Color 3    |
| GPIO 27   | LED 4               | Anode (+)                            | Yellow/Color 4  |
| GPIO 26   | LED 5               | Anode (+)                            | White/Color 5   |
| GPIO 25   | LED 6               | Anode (+)                            | Any color 6     |
| GND       | All LEDs (cathodes) | Common cathode → 220Ω resistor → GND | Shared resistor |

## ASCII Wiring Diagram

```
        ESP32 DEVKIT              LEDs (×6)
      ┌─────────────┐
      │             │       LED1  LED2  LED3  LED4  LED5  LED6
      │  GPIO 13 ●──┼────────┤►├───┐
      │             │               │
      │  GPIO 12 ●──┼────────┤►├───┤
      │             │               │
      │  GPIO 14 ●──┼────────┤►├───┤
      │             │               │
      │  GPIO 27 ●──┼────────┤►├───┤
      │             │               │
      │  GPIO 26 ●──┼────────┤►├───┤
      │             │               │
      │  GPIO 25 ●──┼────────┤►├───┤
      │             │               │
      │             │               │
      │   GND    ●──┼───────────────┴─── [220Ω] ─── GND
      └─────────────┘                    (Single shared resistor)

      WiFi: Creates AP "ESP32-Nihahaha"
      UDP Server: Listens on port 6969
      IP Address: 192.168.4.1

      Receives: "1" to "6" (toggles corresponding LED)
```

## Breadboard Layout

1. Place ESP32 on breadboard
2. Place 6 LEDs on breadboard
3. Connect each LED anode (long leg, +) to corresponding GPIO:
   - LED 1 anode → GPIO 13
   - LED 2 anode → GPIO 12
   - LED 3 anode → GPIO 14
   - LED 4 anode → GPIO 27
   - LED 5 anode → GPIO 26
   - LED 6 anode → GPIO 25
4. Connect all LED cathodes (short leg, -) together
5. Connect common cathode → 220Ω resistor → GND rail
6. **Note**: Single 220Ω resistor to common ground (new requirement)

## Component-Specific Details

### ESP32 DEVKIT

- **WiFi**: 802.11 b/g/n
- **Operating Voltage**: 3.3V logic, 5V power
- **GPIO Output**: 3.3V (sufficient for LED with resistor)
- **WiFi Mode**: Access Point (AP) mode
- **Current per GPIO**: Max 40mA (safe for single LED)

### LEDs

- **Type**: Standard 5mm or 3mm LEDs
- **Forward Voltage**: ~2V (red) to ~3.3V (blue/white)
- **Forward Current**: 20mA (typical)
- **Resistor**: Single 220Ω to common ground
- **Polarity**:
  - **Anode (+)**: Long leg, connects to GPIO
  - **Cathode (-)**: Short leg, connects to GND via resistor

### Resistor Configuration

- **Value**: 220Ω (red-red-brown)
- **Configuration**: Single resistor on common cathode to GND
- **Power Rating**: 1/4W sufficient
- **Purpose**: Current limiting for all LEDs

## Network Configuration

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│(lab-w7-1-client) │   "ESP32-Nihahaha"    │  (This Device)   │
│  192.168.4.x     │                       │  192.168.4.1     │
└──────────────────┘                       └──────────────────┘
       │                                           │
   Serial Input                                    │
   (1-6)                                          │
       │  UDP Packet                              │
       └─────────────────────────────────────────►│
            192.168.4.1:6969                   Port 6969
                                                   │
                                            ┌──────▼──────────┐
                                            │ LED 1 (GPIO 13) │
                                            │ LED 2 (GPIO 12) │
                                            │ LED 3 (GPIO 14) │
                                            │ LED 4 (GPIO 27) │
                                            │ LED 5 (GPIO 26) │
                                            │ LED 6 (GPIO 25) │
                                            └─────────────────┘
```

### Network Details

- **SSID**: "ESP32-Nihahaha"
- **Password**: "12345678"
- **Server IP**: 192.168.4.1 (Access Point IP)
- **Server Port**: 6969
- **Protocol**: UDP (WiFiUDP)
- **WiFi Mode**: AP+STA (Access Point + Station)
- **Client IP**: Assigned by DHCP (192.168.4.x)

## Special Notes

- **WiFi Mode**: Access Point (AP) mode - creates its own WiFi network
- **Protocol**: UDP (connectionless, no handshake)
- **Data Format**: Receives LED number as string (e.g., "1", "2", "6")
- **Toggle Behavior**: Each command toggles LED state (ON↔OFF)
- **State Tracking**: `ledStatus[]` array tracks each LED state
- **Input Validation**: Only accepts 1-6; other values show error
- **Shared Resistor**: Single 220Ω resistor on common cathode
- **Initial State**: All LEDs OFF on startup
- Serial baud rate: 9600

## How It Works

1. **WiFi Setup**:
   - ESP32 creates Access Point "ESP32-Nihahaha"
   - Sets mode to WIFI_AP_STA
   - Obtains IP address 192.168.4.1
   - Prints AP IP to Serial Monitor

2. **UDP Initialization**:
   - `udp.begin(port)` starts listening on port 6969
   - Server ready to receive UDP packets

3. **LED Initialization**:
   - Set all 6 GPIO pins as OUTPUT
   - Initialize all LEDs to LOW (OFF)
   - `ledStatus[]` array all zeros

4. **Packet Reception** (loop):
   - Parse incoming UDP packet
   - Read packet data into buffer
   - Convert to string and integer

5. **Command Processing**:
   - Validate LED number (1-6)
   - Get array index (LED number - 1)
   - Check current LED state
   - Toggle state:
     - If OFF (0): Turn ON, set state to 1
     - If ON (1): Turn OFF, set state to 0
   - Print status to Serial Monitor

6. **Error Handling**:
   - If input < 1 or > 6: Print "ERROR: INPUT_USER"
   - No LED state change

## Testing Instructions

1. **Upload Server Sketch**:
   - Upload this sketch to ESP32 server
   - Open Serial Monitor (9600 baud)

2. **Verify Access Point**:
   - Check Serial Monitor for "AP IP: 192.168.4.1"
   - On your phone/computer, you should see "ESP32-Nihahaha" WiFi network

3. **Check Initial LED State**:
   - All 6 LEDs should be OFF

4. **Setup Client**:
   - Upload lab-w7-1-client.ino to another ESP32
   - Client should connect to "ESP32-Nihahaha" AP
   - Open client's Serial Monitor

5. **Test LED 1**:
   - On client Serial Monitor, type `1` and press Enter
   - Server Serial Monitor shows: "LED 1 ON"
   - LED 1 should light up
   - Type `1` again
   - Server shows: "LED 1 OFF"
   - LED 1 should turn off

6. **Test All LEDs**:
   - Type 2, 3, 4, 5, 6 sequentially
   - Each corresponding LED should toggle
   - Verify independent control

7. **Test Invalid Input**:
   - Type `0` or `7`
   - Server shows: "ERROR: INPUT_USER"
   - No LED changes state

8. **Test Multiple LEDs On**:
   - Turn on LEDs 1, 3, 5
   - All three should be lit
   - Others should be off
   - Toggle any LED independently

## Serial Monitor Output Examples

```
AP IP: 192.168.4.1
LED 1 ON
LED 2 ON
LED 1 OFF
LED 3 ON
LED 4 ON
LED 5 ON
LED 6 ON
LED 6 OFF
ERROR: INPUT_USER
LED 2 OFF
```

## Data Protocol

**Message Format**:

```
UDP Packet:
  LED Number (string, 1 character)

Examples:
  "1"  → Toggle LED 1
  "2"  → Toggle LED 2
  "6"  → Toggle LED 6
  "0"  → Error (invalid)
  "7"  → Error (invalid)
```

**UDP Server Flow**:

```
1. Server listens on port 6969
2. Client sends UDP packet "1"
3. Server receives packet
4. Parse packet: selLED = 1
5. Validate: 1 >= 1 && 1 <= 6 ✓
6. Get index: 1 - 1 = 0
7. Check ledStatus[0] = 0 (OFF)
8. Turn ON: digitalWrite(13, HIGH)
9. Update: ledStatus[0] = 1
10. Print: "LED 1 ON"
11. Server continues listening
```

## LED State Machine

```
LED State Toggle Logic:

  Initial: ledStatus[i] = 0 (OFF)
            GPIO = LOW
       │
       │ Receive command "i+1"
       ▼
  Check: ledStatus[i] == 0?
       │
       ├─ Yes (OFF) ──► digitalWrite(HIGH)
       │                ledStatus[i] = 1
       │                Print "LED X ON"
       │
       └─ No (ON) ───► digitalWrite(LOW)
                        ledStatus[i] = 0
                        Print "LED X OFF"
```

## Troubleshooting

| Issue               | Possible Cause    | Solution                             |
| ------------------- | ----------------- | ------------------------------------ |
| All LEDs always on  | Wiring reversed   | Check anode to GPIO, cathode to GND  |
| LED very dim        | Resistor too high | Verify 220Ω, not 2.2kΩ               |
| LED doesn't light   | Blown LED         | Test LED with 3.3V/GND directly      |
| LEDs too bright     | No resistor       | Add 220Ω resistor to common ground   |
| Wrong LED toggles   | GPIO mismatch     | Verify pin assignments               |
| Random LED behavior | Floating pins     | Check all connections secure         |
| No WiFi AP visible  | Code not running  | Check Serial Monitor, verify upload  |
| UDP not received    | Client wrong IP   | Verify client sends to 192.168.4.1   |
| No serial output    | Wrong baud rate   | Set Serial Monitor to 9600 baud      |
| ESP32 resets        | Too much current  | Check wiring, ensure shared resistor |

## Code Reference

```cpp
#include <WiFi.h>
#include <WiFiUdp.h>

#define LED_1 13
#define LED_2 12
#define LED_3 14
#define LED_4 27
#define LED_5 26
#define LED_6 25

int NUMBER_LED = 6;
int ledStatus[] = {0,0,0,0,0,0};
int ledPins[] = { LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};
int selLED;

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char packetBuffer[255];
WiFiUDP udp;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUMBER_LED; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  udp.begin(port);
}

void loop() {
  int numBytes = udp.parsePacket();

  if (numBytes > 0) {
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String input(packetBuffer);
      selLED = input.toInt();

      if (selLED >= 1 && selLED <= NUMBER_LED) {
        int index = selLED - 1;

        if (ledStatus[index] == 0) {
          digitalWrite(ledPins[index], HIGH);
          ledStatus[index] = 1;
          Serial.println("LED " + input + " ON");
        } else {
          digitalWrite(ledPins[index], LOW);
          ledStatus[index] = 0;
          Serial.println("LED " + input + " OFF");
        }
      }else {
        Serial.println("ERROR: INPUT_USER");
      }
    }
  }
}
```

## LED Circuit Calculation

```
Single 220Ω Resistor Configuration:

Worst Case (All 6 LEDs ON):
  ESP32 3.3V → LED (2V drop) → 220Ω → GND

  Voltage across resistor: 3.3V - 2V = 1.3V
  Current per LED: I = V/R = 1.3V / 220Ω ≈ 6mA

  Total current (6 LEDs): 6 × 6mA = 36mA

  ✓ Safe: ESP32 can source 40mA per pin
  ✓ LED safe: 6mA < 20mA max

Note: Each LED gets less current, so brightness reduced
      compared to individual resistors. Acceptable for indicators.
```

## Applications

- Wireless LED indicators
- Status display boards
- Remote lighting control
- IoT visual feedback
- Smart home displays
- Notification systems
- Equipment status panels
- Security system indicators

## Enhancements

- Add PWM for LED brightness control
- Implement LED patterns (blink, chase, fade)
- Add RGB LEDs (more colors)
- Implement "all on" / "all off" commands
- Add LED groups (control multiple LEDs)
- Store LED states in EEPROM
- Add web interface for control
- Implement LED animation sequences
- Add LED status feedback to client
- Support binary commands (multiple LEDs in one packet)
