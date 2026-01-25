# Lab W6-3-CLIENT: ESP32 WiFi TCP Servo Control Client with Potentiometer

**File**: `lab-w6-3-client.ino`

## Description

This lab demonstrates ESP32 WiFi TCP client for controlling a remote servo motor using a potentiometer. The potentiometer value (0-4095) is mapped to servo angle (0-180°) and sent to a TCP server over WiFi. The server acknowledges each command. This is the **client** side that reads the sensor and sends angle data to the server (lab-w6-3-server).

## Components Required

- ESP32 DEVKIT × 1
- Potentiometer (10kΩ) × 1
- Breadboard
- Jumper wires

## Pin Configuration

| ESP32 Pin | Component     | Connection         | Notes                |
| --------- | ------------- | ------------------ | -------------------- |
| GPIO 34   | Potentiometer | Wiper (middle pin) | ADC1_CH6, input only |
| 3.3V      | Potentiometer | One outer pin      | Power                |
| GND       | Potentiometer | Other outer pin    | Ground               |

## ASCII Wiring Diagram

```
        ESP32 DEVKIT        Potentiometer (10kΩ)
      ┌─────────────┐
      │             │           ┌─────┐
      │  3.3V    ●──┼───────────┤  1  │  (Outer pin 1)
      │             │           │     │
      │             │           │  ╱  │
      │  GPIO 34 ●──┼───────────┤ 2   │  (Wiper/middle)
      │  (ADC)      │           │     │
      │             │           │  ╲  │
      │  GND     ●──┼───────────┤  3  │  (Outer pin 2)
      │             │           └─────┘
      └─────────────┘

      WiFi: Connects to "ESP32-Nihahaha" (server's AP)
      TCP: Connects to 192.168.4.1:6969

      Data Flow:
      Potentiometer → ADC (0-4095) → Map to 0-180° → Send to server
```

## Breadboard Layout

1. Place ESP32 on breadboard
2. Place potentiometer on breadboard
3. Connect potentiometer:
   - One outer pin → 3.3V
   - Middle pin (wiper) → GPIO 34
   - Other outer pin → GND
4. **Note**: Outer pin connections can be swapped (reverses rotation direction)

## Component-Specific Details

### ESP32 DEVKIT

- **WiFi**: 802.11 b/g/n
- **Operating Voltage**: 3.3V logic
- **ADC**: 12-bit resolution (0-4095)
- **GPIO 34**: ADC1_CH6, input only (no output capability)
- **ADC Reference**: 3.3V

### Potentiometer

- **Type**: Rotary potentiometer
- **Resistance**: 10kΩ (1kΩ to 100kΩ works)
- **Rotation**: ~270° (varies by model)
- **Terminals**:
  - Pin 1: One end of resistive track
  - Pin 2: Wiper (variable contact)
  - Pin 3: Other end of resistive track
- **Output**: Variable voltage 0V to 3.3V

## Network Configuration

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│  (This Device)   │   "ESP32-Nihahaha"    │ (lab-w6-3-server)│
│  192.168.4.x     │                       │  192.168.4.1     │
└──────────────────┘                       └──────────────────┘
       │                                           │
       │  Sends: "0&" to "180&"                   │
       └─── TCP Connection ──────────────────────►│
            192.168.4.1:6969                   Port 6969
       ◄─── Response: "E&" ──────────────────────┘
                                                   │
      Potentiometer                         ┌──────▼──────┐
      (0-180°)                              │ Servo Motor │
                                            │   (0-180°)  │
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
- **Connection**: Persistent - connects, sends, waits for response
- **Data Format**: Sends angle value followed by "&" (e.g., "90&")
- **Angle Range**: 0-180° (mapped from ADC 0-4095)
- **Response**: Server sends "E&" acknowledgment
- **Update Rate**: 50ms delay between readings
- **Blocking Read**: Waits for server response before continuing
- **ADC Resolution**: 12-bit (0-4095) on ESP32
- Serial baud rate: 9600

## How It Works

1. **WiFi Connection**:
   - ESP32 connects to "ESP32-Nihahaha" AP
   - Waits until connected
   - Obtains IP address via DHCP
   - Prints "WiFi connected"

2. **Potentiometer Reading** (loop):
   - Read analog value from GPIO 34
   - Value range: 0-4095 (12-bit ADC)

3. **Angle Mapping**:
   - `map(a, 0, 4095, 0, 180)`
   - Maps 0-4095 → 0-180°
   - Constrain to 0-180° (safety)

4. **TCP Communication**:
   - Connect to server (192.168.4.1:6969)
   - Send angle + "&" (e.g., "90&")
   - Flush buffer
   - **Wait** for response from server
   - Read response until "&"
   - Print "Response from server: E"
   - Close connection

5. **Serial Output**:
   - Print response from server
   - Print current angle value
   - 50ms delay before next reading

## Testing Instructions

1. **Setup Server First**:
   - Upload lab-w6-3-server.ino to another ESP32
   - Server creates AP "ESP32-Nihahaha"
   - Server connects servo to GPIO 26

2. **Upload Client Sketch**:
   - Upload this sketch to ESP32 client

3. **Open Serial Monitor** (9600 baud):
   - Should see dots "." while connecting
   - "WiFi connected" when connected
   - Client IP address displayed

4. **Test Potentiometer**:
   - Rotate potentiometer fully counter-clockwise
   - Serial should show angle near 0
   - Servo should move to 0°
   - Rotate potentiometer fully clockwise
   - Serial should show angle near 180
   - Servo should move to 180°

5. **Verify Response**:
   - Each potentiometer change should show:
     ```
     Response from server: E
     90
     ```
   - "E" confirms server received command

6. **Check Servo Movement**:
   - Servo should follow potentiometer rotation
   - Smooth movement as pot is adjusted
   - ~50ms update rate

## Serial Monitor Output Examples

```
.
WiFi connected
192.168.4.2
Response from server: E
0
Response from server: E
15
Response from server: E
30
Response from server: E
90
Response from server: E
135
Response from server: E
180
Response from server: E
90
```

## Data Protocol

**Message Format**:

```
Angle + "&"

Examples:
  "0&"    → Servo to 0°
  "90&"   → Servo to 90°
  "180&"  → Servo to 180°
```

**TCP Transaction** (Request-Response):

```
Client → Server: "90&"
Client waits for response...
Server → Client: "E&"
Client continues to next reading
```

## ADC Mapping

```
Potentiometer Position → ADC Value → Servo Angle

Fully CCW:  0V      →    0    →   0°
1/4 Turn:   0.825V  → 1024   →  45°
1/2 Turn:   1.65V   → 2048   →  90°
3/4 Turn:   2.475V  → 3072   → 135°
Fully CW:   3.3V    → 4095   → 180°

Formula: angle = (ADC_value × 180) / 4095
```

## Troubleshooting

| Issue                   | Possible Cause          | Solution                                |
| ----------------------- | ----------------------- | --------------------------------------- |
| Won't connect to WiFi   | Server not running      | Start server first (lab-w6-3-server)    |
| Stuck on "..."          | Wrong SSID/password     | Verify credentials match server         |
| ADC always reads 0      | Wrong pin               | GPIO 34 is input-only, check connection |
| ADC reads random values | Floating pin            | Check potentiometer wiring              |
| Servo doesn't move      | TCP connection failed   | Check server is running                 |
| Servo jitters           | Noisy ADC reading       | Add 100nF capacitor across pot          |
| Limited angle range     | ADC not full range      | Check pot connected to 3.3V and GND     |
| No response from server | Server not sending "E&" | Check server code                       |
| WiFi disconnects        | Weak signal             | Move ESP32s closer together             |
| Servo angle inverted    | Pot wiring reversed     | Swap 3.3V and GND on pot                |

## Code Reference

```cpp
#include <WiFi.h>

// Wifi credentials
char *ssid = "ESP32-Nihahaha";
char *passwd = "12345678";
char *host = "192.168.4.1";
const uint16_t port = 6969;
WiFiClient client;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
}

void loop() {
  // read analog value from pin
  int a = analogRead(34);
  a = map(a, 0, 4095, 0, 180);
  a = constrain(a, 0, 180);

  if(client.connect(host, port)){
    client.print(String(a) + "&");
    client.flush();
    while (client.available()==0);
    String response = client.readStringUntil('&');
    Serial.print("Response from server: ");
    Serial.println(response);
    client.stop();
  }

  Serial.println(a);
  delay(50);
}
```

## ESP32 ADC Characteristics

```
ESP32 ADC1 Channels (available):
- GPIO 32: ADC1_CH4
- GPIO 33: ADC1_CH5
- GPIO 34: ADC1_CH6 ← Used in this lab
- GPIO 35: ADC1_CH7
- GPIO 36: ADC1_CH0
- GPIO 39: ADC1_CH3

Note:
- GPIO 34-39 are INPUT ONLY (no OUTPUT mode)
- ADC2 channels unavailable when WiFi is used
- 12-bit resolution: 0-4095
- Reference voltage: 3.3V
- Non-linear (use calibration for accurate voltage)
```

## Network Architecture

This lab demonstrates **client-server servo control with feedback**:

- **Server**: Creates WiFi Access Point, controls physical servo
- **Client**: Reads potentiometer, sends angle commands via TCP
- **Protocol**: TCP with request-response pattern
- **Feedback**: Server acknowledges each command with "E&"
- **Update Rate**: 50ms (20 updates per second)

## Applications

- Remote servo control
- Pan-tilt camera control
- Robotic arm joints
- Valve position control
- Antenna positioning
- Remote throttle/steering control
- Volume control
- Dimmer control (with appropriate circuit)

## Enhancements

- Add local servo to mirror remote servo
- Implement smoothing (average multiple ADC readings)
- Add calibration for pot min/max values
- Display angle on OLED
- Add multiple pots for multiple servos
- Implement exponential mapping for finer control
- Add dead zone in center position
- Store calibration in EEPROM
- Add WiFi reconnection logic
- Implement timeout if no server response
