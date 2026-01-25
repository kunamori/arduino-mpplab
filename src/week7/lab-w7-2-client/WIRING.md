# Lab W7-2-CLIENT: ESP32 WiFi UDP DHT22 Sensor Query Client

**File**: `lab-w7-2-client.ino`

## Description
This lab demonstrates ESP32 WiFi UDP client for querying DHT22 temperature/humidity sensor data remotely. The client receives user commands via Serial Monitor ("TEMP" or "HUMI") and sends UDP packets to request sensor readings from the server. The server responds with the requested data. This is the **client** side that queries the sensor server (lab-w7-2-server).

## Components Required
- ESP32 DEVKIT × 1
- USB cable for Serial communication
- Computer with Serial Monitor

## Pin Configuration

| ESP32 Pin | Component | Connection | Notes |
|-----------|-----------|------------|-------|
| USB | Computer | Serial communication | For user input |

**Note**: This client has no physical components - all control is via Serial Monitor.

## ASCII Wiring Diagram

```
        ESP32 DEVKIT
      ┌─────────────┐       
      │             │       
      │             │       No physical components
      │             │       
      │             │       Input: Serial Monitor ("TEMP" or "HUMI")
      │             │       Output: UDP packets & sensor data display
      │             │       
      │   USB    ●──┼───────► Computer
      │             │         (Serial Monitor)
      └─────────────┘       

      WiFi: Connects to "ESP32-Nihahaha" (server's AP)
      UDP: Sends to 192.168.4.1:6969
      Client IP: 192.168.4.5 (static)
      
      User Input (Serial):
        TEMP → Request temperature
        HUMI → Request humidity
      
      Server Response:
        Temperature: e.g., "25.30"
        Humidity: e.g., "60.50"
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
- **WiFi Mode**: Station (STA) mode with static IP
- **IP Configuration**: Static IP 192.168.4.5

### Serial Monitor
- **Baud Rate**: 9600
- **Input**: Text commands ("TEMP" or "HUMI")
- **Line Ending**: Newline (\n)
- **Output**: Displays received sensor data
- **Purpose**: User interface for sensor queries

## Network Configuration

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│  (This Device)   │   "ESP32-Nihahaha"    │ (lab-w7-2-server)│
│  192.168.4.5     │                       │  192.168.4.1     │
│  (Static IP)     │                       │   (Gateway)      │
└──────────────────┘                       └──────────────────┘
       ▲                                           │
       │                                           │
   Serial Input                               DHT22 Sensor
   (TEMP/HUMI)                                (GPIO 33)
       │                                           │
       │  UDP Request ─────────────────────────►  │
       │           "TEMP" or "HUMI"               │
       │                                           │
       ◄─── UDP Response ──────────────────────────┘
                "25.30" or "60.50"
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
- **Protocol**: UDP (connectionless, request-response pattern)
- **Data Format**: 
  - Request: "TEMP" or "HUMI" (text string)
  - Response: Decimal number (e.g., "25.30", "60.50")
- **Input Validation**: Client accepts any text, server validates
- **Response Display**: Shows "(Temperature)" or "(Humidity)" with value
- **No Delimiter**: UDP packets don't use "&" delimiter
- Serial baud rate: 9600

## How It Works

1. **WiFi Connection**:
   - Configure static IP (192.168.4.5, gateway 192.168.4.1, subnet 255.255.255.0)
   - ESP32 connects to "ESP32-Nihahaha" AP
   - Waits until connected
   - Prints "Connected to WiFi" and local IP (192.168.4.5)

2. **UDP Initialization**:
   - `udp.begin(port)` starts listening on port 6969
   - Client can now send and receive UDP packets

3. **User Input** (loop):
   - Check if Serial data available
   - Read string until newline ('\n')
   - Trim whitespace
   - Store in `SENSOR_DATA` variable

4. **UDP Request Transmission**:
   - Begin packet: `udp.beginPacket(host, port)`
   - Print command ("TEMP" or "HUMI") to UDP buffer
   - Send packet: `udp.endPacket()`

5. **UDP Response Reception**:
   - Parse incoming UDP packet
   - Read packet data into buffer
   - Convert to string
   - Check if response matches request:
     - If request was "TEMP": Print "Received (Temperature): XX.XX"
     - If request was "HUMI": Print "Received (Humidity): XX.XX"

## Testing Instructions

1. **Setup Server First**:
   - Upload lab-w7-2-server.ino to another ESP32
   - Server creates AP "ESP32-Nihahaha"
   - Server connects DHT22 to GPIO 33

2. **Upload Client Sketch**:
   - Upload this sketch to ESP32 client

3. **Open Serial Monitor** (9600 baud):
   - Should see "Connecting to WiFi..."
   - When connected: "Connected to WiFi"
   - IP address: "192.168.4.5"

4. **Request Temperature**:
   - Type `TEMP` in Serial Monitor input box
   - Press Enter or click Send
   - Wait 1-2 seconds
   - Should see: "Received (Temperature): 25.30" (actual value varies)

5. **Request Humidity**:
   - Type `HUMI` in Serial Monitor
   - Press Enter
   - Should see: "Received (Humidity): 60.50" (actual value varies)

6. **Test Multiple Requests**:
   - Type "TEMP" → Get temperature
   - Type "HUMI" → Get humidity
   - Type "TEMP" → Get updated temperature
   - Each request shows current sensor reading

7. **Verify Server DHT22**:
   - Server reads DHT22 every 2 seconds
   - Client can request data anytime
   - Values should match server's Serial output

## Serial Monitor Output Examples

```
Connecting to WiFi...
Connecting to WiFi...
Connected to WiFi
192.168.4.5
Received (Temperature): 25.30
Received (Temperature): 25.20
Received (Humidity): 58.70
Received (Humidity): 59.10
Received (Temperature): 24.80
```

## Data Protocol

**Request Format** (Client → Server):
```
Command (string)

Examples:
  "TEMP"  → Request temperature reading
  "HUMI"  → Request humidity reading
```

**Response Format** (Server → Client):
```
Sensor Value (string)

Examples:
  "25.30"  → Temperature 25.30°C
  "60.50"  → Humidity 60.50%
```

**UDP Transaction**:
```
1. User types "TEMP" in Serial Monitor
2. Client reads "TEMP\n"
3. udp.beginPacket(192.168.4.1, 6969)
4. udp.print("TEMP")
5. udp.endPacket()
6. Server receives "TEMP"
7. Server reads DHT22 temperature
8. Server sends UDP response "25.30"
9. Client receives "25.30"
10. Client prints "Received (Temperature): 25.30"
```

## Static IP Configuration

```
IP Configuration:

Client IP:    192.168.4.5
Gateway:      192.168.4.1  (Server AP)
Subnet Mask:  255.255.255.0

Why Static IP?
- Predictable addressing
- Server can respond to specific client
- UDP response routing
- Multiple clients can use different static IPs
  (e.g., 192.168.4.5, 192.168.4.6, 192.168.4.7)

WiFi.config(ip, gateway, subnet) sets static IP
Must be called BEFORE WiFi.begin()
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Won't connect to WiFi | Server not running | Start server first (lab-w7-2-server) |
| "Connecting to WiFi..." loop | Wrong SSID/password | Verify credentials match server |
| Wrong IP address | Static IP not set | Verify WiFi.config() before WiFi.begin() |
| No response to TEMP/HUMI | UDP packet lost | Try sending command again |
| Response shows wrong data | Timing issue | Wait 2+ seconds between requests |
| Input ignored | Serial not ready | Wait for "Connected to WiFi" message |
| WiFi disconnects | Weak signal | Move ESP32s closer together |
| No serial output | Wrong baud rate | Set Serial Monitor to 9600 baud |
| Response timeout | Server not running | Check server Serial Monitor |
| Case sensitivity issue | Wrong command | Type "TEMP" not "temp" |

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
unsigned long timeout = 0;

String SENSOR_DATA;

void setup() {
  Serial.begin(9600);
  WiFi.config(ip, gateway, subnet);

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

  if (Serial.available() > 0) {
    SENSOR_DATA = Serial.readStringUntil('\n');
    SENSOR_DATA.trim();

    udp.beginPacket(host, port);
    udp.print(SENSOR_DATA);
    udp.endPacket();

  }

  int packetSize = udp.parsePacket();
  if (packetSize) {
    char packetBuffer[255];
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String input(packetBuffer);
      if(SENSOR_DATA == "TEMP"){
        Serial.println("Received (Temperature) : " + input);
      }
      if(SENSOR_DATA == "HUMI"){
        Serial.println("Received (Humidity) : " + input);
      }
    }
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
│ Input Box: [TEMP  ][Send]
│            [HUMI  ][Send]
└─────────────────────────┘

Valid Commands:
  TEMP → Temperature request
  HUMI → Humidity request

Note: Case sensitive!
  "TEMP" ✓  "temp" ✗
  "HUMI" ✓  "humi" ✗
```

## UDP Request-Response Pattern

```
Traditional UDP (Week 7-1):
  Client ──► Server
         (fire and forget, no response)

This Lab (Request-Response):
  Client ──► Server: "TEMP"
         ◄── Server: "25.30"
         (UDP with response)

Advantages:
  - Fast (no TCP overhead)
  - Simple request-response
  - Real-time sensor queries

Limitations:
  - No delivery guarantee
  - No automatic retries
  - Packets may be lost or out of order
```

## Network Architecture

This lab demonstrates **UDP request-response communication**:
- **Server**: Creates WiFi Access Point, reads DHT22 sensor
- **Client**: Sends sensor query commands via UDP
- **Protocol**: UDP for fast, low-latency sensor queries
- **Static IP**: Client uses fixed IP for predictable routing
- **Response**: Server sends sensor data back to client

## Applications

- Remote sensor monitoring
- Weather station queries
- Environmental monitoring
- HVAC system data
- Greenhouse automation
- Smart home sensors
- Industrial sensor networks
- IoT data collection

## Enhancements

- Add OLED display to show sensor data
- Implement automatic polling (query every X seconds)
- Add data logging to SD card
- Display trend graphs
- Implement multiple sensor support
- Add timestamp to sensor readings
- Calculate average/min/max values
- Add alert thresholds (high/low temp)
- Implement retry mechanism for failed queries
- Add encryption for data security
- Support multiple clients querying same server
