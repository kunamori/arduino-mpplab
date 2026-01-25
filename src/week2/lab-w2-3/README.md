# Lab W2-3: RGB LED Control via Serial Input

**File**: `lab-w2-3.ino`

## Description

This lab demonstrates PWM (Pulse Width Modulation) control of an RGB LED using serial input. Users can set custom colors by sending RGB values (0-255) through the Serial Monitor in the format "R&G&B\n".

## Components Required

- Arduino UNO R3 × 1
- RGB LED (Common Cathode) × 1
- Resistors 220Ω × 3
- Breadboard
- Jumper wires
- USB cable for serial communication

## Pin Configuration

| Arduino Pin | Component     | Connection                          | Notes         |
| ----------- | ------------- | ----------------------------------- | ------------- |
| Pin 11      | RGB LED Red   | Red anode (through 220Ω resistor)   | PWM capable   |
| Pin 10      | RGB LED Green | Green anode (through 220Ω resistor) | PWM capable   |
| Pin 9       | RGB LED Blue  | Blue anode (through 220Ω resistor)  | PWM capable   |
| GND         | RGB LED       | Common cathode (longest pin)        | Common ground |

## ASCII Wiring Diagram

```
        Arduino UNO              RGB LED (Common Cathode)
      ┌─────────────┐
      │             │              ┌─────┐
      │   Pin 11 ●──┼──[220Ω]──────┤  R  │ Red
      │             │              │     │
      │   Pin 10 ●──┼──[220Ω]──────┤  G  │ Green
      │             │              │     │
      │   Pin 9  ●──┼──[220Ω]──────┤  B  │ Blue
      │             │              │     │
      │   GND    ●──┼──────────────┤ GND │ Common Cathode
      │             │              └─────┘
      │   USB    ●──┼─── To Computer (Serial)
      └─────────────┘
```

## RGB LED Pinout (Common Cathode)

```
    Looking at LED from front:

     R   GND  G   B
     │    │   │   │
    [●]  [●] [●] [●]
     │    │   │   │
   Pin11 GND Pin10 Pin9

   Note: Longest pin is GND (cathode)
```

## Breadboard Layout

1. Insert the RGB LED into the breadboard
2. Identify the longest pin (common cathode) → connect to GND
3. Connect the red anode through 220Ω resistor to pin 11
4. Connect the green anode through 220Ω resistor to pin 10
5. Connect the blue anode through 220Ω resistor to pin 9
6. Connect Arduino to computer via USB

## Component-Specific Details

### RGB LED (Common Cathode)

- **Type**: Common cathode RGB LED
- **Forward Voltage**:
  - Red: ~2.0V
  - Green: ~3.0V
  - Blue: ~3.0V
- **Current**: 20mA per channel (max)
- **Resistor**: 220Ω current limiting resistor for each channel
- **PWM Control**: Pins 9, 10, 11 are PWM-capable on Arduino UNO
- **Color Mixing**: Combine red, green, and blue to create any color

### PWM Values (0-255)

- **0**: LED channel OFF (0% duty cycle)
- **255**: LED channel at maximum brightness (100% duty cycle)
- **127**: LED channel at ~50% brightness (50% duty cycle)

## Special Notes

- Serial communication at 9600 baud rate is required
- Input format: `R&G&B` where R, G, B are integers from 0 to 255
- Values must be terminated with 'N' character (readStringUntil)
- Invalid inputs (>255 or <0) generate error message
- PWM frequency on UNO: ~490 Hz (pins 9, 10) and ~980 Hz (pin 11)

## How It Works

1. **Initialization**: Pins 9, 10, 11 configured as OUTPUT for PWM
2. **Serial Input**: Program waits for input in format "R&G&B"
3. **String Parsing**:
   - Input split by '&' delimiter
   - Three values extracted: Red, Green, Blue
4. **Validation**: Each value checked to be 0-255
5. **PWM Output**: Valid values written to RGB LED pins using `analogWrite()`
6. **Color Display**: LED shows the requested color
7. **Error Handling**: Out-of-range values display error message

## Serial Input Format

**Format**: `R&G&B` followed by newline or 'N'

**Examples**:

- `255&0&0` → Pure Red
- `0&255&0` → Pure Green
- `0&0&255` → Pure Blue
- `255&255&0` → Yellow (Red + Green)
- `255&0&255` → Magenta (Red + Blue)
- `0&255&255` → Cyan (Green + Blue)
- `255&255&255` → White (all colors)
- `0&0&0` → Off (all colors off)
- `128&128&128` → Dim white (50% brightness)

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Open Serial Monitor**:
   - Set baud rate to 9600
   - Set line ending to "Newline" or "Both NL & CR"
3. **Test primary colors**:
   - Type `255&0&0` → LED should be red
   - Type `0&255&0` → LED should be green
   - Type `0&0&255` → LED should be blue
4. **Test color mixing**:
   - Type `255&255&0` → Yellow
   - Type `255&0&255` → Magenta
   - Type `0&255&255` → Cyan
5. **Test brightness**:
   - Type `50&50&50` → Dim white
   - Type `255&255&255` → Bright white
6. **Test error handling**:
   - Type `300&0&0` → Should display error
   - Type `-10&0&0` → Should display error

## Common Colors

| Color   | R   | G   | B   | Serial Input  |
| ------- | --- | --- | --- | ------------- |
| Red     | 255 | 0   | 0   | `255&0&0`     |
| Green   | 0   | 255 | 0   | `0&255&0`     |
| Blue    | 0   | 0   | 255 | `0&0&255`     |
| Yellow  | 255 | 255 | 0   | `255&255&0`   |
| Cyan    | 0   | 255 | 255 | `0&255&255`   |
| Magenta | 255 | 0   | 255 | `255&0&255`   |
| White   | 255 | 255 | 255 | `255&255&255` |
| Orange  | 255 | 128 | 0   | `255&128&0`   |
| Purple  | 128 | 0   | 255 | `128&0&255`   |
| Pink    | 255 | 128 | 128 | `255&128&128` |
| Off     | 0   | 0   | 0   | `0&0&0`       |

## Troubleshooting

| Issue                | Possible Cause   | Solution                                |
| -------------------- | ---------------- | --------------------------------------- |
| LED not lighting     | Wrong LED type   | Ensure common cathode, not common anode |
| Wrong colors         | Pins swapped     | Verify R=11, G=10, B=9                  |
| LED too dim          | Wrong resistor   | Verify 220Ω resistors                   |
| LED too bright/hot   | No resistors     | Add 220Ω resistors to each channel      |
| One color missing    | Bad connection   | Check that channel's resistor and wire  |
| No serial response   | Wrong baud rate  | Set Serial Monitor to 9600 baud         |
| Error on valid input | Format issue     | Use format "R&G&B" with & delimiters    |
| Inverted colors      | Common anode LED | Code is for common cathode              |
| Flickering           | Loose connection | Check all connections                   |
| Values not updating  | Code issue       | Re-upload sketch                        |

## Code Reference

```cpp
#define LED_BLUE 9
#define LED_GREEN 10
#define LED_RED 11
int VAL_RGB[3] = {0,0,0};

// Input parsing:
// Format: "R&G&B"
// Example: "255&128&0" for orange
```

## Understanding PWM

PWM (Pulse Width Modulation) rapidly switches the LED on and off to control brightness:

```
Value 255 (100%): ████████████████ (Always ON)
Value 192 (75%):  ████████████░░░░ (75% ON, 25% OFF)
Value 128 (50%):  ████████░░░░░░░░ (50% ON, 50% OFF)
Value 64  (25%):  ████░░░░░░░░░░░░ (25% ON, 75% OFF)
Value 0   (0%):   ░░░░░░░░░░░░░░░░ (Always OFF)

The switching is so fast (~500-1000 Hz) that the eye perceives it as dimming.
```
