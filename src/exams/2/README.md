# Exam 2 Overview

This project uses two microcontroller boards: a Master (server) connected to an LCD and a Client with two push buttons. The Client counts positive numbers only (two counters) and sends the values to the Master over TCP/IP using a simple message pattern. The Master displays the received integers on the LCD in an "int:int" format.

## Behavior

- Client has two counters: SW1 = N1 and SW2 = N2.
- Each counter increments only (positive numbers) and ranges from 0 to 99.
- When values change, the Client sends the pattern: N1_N2& to the Master via TCP/IP.
- The Master parses the message and displays "N1:N2" on the LCD.

### Network Credentials

SSID wifi07
Password 12345678

### Wiring / Pins

Client
PIN D2 = SW1
PIN D4 = SW2

Server (LCD)
PIN 22 = SCL
PIN 21 = SDA

## Communication Protocol

- Message format: N1_N2& (example: "5_12&").
- '&' is the end-of-message marker.
- The Master should parse N1 and N2 as integers and show them as "N1:N2" on the LCD.

### Usage / Setup

1. Put both boards on the same Wi-Fi network (use the SSID and password above).
2. Start the Master (server) and ensure it is listening for TCP connections (configure IP/port as implemented in your firmware).
3. Start the Client. Press SW1 and SW2 to increment N1 and N2.
4. The Client sends updates to the Master, and the Master updates the LCD display.

### Expected Output

- The LCD must display integers in the format: int:int (e.g., "5:12").

### Notes & Troubleshooting

- Counters are limited to 0–99 and only increment.
- Ensure the message format exactly matches N1_N2&.
- If the LCD does not update, verify network connectivity, IP/port settings, and wiring for buttons and LCD pins.
