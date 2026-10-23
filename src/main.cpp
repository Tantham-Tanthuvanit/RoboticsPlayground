#include <Arduino.h>
#include "drivers/imu.h"

IMUDriver imu;

uint32_t last_time_ms = 0;
uint32_t last_print_ms = 0;

void setup() {
    Serial.begin(115200);
    delay(1000); // Allow Serial Monitor time to connect

    Serial.println("=================================");
    Serial.println("Starting IMU Roll/Pitch/Yaw Test...");
    Serial.println("=================================");

    // Initialize I2C and IMU
    if (!imu.init()) {
        Serial.println("[ERROR] IMU not detected! Check your SDA (GPIO 21) and SCL (GPIO 22) wiring.");
    } else {
        Serial.println("[SUCCESS] IMU initialized successfully!");
    }

    last_time_ms = millis();
}

void loop() {
    uint32_t current_time_ms = millis();
    
    // Calculate exact time delta (dt) in seconds
    float dt = static_cast<float>(current_time_ms - last_time_ms) / 1000.0f;
    last_time_ms = current_time_ms;

    // Update sensor computations
    imu.update(dt);

    // Print to the Serial Monitor / Console every 100ms (10 Hz) to avoid flooding the text stream
    if (current_time_ms - last_print_ms >= 100) {
        last_print_ms = current_time_ms;

        Serial.printf("Roll: %6.2f°  |  Pitch: %6.2f°  |  Yaw: %6.2f°\n",
                      imu.getRoll(),
                      imu.getPitch(),
                      imu.getYaw());
    }

    // Small delay to keep the loop stable
    delay(10);
}