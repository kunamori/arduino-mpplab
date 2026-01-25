# Lab W1-2: LED Control via Serial Input

**File**: `lab-w1-2.ino`

## Description
This lab demonstrates interactive LED control via Serial Monitor input. Users can toggle individual LEDs on/off by sending LED numbers (1-6) through the serial interface, with buzzer feedback.

## Components Required
- Arduino UNO R3 × 1
- LEDs (5mm, various colors) × 6
- Resistors 220Ω × 6
- Buzzer × 1
- Breadboard
- Jumper wires
- USB cable for serial communication

## Pin Configuration

| Arduino Pin | Component | Connection | Notes |
|------------|-----------|------------|-------|
| Pin 10 | LED_1 | Anode (through 220Ω resistor) | LED number 1 |
| Pin 9 | LED_2 | Anode (through 220Ω resistor) | LED number 2 |
| Pin 8 | LED_3 | Anode (through 220Ω resistor) | LED number 3 |
| Pin 7 | LED_4 | Anode (through 220Ω resistor) | LED number 4 |
| Pin 6 | LED_5 | Anode (through 220Ω resistor) | LED number 5 |
| Pin 5 | LED_6 | Anode (through 220Ω resistor) | LED number 6 |
| Pin 4 | Buzzer | Positive terminal | Input confirmation |
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
      │   USB    ●──┼─── To Computer (Serial)
      └─────────────┘              
```

## Breadboard Layout

1. Place all 6 LEDs in a row on the breadboard
2. Connect the anode (longer leg) of each LED to its respective Arduino pin through a 220Ω resistor
3. Connect all LED cathodes (shorter leg) to the ground rail
4. Connect the buzzer positive terminal to pin 4
5. Connect the buzzer negative terminal to the ground rail
6. Connect the ground rail to Arduino GND
7. Connect Arduino to computer via USB for serial communication

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
- **Purpose**: Provides audio confirmation when input is received

## Special Notes

- Serial communication at 9600 baud rate is required
- The code maintains LED state in an array (`ledStatus[]`)
- Sending the same LED number toggles its state (ON→OFF or OFF→ON)
- Invalid inputs (not 1-6) generate an error message
- Each valid input triggers a short buzzer beep (100ms ON, 100ms OFF)

## How It Works

1. **Initialization**: All LED pins (5-10) and buzzer pin (4) are configured as OUTPUT
2. **Serial Input**: Program waits for serial input (numbers 1-6)
3. **Input Validation**: Checks if input is between 1 and 6
4. **State Check**: Reads current LED state from `ledStatus[]` array
5. **Toggle Logic**: 
   - If LED is OFF (0), turn it ON (1)
   - If LED is ON (1), turn it OFF (0)
6. **Feedback**: 
   - Buzzer beeps to confirm input
   - Serial Monitor displays LED number and new state
7. **Error Handling**: Invalid inputs display "ERROR: INPUT_USER"

## Serial Input Format

- **Valid Input**: Single digit from 1 to 6, followed by newline (`\n`)
- **Example**: 
  - Type `1` and press Enter → Toggles LED_1 (pin 10)
  - Type `3` and press Enter → Toggles LED_3 (pin 8)
- **Invalid Input**: Any number < 1 or > 6, or non-numeric input

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Open Serial Monitor**:
   - Set baud rate to 9600
   - Set line ending to "Newline" or "Both NL & CR"
3. **Test LED control**:
   - Type `1` and press Enter → LED_1 should toggle, buzzer beeps
   - Type `1` again → LED_1 should toggle back to previous state
   - Repeat for LEDs 2-6
4. **Verify feedback**:
   - Serial Monitor should display: "LED: X ON" or "LED: X OFF"
   - Buzzer should beep briefly on each valid input
5. **Test error handling**:
   - Type `0` or `7` → Should display "ERROR: INPUT_USER"
   - Type letters → Should display error

## Serial Monitor Output Examples

```
LED: 1 ON
LED: 2 ON
LED: 1 OFF
LED: 3 ON
ERROR: INPUT_USER
LED: 6 ON
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| No serial response | Wrong baud rate | Ensure Serial Monitor is set to 9600 baud |
| LED not responding | Wrong pin connection | Verify pin numbers match LED positions |
| LED not lighting | Wrong polarity | Ensure anode (long leg) to resistor, cathode to GND |
| LED too dim | Wrong resistor value | Verify 220Ω resistor (Red-Red-Brown) |
| Buzzer not working | Wrong polarity | Check positive to pin 4, negative to GND |
| Error on valid input | Input format issue | Ensure newline character is sent (press Enter) |
| State not toggling | Array not updating | Re-upload sketch |
| Random LED behavior | Noise on serial | Check USB cable quality |

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
int ledStatus[] = {0,0,0,0,0,0,0};
int ledPins[] = { LED_1, LED_2, LED_3, LED_4, LED_5, LED_6};
```
