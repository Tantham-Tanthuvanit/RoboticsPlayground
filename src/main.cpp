#include <config.h>
#include <Arduino.h>
#include "drivers/lidar.h"

// iinit lidar with the pins in config.h and on hardware serial 1
LidarSensor lidar(LIDAR_RX_PIN, LIDAR_MOTOR_PWM_PIN, 1);

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Initializing lidar node...");
    lidar.init();

    // command motor to spin
    lidar.setMotorSpeed(180);
}

void loop() {
    // update and get data from the lidar
    lidar.update();

    // check for a new packet
    if (lidar.hasNewScan()) {
        const auto& points = lidar.getLatestPoints();
        
        Serial.printf("Notor Speed: %.1f RPM | Points in packet: %u\n",
                        lidar.getLatestSpeedRPM(),
                        static_cast<unsigned int>(points.size()));

        for (const auto& pt : points) {
            Serial.printf("     Angle: %6.2f° | Distance: %4u mm | Confidence: %3u\n",
                            pt.angle_deg,
                            pt.distance_mm,
                            pt.intensity);
        }
    }
}