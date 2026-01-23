# Microprocessor with Arduino 🤖

Repository for code backup from Microprocessor Practice Subject.

## 🧰 Hardware Requirements

Arduino UNO R3 Atmega328P (Week 1-5) \
ESP32 (Week 6-7) \
Cables and Breadboard \
Various Components (see individual lab instructions)

## 🗂️ Project Structure

Each week's folder contains multiple labs with self-contained Arduino sketches.

### Week-by-Week Topics

| Week | Topics                        | Key Components          |
| ---- | ----------------------------- | ----------------------- |
| 1    | Pin setup, LED control, loops | LEDs, Push buttons      |
| 2    | RGB LED, Analog reading       | RGB LED, Potentiometers |
| 3    | Hardware interrupts           | 7-segment display       |
| 4    | LCD display, Sensors          | LCD 16x2, DHT22, LDR    |
| 5    | Input devices                 | 4x4 Keypad, RFID        |
| 6    | WiFi networking (ESP32)       | TCP/IP communication    |
| 7    | UDP networking (ESP32)        | UDP/IP communication    |

## 🚀 Getting Started

### 1. Hardware Setup

See [COMPONENTS.md](./COMPONENTS.md) for required components.

### 2. Software Setup

#### Install Arduino IDE

Download from [arduino.cc](https://www.arduino.cc/en/software)

#### Install Required Libraries

See [libraries.txt](./libraries.txt) for complete list.

**Quick install via Arduino IDE:**

1. Open Arduino IDE
2. Go to: Sketch > Include Library > Manage Libraries
3. Install libraries listed in `libraries.txt`

**Or use Arduino CLI:**

```bash
arduino-cli lib install "LiquidCrystal_I2C"
arduino-cli lib install "DHT sensor library"
arduino-cli lib install "Adafruit Unified Sensor"
arduino-cli lib install "Keypad"
arduino-cli lib install "MFRC522"
```

## 📖 Usage

1. Navigate to desired week folder (e.g., `src/week-01/lab1/`)
2. Open `.ino` file in Arduino IDE
3. Connect your Arduino/ESP32
4. Select correct board and port
5. Upload the sketch
