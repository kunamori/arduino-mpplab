# Lab W2-1: Push Button Counter with Debouncing

**File**: `lab-w2-1.ino`

## Description

This lab demonstrates reading push button inputs with INPUT_PULLUP mode and software debouncing. Two buttons control a counter (0-99): one increments, one decrements, with values displayed on the Serial Monitor.

## Components Required

- Arduino UNO R3 × 1
- Push buttons × 2
- Breadboard
- Jumper wires

## Pin Configuration

| Arduino Pin | Component    | Connection        | Notes                           |
| ----------- | ------------ | ----------------- | ------------------------------- |
| Pin 2       | PUSH_SW_1    | Button terminal 1 | Increment button (INPUT_PULLUP) |
| Pin 3       | PUSH_SW_2    | Button terminal 1 | Decrement button (INPUT_PULLUP) |
| GND         | Both buttons | Button terminal 2 | Common ground                   |

## ASCII Wiring Diagram

```
        Arduino UNO              Components
      ┌─────────────┐
      │             │          ┌───┐
      │   Pin 2  ●──┼──────────┤ ● │  PUSH_SW_1 (Increment)
      │             │          └─┬─┘
      │             │            │
      │   Pin 3  ●──┼──────┐   ┌─┴─┐
      │             │      │   │ ● │  PUSH_SW_2 (Decrement)
      │             │      │   └─┬─┘
      │   GND    ●──┼──────┴─────┴───── GND
      └─────────────┘
```

## Breadboard Layout

1. Place both push buttons on the breadboard
2. Connect one terminal of PUSH_SW_1 to Arduino pin 2
3. Connect one terminal of PUSH_SW_2 to Arduino pin 3
4. Connect the other terminal of both buttons to the ground rail
5. Connect the ground rail to Arduino GND

## Component-Specific Details

### Push Buttons

- **Type**: Momentary tactile switches
- **Configuration**: INPUT_PULLUP mode (internal pull-up resistors enabled)
- **Active State**: LOW (0) when pressed, HIGH (1) when released
- **Debouncing**: 50ms software delay implemented
- **No external resistors required**: Arduino's internal pull-up resistors are used

## Special Notes

- **INPUT_PULLUP Mode**: Internal pull-up resistor (~20kΩ) keeps pin HIGH when button is not pressed
- **Active LOW Logic**: Button press connects pin to GND, reading as LOW (0)
- **Software Debouncing**: 50ms delay prevents false triggers from mechanical bouncing
- **Counter Range**: 0-99 with wrap-around (99→0, 0→99)
- **State Tracking**: Uses state array to detect button press/release edges
- Serial baud rate: 9600

## How It Works

1. **Initialization**: Pins 2 and 3 configured as INPUT_PULLUP
2. **State Detection**: Each button has two state variables (current and previous)
3. **Edge Detection**: Code detects state change from HIGH to LOW
4. **Debouncing**: 50ms delay after state change before reading again
5. **Counter Logic**:
   - **PUSH_SW_1 (Pin 2)**: Increments counter (wraps at 99→0)
   - **PUSH_SW_2 (Pin 3)**: Decrements counter (wraps at 0→99)
6. **Serial Output**: Counter value printed on each button press
7. **State Update**: Previous state updated for next iteration

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Open Serial Monitor**:
   - Set baud rate to 9600
3. **Test increment button (Pin 2)**:
   - Press PUSH_SW_1 → Counter increments by 1
   - Hold button → Counter increments only once per press
   - At 99, pressing again resets to 0
4. **Test decrement button (Pin 3)**:
   - Press PUSH_SW_2 → Counter decrements by 1
   - At 0, pressing again wraps to 99
5. **Verify debouncing**:
   - Each button press should register exactly once
   - No multiple counts from single press

## Serial Monitor Output Examples

```
0
1
2
3
...
98
99
0
99
98
```

## Troubleshooting

| Issue                     | Possible Cause       | Solution                                |
| ------------------------- | -------------------- | --------------------------------------- |
| Button not responding     | Loose connection     | Check button and wire connections       |
| Multiple counts per press | Debouncing issue     | Verify 50ms delay in code               |
| Always reads pressed      | Wrong wiring         | Ensure button connects to GND, not VCC  |
| No serial output          | Wrong baud rate      | Set Serial Monitor to 9600 baud         |
| Inverted logic            | Missing INPUT_PULLUP | Verify pinMode uses INPUT_PULLUP        |
| Erratic behavior          | Floating pin         | Ensure INPUT_PULLUP is enabled          |
| Counter skips numbers     | Contact bounce       | Clean button contacts or increase delay |
| No wrap-around            | Logic error          | Re-upload sketch                        |

## Understanding INPUT_PULLUP

```
Button Released (Not Pressed):
  Arduino Pin ──[20kΩ]── VCC (5V)
                │
                Button (Open)
                │
               GND
  Result: Pin reads HIGH (1)

Button Pressed:
  Arduino Pin ──[20kΩ]── VCC (5V)
                │
                Button (Closed)
                │
               GND
  Result: Pin reads LOW (0)
```

## Code Reference

```cpp
#define PUSH_SW_1 2  // Increment
#define PUSH_SW_2 3  // Decrement
int state_sw[4] = {0,0,0,0};
int counter = 0;
```

## Debouncing Explanation

When a mechanical button is pressed or released, the contacts can bounce (make and break contact multiple times) for several milliseconds before settling. The 50ms software delay ensures the button state is stable before reading:

```
Button Press Event:
  State Change → Delay 50ms → Read State → Take Action
```

This prevents the counter from incrementing/decrementing multiple times from a single button press.
