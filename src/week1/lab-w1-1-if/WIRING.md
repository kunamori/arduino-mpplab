# Lab W1-1-IF: LED Sequential Control with If Statements

**File**: `lab-w1-1-if.ino`

## Description
This lab demonstrates sequential LED control using `if` statements. Six LEDs turn on one at a time in sequence, with a buzzer providing audio feedback after each action.

## Components Required
- Arduino UNO R3 × 1
- LEDs (5mm, various colors) × 6
- Resistors 220Ω × 6
- Buzzer × 1
- Breadboard
- Jumper wires

## Pin Configuration

| Arduino Pin | Component | Connection | Notes |
|------------|-----------|------------|-------|
| Pin 10 | LED_1 | Anode (through 220Ω resistor) | First LED |
| Pin 9 | LED_2 | Anode (through 220Ω resistor) | Second LED |
| Pin 8 | LED_3 | Anode (through 220Ω resistor) | Third LED |
| Pin 7 | LED_4 | Anode (through 220Ω resistor) | Fourth LED |
| Pin 6 | LED_5 | Anode (through 220Ω resistor) | Fifth LED |
| Pin 5 | LED_6 | Anode (through 220Ω resistor) | Sixth LED |
| Pin 4 | Buzzer | Positive terminal | Audio feedback |
| GND | All LEDs & Buzzer | Cathode/Negative | Common ground |

## ASCII Wiring Diagram

```
        Arduino UNO                    Components
      ┌─────────────┐              
      │             │              
      │   Pin 10 ●──┼────[220Ω]────LED1+  ──┐
      │   Pin 9  ●──┼────[220Ω]────LED2+  ──┤
      │   Pin 8  ●──┼────[220Ω]────LED3+  ──┤
      │   Pin 7  ●──┼────[220Ω]────LED4+  ──├─── GND
      │   Pin 6  ●──┼────[220Ω]────LED5+  ──┤
      │   Pin 5  ●──┼────[220Ω]────LED6+  ──┤
      │   Pin 4  ●──┼────────────Buzzer+  ──┘
      │             │              
      │   GND    ●──┼─────────────────────────
      └─────────────┘              
```

## Breadboard Layout

1. Place all 6 LEDs in a row on the breadboard
2. Connect the anode (longer leg) of each LED to its respective Arduino pin through a 220Ω resistor
3. Connect all LED cathodes (shorter leg) to the ground rail
4. Connect the buzzer positive terminal to pin 4
5. Connect the buzzer negative terminal to the ground rail
6. Connect the ground rail to Arduino GND

## Component-Specific Details

### LEDs
- **Type**: Standard 5mm LEDs
- **Forward Voltage**: ~2V
- **Current**: 20mA (max)
- **Resistor**: 220Ω current limiting resistor required for each LED
- **Calculation**: (5V - 2V) / 0.02A = 150Ω (220Ω provides safety margin)

### Buzzer
- **Type**: Active buzzer (requires DC voltage)
- **Voltage**: 5V
- **Connection**: Direct connection to pin 4 (no resistor needed)

## Special Notes

- The code uses a counter variable `i` and multiple `if` statements
- Only one LED is on at a time (previous LEDs turn off)
- The buzzer beeps (500ms ON, 500ms OFF) after each state change
- After LED_6, all LEDs turn off and the sequence restarts
- Serial communication is initialized at 9600 baud for debugging

## How It Works

1. **Initialization**: All LED pins (5-10) and buzzer pin (4) are configured as OUTPUT
2. **Counter Logic**: Variable `i` increments from 1 to 7
3. **If Statements**: Each `if` statement checks the counter value and turns on the corresponding LED
4. **Audio Feedback**: The buzzer beeps after each action
5. **Reset**: When `i` reaches 7, all LEDs turn off and counter resets to 0
6. **Cycle Repeats**: The loop() function repeats indefinitely

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Observe the pattern**:
   - One LED should be on at a time
   - LEDs light up sequentially from LED_1 to LED_6 (pins 10→5)
   - Buzzer should beep after each transition
   - All LEDs turn off after the 7th step
   - Pattern repeats
3. **Verify timing**:
   - Each step should have a 500ms buzzer ON and 500ms OFF (1 second total)
   - Total cycle time: approximately 7 seconds
4. **Serial Monitor**: Open at 9600 baud to monitor debug messages (if any)

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| LED not lighting | Wrong polarity | Ensure anode (long leg) to resistor, cathode to GND |
| Multiple LEDs on | Wiring error | Check each LED is on its correct pin |
| LED too dim | Wrong resistor value | Verify 220Ω resistor (Red-Red-Brown) |
| LED too bright/hot | No resistor | Add 220Ω current limiting resistor |
| Buzzer not working | Wrong polarity | Check positive to pin 4, negative to GND |
| Buzzer silent | Passive buzzer used | Replace with active buzzer |
| Incorrect sequence | Wrong pin connections | Verify pins 10,9,8,7,6,5 for LED_1 through LED_6 |
| No response | Code not uploaded | Re-upload sketch and check board selection |

## Code Reference

```cpp
#define LED_1 10
#define LED_2 9
#define LED_3 8
#define LED_4 7
#define LED_5 6
#define LED_6 5
#define Buzzer 4
int i = 1;  // Counter variable
```
