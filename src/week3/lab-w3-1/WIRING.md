# Lab W3-1: Button Counter with Hardware Interrupts

**File**: `lab-w3-1.ino`

## Description
This lab demonstrates hardware interrupts using push buttons. Two buttons with interrupt functionality control a counter (0-99) that outputs to the Serial Monitor. This improves on lab-w2-1 by using hardware interrupts instead of polling.

## Components Required
- Arduino UNO R3 × 1
- Push buttons × 2
- Capacitors 0.001μF (1nF) × 2
- Breadboard
- Jumper wires

## Pin Configuration

| Arduino Pin | Component | Connection | Notes |
|------------|-----------|------------|-------|
| Pin 2 | PUSH_SW_1 | Button terminal 1 | Increment (INT0 interrupt) |
| Pin 3 | PUSH_SW_2 | Button terminal 1 | Decrement (INT1 interrupt) |
| GND | Both buttons | Button terminal 2 | Common ground |

## ASCII Wiring Diagram

```
        Arduino UNO              Components
      ┌─────────────┐              
      │             │          ┌───┐
      │   Pin 2  ●──┼────┬─────┤ ● │  PUSH_SW_1 (Increment)
      │  (INT0)     │  [1nF]   └─┬─┘
      │             │    │        │
      │             │   GND       │
      │   Pin 3  ●──┼────┬───┐ ┌─┴─┐
      │  (INT1)     │  [1nF] │ │ ● │  PUSH_SW_2 (Decrement)
      │             │    │   │ └─┬─┘
      │             │   GND  │   │
      │   GND    ●──┼────────┴───┴───── GND
      └─────────────┘              
```

## Breadboard Layout

1. Place both push buttons on the breadboard
2. Connect one terminal of PUSH_SW_1 to Arduino pin 2 (INT0)
3. Connect one terminal of PUSH_SW_2 to Arduino pin 3 (INT1)
4. **Optional**: Add 1nF (0.001μF) capacitors from each pin to GND for hardware debouncing
5. Connect the other terminal of both buttons to the ground rail
6. Connect the ground rail to Arduino GND

## Component-Specific Details

### Push Buttons
- **Type**: Momentary tactile switches
- **Configuration**: INPUT_PULLUP mode (internal pull-up resistors enabled)
- **Active State**: LOW (0) when pressed, HIGH (1) when released
- **Interrupt Trigger**: FALLING edge (HIGH to LOW transition)
- **Software Debouncing**: 200ms using millis() in interrupt service routine
- **Hardware Debouncing (Optional)**: 1nF capacitors

### Capacitors (Optional Hardware Debouncing)
- **Value**: 0.001μF (1nF)
- **Purpose**: Filter mechanical bounce
- **Connection**: Between pin and GND
- **Effect**: Smooths the signal transition

## Special Notes

- **Hardware Interrupts**: Uses pins 2 and 3 (INT0 and INT1 on Arduino UNO)
- **Interrupt Mode**: FALLING (triggers when button pressed, HIGH→LOW)
- **Software Debouncing**: Uses `millis()` timestamp with 200ms threshold
- **Non-blocking**: Interrupts allow immediate response without polling
- **Counter Range**: 0-99 with wrap-around
- **ISR Functions**: `count_func_i()` for increment, `count_func_d()` for decrement
- Serial baud rate: 9600

## How It Works

1. **Initialization**: 
   - Pins 2 and 3 configured as INPUT_PULLUP
   - Interrupts attached to pins 2 and 3 with FALLING trigger
2. **Interrupt Service Routine (ISR)**:
   - When button pressed, interrupt fires immediately
   - ISR checks if 200ms has elapsed since last press (debouncing)
   - If valid, counter increments/decrements with wrap-around
3. **Main Loop**:
   - Checks if counter value changed
   - If changed, prints new value to Serial Monitor
   - No button polling needed - interrupts handle it
4. **Advantages**:
   - Immediate response (no polling delay)
   - More efficient (CPU can do other tasks)
   - Better user experience

## Hardware Interrupts on Arduino UNO

| Pin | Interrupt | Function |
|-----|-----------|----------|
| Pin 2 | INT0 | External interrupt 0 |
| Pin 3 | INT1 | External interrupt 1 |

**Trigger Modes**:
- LOW: Trigger when pin is LOW
- CHANGE: Trigger on any change
- RISING: Trigger on LOW to HIGH
- FALLING: Trigger on HIGH to LOW (used in this lab)

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Open Serial Monitor**:
   - Set baud rate to 9600
3. **Test increment button (Pin 2)**:
   - Press PUSH_SW_1 → Counter increments by 1 immediately
   - Response should be instant (no polling delay)
   - At 99, pressing again resets to 0
4. **Test decrement button (Pin 3)**:
   - Press PUSH_SW_2 → Counter decrements by 1 immediately
   - At 0, pressing again wraps to 99
5. **Test debouncing**:
   - Each button press should register exactly once
   - Rapid presses within 200ms are ignored (debounce protection)
6. **Test simultaneous presses**:
   - Both buttons can be pressed independently
   - Interrupts handle both correctly

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

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Button not responding | Loose connection | Check button and wire connections |
| Multiple counts per press | Insufficient debouncing | Increase debounce time (200ms→300ms) |
| Always reads pressed | Wrong wiring | Ensure button connects to GND, not VCC |
| No serial output | Wrong baud rate | Set Serial Monitor to 9600 baud |
| Inverted logic | Missing INPUT_PULLUP | Verify pinMode uses INPUT_PULLUP |
| Erratic behavior | Missing capacitor | Add 1nF capacitors for hardware debouncing |
| ISR not firing | Wrong interrupt mode | Verify FALLING mode is used |
| Counter doesn't change | ISR not attached | Check attachInterrupt() calls |

## Understanding Interrupts

**Traditional Polling (lab-w2-1)**:
```
loop() {
  Check Button 1 → Check Button 2 → Other tasks → Repeat
  Response time: Depends on loop execution time
}
```

**Interrupt-Driven (lab-w3-1)**:
```
loop() {
  Other tasks (interrupted when button pressed)
}

Button pressed → ISR fires immediately → Update counter → Resume
Response time: Near-instantaneous (microseconds)
```

## Interrupt Debouncing

The code uses timestamp-based debouncing in the ISR:

```cpp
void count_func_i() {
  if(millis()-t>200) {  // Only proceed if 200ms elapsed
    t=millis();         // Update timestamp
    counter++;          // Update counter
  }
}
```

This ensures multiple triggers from button bounce are ignored.

## Code Reference

```cpp
#define PUSH_SW_1 2  // INT0
#define PUSH_SW_2 3  // INT1
int counter = 0;
unsigned long t = 0;  // Timestamp for debouncing

void setup() {
  pinMode(PUSH_SW_1, INPUT_PULLUP);
  pinMode(PUSH_SW_2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PUSH_SW_1), count_func_i, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_SW_2), count_func_d, FALLING);
}

void count_func_i() {
  if(millis()-t>200) {
    t=millis();
    counter = (counter == 99) ? 0 : counter + 1;
  }
}
```

## Comparison with lab-w2-1

| Feature | lab-w2-1 (Polling) | lab-w3-1 (Interrupts) |
|---------|-------------------|----------------------|
| Response Time | Loop-dependent | Immediate |
| CPU Efficiency | Continuous polling | Event-driven |
| Debouncing | 50ms delay | 200ms timestamp |
| Code Complexity | Simpler | More advanced |
| Pins Used | Any digital | Must use INT pins |
