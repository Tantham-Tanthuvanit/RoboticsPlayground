#include "drivers/imu.h"
#include <cmath>
#include <algorithm>

IMUDriver::IMUDriver() {}

bool IMUDriver::init() {
    Wire.begin(IMU_SDA, IMU_SCL); // Explicitly sets the ESP32-S3 I2C pins
    Wire.setClock(100000); // 100 kHz standard I2C for BNO08x stability

    // The BNO08x I2C address is usually 0x4A, or 0x4B
    if (bno08x_.begin_I2C(0x4A, &Wire)) {
        Serial.println("[IMU] BNO08x found at address 0x4A");
    } else if (bno08x_.begin_I2C(0x4B, &Wire)) {
        Serial.println("[IMU] BNO08x found at address 0x4B");
    } else {
        Serial.println("[IMU] ERROR: BNO08x not found at 0x4A or 0x4B!");
        Serial.printf("[IMU] Checked SDA pin %d, SCL pin %d\n", IMU_SDA, IMU_SCL);
        initialized_ = false;
        return false;
    }

    // Ask the BNO086 to calculate and report the Rotation Vector every 10,000 microseconds (10ms / 100Hz)
    if (!bno08x_.enableReport(SH2_ROTATION_VECTOR, 10000)) {
        Serial.println("[IMU] ERROR: Could not enable SH2_ROTATION_VECTOR report");
        initialized_ = false;
        return false;
    }
    
    initialized_ = true;
    Serial.println("[IMU] BNO08x initialized successfully.");
    return true;
}

void IMUDriver::update(float dt) {
    if (!initialized_ || dt <= 0.0f) {
        return;
    }

    // Re-enable reports if the sensor underwent a hardware or software reset
    if (bno08x_.wasReset()) {
        Serial.println("[IMU] Sensor reset detected, re-enabling report...");
        bno08x_.enableReport(SH2_ROTATION_VECTOR, 10000);
    }

    // Check if the BNO086 has a new calculated packet ready
    if (bno08x_.getSensorEvent(&sensorValue_)) {
        if (sensorValue_.sensorId == SH2_ROTATION_VECTOR) {
            // Extract the Quaternions
            float r = sensorValue_.un.rotationVector.real;
            float i = sensorValue_.un.rotationVector.i;
            float j = sensorValue_.un.rotationVector.j;
            float k = sensorValue_.un.rotationVector.k;

            // Convert Quaternions directly into Euler Angles (Yaw, Pitch, Roll) in degrees
            yaw_deg_ = std::atan2(2.0f * (r * k + i * j), 1.0f - 2.0f * (j * j + k * k)) * (180.0f / M_PI);

            // Guard against floating point precision causing asin argument to fall outside [-1, 1]
            float sinp = 2.0f * (r * j - k * i);
            if (std::abs(sinp) >= 1.0f) {
                pitch_deg_ = std::copysign(90.0f, sinp);
            } else {
                pitch_deg_ = std::asin(sinp) * (180.0f / M_PI);
            }

            roll_deg_ = std::atan2(2.0f * (r * i + j * k), 1.0f - 2.0f * (i * i + j * j)) * (180.0f / M_PI);
        }
    }
}