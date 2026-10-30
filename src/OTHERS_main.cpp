/*
#include <config.h>
#include <Arduino.h>
#include "drivers/imu.h"

IMUDriver imu;
uint32_t last_time_ms = 0;

void setup() {
    Serial.begin(115200);
    delay(1000); // Give USB Serial monitor time to connect

    Serial.println("\n--- Initializing IMU ---");
    if (!imu.init()) {
        Serial.println("[WARNING] IMU initialization failed! System running in fallback mode.");
    } else {
        Serial.println("no imu ini");
    }

    last_time_ms = millis();
}

void loop() {
    uint32_t current_time = millis();
    float dt = static_cast<float>(current_time - last_time_ms) / 1000.0f;

    last_time_ms = current_time;

    if (dt <= 0.0f) {
        return;
    }
    
    imu.update(dt);
    if (imu.isInitialized()) {
        Serial.printf("roll: %6.2f | pitch: %6.2f | yaw: %6.2f\n", imu.getRoll(), imu.getPitch(), imu.getYaw());
    } else {
        Serial.println("not init");
    }

    delay(100);
}
*/