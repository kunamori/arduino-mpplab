# Lab W4-3: DHT22 with LCD Display

**File**: `lab-w4-3.ino`

## Description
This lab combines the DHT22 temperature/humidity sensor with an I2C LCD display. Temperature and humidity readings are shown on a 16×2 LCD screen instead of the Serial Monitor, creating a standalone environmental monitoring device.

## Components Required
- Arduino UNO R3 × 1
- DHT22 Temperature/Humidity Sensor × 1
- LCD 16×2 with I2C module × 1
- Resistor 10kΩ × 1 (pull-up for DHT22, often built into sensor module)
- Breadboard
- Jumper wires

## Pin Configuration

| Arduino Pin | Component | Connection | Notes |
|------------|-----------|------------|-------|
| Pin 2 | DHT22 | Data pin | Digital I/O |
| 5V | DHT22 + LCD | VCC/+ | Shared power |
| GND | DHT22 + LCD | GND/- | Shared ground |
| SDA (A4) | LCD I2C | SDA | I2C data line |
| SCL (A5) | LCD I2C | SCL | I2C clock line |

## ASCII Wiring Diagram

```
        Arduino UNO          DHT22        LCD I2C
      ┌─────────────┐      ┌─────┐      ┌─────────┐
      │             │      │DHT22│      │ LCD 16×2│
      │   5V     ●──┼──┬───┤ VCC │      │  I2C    │
      │             │  │   └─────┘      │         │
      │             │  └────────────────┤ VCC     │
      │             │      ┌─────┐      │         │
      │   Pin 2  ●──┼──────┤DATA │      │         │
      │             │      └─────┘      │         │
      │   A4(SDA)●──┼──────────────────┤ SDA     │
      │             │                   │         │
      │   A5(SCL)●──┼──────────────────┤ SCL     │
      │             │      ┌─────┐      │         │
      │   GND    ●──┼──┬───┤ GND │      │         │
      │             │  │   └─────┘      │         │
      │             │  └────────────────┤ GND     │
      └─────────────┘                   └─────────┘
```

## I2C Connection Details

### I2C Pins on Arduino UNO
- **SDA**: Pin A4 (Data line)
- **SCL**: Pin A5 (Clock line)

### LCD I2C Module
- **I2C Address**: 0x27 (common) or 0x3F
- **Pins**: VCC, GND, SDA, SCL
- **Backlight**: Controlled via I2C (usually always on)

## Breadboard Layout

1. Place the LCD on the breadboard
2. Place the DHT22 sensor on the breadboard
3. **Power Connections**:
   - Arduino 5V → LCD VCC
   - Arduino 5V → DHT22 VCC
   - Arduino GND → LCD GND
   - Arduino GND → DHT22 GND
4. **I2C Connections**:
   - Arduino A4 (SDA) → LCD SDA
   - Arduino A5 (SCL) → LCD SCL
5. **DHT22 Data**:
   - Arduino Pin 2 → DHT22 DATA
6. **Optional**: Add 10kΩ pull-up resistor between DHT22 DATA and VCC (if not using module)

## Component-Specific Details

### DHT22 Sensor
- **Type**: Digital temperature and humidity sensor
- **Temperature Range**: -40°C to 80°C (±0.5°C accuracy)
- **Humidity Range**: 0-100% RH (±2% accuracy)
- **Protocol**: One-wire digital communication
- **Sampling Rate**: 0.5 Hz (2 second minimum interval)
- **Library**: DHT sensor library by Adafruit

### LCD 16×2 with I2C
- **Display**: 16 characters × 2 lines
- **Interface**: I2C (2-wire: SDA, SCL)
- **I2C Address**: 0x27 (default, configurable)
- **Backlight**: Blue/Green LED backlight (controlled via I2C)
- **Library**: LiquidCrystal_I2C
- **Advantages**: Reduces pin usage from 6+ to 2 pins

## Special Notes

- **I2C Address**: Code uses 0x27; if LCD doesn't work, try 0x3F
- **Backlight Control**: Can be turned on/off with `lcd.backlight()` / `lcd.noBacklight()`
- **Display Format**: 
  - Line 1: "Temp : XX.XC"
  - Line 2: "Humidity: XX.X%"
- **Continuous Update**: Display updates approximately every 2 seconds
- **No Serial Monitor**: All output shown on LCD
- **Startup Message**: "Power On." displayed for 2 seconds

## How It Works

1. **Initialization**:
   - LCD initialized with I2C address 0x27, 16 columns, 2 rows
   - Backlight turned on
   - Startup message "Power On." displayed for 2 seconds
   - DHT22 sensor initialized

2. **Main Loop**:
   - Read temperature (Celsius) and humidity from DHT22
   - Check for valid readings (not NaN)
   - If valid:
     - Display temperature on line 1: "Temp : XX.XC"
     - Display humidity on line 2: "Humidity: XX.X%"
   - If invalid: Skip update (previous values remain)
   - Repeat continuously

3. **Display Update**:
   - `lcd.setCursor(col, row)` positions cursor
   - `lcd.print()` displays text/values
   - Values overwrite previous display

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Observe startup**:
   - LCD should light up
   - "Power On." should appear for 2 seconds
3. **Check display**:
   - Line 1 should show: "Temp : XX.XC"
   - Line 2 should show: "Humidity: XX.X%"
   - Updates every ~2 seconds
4. **Verify readings**:
   - Temperature should match room temperature
   - Humidity should be reasonable (30-60% typical indoors)
5. **Test response**:
   - Breathe on DHT22 → humidity should increase
   - Place hand near sensor → temperature should increase

## LCD Display Layout

```
┌────────────────┐
│Temp : 23.5C    │  ← Line 0 (top)
│Humidity: 45.2% │  ← Line 1 (bottom)
└────────────────┘
  0123456789ABCDEF   (column positions)
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| LCD blank/no backlight | No power | Check VCC and GND connections |
| LCD shows blocks | Not initialized | Check I2C wiring (SDA, SCL) |
| No text on LCD | Wrong I2C address | Try 0x3F instead of 0x27 in code |
| Garbled text | I2C communication error | Check SDA (A4) and SCL (A5) connections |
| DHT values not updating | Sensor disconnected | Check DHT22 wiring |
| Always shows old values | DHT read error | Check DHT22 DATA pin connection |
| Backlight too bright | Normal | Adjust LCD contrast potentiometer on I2C module |
| Display flickers | Power supply issue | Ensure stable 5V power supply |
| Wrong values | Sensor placement | Ensure DHT22 not near heat source |

## Finding I2C Address

If 0x27 doesn't work, run an I2C scanner sketch to find the correct address:

```cpp
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Scanning I2C...");
  
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

## Code Reference

```cpp
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define PIN_DHT 2
#define DHTTYPE DHT22

DHT dht(PIN_DHT, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address, columns, rows

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.home();
  lcd.print("Power On.");
  delay(2000);
  lcd.clear();
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if(!isnan(h) && !isnan(t)) {
    lcd.setCursor(0,0);
    lcd.print("Temp : ");
    lcd.print(t);
    lcd.print("C");
    
    lcd.setCursor(0,1);
    lcd.print("Humidity: ");
    lcd.print(h);
    lcd.print("%");
  }
}
```

## LCD Control Commands

| Command | Function |
|---------|----------|
| `lcd.init()` | Initialize LCD |
| `lcd.backlight()` | Turn on backlight |
| `lcd.noBacklight()` | Turn off backlight |
| `lcd.clear()` | Clear entire display |
| `lcd.home()` | Move cursor to (0,0) |
| `lcd.setCursor(col, row)` | Position cursor |
| `lcd.print(text)` | Display text/value |

## Libraries Required

1. **LiquidCrystal_I2C**:
   - Install via Library Manager
   - Search for "LiquidCrystal I2C"

2. **DHT sensor library**:
   - By Adafruit
   - Also needs "Adafruit Unified Sensor"

## Applications

- Standalone weather station
- Room climate monitor
- Greenhouse controller
- Server room monitoring
- Incubator temperature/humidity display
- HVAC system monitor

## Advantages of I2C LCD

- **Fewer Pins**: Uses only 2 pins (SDA, SCL) vs 6+ for parallel LCD
- **Easy Wiring**: Simple 4-wire connection
- **Multiple Devices**: Can share I2C bus with other I2C devices
- **Adjustable Contrast**: Potentiometer on I2C module
