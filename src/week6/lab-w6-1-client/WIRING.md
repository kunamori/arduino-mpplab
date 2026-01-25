# Lab W6-1-CLIENT: ESP32 WiFi TCP Counter Client

**File**: `lab-w6-1-client.ino`

## Description
This lab demonstrates ESP32 WiFi TCP client functionality. Two buttons control a counter (0-99) that sends values to a TCP server over WiFi. This is the **client** side that connects to the server (lab-w6-1-server).

## Components Required
- ESP32 DEVKIT × 1
- Push buttons × 2
- Breadboard
- Jumper wires

## Pin Configuration

| ESP32 Pin | Component | Connection | Notes |
|-----------|-----------|------------|-------|
| GPIO 4 | Button 2 | Decrement button | INPUT_PULLUP |
| GPIO 5 | Button 1 | Increment button | INPUT_PULLUP |
| GND | Both buttons | Common ground | Shared GND |

## ASCII Wiring Diagram

```
        ESP32 DEVKIT           Buttons
      ┌─────────────┐       
      │             │       ┌───┐
      │   GPIO 5 ●──┼───────┤ ● │  Button 1 (Increment)
      │             │       └─┬─┘
      │             │         │
      │   GPIO 4 ●──┼─────┐ ┌─┴─┐
      │             │     │ │ ● │  Button 2 (Decrement)
      │             │     │ └─┬─┘
      │   GND    ●──┼─────┴───┴──── GND
      └─────────────┘       

      WiFi: Connects to "ESP32-Nihahaha" (server's AP)
      TCP: Connects to 192.168.4.1:6969
```

## Breadboard Layout

1. Place ESP32 on breadboard
2. Connect Button 1 (increment) between GPIO 5 and GND
3. Connect Button 2 (decrement) between GPIO 4 and GND
4. Use INPUT_PULLUP mode (no external resistors needed)

## Component-Specific Details

### ESP32 DEVKIT
- **WiFi**: 802.11 b/g/n
- **Operating Voltage**: 3.3V logic (5V tolerant on some pins)
- **GPIO**: Multiple GPIO pins available
- **Built-in**: WiFi, Bluetooth, pull-up/pull-down resistors

### Push Buttons
- **Configuration**: INPUT_PULLUP mode (internal pull-ups enabled)
- **Active State**: LOW (0) when pressed
- **Debouncing**: 20ms software delay

## Network Configuration

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│  (This Device)   │   "ESP32-Nihahaha"    │ (lab-w6-1-server)│
│  192.168.4.x     │                       │  192.168.4.1     │
└──────────────────┘                       └──────────────────┘
       │                                           │
       └─── TCP Connection ──────────────────────►│
            192.168.4.1:6969                   Port 6969
```

### Network Details
- **SSID**: "ESP32-Nihahaha"
- **Password**: "12345678"
- **Server IP**: 192.168.4.1 (Access Point)
- **Server Port**: 6969
- **Protocol**: TCP (WiFiClient)
- **Client IP**: Assigned by DHCP (192.168.4.x)

## Special Notes

- **WiFi Mode**: Station (STA) mode - connects to server's Access Point
- **Connection**: Connects to server only when sending data (not persistent)
- **Data Format**: Sends counter value followed by "&" (e.g., "42&")
- **Counter Range**: 0-99 with wrap-around
- **Debouncing**: 20ms delay prevents multiple counts per press
- **Connection Management**: Connects, sends data, disconnects for each button press
- Serial baud rate: 9600

## How It Works

1. **WiFi Connection**:
   - ESP32 connects to "ESP32-Nihahaha" AP
   - Waits until connected
   - Obtains IP address via DHCP

2. **Button Monitoring**:
   - **Button 1 (GPIO 5)**: Increments counter
     - Counter wraps from 99 to 0
   - **Button 2 (GPIO 4)**: Decrements counter
     - Counter wraps from 0 to 99

3. **Button Press Handling**:
   - Detect button state change (edge detection)
   - 20ms debounce delay
   - Update counter value
   - Connect to TCP server
   - Send counter value + "&"
   - Flush and disconnect

4. **TCP Communication**:
   - Create TCP connection to 192.168.4.1:6969
   - Send string (e.g., "42&")
   - Close connection
   - Server receives and displays on LCD

5. **Serial Output**:
   - Displays current counter value for debugging

## Testing Instructions

1. **Setup Server First**:
   - Upload lab-w6-1-server.ino to another ESP32
   - Server creates AP "ESP32-Nihahaha"
   - Server displays IP on LCD

2. **Upload Client Sketch**:
   - Upload this sketch to ESP32 client

3. **Open Serial Monitor** (9600 baud):
   - Should see "Connecting to WiFi..."
   - When connected, WiFi status displayed

4. **Test Increment (GPIO 5)**:
   - Press Button 1
   - Counter increases: 0→1→2...→99→0
   - Serial Monitor shows counter value
   - Server LCD should update

5. **Test Decrement (GPIO 4)**:
   - Press Button 2
   - Counter decreases: 99→98...→1→0→99
   - Server LCD should update

6. **Verify Server Display**:
   - Check server's LCD shows same counter value
   - Each button press updates server display

## Serial Monitor Output Examples

```
Connecting to WiFi...
Connecting to WiFi...
WiFi connected
0
1
2
3
99
0
99
98
```

## Data Protocol

**Message Format**:
```
Counter Value + "&"

Examples:
  "0&"    → Counter = 0
  "42&"   → Counter = 42
  "99&"   → Counter = 99
```

**TCP Transaction**:
```
1. Button pressed
2. client.connect(192.168.4.1, 6969)
3. client.print("42&")
4. client.flush()
5. client.stop()
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Won't connect to WiFi | Server not running | Start server first (lab-w6-1-server) |
| "Connecting to WiFi..." loop | Wrong SSID/password | Verify credentials match server |
| Button not responding | Wrong GPIO pin | Check GPIO 4 and 5 connections |
| Multiple counts per press | Bouncing | Increase debounce delay (20ms→50ms) |
| Server not updating | TCP connection failed | Check server is running and listening |
| No serial output | Wrong baud rate | Set Serial Monitor to 9600 baud |
| WiFi disconnects | Weak signal | Move ESP32s closer together |
| Wrong IP address | Server IP changed | Verify server IP is 192.168.4.1 |

## Code Reference

```cpp
#include <WiFi.h>

bool state1=1, lastState1=1;
bool state2=1, lastState2=1;
int x=0;

// WiFi credentials
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
  // Button 1 (GPIO 5) - Increment
  state1=digitalRead(5);
  if(state1!=lastState1) {
    delay(20);  // Debounce
    if(digitalRead(5)==0) {
      x++;
      if(x>99) x=0;
      if (client.connect(host, port)) {
        client.print(String(x)+"&");
        client.flush();
        client.stop();
      }
    }
  }
  lastState1=state1;
  
  // Button 2 (GPIO 4) - Decrement
  state2=digitalRead(4);
  if(state2!=lastState2) {
    delay(20);  // Debounce
    if(digitalRead(4)==0) {
      x--;
      if(x<0) x=99;
      if (client.connect(host, port)) {
        client.print(String(x)+"&");
        client.flush();
        client.stop();
      }
    }
  }
  lastState2=state2;
  
  Serial.println(x);
}
```

## WiFi Connection States

```
WiFi.status() values:
- WL_IDLE_STATUS: Idle
- WL_CONNECTED: Connected to AP
- WL_CONNECT_FAILED: Connection failed
- WL_NO_SSID_AVAIL: SSID not available
- WL_CONNECTION_LOST: Connection lost
- WL_DISCONNECTED: Disconnected
```

## Network Architecture

This lab demonstrates **client-server TCP communication**:
- **Server**: Creates WiFi Access Point, listens for connections
- **Client**: Connects to AP, initiates TCP connections to send data
- **Protocol**: TCP ensures reliable data delivery
- **Port**: 6969 (arbitrary choice, can be changed)

## Comparison: TCP vs UDP

| Feature | TCP (this lab) | UDP (Week 7) |
|---------|----------------|--------------|
| Connection | Connection-oriented | Connectionless |
| Reliability | Guaranteed delivery | Best-effort |
| Ordering | Ordered | May arrive out of order |
| Overhead | Higher | Lower |
| Use Case | Reliable data transfer | Fast, real-time data |

## Applications

- Remote control systems
- IoT sensor networks
- Home automation controls
- Wireless counters/displays
- Multi-device synchronization
- Remote button interfaces

## Enhancements

- Add more buttons for additional functions
- Implement reconnection on WiFi loss
- Add LCD to client to display counter
- Send button press timestamps
- Implement bidirectional communication
- Add encryption (WPA2)
