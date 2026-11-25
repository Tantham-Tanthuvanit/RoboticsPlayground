# PS4 Controller Teleop for ESP32-S3 Robot

This teleoperation bridge connects a Sony PS4 DualShock 4 controller (or keyboard fallback) to your ESP32-S3 robot over Wi-Fi UDP.

---

## Quick Start (Option A: ESP32-S3 Wi-Fi Bridge)

### 1. Flash the ESP32-S3 Firmware
In your project root, upload the firmware to your ESP32-S3:
```bash
~/.platformio/penv/bin/pio run -t upload
```

When powered on, the ESP32-S3 will host a Wi-Fi Access Point:
* **SSID:** `Robot-S3`
* **Password:** `12345678`
* **Robot IP:** `192.168.4.1`
* **UDP Port:** `4210`

### 2. Connect Your Laptop to the Robot's Wi-Fi
On your laptop/PC:
1. Open Wi-Fi settings and connect to `Robot-S3` with password `12345678`.

### 3. Connect Your PS4 Controller to Your Laptop
1. Hold **Share + PS Button** on the PS4 controller until the light bar flashes rapidly.
2. Pair it via your computer's Bluetooth settings (or plug it via USB).

### 4. Run the Teleop Bridge Script
Install requirements and run:
```bash
pip install -r scripts/requirements.txt
python3 scripts/ps4_teleop.py
```

* **Left Stick (Vertical):** Forward / Reverse
* **Right Stick (Horizontal):** Turn Left / Turn Right
* **Circle / Cross Button:** Emergency Stop

> **Tip:** If you don't have the PS4 controller handy, simply run:
> ```bash
> python3 scripts/ps4_teleop.py --keyboard
> ```
> This allows you to steer the robot immediately using **W/A/S/D** or Arrow keys.

---

## Switching to Option B (Direct Wireless with Classic ESP32)

If you replace the ESP32-S3 with a classic ESP32 (WROOM-32 / DevKit v1) which has native Bluetooth Classic:

1. In [include/config.h](file:///home/tantham-tanthuvanit/projects/RoboticsPlayground/include/config.h), set:
   ```cpp
   #define TELEOP_MODE TELEOP_MODE_BT_PS4
   ```
   or build using the included Classic BT environment:
   ```bash
   ~/.platformio/penv/bin/pio run -e esp32_classic_bt -t upload
   ```
2. Put the PS4 controller in pairing mode (Share + PS Button). It will connect directly to the ESP32 without any laptop or Wi-Fi needed.
