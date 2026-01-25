# Lab W5-2: RFID Access Control with LCD Display

**File**: `lab-w5-2.ino`

## Description
This lab demonstrates an RFID-based access control system. Users scan RFID cards/tags using an RC522 RFID reader. The card UID (Unique Identifier) is displayed on an I2C LCD screen with buzzer feedback for each scan.

## Components Required
- Arduino UNO R3 × 1
- RFID-RC522 Module × 1
- RFID Tags/Cards × 1-2
- LCD 16×2 with I2C module × 1
- Buzzer × 1
- Breadboard
- Jumper wires

## Pin Configuration

| Arduino Pin | Component | Connection | Notes |
|------------|-----------|------------|-------|
| Pin 10 | RFID SS (SDA) | Slave Select | SPI chip select |
| Pin 9 | RFID RST | Reset | Module reset |
| Pin 13 | RFID SCK | Clock | SPI clock |
| Pin 11 | RFID MOSI | Data Out | SPI data Arduino→RFID |
| Pin 12 | RFID MISO | Data In | SPI data RFID→Arduino |
| Pin A0 | Buzzer | Positive | Audio feedback |
| Pin A4 (SDA) | LCD I2C | SDA | I2C data |
| Pin A5 (SCL) | LCD I2C | SCL | I2C clock |
| 3.3V | RFID | VCC | Power (3.3V only!) |
| 5V | LCD + Buzzer | VCC/+ | Power supply |
| GND | All | GND | Common ground |

## ASCII Wiring Diagram

```
    Arduino UNO       RC522 RFID       LCD I2C     Buzzer
  ┌─────────────┐    ┌──────────┐    ┌─────────┐  ┌───┐
  │             │    │  RC522   │    │ LCD 16×2│  │Bzz│
  │   3.3V   ●──┼────┤ VCC      │    │  I2C    │  │   │
  │   Pin 9  ●──┼────┤ RST      │    │         │  │   │
  │   GND    ●──┼──┬─┤ GND      │    │         │  │   │
  │   Pin 13 ●──┼──┼─┤ SCK      │    │         │  │   │
  │   Pin 11 ●──┼──┼─┤ MOSI     │    │         │  │   │
  │   Pin 12 ●──┼──┼─┤ MISO     │    │         │  │   │
  │   Pin 10 ●──┼──┼─┤ SDA(SS)  │    │         │  │   │
  │             │  │ └──────────┘    │         │  │   │
  │   A4(SDA)●──┼──┼─────────────────┤ SDA     │  │   │
  │   A5(SCL)●──┼──┼─────────────────┤ SCL     │  │   │
  │   5V     ●──┼──┼─────────────────┤ VCC     │  │   │
  │   A0     ●──┼──┼─────────────────┼─────────┼──┤ + │
  │   GND    ●──┼──┴─────────────────┤ GND     ├──┤ - │
  └─────────────┘                    └─────────┘  └───┘

  ⚠ IMPORTANT: RC522 uses 3.3V, NOT 5V!
```

## RC522 RFID Module Pinout

```
  ┌─────────────┐
  │   RC522     │
  │   RFID      │
  └┬──┬──┬──┬──┬┘
   │  │  │  │  │
  SDA SCK MOSI IRQ GND RST 3.3V MISO
   │  │   │   │   │   │   │    │
   │  │   │   X   │   │   │    │ (IRQ not connected)
   │  │   │       │   │   │    │
  P10 P13 P11   GND  P9 3.3V  P12
```

### Pin Functions
- **SDA (SS)**: Slave Select / Chip Select (pin 10)
- **SCK**: SPI Clock (pin 13)
- **MOSI**: Master Out Slave In (pin 11)
- **MISO**: Master In Slave Out (pin 12)
- **IRQ**: Interrupt (not used in this lab)
- **GND**: Ground
- **RST**: Reset (pin 9)
- **3.3V**: Power supply (**CRITICAL: Must be 3.3V, not 5V!**)

## Breadboard Layout

1. **RC522 RFID Module**:
   - **CRITICAL**: Connect VCC to Arduino **3.3V** (not 5V!)
   - Connect GND to Arduino GND
   - Connect SDA to pin 10
   - Connect SCK to pin 13
   - Connect MOSI to pin 11
   - Connect MISO to pin 12
   - Connect RST to pin 9
   - Leave IRQ unconnected

2. **LCD I2C**:
   - VCC to Arduino 5V
   - GND to Arduino GND
   - SDA to Arduino A4
   - SCL to Arduino A5

3. **Buzzer**:
   - Positive to Arduino A0
   - Negative to Arduino GND

## Component-Specific Details

### RC522 RFID Module
- **Frequency**: 13.56 MHz
- **Protocol**: ISO/IEC 14443 A/B
- **Operating Voltage**: 3.3V (**NOT 5V - will damage module!**)
- **Current**: ~13-26mA
- **Reading Distance**: 0-6 cm (depending on tag)
- **Interface**: SPI (Serial Peripheral Interface)
- **Library**: MFRC522 by GithubCommunity

### RFID Tags/Cards
- **Type**: MIFARE Classic 1K or similar
- **UID**: Unique 4-byte or 7-byte identifier
- **Frequency**: 13.56 MHz
- **Memory**: 1KB (MIFARE Classic 1K)
- **Form Factor**: Card, key fob, or sticker

### LCD I2C
- **Display**: 16 characters × 2 lines
- **I2C Address**: 0x27 (configurable)
- **Library**: LiquidCrystal_I2C

## Special Notes

- **CRITICAL VOLTAGE WARNING**: RC522 operates at 3.3V only. Connecting 5V will damage it!
- **SPI Interface**: Uses hardware SPI pins (10, 11, 12, 13)
- **UID Format**: Displayed as hex bytes separated by colons (e.g., "AB:CD:EF:01:")
- **Buzzer Feedback**: 500ms beep when card detected
- **LCD Display**: Shows RFID UID on top line
- **Startup Message**: "Power On." for 2 seconds
- Serial baud rate: 9600

## How It Works

1. **Initialization**:
   - LCD initialized and displays "Power On."
   - SPI bus initialized
   - RC522 RFID reader initialized
   - Buzzer configured as OUTPUT

2. **Card Detection Loop**:
   - Continuously check for RFID card presence
   - If card detected, read card serial number (UID)

3. **UID Reading**:
   - Read UID bytes from card
   - Convert each byte to hexadecimal string
   - Format: "XX:XX:XX:XX:" (uppercase)
   - Example: "AB:CD:EF:01:"

4. **Display & Feedback**:
   - Display UID on LCD (line 1)
   - Print UID to Serial Monitor
   - Activate buzzer for 500ms
   - Halt card communication
   - Return to detection loop

## SPI Communication

The RC522 uses SPI protocol for communication:

```
SPI Signals:
- SCK (Clock): Arduino generates clock pulses
- MOSI (Master Out Slave In): Arduino sends data to RC522
- MISO (Master In Slave Out): RC522 sends data to Arduino
- SS (Slave Select): Arduino selects RC522 (pin 10)

Communication Flow:
Arduino → MOSI → RC522
Arduino ← MISO ← RC522
```

## Testing Instructions

1. **Upload the sketch** to Arduino UNO
2. **Verify startup**:
   - LCD should show "Power On." for 2 seconds
   - LCD should clear after startup
3. **Open Serial Monitor** (9600 baud)
4. **Scan RFID tag**:
   - Place card/tag within 6cm of RC522 antenna
   - UID should appear on LCD (e.g., "AB:CD:EF:01:")
   - UID should appear in Serial Monitor
   - Buzzer should beep for 500ms
5. **Test multiple scans**:
   - Remove card and scan again
   - Each scan updates LCD display
6. **Test multiple cards**:
   - If you have multiple tags, verify each has unique UID

## Serial Monitor & LCD Output Examples

**Serial Monitor**:
```
AB:CD:EF:01:
12:34:56:78:
AB:CD:EF:01:
```

**LCD Display**:
```
┌────────────────┐
│AB:CD:EF:01:    │  ← RFID UID
│                │
└────────────────┘
```

## Reading RFID UID

The UID is stored in the RC522's UID structure:

```cpp
mfrc522.uid.size      // UID size in bytes (4 or 7)
mfrc522.uid.uidByte[] // Array of UID bytes

Example UID (4 bytes):
uidByte[0] = 0xAB
uidByte[1] = 0xCD
uidByte[2] = 0xEF
uidByte[3] = 0x01

Displayed as: "AB:CD:EF:01:"
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Module not detected | Wrong voltage | Verify 3.3V to RC522 VCC (not 5V!) |
| No card detection | SPI wiring wrong | Check SCK (13), MOSI (11), MISO (12), SS (10) |
| Module damaged/hot | 5V connected | Module may be damaged; replace and use 3.3V |
| LCD not working | I2C issue | Check SDA (A4) and SCL (A5) connections |
| Buzzer not sounding | Wrong pin | Verify buzzer to A0 |
| Wrong UID displayed | Read error | Move card closer to antenna |
| No response | RST not connected | Connect RST to pin 9 |
| Erratic behavior | Power supply weak | Ensure stable 3.3V and 5V supplies |

## Code Reference

```cpp
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define Buzzer A0

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("Power On.");
  delay(2000);
  lcd.clear();
  
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(Buzzer, OUTPUT);
}

void loop() {
  lcd.setCursor(0,0);
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      String rfidnum = "";
      for(byte i = 0; i < mfrc522.uid.size; i++) {
        rfidnum += toHexStr(mfrc522.uid.uidByte[i]) + ":";
      }
      rfidnum.toUpperCase();
      
      lcd.print(rfidnum);
      Serial.println(rfidnum);
      
      digitalWrite(Buzzer, HIGH);
      delay(500);
      digitalWrite(Buzzer, LOW);
      
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
  }
}
```

## Libraries Required

1. **MFRC522** by GithubCommunity:
   - Install via Library Manager
   - Search for "MFRC522"

2. **LiquidCrystal_I2C**:
   - Search for "LiquidCrystal I2C"

3. **SPI**: Built-in library (no installation needed)

## Applications

- Access control systems
- Attendance tracking
- Inventory management
- Library book checkout
- Parking lot access
- Employee time tracking
- Smart locks

## Security Considerations

- UID can be cloned with proper equipment
- This lab demonstrates UID reading, not authentication
- For security applications:
  - Use encrypted sectors on MIFARE cards
  - Implement backend database for authorization
  - Use challenge-response authentication
  - Consider adding PIN/password requirement

## Enhancements

- Store authorized UIDs in EEPROM
- Add access granted/denied logic with servo (door lock)
- Different LED colors for authorized/unauthorized
- Log access attempts with timestamp
- Add relay to control actual door lock
- Implement time-based access control
