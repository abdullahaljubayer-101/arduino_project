# 🤖 Arduino & ESP32 Projects

![Arduino](https://img.shields.io/badge/Arduino-00878F?style=flat-square&logo=arduino&logoColor=white)
![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=flat-square&logo=espressif&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=flat-square&logo=cplusplus&logoColor=white)

A collection of hardware projects I built with **Arduino and ESP32**: an RFID + fingerprint access system, a Wi-Fi garage door, an ESP32-CAM security camera, a remote-controlled car and some Wi-Fi/servo experiments. 🔌⚡

This is where I got to take code off the screen and make it move things in the real world.

---

## 🧰 Projects

### 🏢 `Team Paradox/`: smart security builds

| Project | Board | What it does |
| ------- | ----- | ------------ |
| 🔐 **Final** | Arduino | The main access-control system. **RFID cards** unlock *Office 01* or *Office 02*, unknown cards trigger an "Access Denied" buzzer, a **fingerprint sensor** adds a second way in, and a **DHT11** turns on a cooling output when it gets ≥ 26 °C. Has *Check* and *Registration* modes. |
| 🖐️ **DoorSecurity** | Arduino | Fingerprint tools: enroll, check, delete and wipe fingerprints, plus a relay controller (`O` = open, `C` = close) for the door lock |
| 🚗 **GarageDoor** | ESP32 | Hosts a small web server. Visit `/open` or `/close` to move the servo-powered garage door |
| 📷 **SecurityCamera** | ESP32-CAM | Streams live camera video over Wi-Fi |
| 🏎️ **Car** | Arduino | A 2-motor car controlled over serial: `F` forward, `B` back, `L` left, `R` right, `T` stop |

Circuit diagrams are included for the car (`Car.png`), the garage door (`GarageDoor.png`) and the RFID wiring (`rfid-wiring.png`). 🗺️

### 🧪 `Main/`: experiments

| Sketch | What it does |
| ------ | ------------ |
| **ServoController** | Type an angle in the Serial Monitor and the servo turns to it |
| **WifiControllerSTA** | ESP32 web server with routes to turn an LED on/off and move a servo left/right |
| **WifiControllerSAP** | Empty starter sketch for an access-point version |

### 📚 `libraries/`

Copies of the Arduino libraries the sketches use, so everything compiles the same way: Adafruit Fingerprint, Adafruit Motor Shield, MFRC522, FPM, DHT, ESP32Servo, AsyncTCP and ESPAsyncWebServer.

## 🚀 How to use

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) and add **ESP32 board support** if you're using the ESP32 sketches.
2. Copy the folders inside `libraries/` into your Arduino `libraries` folder.
3. Open the `.ino` file you want, select your board and port, and upload.
4. For Wi-Fi sketches, change `ssid` and `password` to your own network before uploading.

## 🔩 Hardware used

`Arduino` · `ESP32` · `ESP32-CAM` · `MFRC522 RFID` · `Fingerprint sensor` · `DHT11` · `Servo motors` · `Relay module` · `DC motors` · `Buzzer`

---

Made by **Abdullah Al Jubayer** · [GitHub](https://github.com/abdullahaljubayer-101) · [Portfolio](https://abdullahaljubayer.com)
