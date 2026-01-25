# Lab W5-1: Keypad-Controlled Door Lock with Servo

**File**: `lab-w5-1.ino`

## Description
This lab demonstrates a keypad-controlled servo motor system simulating a door lock. Users enter a 4-digit PIN via a 4×4 matrix keypad. If correct, a servo motor rotates to "unlock" the door. If incorrect, a buzzer sounds.

## Components Required
- Arduino UNO R3 × 1
- 4×4 Matrix Keypad × 1
- Servo Motor (SG90) × 1
- Buzzer × 1
- Breadboard
- Jumper wires

## Pin Configuration

| Arduino Pin | Component | Connection | Notes |
|------------|-----------|------------|-------|
| Pin 9 | Keypad Row 1 | Row connection | Matrix scanning |
| Pin 8 | Keypad Row 2 | Row connection | Matrix scanning |
| Pin 7 | Keypad Row 3 | Row connection | Matrix scanning |
| Pin 6 | Keypad Row 4 | Row connection | Matrix scanning |
| Pin 5 | Keypad Col 1 | Column connection | Matrix scanning |
| Pin 4 | Keypad Col 2 | Column connection | Matrix scanning |
| Pin 3 | Keypad Col 3 | Column connection | Matrix scanning |
| Pin 2 | Keypad Col 4 | Column connection | Matrix scanning |
| Pin A0 | Servo Motor | Signal (orange/yellow) | PWM control |
| Pin A1 | Buzzer | Positive terminal | Audio feedback |
| 5V | Servo Motor | VCC (red) | Power supply |
| GND | Servo Motor + Buzzer | GND (brown/black) | Common ground |

## ASCII Wiring Diagram

```
    Arduino UNO          4×4 Keypad        Servo      Buzzer
  ┌─────────────┐       ┌──────────┐     ┌─────┐    ┌─────┐
  │             │       │ 1 2 3 A  │     │SG90 │    │ Bzz │
  │   Pin 9  ●──┼───────┤ R1       │     │     │    │     │
  │   Pin 8  ●──┼───────┤ R2       │     │     │    │     │
  │   Pin 7  ●──┼───────┤ R3       │     │     │    │     │
  │   Pin 6  ●──┼───────┤ R4       │     │     │    │     │
  │             │       │          │     │     │    │     │
  │   Pin 5  ●──┼───────┤ C1       │     │     │    │     │
  │   Pin 4  ●──┼───────┤ C2       │     │     │    │     │
  │   Pin 3  ●──┼───────┤ C3       │     │     │    │     │
  │   Pin 2  ●──┼───────┤ C4       │     │     │    │     │
  │             │       └──────────┘     │     │    │     │
  │   A0     ●──┼────────────────────────┤ SIG │    │     │
  │             │                        └─────┘    │     │
  │   A1     ●──┼───────────────────────────────────┤  +  │
  │             │                        ┌─────┐    │     │
  │   5V     ●──┼────────────────────────┤ VCC │    │     │
  │             │                        └─────┘    │     │
  │   GND    ●──┼────────┬───────────────┬──────────┤  -  │
  └─────────────┘        │ GND           │          └─────┘
                         └───────────────┘
```

## 4×4 Matrix Keypad Layout

```
        Columns
         1 2 3 A
       ┌─────────┐
    1  │ 1 2 3 A │  Row 1 → Pin 9
R   2  │ 4 5 6 B │  Row 2 → Pin 8
o   3  │ 7 8 9 C │  Row 3 → Pin 7
w   4  │ * 0 # D │  Row 4 → Pin 6
s      └─────────┘
         │ │ │ │
         │ │ │ └──── Col 4 → Pin 2
         │ │ └────── Col 3 → Pin 3
         │ └──────── Col 2 → Pin 4
         └────────── Col 1 → Pin 5

Key Mapping:
  1 2 3 A
  4 5 6 B
  7 8 9 C
  * 0 # D
```

## Breadboard Layout

1. **Keypad**:
   - Place keypad off breadboard or on separate area
   - Connect 8 wires: 4 rows (pins 9,8,7,6) + 4 columns (pins 5,4,3,2)

2. **Servo Motor**:
   - Red wire → Arduino 5V
   - Brown/Black wire → Arduino GND
   - Orange/Yellow wire → Arduino A0

3. **Buzzer**:
   - Positive terminal → Arduino A1
   - Negative terminal → Arduino GND

4. Connect all grounds to common GND rail

## Component-Specific Details

### 4×4 Matrix Keypad
- **Type**: Membrane keypad with 16 keys
- **Keys**: Numbers 0-9, Letters A-D, Symbols *, #
- **Pins**: 8 pins (4 rows + 4 columns)
- **Operation**: Matrix scanning (rows and columns)
- **Library**: Keypad library required
- **Debouncing**: Handled by library

### Servo Motor (SG90)
- **Type**: Standard micro servo
- **Rotation**: 0° to 180°
- **Control**: PWM signal on pin A0
- **Operating Voltage**: 4.8-6V (use 5V)
- **Stall Torque**: ~1.8 kg·cm
- **Speed**: 0.1 sec/60° at 4.8V
- **Wire Colors**:
  - Red: VCC (5V)
  - Brown/Black: GND
  - Orange/Yellow: Signal

### Buzzer
- **Type**: Active buzzer
- **Voltage**: 5V
- **Function**: Error notification (incorrect PIN)
- **Duration**: 500ms ON when PIN incorrect

## Special Notes

- **Default PIN**: "3629" (defined in code)
- **PIN Length**: Exactly 4 characters
- **Servo Position**:
  - 0° = Locked position
  - 180° = Unlocked position (held for 1 second)
- **Feedback**:
  - Correct PIN → Servo rotates, Serial: "PIN Correct!!"
  - Incorrect PIN → Buzzer beeps twice, Serial: "PIN Incorrect."
- **Serial Input/Output**: Displays entered PIN and result
- Serial baud rate: 9600

## How It Works

1. **Initialization**:
   - Keypad configured with row/column pin mapping
   - Servo attached to pin A0
   - Buzzer set as OUTPUT on pin A1

2. **PIN Entry**:
   - Display "PIN INPUT: " on Serial Monitor
   - Wait for 4 keypad presses
   - Each key press is echoed to Serial Monitor
   - Keys stored in `inputPIN[]` array

3. **Validation**:
   - Compare `inputPIN` with `setPIN` using `strncmp()`
   - If match (returns 0): PIN correct
   - If no match: PIN incorrect

4. **Correct PIN Action**:
   - Print "PIN Correct!!" to Serial
   - Servo rotates to 180° (unlocked)
   - Wait 1 second
   - Servo returns to 0° (locked) in next cycle

5. **Incorrect PIN Action**:
   - Print "PIN Incorrect." to Serial
   - Buzzer ON for 500ms
   - Buzzer OFF for 500ms
   - Buzzer ON for 500ms (second beep)
   - Buzzer OFF for 500ms

6. **Repeat**: Loop returns to step 2

## Keypad Matrix Scanning

The keypad library scans the matrix to detect key presses:

```
Scanning Process:
1. Set Row 1 LOW, others HIGH
2. Read all columns
3. If column is LOW, key at (Row 1, Column X) is pressed
4. Repeat for Rows 2, 3, 4
5. Return detected key (or NO_KEY if none)
```

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Open Serial Monitor** (9600 baud)
3. **Test correct PIN**:
   - Press keys: 3, 6, 2, 9
   - Serial should show: "PIN INPUT: 3629"
   - Serial should show: "PIN Correct!!"
   - Servo should rotate to 180° for 1 second
4. **Test incorrect PIN**:
   - Press any other 4-digit combination (e.g., 1234)
   - Serial should show: "PIN INPUT: 1234"
   - Serial should show: "PIN Incorrect."
   - Buzzer should beep twice
   - Servo should remain at 0°
5. **Test all keys**:
   - Verify all 16 keys respond
   - Check numbers, letters, and symbols register

## Serial Monitor Output Examples

```
PIN INPUT: 3629
INFO: PIN Correct!!
PIN INPUT: 1234
ERROR: PIN Incorrect.
PIN INPUT: 3629
INFO: PIN Correct!!
```

## Changing the PIN

To set a different PIN, modify the `setPIN` array in the code:

```cpp
// Current PIN: 3629
const char setPIN[maxPIN] = {'3', '6', '2', '9'};

// Change to 1234:
const char setPIN[maxPIN] = {'1', '2', '3', '4'};

// Change to use letters:
const char setPIN[maxPIN] = {'A', 'B', 'C', 'D'};
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Keypad not responding | Wrong pin connections | Verify row/column pin mapping |
| Wrong keys detected | Pins swapped | Check row pins: 9,8,7,6 and col pins: 5,4,3,2 |
| Multiple keys from one press | Bounce/contact issue | Keypad library handles this; check keypad quality |
| Servo not moving | No power | Verify servo VCC to 5V and GND connections |
| Servo jitters | Insufficient power | Separate servo power or use external 5V supply |
| Buzzer not sounding | Wrong polarity | Try swapping buzzer wires |
| PIN always incorrect | String comparison issue | Ensure exactly 4 characters entered |
| Serial garbled | Wrong baud rate | Set Serial Monitor to 9600 baud |
| Servo doesn't return | Code logic | Servo returns to 0° at start of next cycle |

## Code Reference

```cpp
#include <Keypad.h>
#include <Servo.h>

#define Buzzer A1
#define ServoPin A0

const byte ROWS = 4;
const byte COLS = 4;
const int maxPIN = 4;
const char setPIN[maxPIN] = {'3', '6', '2', '9'};
char inputPIN[maxPIN];

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo myServo;
```

## Applications

- Door lock systems
- Safe/locker access control
- Security systems
- Garage door openers
- Access control for restricted areas
- PIN-protected enclosures

## Enhancements

- Add EEPROM to store PIN (survives resets)
- Implement lockout after 3 failed attempts
- Add LCD to display status
- Use RFID instead of/in addition to keypad
- Add logging of access attempts
- Multiple user PINs with different access levels
