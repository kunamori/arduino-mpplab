# Lab W7-3-CLIENT: ไคลเอนต์ควบคุมไฟ RGB LED ด้วย ESP32 ผ่าน WiFi UDP โดยใช้ปุ่มกดและตัวต้านทานปรับค่าได้

**ไฟล์**: `lab-w7-3-client.ino`

## คำอธิบาย

การทดลองนี้สาธิตการทำงานของ ESP32 ในฐานะ WiFi UDP client สำหรับควบคุมไฟ RGB LED ระยะไกลโดยใช้ปุ่มกดและตัวต้านทานปรับค่าได้ (Potentiometer) ปุ่มกดจะทำหน้าที่วนลูปเลือกสี RGB (แดง/เขียว/น้ำเงิน) และ Potentiometer จะใช้ปรับความสว่าง (0-255) ไคลเอนต์จะส่งแพ็กเก็ต Heartbeat เป็นระยะๆ และตอบสนองต่อการตอบรับ (Acknowledgment) จากเซิร์ฟเวอร์ นี่คือฝั่ง **Client** ที่ทำหน้าที่รับอินพุตจากผู้ใช้เพื่อไปควบคุมไฟ RGB LED ที่อยู่ฝั่ง Server (lab-w7-3-server)

## อุปกรณ์ที่ต้องใช้

- ESP32 DEVKIT × 1
- สวิตช์ปุ่มกด (Push button) × 1
- ตัวต้านทานปรับค่าได้ (Potentiometer 10kΩ) × 1
- Breadboard (โปรโตบอร์ด)
- สายจั๊มเปอร์

## การกำหนดค่าขา (Pin Configuration)

| ขา ESP32 | อุปกรณ์       | การเชื่อมต่อ               | หมายเหตุ                 |
| -------- | ------------- | -------------------------- | ------------------------ |
| GPIO 4   | ปุ่มกด        | ตัวเลือกสี                 | INPUT_PULLUP             |
| GPIO 34  | Potentiometer | ควบคุมความสว่าง (ขา Wiper) | ADC1_CH6, อินพุตเท่านั้น |
| 3.3V     | Potentiometer | ขานอกด้านหนึ่ง             | ไฟเลี้ยง                 |
| GND      | Button + Pot  | กราวด์ร่วม                 | ใช้ GND ร่วมกัน          |

## แผนผังวงจร (ASCII Wiring Diagram)

```
        ESP32 DEVKIT        Button    Potentiometer
      ┌─────────────┐
      │             │       ┌───┐         ┌─────┐
      │   GPIO 4 ●──┼───────┤ ● │         │  1  │  3.3V
      │             │       └─┬─┘         │     │
      │             │         │           │  ╱  │
      │  GPIO 34 ●──┼─────────┼───────────┤ 2   │  Wiper
      │  (ADC)      │         │           │     │
      │             │         │           │  ╲  │
      │  3.3V    ●──┼─────────┼───────────┤  3  │  Outer
      │             │         │           └─────┘
      │   GND    ●──┼─────────┴──────────────────── GND
      └─────────────┘

      WiFi: Connects to "ESP32-Nihahaha" (server's AP)
      UDP: Sends to 192.168.4.1:6969
      Client IP: 192.168.4.5 (static)

      Button: Cycles through R(0), G(1), B(2)
      Potentiometer: Sets brightness 0-255

      Data Sent:
        Heartbeat: "10_20&" (every 1 second)
        RGB Data:  "0_180&" (color_brightness)
                   "1_255&"
                   "2_100&"
```

## การต่อวงจรบน Breadboard

1. วาง ESP32 ลงบน Breadboard
2. ต่อปุ่มกด:

- ขาข้างหนึ่ง → GPIO 4
- ขาอีกข้างหนึ่ง → GND
- ใช้โหมด INPUT_PULLUP (ไม่ต้องต่อตัวต้านทานภายนอก)

3. ต่อ Potentiometer:

- ขานอกด้านหนึ่ง → 3.3V
- ขากลาง (Wiper) → GPIO 34
- ขานอกอีกด้านหนึ่ง → GND

4. **หมายเหตุ**: สลับขานอกซ้าย-ขวาได้หากต้องการกลับทิศทางการหมุน

## รายละเอียดเฉพาะของอุปกรณ์

### ESP32 DEVKIT

- **WiFi**: 802.11 b/g/n
- **แรงดันทำงาน**: Logic 3.3V
- **ADC**: ความละเอียด 12-bit (0-4095) ที่ขา GPIO 34
- **โหมด WiFi**: Station (STA) mode แบบกำหนด Static IP
- **GPIO 34**: ADC1_CH6, เป็นขาอินพุตเท่านั้น (ส่งเอาต์พุตไม่ได้)
- **GPIO 4**: ตั้งค่าเป็น INPUT_PULLUP สำหรับปุ่มกด

### สวิตช์ปุ่มกด (Push Button)

- **การตั้งค่า**: โหมด INPUT_PULLUP (เปิด Pull-up ภายใน)
- **สถานะทำงาน**: เป็น LOW (0) เมื่อถูกกด
- **Debouncing**: หน่วงเวลาซอฟต์แวร์ 20ms เพื่อแก้สัญญาณรบกวน
- **หน้าที่**: วนลูปเลือกสี RGB (0→1→2→0...)

### ตัวต้านทานปรับค่าได้ (Potentiometer)

- **ประเภท**: แบบหมุน (Rotary potentiometer)
- **ความต้านทาน**: 10kΩ (ใช้ค่า 1kΩ ถึง 100kΩ ก็ได้)
- **มุมหมุน**: ~270° (ขึ้นอยู่กับรุ่น)
- **เอาต์พุต**: แรงดันแปรผัน 0V ถึง 3.3V
- **การแปลงค่า (Mapping)**: ADC 0-4095 → PWM 0-255

## การตั้งค่าเครือข่าย

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│  (This Device)   │   "ESP32-Nihahaha"    │ (lab-w7-3-server)│
│  192.168.4.5     │                       │  192.168.4.1     │
│  (Static IP)     │                       │   (Gateway)      │
└──────────────────┘                       └──────────────────┘
       │                                           │
   Button + Pot                                RGB LED
   (Color + Brightness)                     (GPIO 4,2,15)
       │                                           │
       │  Heartbeat: "10_20&" (1 sec)            │
       │  ──────────────────────────────────────► │
       │                                           │
       │  ◄─────────────────────────────────────  │
       │          Response: "A"                    │
       │                                           │
       │  RGB Data: "0_255&" (R, full bright)    │
       │  ──────────────────────────────────────► │
       │                                           │
       │  ◄─────────────────────────────────────  │
       │          Response: "A"                    │
            192.168.4.1:6969
```

### รายละเอียดเครือข่าย

- **SSID**: "ESP32-Nihahaha"
- **Password**: "12345678"
- **Client IP**: 192.168.4.5 (กำหนดคงที่ - Static)
- **Gateway**: 192.168.4.1 (IP ของ Server)
- **Subnet**: 255.255.255.0
- **Server IP**: 192.168.4.1 (Access Point)
- **Server Port**: 6969
- **Protocol**: UDP (WiFiUDP)
- **Client UDP Port**: 6969 (ใช้พอร์ตเดียวกับ Server)

## หมายเหตุพิเศษ

- **โหมด WiFi**: Station (STA) mode - เชื่อมต่อเข้ากับ Access Point ของ Server
- **การตั้งค่า IP**: STATIC IP (192.168.4.5) - ไม่ใช้ DHCP
- **โปรโตคอล**: UDP (Connectionless, สื่อสารสองทิศทาง)
- **รูปแบบข้อมูล**: "color_brightness&" (เช่น "0_180&", "1_255&", "2_100&")
- **Heartbeat**: ส่งค่า "10_20&" ทุกๆ 1 วินาที (เป็นข้อมูลหลอก) เพื่อรักษาการเชื่อมต่อ
- **การตอบรับ (Acknowledgment)**: Server จะตอบกลับด้วย "A" หลังจากได้รับข้อมูล RGB
- **การเลือกสี**: ปุ่มกดจะวนลูป: 0 (แดง) → 1 (เขียว) → 2 (น้ำเงิน) → 0...
- **ความสว่าง**: Potentiometer ถูกแมปค่าจาก 0-4095 → 0-255
- **การจับเวลา (Timing)**: การนับเวลา Heartbeat จะถูกรีเซ็ตเมื่อได้รับ "A" จาก Server
- Serial baud rate: 9600

## หลักการทำงาน

1. **การเชื่อมต่อ WiFi**:

- ตั้งค่า Static IP (192.168.4.5, gateway 192.168.4.1, subnet 255.255.255.0)
- ESP32 เชื่อมต่อกับ AP ชื่อ "ESP32-Nihahaha"
- รอจนกว่าจะเชื่อมต่อสำเร็จ
- พิมพ์ Local IP (192.168.4.5) ออกทาง Serial

2. **การเริ่มต้น UDP**:

- `udp.begin(port)` เริ่มรอรับข้อมูลที่พอร์ต 6969
- Client พร้อมส่งและรับแพ็กเก็ต UDP แล้ว

3. **การตรวจสอบปุ่มกด** (Loop):

- อ่านสถานะปุ่มที่ GPIO 4
- ตรวจจับการกดปุ่ม (Edge detection)
- หน่วงเวลา 20ms เพื่อทำ Debounce
- เพิ่มค่าตัวนับ `selRGB_BTN` (0→1→2→0)
- แปลงค่าเป็น String เพื่อเตรียมส่ง

4. **การอ่านค่า Potentiometer**:

- อ่านค่า Analog จาก GPIO 34 (0-4095)
- แปลงค่า (Map) เป็นช่วง PWM: 0-4095 → 0-255
- แปลงค่าเป็น String

5. **การส่ง Heartbeat**:

- ทำทุกๆ 1 วินาที (ถ้าไม่มีการตอบรับจาก Server)
- ส่งแพ็กเก็ตหลอก "10_20&"
- เพื่อรักษาการเชื่อมต่อให้ตื่นตัวอยู่เสมอ

6. **การรับ Acknowledgment จาก Server**:

- ตรวจสอบแพ็กเก็ต UDP ที่เข้ามา
- อ่านข้อมูลในแพ็กเก็ต
- ถ้าข้อมูลคือ "A"...
- ส่งข้อมูล RGB กลับไป: `selRGB + "_" + brightness + "&"`
- รีเซ็ตตัวจับเวลา 1 วินาที

7. **การแสดงผล Serial**:

- พิมพ์ค่าสีที่เลือกปัจจุบัน (0, 1 หรือ 2)

## ขั้นตอนการทดสอบ

1. **ติดตั้ง Server ก่อน**:

- อัปโหลด `lab-w7-3-server.ino` ลงใน ESP32 อีกตัว
- Server จะสร้าง AP "ESP32-Nihahaha"
- Server ต่อ RGB LED เข้ากับ GPIO 4 (R), 2 (G), 15 (B)

2. **อัปโหลดสเก็ตช์ฝั่ง Client**:

- อัปโหลดสเก็ตช์นี้ลงใน ESP32 ตัวที่เป็น Client

3. **เปิด Serial Monitor** (9600 baud):

- ควรเห็นข้อความ "Connecting to WiFi..."
- เมื่อเชื่อมต่อได้จะโชว์ IP "192.168.4.5"

4. **ทดสอบการเลือกสี**:

- กดปุ่ม (GPIO 4)
- Serial Monitor แสดง: 0 (เลือกสีแดง)
- กดปุ่มอีกครั้ง: 1 (เลือกสีเขียว)
- กดปุ่มอีกครั้ง: 2 (เลือกสีน้ำเงิน)
- กดปุ่มอีกครั้ง: 0 (วนกลับมาสีแดง)

5. **ทดสอบการควบคุมความสว่าง**:

- หมุน Potentiometer ทวนเข็มนาฬิกาสุด
- ความสว่าง = 0 (LED ดับ หรือหรี่สุด)
- หมุน Potentiometer ตามเข็มนาฬิกาสุด
- ความสว่าง = 255 (LED สว่างสุด)

6. **สังเกตผลที่ RGB LED**:

- เลือกสีแดง (กดปุ่มจนได้ 0)
- หมุนปรับ Potentiometer
- ความสว่างของสีแดงควรเปลี่ยน
- ทำซ้ำกับสีเขียว (1) และสีน้ำเงิน (2)

7. **ตรวจสอบการสื่อสารกับ Server**:

- Serial Monitor ฝั่ง Server ควรแสดง:

```
0
180

```

- บรรทัดแรก: การเลือกสี
- บรรทัดที่สอง: ค่าความสว่าง

## ตัวอย่าง Output บน Serial Monitor

```
Connecting to WiFi...
Connecting to WiFi...
192.168.4.5
0
0
1
1
2
2
0
0

```

(ตัวเลขแทนการเลือกสี RGB: 0=แดง, 1=เขียว, 2=น้ำเงิน)

## Data Protocol

**รูปแบบ Heartbeat** (Client → Server):

```
"10_20&"

ส่งทุก 1 วินาที เพื่อรักษาการเชื่อมต่อ
เป็นข้อมูลหลอก Server จะไม่นำไปประมวลผลเพื่อคุมสี RGB

```

**รูปแบบข้อมูล RGB** (Client → Server):

```
"color_brightness&"

ตัวอย่าง:
  "0_0&"    → ช่องสีแดง, ความสว่าง 0 (ปิด)
  "0_180&"  → ช่องสีแดง, ความสว่าง 180
  "1_255&"  → ช่องสีเขียว, ความสว่าง 255 (สูงสุด)
  "2_100&"  → ช่องสีน้ำเงิน, ความสว่าง 100

รหัสสี:
  0 = แดง (GPIO 4)
  1 = เขียว (GPIO 2)
  2 = น้ำเงิน (GPIO 15)

ความสว่าง: 0-255 (PWM duty cycle)

```

**รูปแบบ Acknowledgment** (Server → Client):

```
"A"

Server ส่งกลับเพื่อรับทราบหลังจากประมวลผลข้อมูล RGB
เป็นการกระตุ้นให้ Client ส่งค่า RGB ชุดใหม่

```

**ธุรกรรม UDP**:

```
1. ทุกๆ 1 วินาที: Client ส่ง "10_20&" (heartbeat)
2. เมื่อกดปุ่ม: ค่า selRGB_BTN เพิ่มขึ้น
3. เมื่อหมุน Pot: ค่า brightness เปลี่ยน
4. Server ตอบกลับ: "A"
5. Client ได้รับ "A"
6. Client ส่ง: "1_180&" (สีปัจจุบัน_ความสว่าง)
7. Server ประมวลผลข้อมูล RGB
8. Server ส่ง: "A"
9. ตัวจับเวลาถูกรีเซ็ต, วนกลับไปขั้นตอนที่ 4

```

## State Machine

```
Color Selection State Machine:

  selRGB_BTN = 0 (RED)
       │
       │ Button Press
       ▼
  selRGB_BTN = 1 (GREEN)
       │
       │ Button Press
       ▼
  selRGB_BTN = 2 (BLUE)
       │
       │ Button Press
       ▼
  selRGB_BTN = 0 (RED)  [wraps around]
       │
      ...

```

## การแก้ไขปัญหาเบื้องต้น (Troubleshooting)

| ปัญหา                 | สาเหตุที่เป็นไปได้       | วิธีแก้ไข                                     |
| --------------------- | ------------------------ | --------------------------------------------- |
| เชื่อมต่อ WiFi ไม่ได้ | Server ไม่ได้เปิด        | เปิด Server ก่อน (lab-w7-3-server)            |
| IP Address ผิด        | ไม่ได้ตั้ง Static IP     | ตรวจสอบ WiFi.config() ก่อน WiFi.begin()       |
| ปุ่มกดไม่ตอบสนอง      | ต่อผิดขา                 | ตรวจสอบการเชื่อมต่อขา GPIO 4                  |
| สีเปลี่ยนข้ามขั้น     | สัญญาณกระเด้ง (Bouncing) | เพิ่มค่าหน่วงเวลา Debounce (20ms→50ms)        |
| Pot ปรับค่าไม่ได้     | ต่อผิดขา                 | GPIO 34 เป็น Input-only, เช็คสายไฟ            |
| ค่า Pot แกว่งไปมา     | ขา Floating              | เช็คสายไฟ Pot ที่เข้า 3.3V และ GND            |
| ไฟ RGB LED ไม่เปลี่ยน | แพ็กเก็ต UDP หาย         | เช็คว่า Server ทำงานปกติหรือไม่               |
| ปรับความสว่างไม่ได้   | ADC ไม่อ่านค่า           | ตรวจสอบการเชื่อมต่อขา ADC ที่ GPIO 34         |
| WiFi หลุดบ่อย         | สัญญาณอ่อน               | ขยับ ESP32 ทั้งสองตัวให้ใกล้กันมากขึ้น        |
| แสดงสีผิด             | ดัชนีสีผิด               | ตรวจสอบ selRGB_BTN: 0=แดง, 1=เขียว, 2=น้ำเงิน |

## อ้างอิงโค้ด (Code Reference)

```cpp
#include <WiFi.h>
#include <WiFiUdp.h>

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.1";
WiFiUDP udp;

// IP Configuration
IPAddress ip(192,168,4,5);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
unsigned long time_out = 0;

String selRGB = "0";
int selRGB_BTN = 0;
bool state1=1,lastState1=1;

void setup() {
  Serial.begin(9600);

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());
  udp.begin(port);

  pinMode(4, INPUT_PULLUP);
}

void send_data(String message) {
  udp.beginPacket(host, port);
  char char_message[255];
  sprintf(char_message, "%s", message.c_str());
  udp.print(char_message);
  udp.flush();
  udp.endPacket();
}

void loop() {
  // Button handling
  state1=digitalRead(4);
  if(state1!=lastState1){
      delay(20); //debounce
      if(digitalRead(4)==0){
        selRGB_BTN++;
        if(selRGB_BTN>2){
          selRGB_BTN=0;
        }
      }
  }
  lastState1=state1;
  Serial.println(selRGB_BTN);
  selRGB = String(selRGB_BTN);

  // Potentiometer reading
  int a = analogRead(34);
  a = map(a, 0, 4095, 0, 255);
  String ab = String(a);

  // Heartbeat (every 1 second)
  if(millis()-time_out >= 1000){
    time_out = millis();
    send_data("10_20&");
  }

  // Receive acknowledgment
  int numbuffer = udp.parsePacket();
  if (numbuffer > 0) {
    char packetBuffer[255];
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String s(packetBuffer);

      if (s == "A") {
        send_data(selRGB+"_"+ab+"&");
        time_out = millis();
      }
    }
  }
  delay(10);
}

```

## สถาปัตยกรรมเครือข่าย

การทดลองนี้แสดงให้เห็นถึง **การสื่อสาร UDP แบบสองทิศทางพร้อมการตอบรับ (Acknowledgment)**:

- **Server**: สร้าง WiFi Access Point, ควบคุม RGB LED
- **Client**: ส่งข้อมูลสี/ความสว่าง, รอรับการตอบรับ
- **Protocol**: UDP พร้อมกลไกตอบรับที่สร้างขึ้นเอง
- **Static IP**: ไคลเอนต์ใช้ IP คงที่เพื่อให้การ Routing แน่นอน
- **Heartbeat**: รักษาการเชื่อมต่อให้ Active, เซิร์ฟเวอร์รู้ว่าไคลเอนต์ยังอยู่
- **Responsive**: เซิร์ฟเวอร์ตอบรับทุกการอัปเดตค่า RGB

## การประยุกต์ใช้งาน

- การควบคุมไฟ RGB ระยะไกล
- ไฟสร้างบรรยากาศที่ปรับสีได้
- ระบบไฟเวที
- หลอดไฟ Smart Home RGB
- การแจ้งเตือนด้วยภาพ
- การควบคุมไฟ Ambient
- อุปกรณ์เสริมสำหรับ Gaming
- งานศิลปะติดตั้ง (Art installations)

## การปรับปรุงเพิ่มเติม

- เพิ่มจอ OLED เพื่อแสดงสี/ความสว่างปัจจุบัน
- ทำระบบเปลี่ยนสีแบบนุ่มนวล (Smooth transition)
- เพิ่มรูปแบบสีสำเร็จรูป (Preset patterns)
- บันทึกสีโปรดลงใน EEPROM
- ทำระบบผสมสี (Color mixing - หลายช่องพร้อมกัน)
- เพิ่มเอฟเฟกต์ Fade
- ทำโหมดวนสีอัตโนมัติ (Auto-cycle)
- เพิ่ม RGB LED หลายตัว
- ทำ Web Interface สำหรับเลือกสี
- ใช้ระบบสีแบบ HSV
- เพิ่มเอฟเฟกต์ไฟกระพริบ (Strobe/Pulse)
