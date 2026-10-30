#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO08x.h>
#include "config.h"

class IMUDriver {
public:
    IMUDriver();
    bool init();
    void update(float dt);

    bool isInitialized() const { return initialized_; }
    float getRoll() const { return roll_deg_; }
    float getPitch() const { return pitch_deg_; }
    float getYaw() const { return yaw_deg_; }

private:
    Adafruit_BNO08x bno08x_;
    sh2_SensorValue_t sensorValue_;
    bool initialized_ = false;
    
    float roll_deg_ = 0.0f;
    float pitch_deg_ = 0.0f;
    float yaw_deg_ = 0.0f;
};