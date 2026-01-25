# Lab W4-2: DHT22 Temperature and Humidity Sensor

**File**: `lab-w4-2.ino`

## Description

This lab demonstrates reading temperature and humidity data from a DHT22 digital sensor. The Arduino reads and displays temperature (Celsius and Fahrenheit) and humidity percentage via the Serial Monitor.

## Components Required

- Arduino UNO R3 × 1
- DHT22 Temperature/Humidity Sensor × 1
- Resistor 10kΩ × 1 (pull-up resistor, often built into sensor module)
- Breadboard
- Jumper wires

## Pin Configuration

| Arduino Pin | Component | Connection | Notes                              |
| ----------- | --------- | ---------- | ---------------------------------- |
| Pin 2       | DHT22     | Data pin   | Digital I/O with one-wire protocol |
| 5V          | DHT22     | VCC/+      | Power supply                       |
| GND         | DHT22     | GND/-      | Ground                             |

## ASCII Wiring Diagram

```
        Arduino UNO              DHT22 Sensor
      ┌─────────────┐          ┌─────────────┐
      │             │          │    DHT22    │
      │   5V     ●──┼──────────┤ VCC  (+)    │
      │             │          │             │
      │   Pin 2  ●──┼──────────┤ DATA        │
      │             │          │             │
      │   GND    ●──┼──────────┤ GND  (-)    │
      └─────────────┘          └─────────────┘

Note: Some DHT22 modules have built-in pull-up resistor
```

## DHT22 Sensor Pinout

**4-Pin DHT22 (raw sensor)**:

```
  Front View:
  ┌─────────┐
  │  DHT22  │
  │  Sensor │
  └┬─┬─┬─┬─┘
   1 2 3 4
   │ │ │ │
   │ │ │ └─── NC (Not Connected)
   │ │ └───── DATA → Pin 2
   │ └─────── VCC → 5V
   └───────── GND → GND
```

**3-Pin DHT22 Module**:

```
  ┌─────────┐
  │  DHT22  │
  │ Module  │
  └─┬─┬─┬──┘
    + D -
    │ │ │
    │ │ └──── GND → GND
    │ └────── DATA → Pin 2
    └──────── VCC → 5V
```

## Breadboard Layout

1. Place the DHT22 sensor on the breadboard
2. Connect VCC/+ pin to Arduino 5V
3. Connect DATA pin to Arduino pin 2
4. Connect GND/- pin to Arduino GND
5. **If using raw 4-pin sensor**: Add 10kΩ pull-up resistor between DATA and VCC
6. **If using module**: Pull-up resistor usually included on board

## Component-Specific Details

### DHT22 Sensor

- **Type**: Digital temperature and humidity sensor
- **Temperature Range**: -40°C to 80°C (±0.5°C accuracy)
- **Humidity Range**: 0-100% RH (±2% accuracy)
- **Protocol**: One-wire digital communication
- **Sampling Rate**: Maximum 0.5 Hz (once every 2 seconds)
- **Operating Voltage**: 3.3-5.5V
- **Library Required**: DHT sensor library by Adafruit

### Pull-up Resistor

- **Value**: 10kΩ
- **Purpose**: Required for one-wire communication
- **Note**: Often integrated in pre-assembled modules

## Special Notes

- **Reading Interval**: Wait at least 2 seconds between readings
- **Library**: Uses Adafruit DHT sensor library
- **Data Protocol**: Custom one-wire protocol (not I2C or SPI)
- **Error Handling**: Code checks for NaN (Not a Number) to detect read failures
- **Temperature Units**: Displays both Celsius and Fahrenheit
- Serial baud rate: 9600

## How It Works

1. **Initialization**:
   - DHT library initialized with pin 2 and sensor type (DHT22)
   - Sensor calibration begins

2. **Data Reading**:
   - Wait 2 seconds (sensor sampling limitation)
   - Request temperature and humidity from sensor
   - Sensor returns digital data via one-wire protocol

3. **Data Processing**:
   - `readHumidity()`: Returns humidity percentage
   - `readTemperature()`: Returns temperature in Celsius
   - `readTemperature(true)`: Returns temperature in Fahrenheit

4. **Error Checking**:
   - `isnan()` checks if reading failed
   - If failed, display error and skip this reading

5. **Serial Output**:
   - Display humidity, temperature (C), and temperature (F)
   - Separator line for readability

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Open Serial Monitor**:
   - Set baud rate to 9600
3. **Wait for readings**:
   - First reading appears after 2 seconds
   - Updates every 2 seconds
4. **Verify temperature**:
   - Compare with room thermometer
   - Should be within ±0.5°C
5. **Verify humidity**:
   - Typical indoor humidity: 30-60%
   - Should be within ±2%
6. **Test response**:
   - Breathe on sensor → humidity should increase
   - Place hand near sensor → temperature should increase slightly

## Serial Monitor Output Examples

```
Humidity: 45.20
Temp (C): 23.50
Temp (F): 74.30
===================
Humidity: 45.30
Temp (C): 23.60
Temp (F): 74.48
===================
```

## Troubleshooting

| Issue                       | Possible Cause         | Solution                                 |
| --------------------------- | ---------------------- | ---------------------------------------- |
| "Failed to read" error      | Loose connection       | Check all wire connections               |
| Always reads same value     | Sensor not responding  | Check power connections (VCC, GND)       |
| No output                   | Wrong baud rate        | Set Serial Monitor to 9600 baud          |
| Erratic readings            | No pull-up resistor    | Add 10kΩ between DATA and VCC            |
| Reading too slow            | Normal behavior        | DHT22 limited to 0.5 Hz sampling         |
| Temperature off by constant | Sensor calibration     | Note offset and apply correction in code |
| NaN errors                  | Timing issue           | Increase delay between readings          |
| Sensor hot                  | Wrong power connection | Immediately disconnect, check wiring     |

## Understanding DHT22 Communication

The DHT22 uses a custom one-wire protocol:

```
Arduino sends start signal → DHT22 responds →
DHT22 sends 40 bits of data:
  - 16 bits: Humidity (0.1% resolution)
  - 16 bits: Temperature (0.1°C resolution)
  - 8 bits: Checksum
Arduino verifies checksum → Extract values
```

## Code Reference

```cpp
#include "DHT.h"

#define PIN_DHT 2
#define DHTTYPE DHT22

DHT dht(PIN_DHT, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000);  // DHT22 requires 2 second minimum interval
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if(isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("ERROR: Failed to Read from DHT Sensors.");
    return;
  }
  // Display values...
}
```

## Temperature Conversion

The sensor can return temperature in Celsius or Fahrenheit:

```cpp
readTemperature()      // Returns Celsius
readTemperature(true)  // Returns Fahrenheit

Formula: °F = (°C × 9/5) + 32
Example: 23.5°C = (23.5 × 1.8) + 32 = 74.3°F
```

## Library Installation

Required library: **DHT sensor library by Adafruit**

Install via Arduino IDE:

1. Sketch → Include Library → Manage Libraries
2. Search for "DHT sensor library"
3. Install "DHT sensor library by Adafruit"
4. Also install "Adafruit Unified Sensor" (dependency)

## Applications

- Weather stations
- Home automation (HVAC control)
- Greenhouse monitoring
- Server room monitoring
- Humidity-controlled systems
- Data logging

## Comparison: DHT22 vs DHT11

| Feature              | DHT22       | DHT11     |
| -------------------- | ----------- | --------- |
| Temperature Range    | -40 to 80°C | 0 to 50°C |
| Temperature Accuracy | ±0.5°C      | ±2°C      |
| Humidity Range       | 0-100%      | 20-80%    |
| Humidity Accuracy    | ±2%         | ±5%       |
| Sampling Rate        | 0.5 Hz      | 1 Hz      |
| Price                | Higher      | Lower     |
