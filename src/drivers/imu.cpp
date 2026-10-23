#include "drivers/imu.h"
#include <cmath>

// Standard I2C address for MPU6050 / similar IMUs
constexpr uint8_t IMU_I2C_ADDR = 0x68;

IMUDriver::IMUDriver() {}

bool IMUDriver::init() {
    Wire.begin(); // ESP32 default pins: SDA = GPIO 21, SCL = GPIO 22

    // Wake up the IMU (Power Management register 1)
    Wire.beginTransmission(IMU_I2C_ADDR);
    Wire.write(0x6B); 
    Wire.write(0x00); 
    uint8_t error = Wire.endTransmission();

    return (error == 0); // Returns true if sensor acknowledged the I2C transmission
}

void IMUDriver::update(float dt) {
    if (dt <= 0.0f) return;

    // 1. Request raw accelerometer data from register 0x3B (6 bytes: X, Y, Z high/low)
    Wire.beginTransmission(IMU_I2C_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(IMU_I2C_ADDR, static_cast<uint8_t>(6), true);

    if (Wire.available() >= 6) {
        int16_t ax = (Wire.read() << 8) | Wire.read();
        int16_t ay = (Wire.read() << 8) | Wire.read();
        int16_t az = (Wire.read() << 8) | Wire.read();

        // Convert raw bits to g's (assuming default ±2g sensitivity scale: 16384 LSB/g)
        float accel_x = static_cast<float>(ax) / 16384.0f;
        float accel_y = static_cast<float>(ay) / 16384.0f;
        float accel_z = static_cast<float>(az) / 16384.0f;

        // 2. Compute Roll and Pitch from gravity vector using trigonometry
        roll_deg_  = std::atan2(accel_y, std::sqrt(accel_x * accel_x + accel_z * accel_z)) * (180.0f / M_PI);
        pitch_deg_ = std::atan2(-accel_x, std::sqrt(accel_y * accel_y + accel_z * accel_z)) * (180.0f / M_PI);
    }

    // 3. For Yaw, you integrate gyroscope Z-axis rate over dt. 
    // (Placeholder integration example; replace with your gyro Z reading if available)
    // float gyro_z_deg_s = ...; 
    // yaw_deg_ += gyro_z_deg_s * dt;
}