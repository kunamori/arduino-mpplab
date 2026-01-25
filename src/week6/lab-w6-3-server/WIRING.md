# Lab W6-3-SERVER: ESP32 WiFi TCP Servo Control Server

**File**: `lab-w6-3-server.ino`

## Description
This lab demonstrates ESP32 WiFi TCP server for servo motor control. The ESP32 creates a WiFi Access Point and listens for angle commands from a TCP client (lab-w6-3-client). The servo motor rotates to the received angle (0-180°) and sends an acknowledgment back to the client. This is the **server** side that controls the physical servo motor.

## Components Required
- ESP32 DEVKIT × 1
- Servo Motor (SG90 or similar) × 1
- Breadboard
- Jumper wires
- External 5V power supply (recommended for servo)

## Pin Configuration

| ESP32 Pin | Component | Connection | Notes |
|-----------|-----------|------------|-------|
| GPIO 26 | Servo Motor | Signal (orange/yellow) | PWM control |
| 5V | Servo Motor | VCC (red) | Power supply |
| GND | Servo Motor | GND (brown/black) | Common ground |

## ASCII Wiring Diagram

```
        ESP32 DEVKIT         Servo Motor (SG90)
      ┌─────────────┐       
      │             │           ┌─────────┐
      │  GPIO 26 ●──┼───────────┤  Signal │  Orange/Yellow
      │             │           │         │
      │   5V     ●──┼───────────┤  VCC    │  Red
      │             │           │         │
      │   GND    ●──┼───────────┤  GND    │  Brown/Black
      └─────────────┘           └─────────┘
      
      WiFi: Creates AP "ESP32-Nihahaha"
      TCP Server: Listens on port 6969
      IP Address: 192.168.4.1
      
      Receives: "0&" to "180&"
      Sends: "E&" (acknowledgment)
      
      Servo Position: 0° to 180°
```

## Breadboard Layout

1. Place ESP32 on breadboard
2. Connect Servo Motor:
   - Signal wire (orange/yellow) → GPIO 26
   - VCC wire (red) → 5V
   - GND wire (brown/black) → GND
3. **Optional**: Use external 5V power supply for servo (common GND with ESP32)

## Component-Specific Details

### ESP32 DEVKIT
- **WiFi**: 802.11 b/g/n
- **Operating Voltage**: 3.3V logic, 5V power
- **PWM**: LEDC (LED Control) peripheral for servo control
- **WiFi Mode**: Access Point (AP) mode
- **Servo Library**: ESP32Servo library required

### Servo Motor (SG90)
- **Type**: Standard micro servo
- **Rotation**: 0° to 180°
- **Control**: PWM signal on GPIO 26
- **Operating Voltage**: 4.8V-6V (use 5V)
- **Stall Torque**: ~1.8 kg·cm @ 4.8V
- **Speed**: 0.1 sec/60° @ 4.8V
- **Pulse Width**: 500-2400µs (varies by servo)
- **Frequency**: 50Hz (20ms period)
- **Wire Colors**:
  - **Red**: VCC (5V)
  - **Brown/Black**: GND
  - **Orange/Yellow**: Signal (PWM)

## Network Configuration

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│(lab-w6-3-client) │   "ESP32-Nihahaha"    │  (This Device)   │
│  192.168.4.x     │                       │  192.168.4.1     │
└──────────────────┘                       └──────────────────┘
       │                                           │
       │  Request: "90&"                          │
       └─── TCP Connection ──────────────────────►│
            192.168.4.1:6969                   Port 6969
       ◄─── Response: "E&" ──────────────────────┘
                                                   │
      Potentiometer                         ┌──────▼──────┐
      (controls angle)                      │ Servo Motor │
                                            │  GPIO 26    │
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
- **Data Format**: Receives angle followed by "&" (e.g., "90&")
- **Response**: Sends "E&" after moving servo
- **Servo Range**: 0-180° (standard servo)
- **Connection Management**: Persistent server, accepts multiple sequential connections
- **ESP32Servo Library**: Must be installed (different from Arduino Servo library)
- Serial baud rate: 9600

## How It Works

1. **WiFi Setup**:
   - ESP32 creates Access Point "ESP32-Nihahaha"
   - Sets mode to WIFI_AP_STA
   - Obtains IP address 192.168.4.1
   - Starts TCP server on port 6969

2. **Servo Initialization**:
   - Attach servo to GPIO 26
   - Servo ready to receive position commands
   - Initial position: undefined (set on first command)

3. **Client Connection**:
   - Wait for client connection
   - When client connects, print "New Client connected"
   - Maintain connection while client is connected

4. **Command Processing**:
   - Read incoming data until "&" delimiter
   - Parse angle value from string (e.g., "90" from "90&")
   - Convert string to integer: `request.toInt()`
   - Move servo to angle: `Servo1.write(angle)`
   - Send acknowledgment: `client.print("E&")`
   - Print received angle to Serial Monitor

5. **Connection Closure**:
   - When client disconnects, close connection
   - Print "Client disconnected"
   - Return to waiting for next client
   - Servo maintains its last position

## Testing Instructions

1. **Install ESP32Servo Library**:
   - Arduino IDE → Tools → Manage Libraries
   - Search "ESP32Servo"
   - Install "ESP32Servo" by Kevin Harrington

2. **Upload Server Sketch**:
   - Upload this sketch to ESP32 server
   - Open Serial Monitor (9600 baud)

3. **Verify Access Point**:
   - Check Serial Monitor for "AP IP address: 192.168.4.1"
   - On your phone/computer, you should see "ESP32-Nihahaha" WiFi network

4. **Check Servo**:
   - Servo should be freely movable (no position set yet)
   - Should NOT be buzzing or jittering

5. **Setup Client**:
   - Upload lab-w6-3-client.ino to another ESP32
   - Connect potentiometer to client (GPIO 34)
   - Client should connect to "ESP32-Nihahaha" AP

6. **Test Servo Control**:
   - Rotate client's potentiometer fully counter-clockwise
   - Servo should move to 0° position
   - Serial Monitor shows "Received request: 0"
   - Rotate potentiometer fully clockwise
   - Servo should move to 180° position
   - Serial Monitor shows "Received request: 180"

7. **Test Intermediate Angles**:
   - Rotate potentiometer to middle
   - Servo should move to ~90°
   - Servo should smoothly follow potentiometer
   - Each movement confirmed in Serial Monitor

## Serial Monitor Output Examples

```
AP IP address: 192.168.4.1
New Client connected
Received request: 0
Received request: 15
Received request: 30
Received request: 45
Received request: 90
Received request: 135
Received request: 180
Received request: 90
Client disconnected
New Client connected
Received request: 45
```

## Data Protocol

**Message Format**:
```
Request (Client → Server):
  Angle + "&"
  Examples: "0&", "90&", "180&"

Response (Server → Client):
  "E&"
  Acknowledgment that servo moved
```

**TCP Server Flow**:
```
1. Server listens on port 6969
2. Client connects
3. Client sends "90&"
4. Server reads until '&'
5. Server parses "90"
6. Server executes Servo1.write(90)
7. Server sends "E&"
8. Server flushes buffer
9. Client receives acknowledgment
10. Repeat from step 3 while connected
```

## Servo Control Details

```
PWM Signal for Servo:

  0° Position:     500µs pulse (approx)
  90° Position:   1500µs pulse
 180° Position:   2400µs pulse
 
 Frequency: 50Hz (20ms period)
 
     ┌─┐                    20ms
  0° │ │                  ◄──────►
     ┴─┴──────────────────
     500µs
     
     ┌────┐                20ms
 90° │    │              ◄──────►
     ┴────┴──────────────
      1500µs
      
     ┌──────┐              20ms
180° │      │            ◄──────►
     ┴──────┴────────────
      2400µs
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Servo not moving | No power to servo | Check 5V and GND connections |
| Servo jittering | Insufficient power | Use external 5V supply for servo |
| Servo buzzing | Bad PWM signal | Check GPIO 26 connection |
| Limited rotation range | Wrong servo type | Verify servo is 0-180° (not 360° continuous) |
| No WiFi AP visible | Code not running | Check Serial Monitor, verify upload |
| Client can't connect | Server not started | Ensure server sketch running first |
| No serial output | Wrong baud rate | Set Serial Monitor to 9600 baud |
| Servo moves erratically | Noise on signal line | Keep signal wire short, away from power wires |
| ESP32 resets when servo moves | Power supply issue | Use separate 5V supply with common GND |
| Servo stuck at one angle | Not receiving commands | Check TCP connection, verify client sending data |
| Wrong angles | Servo calibration | Some servos need pulse width adjustment |

## Power Considerations

⚠️ **IMPORTANT**: Servos can draw significant current (>500mA under load)

**Power Options**:

1. **ESP32 5V Pin** (light loads only):
   - ✅ Simple, no extra hardware
   - ❌ Limited current (~500mA from USB)
   - ❌ May cause ESP32 brownouts/resets

2. **External 5V Supply** (recommended):
   - ✅ Stable servo operation
   - ✅ No brownouts
   - ✅ Multiple servos possible
   - **MUST** connect GND together

```
External Power Configuration:

  5V Supply      ESP32      Servo
  ┌──────┐     ┌──────┐   ┌──────┐
  │  +5V │────►│ 5V   │   │ VCC  │
  │      │     │      │   │      │
  │  GND │────►│ GND  │   │ GND  │
  └──────┘     │      │   │      │
               │GPIO26├──►│ SIG  │
               └──────┘   └──────┘
                   │          ▲
                   └──────────┘
                   (Common GND required)
```

## Code Reference

```cpp
#include <WiFi.h>
#include <ESP32Servo.h>

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
WiFiServer server(port);

Servo Servo1; // servo object

void setup() {
  Serial.begin(9600);

  WiFi.softAP(ssid, password);
  WiFi.mode(WIFI_AP_STA);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin(port);

  Servo1.attach(26);
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

        int angle = request.toInt();
        Servo1.write(angle);
        client.print("E&");
        client.flush();
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
```

## ESP32Servo Library vs Arduino Servo

| Feature | Arduino Servo | ESP32Servo |
|---------|---------------|------------|
| Platform | AVR Arduino | ESP32 |
| PWM | Timer-based | LEDC peripheral |
| Channels | 12 max | 16 max |
| Compatibility | Not ESP32 | ESP32 only |
| Installation | Built-in | Must install |

**MUST use ESP32Servo library for ESP32!**

## Applications

- Remote camera pan-tilt
- Robotic arm control
- RC car steering
- Antenna positioning
- Valve control
- Throttle control
- Door lock mechanism
- Solar panel tracking
- Gripper control
- Laser pointer positioning

## Enhancements

- Add multiple servos (up to 16 with ESP32Servo)
- Implement smooth servo movement (interpolation)
- Add speed control (move servo gradually)
- Store servo positions in EEPROM
- Add position feedback (use servo with feedback pin)
- Implement preset positions (buttons for 0°, 90°, 180°)
- Add web interface for control
- Implement servo calibration routine
- Add safety limits (prevent over-rotation)
- Implement sequence recording/playback
