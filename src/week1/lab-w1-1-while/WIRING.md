# Lab W1-1-WHILE: LED Sequential Control with While Loop

**File**: `lab-w1-1-while.ino`

## Description

This lab demonstrates sequential LED control using a `while` loop. Six LEDs turn on sequentially, all remain on, then all turn off together, with a buzzer providing audio feedback.

## Components Required

- Arduino UNO R3 × 1
- LEDs (5mm, various colors) × 6
- Resistor 220Ω × 1
- Buzzer × 1
- Breadboard
- Jumper wires

## Pin Configuration

| Arduino Pin | Component         | Connection               | Notes          |
| ----------- | ----------------- | ------------------------ | -------------- |
| Pin 10      | LED_1             | Anode (positive leg)     | First LED      |
| Pin 9       | LED_2             | Anode (positive leg)     | Second LED     |
| Pin 8       | LED_3             | Anode (positive leg)     | Third LED      |
| Pin 7       | LED_4             | Anode (positive leg)     | Fourth LED     |
| Pin 6       | LED_5             | Anode (positive leg)     | Fifth LED      |
| Pin 5       | LED_6             | Anode (positive leg)     | Sixth LED      |
| Pin 4       | Buzzer            | Positive terminal        | Audio feedback |
| GND         | All LEDs & Buzzer | Via 220Ω resistor/Direct | Common ground  |

## ASCII Wiring Diagram

```
        Arduino UNO                    Components
      ┌─────────────┐
      │             │
      │   Pin 10 ●──┼────────────LED1+  ──┐
      │   Pin 9  ●──┼────────────LED2+  ──┤
      │   Pin 8  ●──┼────────────LED3+  ──┤
      │   Pin 7  ●──┼────────────LED4+  ──├───[220Ω]─── GND
      │   Pin 6  ●──┼────────────LED5+  ──┤
      │   Pin 5  ●──┼────────────LED6+  ──┤
      │   Pin 4  ●──┼────────────Buzzer+ ─┘
      │             │
      │   GND    ●──┼─────────────────────────
      └─────────────┘
```

## Breadboard Layout

1. Place all 6 LEDs in a row on the breadboard
2. Connect the anode (longer leg) of each LED directly to its respective Arduino pin
3. Connect all LED cathodes (shorter leg) together to a common point
4. Connect a single 220Ω resistor from the common cathode point to the ground rail
5. Connect the buzzer positive terminal to pin 4
6. Connect the buzzer negative terminal directly to the ground rail
7. Connect the ground rail to Arduino GND

## Component-Specific Details

### LEDs

- **Type**: Standard 5mm LEDs
- **Forward Voltage**: ~2V
- **Current**: ~20mA per LED
- **Resistor**: Single 220Ω current limiting resistor shared by all LEDs on common cathode
- **Calculation**: (5V - 2V) / 0.02A = 150Ω (220Ω provides safety margin)
- **Note**: Multiple LEDs can be on simultaneously; current increases proportionally

### Buzzer

- **Type**: Active buzzer (requires DC voltage)
- **Voltage**: 5V
- **Connection**: Direct connection to pin 4 (no resistor needed)

## Special Notes

- The code uses two `while` loops: one to turn LEDs on, one to turn them off
- All LEDs turn on sequentially and remain on
- After all are on, they all turn off sequentially
- The buzzer beeps (500ms ON, 500ms OFF) after each LED operation
- Serial communication is initialized at 9600 baud for debugging

## How It Works

1. **Initialization**: All LED pins (5-10) and buzzer pin (4) are configured as OUTPUT
2. **First While Loop**: Turns on LEDs sequentially from 0 to 5
   - Each LED turns on and stays on
   - Buzzer beeps after each LED
3. **Second While Loop**: Turns off all LEDs sequentially from 0 to 5
   - Each LED turns off
4. **Final Beep**: Buzzer beeps after all LEDs are off
5. **Cycle Repeats**: The loop() function repeats indefinitely

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Observe the pattern**:
   - LEDs should turn on sequentially from left to right (pins 10→5)
   - All LEDs remain on during the sequence
   - Buzzer should beep after each LED turns on
   - All LEDs turn off sequentially
   - Final buzzer beep after all LEDs are off
   - Pattern repeats
3. **Verify timing**:
   - Each LED operation has a 500ms buzzer ON and 500ms OFF (1 second total)
   - Total cycle time: approximately 13 seconds (6 on + 6 off + 1 final beep)
4. **Serial Monitor**: Open at 9600 baud to monitor debug messages (if any)

## Troubleshooting

| Issue               | Possible Cause        | Solution                                            |
| ------------------- | --------------------- | --------------------------------------------------- |
| LED not lighting    | Wrong polarity        | Ensure anode (long leg) to pin, cathode to resistor |
| LED too dim         | Wrong resistor value  | Verify 220Ω resistor (Red-Red-Brown)                |
| LED too bright/hot  | No resistor           | Add 220Ω resistor to common cathode                 |
| Buzzer not working  | Wrong polarity        | Check positive to pin 4, negative to GND            |
| Buzzer silent       | Passive buzzer used   | Replace with active buzzer                          |
| Incorrect sequence  | Wrong pin connections | Verify pins 10,9,8,7,6,5 for LED_1 through LED_6    |
| LEDs don't turn off | Logic error           | Check code uploaded correctly                       |
| No response         | Code not uploaded     | Re-upload sketch and check board selection          |

## Code Reference

```cpp
#define LED_1 10
#define LED_2 9
#define LED_3 8
#define LED_4 7
#define LED_5 6
#define LED_6 5
#define Buzzer 4
int NUMBER_LED = 6;
int ledPins[] = { LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};
```
