# Lab W4-1: LDR Light Sensor Reader

**File**: `lab-w4-1.ino`

## Description
This lab demonstrates analog sensor reading using a Light Dependent Resistor (LDR). The Arduino reads light intensity levels and displays them as a percentage (0-100%) on the Serial Monitor.

## Components Required
- Arduino UNO R3 × 1
- LDR (Light Dependent Resistor) × 1
- Resistor 10kΩ × 1 (pull-down resistor)
- Breadboard
- Jumper wires

## Pin Configuration

| Arduino Pin | Component | Connection | Notes |
|------------|-----------|------------|-------|
| A0 | LDR | Voltage divider midpoint | Analog input |
| 5V | LDR | One terminal | Power supply |
| GND | Resistor 10kΩ | One terminal | Ground |

## ASCII Wiring Diagram

```
        Arduino UNO              LDR Circuit
      ┌─────────────┐              
      │             │          5V
      │   5V     ●──┼───────────┬─────── LDR ─────┐
      │             │           │                  │
      │             │         [10kΩ]               │
      │             │           │                  │
      │   A0     ●──┼───────────┴──────────────────┘
      │             │           │
      │   GND    ●──┼───────────┘
      └─────────────┘
      
      Voltage Divider:
      5V ──── LDR ──── A0 ──── 10kΩ ──── GND
```

## Breadboard Layout

1. Connect one terminal of the LDR to Arduino 5V
2. Connect the other terminal of the LDR to Arduino pin A0
3. Connect one terminal of the 10kΩ resistor to Arduino pin A0 (same point as LDR)
4. Connect the other terminal of the 10kΩ resistor to GND
5. This forms a voltage divider circuit

## Component-Specific Details

### LDR (Light Dependent Resistor)
- **Type**: Photoresistor/photocell
- **Resistance Range**: 
  - Bright light: ~1kΩ
  - Darkness: ~100kΩ or higher
- **Response Time**: Typically 8-12ms
- **Operation**: Resistance decreases as light intensity increases
- **No polarity**: Can be connected either way

### 10kΩ Pull-down Resistor
- **Purpose**: Forms voltage divider with LDR
- **Function**: Converts resistance variation to voltage variation
- **Value**: 10kΩ provides good sensitivity range

## Special Notes

- **Voltage Divider Principle**: Output voltage varies with LDR resistance
- **ADC Reading**: Arduino's 10-bit ADC (0-1023) reads voltage at A0
- **Mapping**: Code maps ADC value (0-950) to percentage (0-100%)
- **Range Adjustment**: Maximum ADC value set to 950 (can be adjusted based on ambient light)
- **Update Rate**: Continuous reading (no delay in loop)
- Serial baud rate: 9600

## How It Works

1. **Voltage Divider**:
   - In bright light: LDR resistance is low → Higher voltage at A0
   - In darkness: LDR resistance is high → Lower voltage at A0

2. **Voltage Calculation**:
   ```
   V(A0) = 5V × (R_pulldown / (R_LDR + R_pulldown))
   ```

3. **ADC Reading**:
   - Arduino reads analog voltage at pin A0
   - Converts to digital value (0-1023)

4. **Mapping**:
   - Raw ADC value mapped from 0-950 to 0-100%
   - 0 = darkest, 100% = brightest

5. **Serial Output**:
   - Displays light intensity as percentage

## Voltage Divider Behavior

```
Bright Light (LDR = 1kΩ):
  V(A0) = 5V × (10kΩ / (1kΩ + 10kΩ)) = 4.55V
  ADC ≈ 931
  Display: ~100%

Moderate Light (LDR = 10kΩ):
  V(A0) = 5V × (10kΩ / (10kΩ + 10kΩ)) = 2.5V
  ADC ≈ 512
  Display: ~54%

Darkness (LDR = 100kΩ):
  V(A0) = 5V × (10kΩ / (100kΩ + 10kΩ)) = 0.45V
  ADC ≈ 93
  Display: ~10%
```

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Open Serial Monitor**:
   - Set baud rate to 9600
3. **Test in bright light**:
   - Point LDR at a bright light source
   - Reading should be high (70-100%)
4. **Test in darkness**:
   - Cover LDR with your hand
   - Reading should drop significantly (0-30%)
5. **Test in ambient light**:
   - Normal room lighting
   - Should read moderate values (30-70%)
6. **Verify response**:
   - LDR should respond quickly to light changes
   - Values should update continuously

## Serial Monitor Output Examples

```
87%
85%
42%
15%
3%
89%
100%
```

## Calibration

If your readings don't reach 0% or 100%, adjust the mapping range in the code:

```cpp
// Original
int LDR_DIS = map(LDR_OUT, 0, 950, 0, 100);

// Adjust upper limit based on brightest reading
int LDR_DIS = map(LDR_OUT, 0, 900, 0, 100);  // If max reading is ~900
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Always reads 0% | LDR not connected to 5V | Check LDR to 5V connection |
| Always reads 100% | Missing pull-down resistor | Verify 10kΩ resistor to GND |
| No variation | Wrong resistor value | Confirm 10kΩ resistor (Brown-Black-Orange) |
| Inverted readings | LDR and resistor swapped | Swap positions (LDR to 5V, resistor to GND) |
| Erratic readings | Loose connections | Check all wire connections |
| No serial output | Wrong baud rate | Set Serial Monitor to 9600 baud |
| Reading stuck at mid-value | A0 floating | Ensure A0 connected to junction point |
| Too sensitive/insensitive | Wrong resistor value | Try different pull-down values (4.7kΩ-22kΩ) |

## Understanding the Circuit

**Voltage Divider Configuration**:
```
        5V
         │
        LDR  ← Variable resistance (light dependent)
         │
         ├──── A0 (Arduino measures this voltage)
         │
       10kΩ  ← Fixed resistance
         │
        GND
```

**How it works**:
- More light → LDR resistance decreases → A0 voltage increases
- Less light → LDR resistance increases → A0 voltage decreases

## Code Reference

```cpp
#define LDR A0

void loop() {
  int LDR_OUT = analogRead(LDR);
  int LDR_DIS = map(LDR_OUT, 0, 950, 0, 100);
  Serial.print(LDR_DIS);
  Serial.println("%");
}
```

## Applications

- Automatic night lights (turn on LED when dark)
- Light level monitoring
- Solar panel tracking
- Camera exposure meters
- Energy-saving lighting systems

## Optional Enhancements

- Add LED that turns on when LDR_DIS < 30% (dark)
- Add averaging to smooth readings
- Display on LCD instead of Serial Monitor
- Trigger actions based on light thresholds
- Log data over time for analysis
