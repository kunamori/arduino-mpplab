# Lab W3-3: 7-Segment Display with Serial Input

**File**: `lab-w3-3.ino`

## Description

This lab demonstrates multiplexed dual 7-segment display control via serial input. Users can display any number from 00-99 by sending values through the Serial Monitor, using direct port manipulation for efficient display refresh.

## Components Required

- Arduino UNO R3 × 1
- 18-pin Dual 7-Segment Display (Common Anode) × 1
- Resistors 220Ω × 7 (optional, for current limiting)
- Variable Resistor 10kΩ × 1 (optional, for brightness control)
- Breadboard
- Jumper wires
- USB cable for serial communication

## Pin Configuration

| Arduino Pin | Function        | Connection             | Notes          |
| ----------- | --------------- | ---------------------- | -------------- |
| Pin 8       | Segment A       | Display pins 10, 18    | PORTB bit 0    |
| Pin 9       | Segment B       | Display pins 7, 16     | PORTB bit 1    |
| Pin 10      | Segment C       | Display pins 4, 14     | PORTB bit 2    |
| Pin 11      | Segment D       | Display pins 2, 12     | PORTB bit 3    |
| Pin 12      | Segment E       | Display pins 1, 11     | PORTB bit 4    |
| Pin 13      | Segment F       | Display pins 9, 17     | PORTB bit 5    |
| Pin 6       | Segment G       | Display pins 5, 15     | Digital output |
| Pin 5       | Digit 1 Control | Display pin 6 (CA)     | Common Anode 1 |
| Pin 4       | Digit 2 Control | Display pin 8 (CA)     | Common Anode 2 |
| GND         | Common          | Segments via resistors | Ground         |

## ASCII Wiring Diagram

```
    Arduino UNO                 7-Segment Display
  ┌─────────────┐              ┌─────────────────┐
  │             │              │  ┌───┐  ┌───┐  │
  │ PORTB (8-13)├──────────────┤  │ 8 │  │ 8 │  │
  │             │   Segments   │  └───┘  └───┘  │
  │   Pin 6  ●──┼─────────────Seg G              │
  │   Pin 5  ●──┼─────────────CA Digit 1         │
  │   Pin 4  ●──┼─────────────CA Digit 2         │
  │             │                                 │
  │   GND    ●──┼─────────────────────────────────
  │   USB    ●──┼─── To Computer (Serial Input)
  └─────────────┘
```

## 18-Pin Dual 7-Segment Display Pinout (Common Anode)

```
       Top View of Display
    ┌─────────────────────┐
    │  ┌───┐       ┌───┐  │
    │  │ 8 │       │ 8 │  │
    │  └───┘       └───┘  │
    │  Digit 1    Digit 2 │
    └─────────────────────┘
     1  2  3  4  5  6  7  8  9
     │  │  │  │  │  │  │  │  │
    10 11 12 13 14 15 16 17 18

Pin Mapping:
Pin 1:  E (Digit 1)  → Arduino Pin 12
Pin 2:  D (Digit 1)  → Arduino Pin 11
Pin 3:  DP (Digit 1) → Not used
Pin 4:  C (Digit 1)  → Arduino Pin 10
Pin 5:  G (Digit 1)  → Arduino Pin 6
Pin 6:  CA (Digit 1) → Arduino Pin 5 (Control)
Pin 7:  B (Digit 1)  → Arduino Pin 9
Pin 8:  CA (Digit 2) → Arduino Pin 4 (Control)
Pin 9:  F (Digit 1)  → Arduino Pin 13
Pin 10: A (Digit 1)  → Arduino Pin 8
Pin 11: E (Digit 2)  → Arduino Pin 12 (shared)
Pin 12: D (Digit 2)  → Arduino Pin 11 (shared)
Pin 13: DP (Digit 2) → Not used
Pin 14: C (Digit 2)  → Arduino Pin 10 (shared)
Pin 15: G (Digit 2)  → Arduino Pin 6 (shared)
Pin 16: B (Digit 2)  → Arduino Pin 9 (shared)
Pin 17: F (Digit 2)  → Arduino Pin 13 (shared)
Pin 18: A (Digit 2)  → Arduino Pin 8 (shared)
```

## 7-Segment Layout

```
     AAA
    F   B
     GGG
    E   C
     DDD  DP

Segments: A, B, C, D, E, F, G, DP (decimal point)
```

## Breadboard Layout

1. Place the 18-pin dual 7-segment display on breadboard (across center gap)
2. **Segment Connections** (pins 8-13 use PORTB for fast control):
   - Pin 8 → Segment A (display pins 10, 18)
   - Pin 9 → Segment B (display pins 7, 16)
   - Pin 10 → Segment C (display pins 4, 14)
   - Pin 11 → Segment D (display pins 2, 12)
   - Pin 12 → Segment E (display pins 1, 11)
   - Pin 13 → Segment F (display pins 9, 17)
   - Pin 6 → Segment G (display pins 5, 15)
3. **Digit Control**:
   - Pin 5 → Common Anode Digit 1 (display pin 6)
   - Pin 4 → Common Anode Digit 2 (display pin 8)
4. **Optional**: Add 220Ω resistors in series with each segment for current limiting
5. **Optional**: Use 10kΩ variable resistor for brightness control
6. Connect Arduino to computer via USB for serial communication

## Component-Specific Details

### 7-Segment Display

- **Type**: Common Anode (CA)
- **Digits**: 2 digits (00-99)
- **Segments**: 7 segments per digit (A-G) + decimal point (unused)
- **Control**: Multiplexed (display one digit at a time, rapidly switching)
- **Logic**: LOW to light segment (common anode), HIGH to turn off
- **Refresh Rate**: 7ms per digit = ~71 Hz total refresh

### Display Encoding (Common Anode)

The code uses hexadecimal values to encode digits (inverted for common anode):

```cpp
int display_num[10] = {
  0x40,  // 0: Segments A,B,C,D,E,F ON
  0x79,  // 1: Segments B,C ON
  0x24,  // 2: Segments A,B,D,E,G ON
  0x30,  // 3: Segments A,B,C,D,G ON
  0x19,  // 4: Segments B,C,F,G ON
  0x12,  // 5: Segments A,C,D,F,G ON
  0x02,  // 6: Segments A,C,D,E,F,G ON
  0x78,  // 7: Segments A,B,C ON
  0x00,  // 8: All segments ON
  0x10   // 9: Segments A,B,C,D,F,G ON
};
```

### Port Manipulation

The code uses PORTB (pins 8-13) for fast parallel control:

```cpp
DDRB = 0xFF;  // Set all PORTB pins as OUTPUT
PORTB = display_num[digit];  // Write all 6 segments at once
```

## Special Notes

- **Multiplexing**: Only one digit is on at any time, switching every 7ms
- **Persistence of Vision**: Rapid switching (71Hz) makes both digits appear continuously lit
- **PORTB Direct Control**: Pins 8-13 controlled together for efficiency
- **Segment G**: Separate control on pin 6 (not part of PORTB)
- **Serial Input**: Accepts values 0-99 via Serial Monitor
- **Input Validation**: Values outside 0-99 range are rejected
- **No Current Limiting in Code**: Optional 220Ω resistors recommended
- Serial baud rate: 9600

## How It Works

1. **Initialization**:
   - PORTB (pins 8-13) set as OUTPUT for segments A-F
   - Pin 6 set as OUTPUT for segment G
   - Pins 5, 4 set as OUTPUT for digit control
   - Serial communication started at 9600 baud

2. **Serial Input Processing**:
   - Read number from Serial Monitor (0-99)
   - Validate input range
   - If valid, update display value
   - If invalid (>99 or <0), reject and keep previous value

3. **Display Multiplexing** (in loop):
   - **Step 1**: Display tens digit
     - Write segment pattern to PORTB
     - Set Segment G
     - Turn on Digit 1 (pin 5 HIGH)
     - Turn off Digit 2 (pin 4 LOW)
     - Wait 7ms
   - **Step 2**: Display ones digit
     - Write segment pattern to PORTB
     - Set Segment G
     - Turn off Digit 1 (pin 5 LOW)
     - Turn on Digit 2 (pin 4 HIGH)
     - Wait 7ms
   - Repeat continuously

4. **Value Display**:
   - Tens digit: value / 10
   - Ones digit: value % 10
   - Display persists until new valid input received

## Serial Input Format

- **Valid Input**: Integer from 0 to 99, followed by newline (`\n`)
- **Examples**:
  - Type `0` → Display shows "00"
  - Type `7` → Display shows "07"
  - Type `42` → Display shows "42"
  - Type `99` → Display shows "99"
- **Invalid Input**: Any number < 0 or > 99 (rejected)

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Open Serial Monitor**:
   - Set baud rate to 9600
   - Set line ending to "Newline" or "Both NL & CR"
3. **Verify initial display**:
   - Display should show "00"
   - Both digits should be stable (no flickering)
4. **Test single digits**:
   - Type `0` → Display shows "00"
   - Type `1` → Display shows "01"
   - Type `9` → Display shows "09"
5. **Test double digits**:
   - Type `10` → Display shows "10"
   - Type `42` → Display shows "42"
   - Type `99` → Display shows "99"
6. **Test all digits**:
   - Enter values to verify all numbers 0-9 display correctly
   - Check both digit positions
7. **Test validation**:
   - Type `100` → Should be rejected (out of range)
   - Type `-5` → Should be rejected (negative)
   - Display should retain previous valid value
8. **Check for flickering**:
   - Digits should appear solid (71Hz refresh)

## Serial Monitor Interaction Examples

```
Input: 0
Display: 00

Input: 7
Display: 07

Input: 42
Display: 42

Input: 99
Display: 99

Input: 100
Display: 99 (rejected, keeps previous value)
```

## Troubleshooting

| Issue                       | Possible Cause           | Solution                                    |
| --------------------------- | ------------------------ | ------------------------------------------- |
| No display                  | Wrong power/ground       | Check CA pins to Arduino control pins       |
| Segments always on          | Common cathode display   | This code requires common anode             |
| Dim display                 | High resistance          | Remove or reduce current limiting resistors |
| One digit missing           | CA control issue         | Check pins 4 or 5 connections               |
| Wrong segments              | Incorrect wiring         | Verify pin mapping matches table above      |
| Flickering                  | Slow refresh             | Reduce delay from 7ms to 5ms                |
| Ghost images                | Overlap timing           | Add brief off period between digit switches |
| No serial response          | Wrong baud rate          | Set Serial Monitor to 9600 baud             |
| Input not updating          | Format issue             | Ensure newline is sent (press Enter)        |
| Display shows wrong numbers | Encoding error           | Verify display_num array values             |
| One segment always off      | Bad connection           | Check that segment's wire and resistor      |
| Leading zero missing        | Normal for single digits | Type "05" not "5" for leading zero          |

## Display Multiplexing Timing

```
Time:     0ms    7ms    14ms   21ms   28ms
Digit 1:  [ON]   OFF    [ON]   OFF    [ON]
Digit 2:  OFF    [ON]   OFF    [ON]   OFF

Display shows: 42
User sees: Both digits appear continuously lit
Refresh rate: ~71 Hz (71 complete cycles per second)
```

## Code Reference

```cpp
#define S_D1 5  // Digit 1 common anode
#define S_D2 4  // Digit 2 common anode
#define S_G 6   // Segment G

int display_num[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
int x = 0;  // Current display value

void setup() {
  Serial.begin(9600);
  DDRB = 0xFF;  // Pins 8-13 as OUTPUT
  pinMode(S_G, OUTPUT);
  pinMode(S_D1, OUTPUT);
  pinMode(S_D2, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    int lx = input.toInt();
    if (lx<=99 && lx>=0) {
      x = lx;  // Update display value
    }
  }
  // Multiplexing code...
}
```

## Comparison with lab-w3-2

| Feature      | lab-w3-2 (Buttons)         | lab-w3-3 (Serial)   |
| ------------ | -------------------------- | ------------------- |
| Input Method | Push buttons               | Serial Monitor      |
| Interrupts   | Yes (pins 2, 3)            | No                  |
| Input Range  | 0-99 (increment/decrement) | 0-99 (direct entry) |
| Debouncing   | Required                   | Not needed          |
| Complexity   | More hardware              | Simpler hardware    |
| Use Case     | Standalone counter         | Development/testing |
