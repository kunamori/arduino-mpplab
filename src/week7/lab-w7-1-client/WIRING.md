# Lab W7-1-CLIENT: ESP32 WiFi UDP LED Control Client

**File**: `lab-w7-1-client.ino`

## Description

This lab demonstrates ESP32 WiFi UDP client functionality for controlling LEDs remotely. The client receives user input via Serial Monitor (numbers 1-6) and sends UDP packets to control 6 LEDs on the server. Unlike TCP, UDP is connectionless and faster. This is the **client** side that sends LED control commands to the server (lab-w7-1-server).

## Components Required

- ESP32 DEVKIT × 1
- USB cable for Serial communication
- Computer with Serial Monitor

## Pin Configuration

| ESP32 Pin | Component | Connection           | Notes          |
| --------- | --------- | -------------------- | -------------- |
| USB       | Computer  | Serial communication | For user input |

**Note**: This client has no physical components - all control is via Serial Monitor.

## ASCII Wiring Diagram

```
        ESP32 DEVKIT
      ┌─────────────┐
      │             │
      │             │       No physical components
      │             │
      │             │       Input: Serial Monitor (1-6)
      │             │       Output: UDP packets
      │             │
      │   USB    ●──┼───────► Computer
      │             │         (Serial Monitor)
      └─────────────┘

      WiFi: Connects to "ESP32-Nihahaha" (server's AP)
      UDP: Sends to 192.168.4.1:6969

      User Input (Serial):
        1 → Toggle LED 1
        2 → Toggle LED 2
        3 → Toggle LED 3
        4 → Toggle LED 4
        5 → Toggle LED 5
        6 → Toggle LED 6
```

## Breadboard Layout

1. Place ESP32 on breadboard (or connect directly via USB)
2. Connect USB cable to computer
3. No other connections needed

## Component-Specific Details

### ESP32 DEVKIT

- **WiFi**: 802.11 b/g/n
- **Operating Voltage**: 3.3V logic, 5V USB power
- **Serial**: USB-to-Serial converter built-in
- **WiFi Mode**: Station (STA) mode

### Serial Monitor

- **Baud Rate**: 9600
- **Input**: Single digit numbers (1-6)
- **Line Ending**: Newline (\n)
- **Purpose**: User interface for LED control

## Network Configuration

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│  (This Device)   │   "ESP32-Nihahaha"    │ (lab-w7-1-server)│
│  192.168.4.x     │                       │  192.168.4.1     │
└──────────────────┘                       └──────────────────┘
       ▲                                           │
       │                                           │
   Serial Input                               6 LEDs on:
   (1-6)                                      GPIO 13,12,14,
       │                                           27,26,25
       │  UDP Packet (no connection)              │
       └───────────────────────────────────────►  │
            192.168.4.1:6969                      │
                                            ┌──────▼──────┐
                                            │   LED 1-6   │
                                            │ Toggle ON/OFF│
                                            └─────────────┘
```

### Network Details

- **SSID**: "ESP32-Nihahaha"
- **Password**: "12345678"
- **Server IP**: 192.168.4.1 (Access Point)
- **Server Port**: 6969
- **Protocol**: UDP (WiFiUDP)
- **Client IP**: Assigned by DHCP (192.168.4.x)
- **Client UDP Port**: 6969 (same as server)

## Special Notes

- **WiFi Mode**: Station (STA) mode - connects to server's Access Point
- **Protocol**: UDP (connectionless, no handshake)
- **Data Format**: Sends LED number as string (e.g., "1", "2", "6")
- **No Delimiter**: Unlike TCP labs, no "&" delimiter needed
- **Packet Size**: Single digit (1 byte + null terminator)
- **Update Rate**: On-demand (when user enters command)
- **No Response**: UDP is fire-and-forget (no acknowledgment)
- **Input Validation**: Server validates LED number (1-6)
- Serial baud rate: 9600

## How It Works

1. **WiFi Connection**:
   - ESP32 connects to "ESP32-Nihahaha" AP
   - Waits until connected
   - Obtains IP address via DHCP
   - Prints "Connected to WiFi" and local IP

2. **UDP Initialization**:
   - `udp.begin(port)` starts listening on port 6969
   - Client can now send and receive UDP packets

3. **User Input** (loop):
   - Check if Serial data available
   - Read string until newline ('\n')
   - Store in `selLED` variable

4. **UDP Packet Transmission**:
   - Begin packet: `udp.beginPacket(host, port)`
   - Convert string to char array
   - Print data to UDP buffer
   - Flush buffer
   - Send packet: `udp.endPacket()`

5. **Delay**:
   - 1 second delay after sending
   - Prevents flooding server with packets

## Testing Instructions

1. **Setup Server First**:
   - Upload lab-w7-1-server.ino to another ESP32
   - Server creates AP "ESP32-Nihahaha"
   - Server connects 6 LEDs to GPIO 13,12,14,27,26,25

2. **Upload Client Sketch**:
   - Upload this sketch to ESP32 client

3. **Open Serial Monitor** (9600 baud):
   - Should see "Connecting to WiFi..."
   - When connected: "Connected to WiFi"
   - IP address displayed (e.g., 192.168.4.2)

4. **Test LED 1**:
   - Type `1` in Serial Monitor input box
   - Press Enter or click Send
   - LED 1 on server should toggle
   - First press: ON
   - Second press: OFF

5. **Test Other LEDs**:
   - Type `2`, `3`, `4`, `5`, `6`
   - Each LED should toggle independently
   - Server Serial Monitor shows:
     ```
     LED 1 ON
     LED 2 ON
     LED 3 OFF
     ```

6. **Test Invalid Input**:
   - Type `7` or `0`
   - Server shows: "ERROR: INPUT_USER"
   - No LED state change

## Serial Monitor Output Examples

```
Connecting to WiFi...
Connecting to WiFi...
Connected to WiFi
192.168.4.2
(User types "1" and presses Enter - no output)
(User types "2" and presses Enter - no output)
(User types "6" and presses Enter - no output)
```

**Note**: Client doesn't print received commands. Check server Serial Monitor for LED status.

## Data Protocol

**Message Format**:

```
UDP Packet:
  LED Number (string)

Examples:
  "1"  → Toggle LED 1
  "2"  → Toggle LED 2
  "6"  → Toggle LED 6
```

**UDP Transaction**:

```
1. User types "1" in Serial Monitor
2. Client reads "1\n"
3. udp.beginPacket(192.168.4.1, 6969)
4. udp.print("1")
5. udp.flush()
6. udp.endPacket()
7. Packet sent (no wait for response)
8. delay(1000)
```

## UDP vs TCP Comparison

| Feature        | UDP (this lab)     | TCP (Week 6)           |
| -------------- | ------------------ | ---------------------- |
| Connection     | Connectionless     | Connection-oriented    |
| Reliability    | No guarantee       | Guaranteed delivery    |
| Speed          | Faster             | Slower (overhead)      |
| Ordering       | No order guarantee | Ordered delivery       |
| Overhead       | Low                | High                   |
| Acknowledgment | None               | ACK for each packet    |
| Use Case       | Real-time control  | Reliable data transfer |

## Troubleshooting

| Issue                        | Possible Cause       | Solution                             |
| ---------------------------- | -------------------- | ------------------------------------ |
| Won't connect to WiFi        | Server not running   | Start server first (lab-w7-1-server) |
| "Connecting to WiFi..." loop | Wrong SSID/password  | Verify credentials match server      |
| LED not toggling             | UDP packet lost      | Try sending command again            |
| No response to input         | Serial not ready     | Wait for "Connected to WiFi" message |
| Wrong LED toggling           | Wrong number entered | Verify input 1-6                     |
| Multiple LEDs toggle         | Send too fast        | Wait 1 second between commands       |
| WiFi disconnects             | Weak signal          | Move ESP32s closer together          |
| No serial output             | Wrong baud rate      | Set Serial Monitor to 9600 baud      |
| Input ignored                | Missing newline      | Ensure Serial Monitor sends NL       |

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

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print(WiFi.localIP());
  udp.begin(port);
}

void loop() {
  udp.beginPacket(host, port);

  if(Serial.available() > 0 ){
    String selLED = Serial.readStringUntil('\n');
    char char_message[255];
    sprintf(char_message, "%s", selLED.c_str());
    udp.print(char_message);
    udp.flush();
    udp.endPacket();
    delay(1000);
  }
}
```

## Serial Monitor Settings

```
Serial Monitor Configuration:
┌─────────────────────────┐
│ Baud Rate: 9600        │
│ Line Ending: Newline   │  ← Important!
│                         │
│ Input Box: [1     ][Send]
└─────────────────────────┘

Valid Inputs:
  1, 2, 3, 4, 5, 6

Invalid Inputs:
  0, 7, 8, 9, letters, symbols
  (Server will show "ERROR: INPUT_USER")
```

## UDP Packet Structure

```
UDP Packet (simplified):

┌─────────────┬─────────────┬─────────┬─────────┐
│ Source IP   │ Source Port │ Dest IP │Dest Port│
├─────────────┼─────────────┼─────────┼─────────┤
│192.168.4.2  │ 6969        │192.168. │ 6969    │
│             │             │ 4.1     │         │
└─────────────┴─────────────┴─────────┴─────────┘
                      │
                      ▼
              ┌──────────────┐
              │   Payload    │
              │     "1"      │
              └──────────────┘
```

## Network Architecture

This lab demonstrates **connectionless UDP communication**:

- **Server**: Creates WiFi Access Point, controls 6 LEDs
- **Client**: Sends LED commands via UDP (no connection setup)
- **Protocol**: UDP for fast, low-latency control
- **Port**: 6969 on both client and server
- **No Feedback**: Client doesn't know if command received

## Applications

- Real-time LED control
- Simple remote controls
- Status indicators
- Lighting systems
- Display boards
- Emergency signals
- Smart home switches
- IoT sensor networks

## Enhancements

- Add physical buttons to client
- Display LED states on OLED
- Implement "all on" and "all off" commands
- Add LED brightness control (PWM)
- Implement patterns (blink, chase, fade)
- Add bidirectional communication (LED state feedback)
- Implement multicast (control multiple servers)
- Add command history
- Implement retry mechanism for critical commands
- Add encryption for security
