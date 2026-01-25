# Lab W7-2-SERVER: ESP32 WiFi UDP DHT22 Temperature/Humidity Sensor Server

**File**: `lab-w7-2-server.ino`

## Description
This lab demonstrates ESP32 WiFi UDP server with DHT22 temperature/humidity sensor. The ESP32 creates a WiFi Access Point, continuously reads sensor data, and responds to UDP requests for temperature ("TEMP") or humidity ("HUMI") values. This is the **server** side that reads the DHT22 sensor and responds to client queries (lab-w7-2-client).

## Components Required
- ESP32 DEVKIT × 1
- DHT22 (AM2302) Temperature/Humidity Sensor × 1
- 10kΩ pull-up resistor × 1 (some DHT22 modules have built-in resistor)
- Breadboard
- Jumper wires

## Pin Configuration

| ESP32 Pin | Component | Connection | Notes |
|-----------|-----------|------------|-------|
| GPIO 33 | DHT22 | Data pin (middle) | Digital I/O |
| 3.3V | DHT22 | VCC (left/+) | Power supply |
| GND | DHT22 | GND (right/-) | Common ground |
| GPIO 33 to 3.3V | 10kΩ resistor | Pull-up | May be built into module |

## ASCII Wiring Diagram

```
        ESP32 DEVKIT         DHT22 Sensor
      ┌─────────────┐       
      │             │       ┌────────────┐
      │  3.3V    ●──┼───────┤ VCC   (+)  │
      │             │       │            │
      │             │   ┌───┤ DATA       │
      │  GPIO 33 ●──┼───┤   │            │
      │             │   │   │            │
      │             │   │   │            │
      │  GND     ●──┼───┼───┤ GND   (-)  │
      │             │   │   └────────────┘
      └─────────────┘   │
                        │
                   [10kΩ Pull-up]
                        │
        3.3V ───────────┘
      
      WiFi: Creates AP "ESP32-Nihahaha"
      UDP Server: Listens on port 6969
      IP Address: 192.168.4.1
      
      Receives: "TEMP" or "HUMI"
      Sends: Temperature or Humidity value
      
      Example:
        Request: "TEMP"  → Response: "25.30"
        Request: "HUMI"  → Response: "60.50"
```

## Breadboard Layout

1. Place ESP32 on breadboard
2. Place DHT22 sensor on breadboard
3. Connect DHT22:
   - VCC (leftmost pin, facing front) → 3.3V
   - DATA (middle pin) → GPIO 33
   - GND (rightmost pin) → GND
4. Add 10kΩ pull-up resistor between DATA (GPIO 33) and 3.3V
5. **Note**: Some DHT22 modules have built-in pull-up resistor

## Component-Specific Details

### ESP32 DEVKIT
- **WiFi**: 802.11 b/g/n
- **Operating Voltage**: 3.3V logic, 5V power
- **GPIO 33**: ADC1_CH5, suitable for digital I/O
- **WiFi Mode**: Access Point (AP) mode

### DHT22 (AM2302) Sensor
- **Type**: Digital temperature and humidity sensor
- **Temperature Range**: -40°C to 80°C (±0.5°C accuracy)
- **Humidity Range**: 0% to 100% RH (±2% accuracy)
- **Operating Voltage**: 3.3V to 6V (3.3V recommended for ESP32)
- **Data Protocol**: Single-wire digital (proprietary)
- **Sampling Period**: 2 seconds (max polling rate)
- **Pins** (facing front, left to right):
  1. VCC (power, +)
  2. DATA (signal)
  3. NC (not connected)
  4. GND (ground, -)
- **Pull-up Resistor**: 10kΩ required on DATA line

### DHT Library
- **Library**: "DHT sensor library" by Adafruit
- **Dependencies**: "Adafruit Unified Sensor" library
- **Functions**:
  - `readTemperature()`: Returns temperature in °C
  - `readTemperature(true)`: Returns temperature in °F
  - `readHumidity()`: Returns relative humidity %

## Network Configuration

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│(lab-w7-2-client) │   "ESP32-Nihahaha"    │  (This Device)   │
│  192.168.4.5     │                       │  192.168.4.1     │
└──────────────────┘                       └──────────────────┘
       │                                           │
   Serial Input                                    │
   (TEMP/HUMI)                              DHT22 (GPIO 33)
       │                                           │
       │  UDP Request: "TEMP" or "HUMI"           │
       └─────────────────────────────────────────►│
       ◄──────────────────────────────────────────┘
         UDP Response: "25.30" or "60.50"
            192.168.4.1:6969
```

### Network Details
- **SSID**: "ESP32-Nihahaha"
- **Password**: "12345678"
- **Server IP**: 192.168.4.1 (Access Point IP)
- **Server Port**: 6969
- **Protocol**: UDP (WiFiUDP)
- **WiFi Mode**: AP+STA (Access Point + Station)
- **Client IP**: Typically 192.168.4.5 (static on client)

## Special Notes

- **WiFi Mode**: Access Point (AP) mode - creates its own WiFi network
- **Protocol**: UDP (connectionless, request-response pattern)
- **Data Format**: 
  - Request: "TEMP" or "HUMI"
  - Response: Decimal number string (e.g., "25.30")
- **Sensor Reading**: Every 2 seconds in loop
- **Error Handling**: Checks for NaN (failed sensor read)
- **Response Routing**: Uses `udp.remoteIP()` and `udp.remotePort()` to reply
- **Case Sensitive**: Commands must match exactly ("TEMP" not "temp")
- Serial baud rate: 9600 (note: code has lowercase 'serial', may cause compile error)

## How It Works

1. **WiFi Setup**:
   - ESP32 creates Access Point "ESP32-Nihahaha"
   - Sets mode to WIFI_AP_STA
   - Obtains IP address 192.168.4.1
   - Prints AP IP to Serial Monitor

2. **UDP Initialization**:
   - `udp.begin(port)` starts listening on port 6969
   - Server ready to receive UDP packets

3. **DHT22 Initialization**:
   - Initialize DHT22 sensor on GPIO 33
   - Sensor ready to provide readings

4. **Sensor Reading Loop**:
   - 2-second delay (DHT22 requirement)
   - Read humidity: `dht.readHumidity()`
   - Read temperature (Celsius): `dht.readTemperature()`
   - Read temperature (Fahrenheit): `dht.readTemperature(true)`
   - Validate readings (check for NaN)

5. **UDP Request Processing**:
   - Parse incoming UDP packet
   - Read packet data into buffer
   - Convert to string
   - Check command:
     - If "TEMP": Send temperature value
     - If "HUMI": Send humidity value
   - Use `udp.remoteIP()` and `udp.remotePort()` for response routing

6. **UDP Response**:
   - Begin packet to client's IP and port
   - Convert sensor value to string
   - Send response
   - End packet

## Testing Instructions

1. **Install DHT Sensor Library**:
   - Arduino IDE → Tools → Manage Libraries
   - Search "DHT sensor library"
   - Install "DHT sensor library" by Adafruit
   - Also install "Adafruit Unified Sensor"

2. **Upload Server Sketch**:
   - Upload this sketch to ESP32 server
   - Open Serial Monitor (9600 baud)

3. **Verify Access Point**:
   - Check Serial Monitor for "AP IP: 192.168.4.1"
   - On your phone/computer, you should see "ESP32-Nihahaha" WiFi network

4. **Check DHT22 Readings**:
   - Wait 2 seconds
   - Server should start reading sensor
   - If sensor working, no error messages
   - If "ERROR: Failed to read from DHT sensor", check wiring

5. **Setup Client**:
   - Upload lab-w7-2-client.ino to another ESP32
   - Client should connect to "ESP32-Nihahaha" AP
   - Open client's Serial Monitor

6. **Test Temperature Query**:
   - On client, type `TEMP`
   - Client should receive temperature value
   - Client displays: "Received (Temperature): 25.30"
   - Value should match server's current reading

7. **Test Humidity Query**:
   - On client, type `HUMI`
   - Client should receive humidity value
   - Client displays: "Received (Humidity): 58.70"

8. **Verify Sensor Accuracy**:
   - Compare readings with known temperature
   - Breathe on sensor (humidity should increase)
   - Hold sensor (temperature should increase)

## Serial Monitor Output Examples

```
ap ip: 192.168.4.1
(If sensor working, no error messages)

(Server continuously reads sensor every 2 seconds,
 but doesn't print readings to Serial in this code)

(When client sends request, no visible output)
```

**Note**: This server code doesn't print sensor values or received requests to Serial Monitor. Add debug prints if needed.

## Data Protocol

**Request Format** (Client → Server):
```
Command (string)

Examples:
  "TEMP"  → Request temperature
  "HUMI"  → Request humidity
```

**Response Format** (Server → Client):
```
Sensor Value (string)

Examples:
  "25.30"  → Temperature 25.30°C
  "60.50"  → Humidity 60.50% RH
```

**UDP Transaction**:
```
1. Server reads DHT22 every 2 seconds
   - h = 58.70 (humidity)
   - t = 25.30 (temperature)
2. Client sends UDP "TEMP" to 192.168.4.1:6969
3. Server receives packet from 192.168.4.5:6969
4. Server parses: input = "TEMP"
5. Server checks: if (input == "TEMP")
6. Server sends UDP to 192.168.4.5:6969
7. Server writes: String(t) = "25.30"
8. Client receives "25.30"
```

## DHT22 Timing

```
DHT22 Specifications:

Sampling Rate: 0.5 Hz (max)
Minimum Interval: 2 seconds between readings

Reading Sequence:
  0ms:  Start signal
  1ms:  Response signal
  40ms: Data transmission (40 bits)
  
Temperature Data:
  16-bit: Sign + 15-bit value
  Resolution: 0.1°C
  
Humidity Data:
  16-bit: 0-100% RH
  Resolution: 0.1%
  
Error Checking:
  8-bit checksum

If read < 2 seconds apart:
  Previous value returned or NaN
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| "Failed to read from DHT sensor" | Wiring error | Check VCC, DATA, GND connections |
| Sensor always reads 0 | No pull-up resistor | Add 10kΩ resistor DATA to 3.3V |
| Erratic readings | Bad connection | Check breadboard connections |
| Sensor too slow | Reading too fast | Ensure 2-second delay in loop |
| No WiFi AP visible | Code not running | Check Serial Monitor, verify upload |
| Client no response | UDP port mismatch | Verify both use port 6969 |
| Compile error "serial" | Case sensitivity | Change 'serial' to 'Serial' in code |
| Compile error "wifi" | Case sensitivity | Change 'wifi' to 'WiFi' in code |
| Wrong temperature range | °F instead of °C | Use readTemperature() not readTemperature(true) |
| NaN values | Sensor failure | Replace DHT22 sensor |

## Code Issues & Fixes

⚠️ **Code has case sensitivity errors**:

```cpp
// WRONG (in original code):
serial.begin(9600);
serial.println("ap ip: ");
wifi.mode(wifi_ap_sta);

// CORRECT:
Serial.begin(9600);
Serial.println("AP IP: ");
WiFi.mode(WIFI_AP_STA);
```

## Code Reference (Corrected)

```cpp
#include <WiFi.h>
#include <WiFiUdp.h>
#include "DHT.h"

#define PIN_DHT 33
#define DHTTYPE DHT22

// DHT sensor object
DHT dht(PIN_DHT, DHTTYPE);

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
WiFiUDP udp;

char packetBuffer[255];

void setup() {
  Serial.begin(9600);  // FIXED: capital S

  WiFi.mode(WIFI_AP_STA);  // FIXED: capital W
  WiFi.softAP(ssid, password);
  Serial.print("AP IP: ");  // FIXED: capital S
  Serial.println(WiFi.softAPIP());

  udp.begin(port);
  dht.begin();
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("ERROR: Failed to read from DHT sensor");
    return;
  }

  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String input(packetBuffer);

      if (input == "TEMP") {
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.print(String(t));
        udp.endPacket();
      }
      if (input == "HUMI") {
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.print(String(h));
        udp.endPacket();
      }
    }
  }
}
```

## DHT22 vs DHT11

| Feature | DHT22 (this lab) | DHT11 |
|---------|------------------|-------|
| Temperature Range | -40 to 80°C | 0 to 50°C |
| Temperature Accuracy | ±0.5°C | ±2°C |
| Humidity Range | 0-100% | 20-80% |
| Humidity Accuracy | ±2% | ±5% |
| Resolution | 0.1° / 0.1% | 1° / 1% |
| Sampling Rate | 0.5 Hz (2 sec) | 1 Hz (1 sec) |
| Price | Higher | Lower |
| Pins | 4-pin | 4-pin or 3-pin |

## Network Architecture

This lab demonstrates **UDP sensor query server**:
- **Server**: Creates WiFi Access Point, reads DHT22 sensor
- **Client**: Sends query commands, receives sensor data
- **Protocol**: UDP request-response pattern
- **Real-time**: Sensor data available on-demand
- **Remote IP**: Uses `udp.remoteIP()` for response routing

## Applications

- Weather stations
- Environmental monitoring
- Greenhouse automation
- HVAC control systems
- Smart home climate sensors
- Server room monitoring
- Food storage monitoring
- Museum/archive climate control
- Incubator monitoring
- Terrarium/aquarium monitoring

## Enhancements

- Add multiple DHT22 sensors
- Store historical data (time series)
- Implement data logging to SD card
- Add LCD display for local monitoring
- Calculate heat index / dew point
- Add alert thresholds (high/low)
- Implement auto-reporting (push data periodically)
- Add other sensors (pressure, light, CO2)
- Web interface for data visualization
- MQTT integration for IoT platforms
- Add timestamp to sensor readings
