# Lab W3-2: 7-Segment Display with Button Counter

**File**: `lab-w3-2.ino`

## Description

This lab demonstrates hardware interrupts with push buttons controlling a dual 7-segment display counter (00-99). It combines interrupt-driven button input with multiplexed 7-segment display control using direct port manipulation for efficiency.

## Components Required

- Arduino UNO R3 × 1
- 18-pin Dual 7-Segment Display (Common Anode) × 1
- Push buttons × 2
- Capacitors 0.001μF × 2 (optional, for debouncing)
- Resistors 220Ω × 7 (optional, for current limiting)
- Variable Resistor 10kΩ × 1 (optional, for brightness control)
- Breadboard
- Jumper wires

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
| Pin 2       | PUSH_SW_1       | Increment button       | INT0 interrupt |
| Pin 3       | PUSH_SW_2       | Decrement button       | INT1 interrupt |
| GND         | Common          | Segments via resistors | Ground         |

## ASCII Wiring Diagram

```
    Arduino UNO                 7-Segment Display          Buttons
  ┌─────────────┐              ┌─────────────────┐
  │             │              │  ┌───┐  ┌───┐  │
  │ PORTB (8-13)├──────────────┤  │ 8 │  │ 8 │  │
  │             │   Segments   │  └───┘  └───┘  │
  │   Pin 6  ●──┼─────────────Seg G              │
  │   Pin 5  ●──┼─────────────CA Digit 1         │
  │   Pin 4  ●──┼─────────────CA Digit 2         │         ┌───┐
  │   Pin 2  ●──┼────────────────────────────────┤ SW1 │
  │  (INT0)     │                                └───┘
  │   Pin 3  ●──┼────────────────────────┌───┐
  │  (INT1)     │                        │ SW2 │
  │             │                        └───┘
  │   GND    ●──┼────────────────────────────────
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
4. **Buttons**:
   - Pin 2 → PUSH_SW_1 (increment)
   - Pin 3 → PUSH_SW_2 (decrement)
   - Both buttons to GND
5. **Optional**: Add 220Ω resistors in series with each segment for current limiting
6. **Optional**: Use 10kΩ variable resistor for brightness control

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
- **Interrupt Debouncing**: 200ms timestamp-based debouncing
- **Counter Range**: 0-99 with wrap-around
- **No Current Limiting in Code**: Optional 220Ω resistors recommended

## How It Works

1. **Initialization**:
   - PORTB (pins 8-13) set as OUTPUT for segments A-F
   - Pin 6 set as OUTPUT for segment G
   - Pins 5, 4 set as OUTPUT for digit control
   - Pins 2, 3 configured with interrupts for buttons

2. **Button Interrupts**:
   - PUSH_SW_1 (pin 2) increments counter when pressed
   - PUSH_SW_2 (pin 3) decrements counter when pressed
   - 200ms debouncing prevents multiple counts

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
   - Tens digit: counter / 10
   - Ones digit: counter % 10

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Verify initial display**:
   - Display should show "00"
   - Both digits should be stable (no flickering)
3. **Test increment (Pin 2)**:
   - Press PUSH_SW_1 → Display increments: 00→01→02...
   - At 99, wraps to 00
4. **Test decrement (Pin 3)**:
   - Press PUSH_SW_2 → Display decrements
   - At 00, wraps to 99
5. **Test all digits**:
   - Verify all numbers 0-9 display correctly on both digits
   - Check that segments form proper shapes
6. **Serial Monitor** (9600 baud):
   - Counter value printed on each button press
7. **Check for flickering**:
   - Digits should appear solid (71Hz refresh)
   - If flickering, check connections and resistor values

## Troubleshooting

| Issue                       | Possible Cause         | Solution                                    |
| --------------------------- | ---------------------- | ------------------------------------------- |
| No display                  | Wrong power/ground     | Check CA pins to Arduino control pins       |
| Segments always on          | Common cathode display | This code requires common anode             |
| Dim display                 | High resistance        | Remove or reduce current limiting resistors |
| One digit missing           | CA control issue       | Check pins 4 or 5 connections               |
| Wrong segments              | Incorrect wiring       | Verify pin mapping matches table above      |
| Flickering                  | Slow refresh           | Reduce delay from 7ms to 5ms                |
| Ghost images                | Overlap timing         | Add brief off period between digit switches |
| Buttons not working         | Interrupt issue        | Check pins 2, 3 to buttons and GND          |
| Display shows wrong numbers | Encoding error         | Verify display_num array values             |
| One segment always off      | Bad connection         | Check that segment's wire and resistor      |

## Display Multiplexing Timing

```
Time:     0ms    7ms    14ms   21ms   28ms
Digit 1:  [ON]   OFF    [ON]   OFF    [ON]
Digit 2:  OFF    [ON]   OFF    [ON]   OFF

Display appears: 42
User sees: Both digits appear continuously lit
Refresh rate: ~71 Hz (71 complete cycles per second)
```

## Code Reference

```cpp
#define PUSH_SW_1 2
#define PUSH_SW_2 3
#define S_D1 5  // Digit 1 common anode
#define S_D2 4  // Digit 2 common anode
#define S_G 6   // Segment G

int display_num[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
int counter = 0;

void setup() {
  DDRB = 0xFF;  // Pins 8-13 as OUTPUT
  attachInterrupt(digitalPinToInterrupt(PUSH_SW_1), count_func_i, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_SW_2), count_func_d, FALLING);
}
```
