# Lab W7-3-SERVER: เซิร์ฟเวอร์ควบคุมไฟ RGB LED ผ่าน ESP32 WiFi UDP

**ไฟล์**: `lab-w7-3-server.ino`

## คำอธิบาย

การทดลองนี้สาธิตการทำงานของเซิร์ฟเวอร์ ESP32 WiFi UDP สำหรับควบคุมไฟ RGB LED โดย ESP32 จะสร้าง WiFi Access Point และรอรับแพ็กเก็ต UDP ที่บรรจุข้อมูลสีและความสว่าง ไฟ RGB LED จะแสดงสีที่เลือก (แดง, เขียว หรือ น้ำเงิน) ตามระดับความสว่างที่กำหนด (0-255) และเซิร์ฟเวอร์จะส่งสัญญาณตอบรับ (acknowledge) กลับไปทุกครั้งที่มีการอัปเดตค่า นี่คือฝั่ง **Server** ที่ทำหน้าที่ควบคุมอุปกรณ์ RGB LED จริง ตามคำสั่งที่ได้รับจากฝั่ง Client (lab-w7-3-client)

## อุปกรณ์ที่ต้องใช้

- ESP32 DEVKIT × 1
- RGB LED (แบบ Common Cathode) × 1
- ตัวต้านทาน 220Ω × 1 (ใช้ร่วมกันทุกสี)
- Breadboard (โปรโตบอร์ด)
- สายจั๊มเปอร์

## การกำหนดค่าขา (Pin Configuration)

| ขา ESP32 | อุปกรณ์ | การเชื่อมต่อ                              | หมายเหตุ             |
| -------- | ------- | ----------------------------------------- | -------------------- |
| GPIO 4   | RGB LED | ขา Anode สีแดง                            | รองรับ PWM           |
| GPIO 2   | RGB LED | ขา Anode สีเขียว                          | รองรับ PWM           |
| GPIO 15  | RGB LED | ขา Anode สีน้ำเงิน                        | รองรับ PWM           |
| GND      | RGB LED | ขา Common Cathode → ตัวต้านทาน 220Ω → GND | ใช้ตัวต้านทานร่วมกัน |

## แผนผังวงจร (ASCII Wiring Diagram)

```
        ESP32 DEVKIT         RGB LED (Common Cathode)
      ┌─────────────┐
      │             │              ┌───────┐
      │   GPIO 4 ●──┼──────────────┤ R (+) │
      │             │              │       │
      │   GPIO 2 ●──┼──────────────┤ G (+) │
      │             │              │ RGB   │
      │  GPIO 15 ●──┼──────────────┤ B (+) │
      │             │              │       │
      │             │              │ COM(-)│───┐
      │             │              └───────┘   │
      │   GND    ●──┼──────────────────────────┴── [220Ω] ── GND
      └─────────────┘                              (Single resistor)

      WiFi: Creates AP "ESP32-Nihahaha"
      UDP Server: Listens on port 6969
      IP Address: 192.168.4.1
      Server responds to: 192.168.4.5

      Receives: "color_brightness&"
      Examples: "0_255&" (Red, full brightness)
                "1_128&" (Green, half brightness)
                "2_0&"   (Blue, off)
      Sends: "A" (acknowledgment)

      Color Codes:
        0 = Red (GPIO 4)
        1 = Green (GPIO 2)
        2 = Blue (GPIO 15)

```

## การต่อวงจรบน Breadboard

1. วาง ESP32 ลงบน Breadboard
2. วาง RGB LED ลงบน Breadboard (ระบุตำแหน่งขาให้ถูกต้อง)
3. เชื่อมต่อขา Anode ของ RGB LED (ขายาวรองลงมา หรือขาสั้น):

- Anode สีแดง → GPIO 4
- Anode สีเขียว → GPIO 2
- Anode สีน้ำเงิน → GPIO 15

4. เชื่อมต่อขา Common Cathode ของ RGB LED (ขายาวที่สุด หรือที่มีสัญลักษณ์):

- Common Cathode → ตัวต้านทาน 220Ω → ราง GND

5. **หมายเหตุ**: ใช้ตัวต้านทาน 220Ω เพียงตัวเดียวต่อลงกราวด์ร่วมกัน (ตามข้อกำหนดใหม่)

## รายละเอียดเฉพาะของอุปกรณ์

### ESP32 DEVKIT

- **WiFi**: 802.11 b/g/n
- **แรงดันทำงาน**: Logic 3.3V, แหล่งจ่ายไฟ 5V
- **PWM**: ใช้โมดูล LEDC (LED Control) สำหรับควบคุมความสว่าง
- **โหมด WiFi**: โหมด Access Point (AP)
- **GPIO 4, 2, 15**: ทุกขารองรับ PWM

### RGB LED (แบบ Common Cathode)

- **ประเภท**: LED RGB 4 ขา แบบ Common Cathode
- **การระบุขา**:
- **ขายาวที่สุด**: Common Cathode (-)
- ขาสั้นอีกสามขา: Anode (+) ของ R, G, B
- โดยทั่วไปเรียงเป็น: R, Common(-), G, B (แต่ควรตรวจสอบ Datasheet อีกครั้ง)

- **แรงดันตกคร่อม (Forward Voltage)**:
- แดง: ~2.0V
- เขียว: ~3.0V
- น้ำเงิน: ~3.0V

- **กระแส (Forward Current)**: 20mA ต่อสี (โดยทั่วไป)
- **Common Cathode**: ขา Cathode ทั้งหมดเชื่อมต่อกันภายใน
- **การควบคุม PWM**: ปรับความสว่างผ่าน Duty Cycle (0-255)

### การกำหนดค่าตัวต้านทาน

- **ค่า**: 220Ω (แดง-แดง-น้ำตาล)
- **การติดตั้ง**: ใช้ตัวต้านทานตัวเดียวที่ขา Common Cathode ต่อลง GND
- **วัตถุประสงค์**: จำกัดกระแสสำหรับทั้งสามสี
- **อัตราทนกำลังไฟ**: 1/4W ก็เพียงพอ

## การตั้งค่าเครือข่าย

```
Network Topology:

┌──────────────────┐         WiFi          ┌──────────────────┐
│   ESP32 Client   │  ◄──────────────►     │   ESP32 Server   │
│(lab-w7-3-client) │   "ESP32-Nihahaha"    │  (This Device)   │
│  192.168.4.5     │                       │  192.168.4.1     │
└──────────────────┘                       └──────────────────┘
       │                                           │
   Button + Pot                                    │
   (Color + Brightness)                     RGB LED (GPIO 4,2,15)
       │                                           │
       │  RGB Data: "1_180&"                      │
       └─────────────────────────────────────────►│
       ◄──────────────────────────────────────────┘
                Response: "A"
            192.168.4.1:6969 ◄──► 192.168.4.5:6969
```

### รายละเอียดเครือข่าย

- **SSID**: "ESP32-Nihahaha"
- **Password**: "12345678"
- **Server IP**: 192.168.4.1 (IP ของ Access Point)
- **Server Port**: 6969
- **Protocol**: UDP (WiFiUDP)
- **WiFi Mode**: AP+STA (Access Point + Station)
- **Client IP**: 192.168.4.5 (กำหนดคงที่ (Static) ที่ฝั่ง Client)
- **เป้าหมายการตอบกลับ**: กำหนดตายตัว (Hardcoded) ไปที่ 192.168.4.5

## หมายเหตุพิเศษ

- **โหมด WiFi**: Access Point (AP) mode - สร้างเครือข่าย WiFi ของตัวเอง
- **โปรโตคอล**: UDP (ไม่มีการเชื่อมต่อค้างไว้, สื่อสารสองทิศทาง)
- **รูปแบบข้อมูล**: รับค่า "color_brightness&" (เช่น "0_180&", "1_255&", "2_100&")
- **การประมวลผล (Parsing)**: แยกข้อความ (String) ด้วยตัวคั่น "\_" และ "&"
- **รหัสสี**: 0=แดง, 1=เขียว, 2=น้ำเงิน
- **ความสว่าง**: 0-255 (PWM duty cycle)
- **การตอบรับ (Acknowledgment)**: ส่ง "A" กลับไปยัง 192.168.4.5 หลังจากประมวลผลเสร็จ
- **การติดตามสถานะ**: อาร์เรย์ `led_data[3]` จะเก็บค่าความสว่างของ R, G, B ไว้
- **ตัวต้านทานร่วม**: ใช้ 220Ω ตัวเดียวที่ขา Common Cathode
- **Hardcoded Client**: ตอบกลับเฉพาะ IP 192.168.4.5 เท่านั้น
- Serial baud rate: 9600

## หลักการทำงาน

1. **การตั้งค่า WiFi**:

- ESP32 สร้าง Access Point ชื่อ "ESP32-Nihahaha"
- ตั้งโหมดเป็น WIFI_AP_STA
- ได้รับ IP Address 192.168.4.1
- พิมพ์ "AP IP: 192.168.4.1" ออกทาง Serial

2. **การเริ่มต้น UDP**:

- `udp.begin(port)` เริ่มรอรับข้อมูลที่พอร์ต 6969
- Server พร้อมรับแพ็กเก็ต UDP

3. **การเริ่มต้น RGB LED**:

- กำหนด GPIO 4, 2, 15 เป็น PWM
- อาร์เรย์ `led_data[]` ถูกตั้งค่าเริ่มต้นเป็น {0, 0, 0} (ปิดทั้งหมด)

4. **การรับแพ็กเก็ต** (Loop):

- ตรวจสอบแพ็กเก็ต UDP ที่เข้ามา
- อ่านข้อมูลแพ็กเก็ตลงใน Buffer
- แปลงข้อมูลเป็น String

5. **การแยกข้อมูล (Data Parsing)**:

- ฟังก์ชัน `splitString()` ประมวลผลข้อมูลอินพุต
- แยกข้อมูลด้วยตัวคั่น "\_" และ "&"
- ดึงค่าออกมา 2 ค่า:
- `data[0]`: การเลือกสี (0, 1 หรือ 2)
- `data[1]`: ความสว่าง (0-255)

6. **การอัปเดต RGB**:

- อัปเดตอาร์เรย์: `led_data[data[0]] = data[1]`
- เขียนค่า PWM ไปยังทั้งสามสี:
- `analogWrite(GPIO_4, led_data[0])` - สีแดง
- `analogWrite(GPIO_2, led_data[1])` - สีเขียว
- `analogWrite(GPIO_15, led_data[2])` - สีน้ำเงิน

- หน่วงเวลา 50ms เพื่อความเสถียร

7. **การตอบรับ (Acknowledgment)**:

- เริ่มสร้างแพ็กเก็ต UDP ส่งไปยัง 192.168.4.5:6969
- ส่งข้อความ "A"
- จบการส่งแพ็กเก็ต

8. **การแสดงผล Serial**:

- พิมพ์ดัชนีสี (data[0])
- พิมพ์ค่าความสว่าง (data[1])

## ขั้นตอนการทดสอบ

1. **อัปโหลดสเก็ตช์ฝั่ง Server**:

- อัปโหลดสเก็ตช์นี้ลงใน ESP32 ตัวที่เป็น Server
- เปิด Serial Monitor (9600 baud)

2. **ตรวจสอบ Access Point**:

- ตรวจสอบ Serial Monitor ว่าขึ้น "AP IP: 192.168.4.1" หรือไม่
- บนมือถือหรือคอมพิวเตอร์ คุณควรจะเห็นชื่อ WiFi "ESP32-Nihahaha"

3. **ตรวจสอบสถานะเริ่มต้นของ RGB**:

- RGB LED ควรจะดับอยู่ (ทุกสีค่าเป็น 0)

4. **ตั้งค่าฝั่ง Client**:

- อัปโหลดไฟล์ lab-w7-3-client.ino ลงใน ESP32 อีกตัวหนึ่ง
- ต่อปุ่มกดเข้ากับ GPIO 4
- ต่อตัวต้านทานปรับค่าได้ (Potentiometer) เข้ากับ GPIO 34
- Client ควรจะเชื่อมต่อกับ AP "ESP32-Nihahaha" ได้

5. **ทดสอบช่องสีแดง**:

- กดปุ่มที่ Client เพื่อเลือกสี 0 (แดง)
- หมุนปรับ Potentiometer ที่ Client
- Serial Monitor ฝั่ง Server จะแสดง:

```
0
180

```

- ส่วนสีแดงของ RGB LED จะเปลี่ยนความสว่าง

6. **ทดสอบช่องสีเขียว**:

- กดปุ่มที่ Client เพื่อเลือกสี 1 (เขียว)
- หมุนปรับ Potentiometer
- Server จะแสดง:

```
1
200

```

- ส่วนสีเขียวของ RGB LED จะเปลี่ยนความสว่าง

7. **ทดสอบช่องสีน้ำเงิน**:

- กดปุ่มที่ Client เพื่อเลือกสี 2 (น้ำเงิน)
- หมุนปรับ Potentiometer
- Server จะแสดง:

```
2
150

```

- ส่วนสีน้ำเงินของ RGB LED จะเปลี่ยนความสว่าง

8. **ทดสอบการผสมสี**:

- ตั้งค่าสีแดงเป็น 255 (สูงสุด)
- ตั้งค่าสีเขียวเป็น 255 (สูงสุด)
- ผลลัพธ์: สีเหลือง (R+G)
- ตั้งค่าสีน้ำเงินเป็น 255 (สูงสุด)
- ผลลัพธ์: สีขาว (R+G+B)

## ตัวอย่าง Output บน Serial Monitor

```
AP IP: 192.168.4.1
0
180
0
200
0
255
1
100
1
150
2
50
2
255

```

(บรรทัดแรก: ดัชนีสี, บรรทัดที่สอง: ค่าความสว่าง)

## Data Protocol

**รูปแบบคำขอ** (Client → Server):

```
"color_brightness&"

ตัวอย่าง:
  "0_255&"  → ช่องสีแดง, ความสว่าง 255 (สูงสุด)
  "1_128&"  → ช่องสีเขียว, ความสว่าง 128 (ครึ่งเดียว)
  "2_0&"    → ช่องสีน้ำเงิน, ความสว่าง 0 (ปิด)

รหัสสี:
  0 = แดง (GPIO 4)
  1 = เขียว (GPIO 2)
  2 = น้ำเงิน (GPIO 15)

ความสว่าง: 0-255 (PWM duty cycle)
  0 = ปิด
  128 = สว่างครึ่งหนึ่ง
  255 = สว่างเต็มที่

```

**รูปแบบการตอบกลับ** (Server → Client):

```
"A"

ตัวอักษรเดียวเพื่อแจ้งรับทราบ (Acknowledgment)
ส่งไปยัง 192.168.4.5:6969

```

**ธุรกรรม UDP**:

```
1. Client ส่ง UDP "1_180&" ไปที่ 192.168.4.1:6969
2. Server รับแพ็กเก็ต
3. Server แยกข้อมูล: data[0]=1 (เขียว), data[1]=180
4. Server อัปเดตค่า: led_data[1] = 180
5. Server สั่งงาน PWM:
   - analogWrite(4, led_data[0])   // แดง
   - analogWrite(2, led_data[1])   // เขียว (180)
   - analogWrite(15, led_data[2])  // น้ำเงิน
6. Server ส่ง UDP "A" ไปที่ 192.168.4.5:6969
7. Client ได้รับ "A"
8. Client ส่งค่าอัปเดตถัดไป

```

## การผสมสี RGB

```
สีแม่สี (Primary Colors - ช่องเดียว):
  แดง:    (255, 0,   0)
  เขียว:  (0,   255, 0)
  น้ำเงิน: (0,   0,   255)

สีทุติยภูมิ (Secondary Colors - สองช่อง):
  เหลือง (Yellow):  (255, 255, 0)   → แดง + เขียว
  ฟ้าอมเขียว (Cyan):    (0,   255, 255) → เขียว + น้ำเงิน
  ม่วงแดง (Magenta): (255, 0,   255) → แดง + น้ำเงิน

สีขาว (White - ทุกช่อง):
  ขาว: (255, 255, 255)

ปิด (Off):
  ดำ: (0, 0, 0)

ตัวอย่างในการทดลองนี้:
1. ตั้งค่า R=255, G=0, B=0    → สีแดง
2. ตั้งค่า R=255, G=255, B=0 → สีเหลือง
3. ตั้งค่า R=255, G=255, B=255 → สีขาว

```

## การแก้ไขปัญหาเบื้องต้น (Troubleshooting)

| ปัญหา                      | สาเหตุที่เป็นไปได้     | วิธีแก้ไข                                         |
| -------------------------- | ---------------------- | ------------------------------------------------- |
| ไฟ RGB LED ไม่ติด          | ขั้วผิด                | ตรวจสอบขา Common Cathode ลง GND, Anodes เข้า GPIO |
| ติดแค่สีเดียว              | จับคู่ขาผิด            | ตรวจสอบ R→4, G→2, B→15                            |
| แสดงสีผิดเพี้ยน            | สลับขา RGB             | ตรวจสอบตำแหน่งขา RGB LED กับ Datasheet            |
| ไฟ RGB LED หรี่เกินไป      | ตัวต้านทานค่าสูงไป     | ตรวจสอบว่าเป็น 220Ω ไม่ใช่ 2.2kΩ                  |
| ไฟ RGB LED สว่างจ้าเกินไป  | ไม่มีตัวต้านทาน        | ใส่ตัวต้านทาน 220Ω ที่ขา Common Cathode           |
| สีไม่ผสมกัน                | ชนิด LED ผิด           | ต้องใช้แบบ Common Cathode ไม่ใช่ Common Anode     |
| มองไม่เห็น WiFi AP         | โค้ดยังไม่ทำงาน        | ตรวจสอบ Serial Monitor, ลองอัปโหลดใหม่            |
| Client ไม่ตอบสนอง          | Client IP ผิด          | Server กำหนดตายตัวไว้ที่ 192.168.4.5              |
| ไม่มีการส่ง Acknowledgment | การตอบกลับ UDP ล้มเหลว | ตรวจสอบว่า IP ของ Client คือ 192.168.4.5          |
| Serial แสดงค่าผิด          | การแยกข้อความผิดพลาด   | ตรวจสอบรูปแบบ String ว่าเป็น "X_Y&" หรือไม่       |

## อ้างอิงโค้ด (Code Reference)

```cpp
#include <WiFi.h>
#include <WiFiUdp.h>

#define LED_RED_PIN 4
#define LED_GREEN_PIN 2
#define LED_BLUE_PIN 15

// Wifi credentials
const char* ssid = "ESP32-Nihahaha";
const char* password = "12345678";
const uint16_t port = 6969;
char *host = "192.168.4.5";
WiFiUDP udp;

char packetBuffer[255];

int data[2] = {0};
int led_data[3] = {0,0,0};

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  udp.begin(port);
}

void loop() {
  int numbuffer = udp.parsePacket();
  if (numbuffer > 0) {
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String s(packetBuffer);

      splitString(s);
      led_data[data[0]] = data[1];
      analogWrite(LED_RED_PIN, led_data[0]);
      analogWrite(LED_GREEN_PIN, led_data[1]);
      analogWrite(LED_BLUE_PIN, led_data[2]);
      delay(50);

      Serial.println(data[0]);
      Serial.println(data[1]);
    }

    udp.beginPacket(host, port);
    udp.print("A");
    udp.flush();
    udp.endPacket();
  }
}

void splitString(String str) {
  int index = 0;
  String s = "";
  for (int i = 0; i < str.length(); i++) {
    if (str[i] != '_' && str[i] != '&') {
      s += str[i];
    } else {
      data[index] = s.toInt();
      index++;
      s = "";
    }
  }
  data[index] = s.toInt();
}

```

## การควบคุมความสว่างด้วย PWM

```
ESP32 PWM (LEDC):

analogWrite(pin, value);
  - value: 0-255
  - 0 = 0% duty cycle (ปิด)
  - 128 = 50% duty cycle (สว่างครึ่งหนึ่ง)
  - 255 = 100% duty cycle (สว่างเต็มที่)

PWM Waveform (50% duty cycle):
     ┌────┐    ┌────┐    ┌────┐
     │    │    │    │    │    │
  ───┘    └────┘    └────┘    └────

  ON: 50% ของเวลาทั้งหมด
  OFF: 50% ของเวลาทั้งหมด
  Average current: 50% ของกระแสสูงสุด
  Brightness: 50%

LED จะรับรู้ความเข้มแสงเฉลี่ย (Average light intensity)

```

## ชนิดของ RGB LED

⚠️ **สำคัญ**: การทดลองนี้ใช้ RGB LED แบบ COMMON CATHODE

**Common Cathode** (ใช้ในการทดลองนี้):

- ขา Common: Cathode (-) ต่อลง GND
- ขา R, G, B: Anodes (+) ต่อเข้า GPIO
- GPIO HIGH = ไฟติด
- GPIO LOW = ไฟดับ
- PWM HIGH % = ความสว่างมาก

**Common Anode** (ไม่ได้ใช้ในการทดลองนี้):

- ขา Common: Anode (+) ต่อเข้า VCC
- ขา R, G, B: Cathodes (-) ต่อเข้า GPIO
- GPIO LOW = ไฟติด
- GPIO HIGH = ไฟดับ
- PWM LOW % = ความสว่างมาก
- ต้องแก้โค้ด (กลับค่า PWM)

## การคำนวณวงจร

```
การใช้ตัวต้านทาน 220Ω ตัวเดียวที่ขา Common Cathode:

สมมติว่า LED สีแดงสว่างสุด (255):
  ESP32 3.3V → Red LED (แรงดันตกคร่อม 2V) → 220Ω → GND

  แรงดันตกคร่อมตัวต้านทาน: 3.3V - 2V = 1.3V
  กระแส: I = V/R = 1.3V / 220Ω ≈ 6mA

  ✓ ปลอดภัย: 6mA < 20mA (กระแสสูงสุดของ LED)
  ✓ ปลอดภัยต่อ GPIO: 6mA < 40mA (สูงสุดต่อขา)

ถ้าเปิดหลายสีพร้อมกัน:
  แต่ละสีใช้ขา GPIO แยกกัน
  กระแสของแต่ละสีถูกจำกัดโดยตัวต้านทานที่ใช้ร่วมกัน
  กระแสรวมที่ไหลลง GND = ผลรวมของกระแสแต่ละสีที่เปิด
  สูงสุด (เปิดเต็มที่ทั้ง 3 สี): ~18mA รวม
  ✓ ปลอดภัยสำหรับขา GND ของ ESP32

```

## สถาปัตยกรรมเครือข่าย

การทดลองนี้แสดงให้เห็นถึง **การควบคุม RGB แบบสองทิศทางผ่าน UDP**:

- **Server**: สร้าง WiFi Access Point และควบคุม RGB LED ผ่าน PWM
- **Client**: ส่งคำสั่ง สี/ความสว่าง และรอรับการตอบรับ (acknowledgments)
- **Protocol**: UDP พร้อมระบบตอบรับที่สร้างขึ้นเอง
- **Hardcoded Response**: Server จะตอบกลับไปที่ 192.168.4.5 เสมอ
- **Real-time**: การอัปเดตสี/ความสว่างมีความหน่วงต่ำ (Low-latency)
- **State Persistence**: RGB LED จะคงค่าสีล่าสุดไว้จนกว่าจะมีการเปลี่ยนแปลง

## การประยุกต์ใช้งาน

- ไฟสร้างบรรยากาศแบบ RGB (Mood lighting)
- หลอดไฟ LED อัจฉริยะ
- ของตกแต่งที่เปลี่ยนสีได้
- การแจ้งเตือนด้วยแสง (ไฟบอกสถานะ)
- ไฟเวทีหรือไฟในงานอีเวนต์
- ไฟตกแต่งห้องเกม (Gaming ambient lighting)
- การแสดงแสงสีตามเสียงเพลง (Music visualization)
- งานศิลปะติดตั้ง (Art installations)
- อุปกรณ์สวมใส่มีไฟ LED
- อุปกรณ์บำบัดด้วยแสงสี (Color therapy)

## การปรับปรุงเพิ่มเติม

- รองรับ Client หลายตัว (แยกตาม IP)
- เพิ่มการเปลี่ยนสีแบบนุ่มนวล (Fade transition)
- ใช้งานระบบสีแบบ HSV
- เพิ่มชุดสีที่ตั้งค่าไว้ล่วงหน้า (Presets/Scenes)
- บันทึกสีโปรดลงใน EEPROM
- เพิ่มเอฟเฟกต์ไฟกระพริบ (Strobe/Pulse)
- ทำไฟสีรุ้งหมุนวน (Rainbow cycle)
- เพิ่มหน้าเว็บสำหรับควบคุม (Web interface)
- รองรับ RGB LED หลายตัว
- เปลี่ยนไปใช้หลอดไฟแบบ Addressable (WS2812B)
- ทำระบบซิงค์ไฟกับจังหวะเพลง
- เพิ่มการตั้งเวลา (Scheduling) สำหรับเปลี่ยนสี
