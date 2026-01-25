# Lab W6-1-SERVER: ESP32 WiFi TCP Counter Server with LCD

**File**: `lab-w6-1-server.ino`

## Description
This lab demonstrates ESP32 WiFi TCP server functionality with I2C LCD display. The ESP32 creates a WiFi Access Point and listens for TCP connections from a client (lab-w6-1-client). Counter values received from the client are displayed on a 16×2 I2C LCD. This is the **server** side that receives data from the client.

## Components Required
- ESP32 DEVKIT × 1
- LCD 16×2 with I2C module × 1
- Breadboard
- Jumper wires

## Pin Configuration

| ESP32 Pin | Component | Connection | Notes |
|-----------|-----------|------------|-------|
| GPIO 21 (SDA) | LCD I2C | SDA | I2C data line |
| GPIO 22 (SCL) | LCD I2C | SCL | I2C clock line |
| 5V | LCD I2C | VCC | Power supply |
| GND | LCD I2C | GND | Common ground |

## ASCII Wiring Diagram

```
        ESP32 DEVKIT           LCD I2C (16×2)
      ┌─────────────┐       ┌──────────────┐
      │             │       │   ┌──────┐   │
      │  GPIO 21 ●──┼───────┼───┤ SDA  │   │
      │   (SDA)     │       │   │      │   │
      │             │       │   │ I2C  │   │
      │  GPIO 22 ●──┼───────┼───┤ SCL  │   │
      │   (SCL)     │       │   │      │   │
      │             │       │   │ 0x27 │   │
      │   5V     ●──┼───────┼───┤ VCC  │   │
      │             │       │   │      │   │
      │   GND    ●──┼───────┼───┤ GND  │   │
      └─────────────┘       │   └──────┘   │
                            └──────────────┘
      
      WiFi: Creates AP "ESP32-Nihahaha"
      TCP Server: Listens on port 6969
      IP Address: 192.168.4.1
```

## Breadboard Layout

1. Place ESP32 on breadboard
2. Connect LCD I2C module:
   - SDA → GPIO 21
   - SCL → GPIO 22
   - VCC → 5V
   - GND → GND
3. No external pull-up resistors needed (I2C module has them)

## Component-Specific Details

### ESP32 DEVKIT
- **WiFi**: 802.11 b/g/n
- **Operating Voltage**: 3.3V logic, 5V power
- **I2C**: Hardware I2C on GPIO 21 (SDA), GPIO 22 (SCL)
- **WiFi Mode**: Access Point (AP) mode

### LCD 16×2 with I2C Module
- **Display**: 16 characters × 2 lines
- **Interface**: I2C (2 wires: SDA, SCL)
- **I2C Address**: 0x27 (default, may be 0x3F on some modules)
- **Backlight**: Controllable via I2C
- **Operating Voltage**: 5V
- **Library**: LiquidCrystal_I2C

## Network Configuration

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│(lab-w6-1-client) │   "ESP32-Nihahaha"    │  (This Device)   │
│  192.168.4.x     │                       │  192.168.4.1     │
└──────────────────┘                       └──────────────────┘
       │                                           │
       └─── TCP Connection ──────────────────────►│
            192.168.4.1:6969                   Port 6969
                                                   │
                                            ┌──────▼──────┐
                                            │  LCD Display │
                                            │  Shows counter│
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
- **Data Format**: Receives counter value followed by "&" (e.g., "42&")
- **Display**: Counter value shown on LCD line 1
- **IP Display**: On startup, LCD shows server IP for 2 seconds
- **Connection Management**: Persistent server, accepts multiple sequential connections
- **I2C Address**: 0x27 (if LCD doesn't work, try 0x3F)
- Serial baud rate: 9600

## How It Works

1. **WiFi Setup**:
   - ESP32 creates Access Point "ESP32-Nihahaha"
   - Sets mode to WIFI_AP_STA
   - Obtains IP address 192.168.4.1
   - Starts TCP server on port 6969

2. **LCD Initialization**:
   - Initialize LCD with I2C address 0x27
   - Turn on backlight
   - Display "Server Started" and IP address
   - Clear after 2 seconds

3. **Client Connection**:
   - Wait for client connection
   - When client connects, print "New Client connected"
   - Maintain connection while client is connected

4. **Data Reception**:
   - Read incoming data until "&" delimiter
   - Parse received counter value
   - Clear LCD and display counter value
   - Print value to Serial Monitor

5. **Connection Closure**:
   - When client disconnects, close connection
   - Print "Client disconnected"
   - Return to waiting for next client

## Testing Instructions

1. **Upload Server Sketch**:
   - Upload this sketch to ESP32 server
   - Open Serial Monitor (9600 baud)

2. **Verify Access Point**:
   - Check Serial Monitor for "AP IP address: 192.168.4.1"
   - LCD should show "Server Started" then IP address
   - On your phone/computer, you should see "ESP32-Nihahaha" WiFi network

3. **Setup Client**:
   - Upload lab-w6-1-client.ino to another ESP32
   - Client should connect to "ESP32-Nihahaha" AP

4. **Test Counter Display**:
   - Press buttons on client
   - Serial Monitor shows:
     - "New Client connected" on each button press
     - "Received request: X" (counter value)
     - "Client disconnected"
   - LCD should update to show counter value

5. **Verify LCD Updates**:
   - Each button press on client updates LCD
   - Values 0-99 should display correctly
   - LCD clears before showing new value

## Serial Monitor Output Examples

```
AP IP address: 192.168.4.1
New Client connected
Received request: 0
Client disconnected
New Client connected
Received request: 1
Client disconnected
New Client connected
Received request: 2
Client disconnected
New Client connected
Received request: 99
Client disconnected
```

## LCD Display Examples

```
Startup (2 seconds):
┌────────────────┐
│Server Started  │
│IP:192.168.4.1  │
└────────────────┘

During Operation:
┌────────────────┐
│0               │
│                │
└────────────────┘

┌────────────────┐
│42              │
│                │
└────────────────┘

┌────────────────┐
│99              │
│                │
└────────────────┘
```

## Data Protocol

**Message Format**:
```
Counter Value + "&"

Examples:
  "0&"    → Display "0"
  "42&"   → Display "42"
  "99&"   → Display "99"
```

**TCP Server Flow**:
```
1. Server listens on port 6969
2. Client connects
3. Client sends "42&"
4. Server reads until '&'
5. Server displays "42" on LCD
6. Client disconnects
7. Server returns to listening
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| LCD shows nothing | No power | Check 5V and GND connections |
| LCD backlight only | Wrong I2C address | Try 0x3F instead of 0x27 |
| LCD garbled text | I2C wiring issue | Check SDA (21) and SCL (22) connections |
| No WiFi AP visible | Code not running | Check Serial Monitor, verify upload |
| AP visible but can't connect | Wrong password | Verify "12345678" password |
| Client can't connect | Server not started | Ensure server sketch running first |
| No serial output | Wrong baud rate | Set Serial Monitor to 9600 baud |
| LCD not updating | TCP port issue | Verify client connects to port 6969 |
| IP shows 0.0.0.0 | WiFi not started | Check WiFi.softAP() call |

## Finding I2C Address

If LCD doesn't work with address 0x27, scan for correct address:

```cpp
#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("I2C Scanner");
  
  for(byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if(Wire.endTransmission() == 0) {
      Serial.print("Found I2C device at 0x");
      Serial.println(addr, HEX);
    }
  }
}

void loop() {}
```

Common addresses: 0x27, 0x3F

## Code Reference

```cpp
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

// initialize the LCD library with the I2C address and dimensions
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
WiFiServer server(port);

void setup() {
  Serial.begin(9600);

  WiFi.softAP(ssid, password);
  WiFi.mode(WIFI_AP_STA);
  server.begin();
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Server Started");
  lcd.setCursor(0, 1);
  lcd.print("IP:");
  lcd.print(WiFi.softAPIP());
  delay(2000);
  lcd.clear();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client connected");

    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('&');
        Serial.print("Received request: ");
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print(request);
        Serial.println(request);
      }
    }
    client.stop();

    Serial.println("Client disconnected");
  }
}
```

## WiFi Modes

```
ESP32 WiFi Modes:
- WIFI_STA: Station mode (connects to AP)
- WIFI_AP: Access Point mode (creates AP)
- WIFI_AP_STA: Both AP and Station (this lab)
```

## TCP Server vs Client

| Feature | Server (this lab) | Client (lab-w6-1-client) |
|---------|-------------------|--------------------------|
| WiFi Mode | Access Point | Station |
| Creates Network | Yes | No |
| Initiates Connection | No | Yes |
| Listens | Yes | No |
| IP Address | Fixed (192.168.4.1) | DHCP assigned |

## Applications

- Wireless display systems
- IoT data visualization
- Remote monitoring dashboards
- Sensor data display
- Status displays
- Wireless counters
- Home automation displays

## Enhancements

- Add timestamp to LCD display
- Display multiple values on LCD
- Store received values in array
- Add OLED display instead of LCD
- Implement authentication
- Add web server interface
- Log data to SD card
- Support multiple simultaneous clients
