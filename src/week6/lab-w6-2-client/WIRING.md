# Lab W6-2-CLIENT: ESP32 WiFi TCP Relay Control Client

**File**: `lab-w6-2-client.ino`

## Description
This lab demonstrates ESP32 WiFi TCP client for controlling relays remotely. Two buttons toggle the state of two relays on a TCP server over WiFi. Each button press sends ON/OFF commands to the server. This is the **client** side that connects to the server (lab-w6-2-server).

## Components Required
- ESP32 DEVKIT × 1
- Push buttons × 2
- Breadboard
- Jumper wires

## Pin Configuration

| ESP32 Pin | Component | Connection | Notes |
|-----------|-----------|------------|-------|
| GPIO 5 | Button 1 | Relay 1 control | INPUT_PULLUP |
| GPIO 4 | Button 2 | Relay 2 control | INPUT_PULLUP |
| GND | Both buttons | Common ground | Shared GND |

## ASCII Wiring Diagram

```
        ESP32 DEVKIT           Buttons
      ┌─────────────┐       
      │             │       ┌───┐
      │   GPIO 5 ●──┼───────┤ ● │  Button 1 (Relay 1 Toggle)
      │             │       └─┬─┘
      │             │         │
      │   GPIO 4 ●──┼─────┐ ┌─┴─┐
      │             │     │ │ ● │  Button 2 (Relay 2 Toggle)
      │             │     │ └─┬─┘
      │   GND    ●──┼─────┴───┴──── GND
      └─────────────┘       

      WiFi: Connects to "ESP32-Nihahaha" (server's AP)
      TCP: Connects to 192.168.4.1:6969
      
      Commands Sent:
      Button 1: "ON1&" or "OFF1&"
      Button 2: "ON2&" or "OFF2&"
```

## Breadboard Layout

1. Place ESP32 on breadboard
2. Connect Button 1 (Relay 1) between GPIO 5 and GND
3. Connect Button 2 (Relay 2) between GPIO 4 and GND
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
- **Function**: Toggle relay state on each press

## Network Configuration

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│  (This Device)   │   "ESP32-Nihahaha"    │ (lab-w6-2-server)│
│  192.168.4.x     │                       │  192.168.4.1     │
└──────────────────┘                       └──────────────────┘
       │                                           │
       │  Button 1: "ON1&" / "OFF1&"              │
       │  Button 2: "ON2&" / "OFF2&"              │
       └─── TCP Connection ──────────────────────►│
            192.168.4.1:6969                   Port 6969
                                                   │
                                            ┌──────▼──────┐
                                            │   Relay 1   │
                                            │   Relay 2   │
                                            └─────────────┘
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
- **Connection**: Connects to server only when sending commands (not persistent)
- **Data Format**: Sends relay command followed by "&" (e.g., "ON1&", "OFF2&")
- **State Tracking**: Client tracks relay states locally (state_relay array)
- **Toggle Behavior**: Each button press toggles between ON and OFF
- **Debouncing**: 20ms delay prevents multiple toggles per press
- **Connection Management**: Connects, sends command, disconnects for each button press
- Serial baud rate: 9600

## How It Works

1. **WiFi Connection**:
   - ESP32 connects to "ESP32-Nihahaha" AP
   - Waits until connected
   - Obtains IP address via DHCP

2. **State Management**:
   - `state_relay[2]` tracks relay states: {0, 0} (both OFF initially)
   - `state_relay[0]` for Relay 1
   - `state_relay[1]` for Relay 2

3. **Button 1 (GPIO 5) - Relay 1**:
   - Detect button state change (edge detection)
   - 20ms debounce delay
   - Check current relay state
   - If OFF (0): Send "ON1&", set state to 1
   - If ON (1): Send "OFF1&", set state to 0

4. **Button 2 (GPIO 4) - Relay 2**:
   - Same logic as Button 1
   - Controls Relay 2 on server
   - Sends "ON2&" or "OFF2&"

5. **TCP Communication**:
   - Create TCP connection to 192.168.4.1:6969
   - Send command string (e.g., "ON1&")
   - Flush buffer
   - Close connection

## Testing Instructions

1. **Setup Server First**:
   - Upload lab-w6-2-server.ino to another ESP32
   - Server creates AP "ESP32-Nihahaha"
   - Server connects relays to GPIO 12 and 13

2. **Upload Client Sketch**:
   - Upload this sketch to ESP32 client

3. **Open Serial Monitor** (9600 baud):
   - Should see "Connecting to WiFi..."
   - When connected, WiFi status displayed

4. **Test Button 1 (GPIO 5)**:
   - Press Button 1
   - First press: Sends "ON1&" → Relay 1 ON
   - Second press: Sends "OFF1&" → Relay 1 OFF
   - Verify server's Relay 1 toggles

5. **Test Button 2 (GPIO 4)**:
   - Press Button 2
   - First press: Sends "ON2&" → Relay 2 ON
   - Second press: Sends "OFF2&" → Relay 2 OFF
   - Verify server's Relay 2 toggles

6. **Test Both Buttons**:
   - Press both buttons alternately
   - Each relay should toggle independently
   - Server Serial Monitor shows received commands

## Serial Monitor Output Examples

```
Connecting to WiFi...
Connecting to WiFi...
WiFi connected
(Button 1 pressed - no output on client)
(Button 1 pressed again)
(Button 2 pressed)
(Button 2 pressed again)
```

**Note**: This client doesn't print to Serial in the main loop. Check server Serial Monitor for command reception.

## Data Protocol

**Message Format**:
```
Command + "&"

Relay 1 Commands:
  "ON1&"    → Turn Relay 1 ON
  "OFF1&"   → Turn Relay 1 OFF

Relay 2 Commands:
  "ON2&"    → Turn Relay 2 ON
  "OFF2&"   → Turn Relay 2 OFF
```

**TCP Transaction**:
```
1. Button pressed
2. Check current relay state
3. Determine command (ON/OFF)
4. client.connect(192.168.4.1, 6969)
5. client.print("ON1&")
6. client.flush()
7. client.stop()
8. Update local state
```

## State Machine

```
Button 1 State Machine:

  state_relay[0] = 0 (OFF)
         │
         │ Button Press
         ▼
    Send "ON1&"
         │
         ▼
  state_relay[0] = 1 (ON)
         │
         │ Button Press
         ▼
    Send "OFF1&"
         │
         ▼
  state_relay[0] = 0 (OFF)
         │
        ...
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Won't connect to WiFi | Server not running | Start server first (lab-w6-2-server) |
| "Connecting to WiFi..." loop | Wrong SSID/password | Verify credentials match server |
| Button not responding | Wrong GPIO pin | Check GPIO 4 and 5 connections |
| Multiple toggles per press | Bouncing | Increase debounce delay (20ms→50ms) |
| Relay not responding | TCP connection failed | Check server is running and listening |
| Relay state out of sync | State tracking issue | Reset both ESP32s to sync states |
| WiFi disconnects | Weak signal | Move ESP32s closer together |
| Wrong relay toggling | Buttons swapped | Verify GPIO 5→Relay1, GPIO 4→Relay2 |

## Code Reference

```cpp
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
  // Button 1 (GPIO 5) - Relay 1
  state1=digitalRead(5);
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

  // Button 2 (GPIO 4) - Relay 2
  state2=digitalRead(4);
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
```

## Button Debouncing

```
Physical Button Press:
  HIGH ──┐    ┌─── Bouncing ───┐    ┌─── Stable LOW
         │    │                │    │
         └────┘                └────┘
         
Software Debouncing:
  1. Detect edge (state change)
  2. Wait 20ms (debounce delay)
  3. Read button again
  4. If still pressed, process action
```

## Network Architecture

This lab demonstrates **client-server relay control**:
- **Server**: Creates WiFi Access Point, controls physical relays
- **Client**: Connects to AP, sends relay commands via TCP
- **Protocol**: TCP ensures reliable command delivery
- **Port**: 6969 (arbitrary choice, can be changed)

## Applications

- Remote relay control systems
- Home automation switches
- IoT device control
- Wireless light switches
- Appliance control
- Garage door opener
- Motor control systems
- Security system control

## Enhancements

- Add status LEDs to show relay states
- Implement feedback from server (relay acknowledgment)
- Add more buttons for additional relays
- Implement reconnection on WiFi loss
- Add OLED display to show connection status
- Send relay state timestamps
- Implement scheduled relay control
- Add encryption for command security
