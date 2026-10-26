#include <config.h>
#include <Arduino.h>
#include "drivers/lidar.h"

// iinit lidar with the pins in config.h and on hardware serial 1
LidarSensor lidar(LIDAR_RX_PIN, LIDAR_MOTOR_PWM_PIN, 1);

unsigned long last_post_time = 0;

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
    if (lidar.hasNewScan())
    {
        unsigned long current_time = millis();

        // send telemetry every 3 sec
        if (current_time - last_post_time >= post_interval_ms) {
            last_post_time = current_time;

            const auto &points = lidar.getLatestPoints();

            Serial.print("{\"rpm\": ");
            Serial.print(lidar.getLatestSpeedRPM(), 1);
            Serial.print(", \"points\": [");

            for (size_t i = 0; i < points.size(); ++i) {
                Serial.print("{\"a\":");
                Serial.print(points[i].angle_deg, 1);
                Serial.print(",\"d\":");
                Serial.print(points[i].distance_mm);
                Serial.print(",\"c\":");
                Serial.print(points[i].intensity);
                Serial.print("}");
                if (i < points.size() - 1)
                    Serial.print(",");
            }
            Serial.println("]}");
        }
    }

}