#pragma once
#include <Arduino.h>
#include <Wire.h>

class IMUDriver {
public:
    IMUDriver();

    // Initializes the I2C bus and wakes up the sensor
    bool init();

    // Updates calculations (reads accelerometer/gyro)
    void update(float dt);

    // Getters for orientation angles in degrees
    float getRoll() const { return roll_deg_; }
    float getPitch() const { return pitch_deg_; }
    float getYaw() const { return yaw_deg_; }

    // Resets yaw heading to zero
    void resetYaw() { yaw_deg_ = 0.0f; }

private:
    float roll_deg_ = 0.0f;
    float pitch_deg_ = 0.0f;
    float yaw_deg_ = 0.0f;
};