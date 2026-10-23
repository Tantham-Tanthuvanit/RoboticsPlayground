#include "drivers/imu.h"
#include <cmath>

IMUDriver::IMUDriver() {}

bool IMUDriver::init() {
    Wire.begin(IMU_SDA, IMU_SCL); // Explicitly sets the ESP32-S3 I2C pins[cite: 1]
    
    // The BNO086 I2C address is usually 0x4A, or sometimes 0x4B
    if (!bno08x_.begin_I2C(0x4A, &Wire)) {
        if (!bno08x_.begin_I2C(0x4B, &Wire)) {
            return false; // Sensor not found
        }
    }

    // Ask the BNO086 to calculate and report the Rotation Vector every 10,000 microseconds (10ms / 100Hz)
    if (!bno08x_.enableReport(SH2_ROTATION_VECTOR, 10000)) {
        return false;
    }
    
    return true;
}

void IMUDriver::update(float dt) {
    if (dt <= 0.0f) return;


    // Check if the BNO086 has a new calculated packet ready
    if (bno08x_.getSensorEvent(&sensorValue_)) {
        if (sensorValue_.sensorId == SH2_ROTATION_VECTOR) {
            
            // Extract the Quaternions
            float r = sensorValue_.un.rotationVector.real;
            float i = sensorValue_.un.rotationVector.i;
            float j = sensorValue_.un.rotationVector.j;
            float k = sensorValue_.un.rotationVector.k;


            // Convert Quaternions directly into Euler Angles (Yaw, Pitch, Roll) in degrees
            yaw_deg_   = std::atan2(2.0f * (r * k + i * j), 1.0f - 2.0f * (j * j + k * k)) * (180.0f / M_PI);
            pitch_deg_ = std::asin(2.0f * (r * j - k * i)) * (180.0f / M_PI);
            roll_deg_  = std::atan2(2.0f * (r * i + j * k), 1.0f - 2.0f * (i * i + j * j)) * (180.0f / M_PI);
        }
    }
}