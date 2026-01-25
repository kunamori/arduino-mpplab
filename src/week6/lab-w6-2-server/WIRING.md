# Lab W6-2-SERVER: ESP32 WiFi TCP Relay Control Server

**File**: `lab-w6-2-server.ino`

## Description
This lab demonstrates ESP32 WiFi TCP server for relay control. The ESP32 creates a WiFi Access Point and listens for relay control commands from a TCP client (lab-w6-2-client). Two relays are controlled via commands: ON1/OFF1 for Relay 1, ON2/OFF2 for Relay 2. This is the **server** side that controls physical relays.

## Components Required
- ESP32 DEVKIT × 1
- 5V Relay modules × 2 (or 2-channel relay module)
- Breadboard
- Jumper wires
- External 5V power supply (optional, for relay coils)

## Pin Configuration

| ESP32 Pin | Component | Connection | Notes |
|-----------|-----------|------------|-------|
| GPIO 12 | Relay 1 | Control signal (IN1) | Active LOW |
| GPIO 13 | Relay 2 | Control signal (IN2) | Active LOW |
| 5V | Relays | VCC | Power for relay coils |
| GND | Relays | GND | Common ground |

## ASCII Wiring Diagram

```
        ESP32 DEVKIT        Relay Module (2-Channel)
      ┌─────────────┐       ┌──────────────────┐
      │             │       │  Relay 1  Relay 2│
      │  GPIO 12 ●──┼───────┤ IN1              │
      │             │       │                  │
      │  GPIO 13 ●──┼───────┤       IN2        │
      │             │       │                  │
      │   5V     ●──┼───────┤ VCC              │
      │             │       │                  │
      │   GND    ●──┼───────┤ GND              │
      └─────────────┘       └──────────────────┘
                                  │      │
                            Relay Contacts:
                            NO COM NC  NO COM NC
                             │  │  │   │  │  │
      
      WiFi: Creates AP "ESP32-Nihahaha"
      TCP Server: Listens on port 6969
      IP Address: 192.168.4.1
      
      Relay Logic (Active LOW):
      - GPIO HIGH (1) → Relay OFF
      - GPIO LOW (0)  → Relay ON
```

## Breadboard Layout

1. Place ESP32 on breadboard
2. Connect Relay Module:
   - IN1 → GPIO 12 (Relay 1 control)
   - IN2 → GPIO 13 (Relay 2 control)
   - VCC → 5V
   - GND → GND
3. **IMPORTANT**: Relays are ACTIVE LOW
   - HIGH signal = Relay OFF
   - LOW signal = Relay ON

## Component-Specific Details

### ESP32 DEVKIT
- **WiFi**: 802.11 b/g/n
- **Operating Voltage**: 3.3V logic, 5V power
- **GPIO Output**: 3.3V logic level (sufficient for most relay modules)
- **WiFi Mode**: Access Point (AP) mode

### Relay Module
- **Type**: Electromechanical relay (typically SRD-05VDC-SL-C)
- **Control Voltage**: 5V coil
- **Control Logic**: Active LOW (LOW=ON, HIGH=OFF)
- **Contacts**: SPDT (Single Pole Double Throw)
  - **NO**: Normally Open (closed when relay energized)
  - **COM**: Common terminal
  - **NC**: Normally Closed (open when relay energized)
- **Switching Capacity**: Typically 10A @ 250VAC, 10A @ 30VDC
- **Isolation**: Optocoupler isolation between control and load
- **LED Indicator**: Most modules have LED showing relay state

## Network Configuration

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│(lab-w6-2-client) │   "ESP32-Nihahaha"    │  (This Device)   │
│  192.168.4.x     │                       │  192.168.4.1     │
└──────────────────┘                       └──────────────────┘
       │                                           │
       │  Commands: ON1/OFF1/ON2/OFF2             │
       └─── TCP Connection ──────────────────────►│
            192.168.4.1:6969                   Port 6969
                                                   │
                                            ┌──────▼──────┐
                                            │  GPIO 12    │──► Relay 1
                                            │  GPIO 13    │──► Relay 2
                                            └─────────────┘
```

### Network Details
- **SSID**: "ESP32-Nihahaha"
- **Password**: "12345678"
- **Server IP**: 192.168.4.1 (Access Point IP)
- **Server Port**: 6969
- **Protocol**: TCP (WiFiServer)
- **WiFi Mode**: AP+STA (Access Point + Station)
- **Client IP**: Assigned by DHCP (192.168.4.x)

## Special Notes

- **WiFi Mode**: Access Point (AP) mode - creates its own WiFi network
- **Server Type**: TCP server - accepts incoming connections
- **Data Format**: Receives relay commands followed by "&" (e.g., "ON1&", "OFF2&")
- **Relay Logic**: ACTIVE LOW (HIGH=OFF, LOW=ON)
- **Initial State**: Both relays OFF (GPIO HIGH) on startup
- **Commands**: ON1, OFF1, ON2, OFF2
- **Connection Management**: Persistent server, accepts multiple sequential connections
- Serial baud rate: 9600

## How It Works

1. **WiFi Setup**:
   - ESP32 creates Access Point "ESP32-Nihahaha"
   - Sets mode to WIFI_AP_STA
   - Obtains IP address 192.168.4.1
   - Starts TCP server on port 6969

2. **Relay Initialization**:
   - Set GPIO 12 and 13 as OUTPUT
   - Set both pins HIGH (relays OFF)
   - This is the default state on boot

3. **Client Connection**:
   - Wait for client connection
   - When client connects, print "New Client connected"
   - Maintain connection while client is connected

4. **Command Processing**:
   - Read incoming data until "&" delimiter
   - Parse command string
   - Execute relay action:
     - "ON1" → digitalWrite(12, LOW) - Relay 1 ON
     - "OFF1" → digitalWrite(12, HIGH) - Relay 1 OFF
     - "ON2" → digitalWrite(13, LOW) - Relay 2 ON
     - "OFF2" → digitalWrite(13, HIGH) - Relay 2 OFF
   - Print command to Serial Monitor

5. **Connection Closure**:
   - When client disconnects, close connection
   - Print "Client disconnected"
   - Return to waiting for next client
   - Relays maintain their last state

## Testing Instructions

1. **Upload Server Sketch**:
   - Upload this sketch to ESP32 server
   - Open Serial Monitor (9600 baud)

2. **Verify Access Point**:
   - Check Serial Monitor for "AP IP address: 192.168.4.1"
   - On your phone/computer, you should see "ESP32-Nihahaha" WiFi network

3. **Check Initial Relay State**:
   - Both relays should be OFF (LEDs off on relay module)
   - You should hear NO clicking sound

4. **Setup Client**:
   - Upload lab-w6-2-client.ino to another ESP32
   - Client should connect to "ESP32-Nihahaha" AP

5. **Test Relay 1**:
   - Press Button 1 on client
   - Serial Monitor shows "Received request: ON1"
   - Relay 1 should turn ON (LED on, click sound)
   - Press Button 1 again
   - Serial Monitor shows "Received request: OFF1"
   - Relay 1 should turn OFF

6. **Test Relay 2**:
   - Press Button 2 on client
   - Serial Monitor shows "Received request: ON2"
   - Relay 2 should turn ON
   - Press Button 2 again
   - Serial Monitor shows "Received request: OFF2"
   - Relay 2 should turn OFF

7. **Test Both Relays**:
   - Turn both relays ON
   - Verify both are energized
   - Turn them OFF in different order
   - Verify independent control

## Serial Monitor Output Examples

```
AP IP address: 192.168.4.1
New Client connected
Received request: ON1
Client disconnected
New Client connected
Received request: OFF1
Client disconnected
New Client connected
Received request: ON2
Client disconnected
New Client connected
Received request: OFF2
Client disconnected
New Client connected
Received request: ON1
Client disconnected
New Client connected
Received request: ON2
Client disconnected
```

## Data Protocol

**Message Format**:
```
Command + "&"

Relay 1 Commands:
  "ON1&"  → digitalWrite(12, LOW)  - Relay 1 ON
  "OFF1&" → digitalWrite(12, HIGH) - Relay 1 OFF

Relay 2 Commands:
  "ON2&"  → digitalWrite(13, LOW)  - Relay 2 ON
  "OFF2&" → digitalWrite(13, HIGH) - Relay 2 OFF
```

**TCP Server Flow**:
```
1. Server listens on port 6969
2. Client connects
3. Client sends "ON1&"
4. Server reads until '&'
5. Server parses "ON1"
6. Server executes digitalWrite(12, LOW)
7. Relay 1 turns ON
8. Client disconnects
9. Server returns to listening
10. Relay 1 stays ON
```

## Relay State Truth Table

| Command | GPIO 12 | GPIO 13 | Relay 1 | Relay 2 |
|---------|---------|---------|---------|---------|
| Initial | HIGH    | HIGH    | OFF     | OFF     |
| ON1     | LOW     | HIGH    | ON      | OFF     |
| ON2     | HIGH    | LOW     | OFF     | ON      |
| ON1+ON2 | LOW     | LOW     | ON      | ON      |
| OFF1    | HIGH    | LOW     | OFF     | ON      |
| OFF2    | HIGH    | HIGH    | OFF     | OFF     |

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Relay not clicking | No power to relay | Check 5V and GND connections |
| Relay always ON | Active LOW logic | HIGH=OFF, LOW=ON (check code) |
| Relay LED on, no switching | Relay coil not energized | Check relay module VCC connection |
| Both relays respond together | Wiring crossed | Verify GPIO 12→IN1, GPIO 13→IN2 |
| No WiFi AP visible | Code not running | Check Serial Monitor, verify upload |
| Client can't connect | Server not started | Ensure server sketch running first |
| No serial output | Wrong baud rate | Set Serial Monitor to 9600 baud |
| Command not working | Wrong delimiter | Ensure client sends "&" after command |
| Relay state random on boot | Floating pins | Code sets HIGH on startup |
| ESP32 resets when relay switches | Power supply issue | Use separate 5V supply for relays |

## Safety Warnings

⚠️ **ELECTRICAL SAFETY**:
- **HIGH VOLTAGE**: Relay contacts can switch mains voltage (110V/220V AC)
- **NEVER** connect mains voltage while breadboarding
- Use relays only with low voltage loads (12V DC) for testing
- Ensure proper isolation between control and load sides
- If using mains voltage:
  - Use qualified electrician
  - Proper enclosure required
  - Follow local electrical codes
  - Never touch relay contacts when powered

## Code Reference

```cpp
#include <WiFi.h>

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
WiFiServer server(port);

void setup() {
  Serial.begin(9600);

  WiFi.softAP(ssid, password);
  WiFi.mode(WIFI_AP_STA);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  // turn off both relays initially (ACTIVE LOW)
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client connected");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('&');
        Serial.print("Received request: ");
        Serial.println(request);

        if (request == "ON1") {
          digitalWrite(12, LOW);
        } else if (request == "OFF1") {
          digitalWrite(12, HIGH);
        } else if (request == "ON2") {
          digitalWrite(13, LOW);
        } else if (request == "OFF2") {
          digitalWrite(13, HIGH);
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
```

## Relay Module Connections

```
Relay Module Pinout:

Control Side (Low Voltage):
┌──────────────┐
│ VCC  GND     │  Power for relay coil
│ IN1  IN2     │  Control signals from ESP32
└──────────────┘

Load Side (High Voltage):
┌──────────────┐
│ NO1 COM1 NC1 │  Relay 1 contacts
│ NO2 COM2 NC2 │  Relay 2 contacts
└──────────────┘

Example Load Connection (12V DC):
  +12V ──► NO1
  COM1 ──► Load (e.g., lamp)
  Load ──► GND
  
  When Relay 1 ON: +12V flows through load
  When Relay 1 OFF: No connection
```

## Applications

- Home automation (lights, fans, appliances)
- Garage door control
- Irrigation systems
- Motor control
- Heating/cooling control
- Security system integration
- Industrial automation
- Remote power switching

## Enhancements

- Add relay state feedback to client
- Implement relay timers (auto-off after delay)
- Add current sensing for load monitoring
- Store relay states in EEPROM (survive reset)
- Add web interface for control
- Implement scheduling (time-based control)
- Add multiple relay support (8-channel, 16-channel)
- Implement safety interlocks (prevent both relays on simultaneously)
- Add authentication/password protection
