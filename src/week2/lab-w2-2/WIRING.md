# Lab W2-2: Potentiometer Voltage Reader

**File**: `lab-w2-2.ino`

## Description
This lab demonstrates analog input reading using a potentiometer. The Arduino reads the analog voltage (0-5V) from the potentiometer and displays the value on the Serial Monitor.

## Components Required
- Arduino UNO R3 × 1
- Potentiometer 10kΩ × 1
- Breadboard
- Jumper wires

## Pin Configuration

| Arduino Pin | Component | Connection | Notes |
|------------|-----------|------------|-------|
| A0 | Potentiometer | Wiper (middle pin) | Analog input |
| 5V | Potentiometer | One end terminal | Power supply |
| GND | Potentiometer | Other end terminal | Ground |

## ASCII Wiring Diagram

```
        Arduino UNO              Potentiometer
      ┌─────────────┐              
      │             │          ┌───────────┐
      │   5V     ●──┼──────────┤ Terminal 1│ (VCC)
      │             │          │           │
      │   A0     ●──┼──────────┤  Wiper    │ (Variable)
      │             │          │           │
      │   GND    ●──┼──────────┤ Terminal 2│ (GND)
      └─────────────┘          └───────────┘
```

## Breadboard Layout

1. Place the potentiometer on the breadboard
2. Connect one outer terminal to Arduino 5V (power rail)
3. Connect the center terminal (wiper) to Arduino pin A0
4. Connect the other outer terminal to Arduino GND (ground rail)
5. The wiper voltage varies from 0V to 5V as the potentiometer is rotated

## Component-Specific Details

### Potentiometer (10kΩ)
- **Type**: Variable resistor (rotary potentiometer)
- **Resistance**: 10kΩ (10,000 ohms)
- **Rotation**: Typically 270-300 degrees
- **Terminals**:
  - Terminal 1: Connected to 5V
  - Wiper (middle): Connected to A0, outputs 0-5V
  - Terminal 2: Connected to GND
- **Operation**: As you rotate, the wiper voltage varies linearly from 0V to 5V

## Special Notes

- **ADC Resolution**: Arduino UNO has a 10-bit ADC (0-1023 values)
- **Voltage Mapping**: 0-1023 mapped to 0.00-5.00 volts
- **Calculation**: Voltage = (ADC_Value / 1023) × 5
- **Reading Interval**: 100ms delay between readings
- **Serial Output**: Displays voltage in volts (e.g., "2.45")
- Serial baud rate: 9600

## How It Works

1. **Analog Reading**: Arduino reads analog value from pin A0 (0-1023)
2. **Voltage Conversion**: 
   - Raw value (0-1023) is read from `analogRead(A0)`
   - Divided by 1023 to get ratio (0.0 to 1.0)
   - Multiplied by 5 to get voltage (0.0V to 5.0V)
3. **Serial Output**: Calculated voltage printed to Serial Monitor
4. **Update Rate**: Readings taken every 100ms

## Voltage Calculation

```
ADC Value Range: 0 to 1023
Voltage Range: 0V to 5V

Formula: Voltage = (ADC_Value / 1023) × 5

Examples:
  ADC = 0    → Voltage = (0/1023) × 5 = 0.00V
  ADC = 512  → Voltage = (512/1023) × 5 = 2.50V
  ADC = 1023 → Voltage = (1023/1023) × 5 = 5.00V
```

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Open Serial Monitor**:
   - Set baud rate to 9600
3. **Rotate potentiometer**:
   - Turn fully counter-clockwise → Should read ~0.00V
   - Turn to middle position → Should read ~2.50V
   - Turn fully clockwise → Should read ~5.00V
4. **Verify smooth transition**:
   - Slowly rotate and observe voltage change smoothly
   - Values should update approximately 10 times per second
5. **Check accuracy**:
   - Compare with multimeter reading if available
   - Should match within ±0.05V

## Serial Monitor Output Examples

```
0.00
0.12
0.49
1.23
2.50
3.87
4.91
5.00
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Always reads 0.00V | Wiper not connected to A0 | Check middle pin to A0 connection |
| Always reads 5.00V | Wrong wiper connection | Verify middle pin is to A0, not outer pin |
| No change when rotating | Wrong potentiometer terminal | Swap the outer terminals |
| Erratic readings | Loose connection | Check all connections, especially wiper |
| Reading noise/jitter | Normal ADC noise | Add small capacitor (0.1µF) across A0 and GND |
| No serial output | Wrong baud rate | Set Serial Monitor to 9600 baud |
| Value backwards | Terminals swapped | Swap the 5V and GND connections |
| Reading stuck | Faulty potentiometer | Test continuity, replace if needed |

## Understanding Analog Input

The Arduino's analog-to-digital converter (ADC) measures the voltage at pin A0 and converts it to a digital value:

```
Physical Voltage → ADC → Digital Value (0-1023)

0.0V  →  ADC  →  0
1.0V  →  ADC  →  204
2.5V  →  ADC  →  512
3.3V  →  ADC  →  675
5.0V  →  ADC  →  1023
```

The code then converts this back to voltage for display:
```
Digital Value → Calculation → Voltage Display
```

## Code Reference

```cpp
#define POTEN_1 A0
float POTEN_VALUE;

void loop() {
  POTEN_VALUE = analogRead(POTEN_1);
  POTEN_VALUE = (POTEN_VALUE/1023) * 5;
  Serial.println(POTEN_VALUE);
  delay(100);
}
```

## Optional Enhancements

- Add averaging to reduce noise (read 10 times, take average)
- Map voltage to percentage (0-100%)
- Use voltage to control LED brightness with PWM
- Display on LCD instead of Serial Monitor
