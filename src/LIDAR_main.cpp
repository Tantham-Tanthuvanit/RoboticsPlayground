/*
#include <config.h>
#include <Arduino.h>
#include "drivers/lidar.h"

// iinit lidar with the pins in config.h and on hardware serial 1
LidarSensor lidar(LIDAR_RX_PIN, LIDAR_MOTOR_PWM_PIN, 1);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("Initializing lidar node...");
    lidar.init();

    // command motor to spin (approx. 70% PWM duty cycle targets ~300 RPM for Xiaomi LDS02RR)
    lidar.setMotorSpeed(0);
}

void loop()
{
    // update and get data from the lidar
    lidar.update();

    // check for a new packet
    if (lidar.hasNewScan())
    {
        const auto &points = lidar.getLatestPoints();

        Serial.printf( "-\n{\n");
        Serial.printf("    \"motor_speed_rpm\": %.1f,\n", lidar.getLatestSpeedRPM());
        Serial.printf("    \"points_count\": %u,\n", static_cast<unsigned int>(points.size()));
        Serial.printf("    \"points\": [\n");

        for (size_t i = 0; i < points.size(); ++i)
        {
            const auto &pt = points[i];

            Serial.printf(
                "        {\n"
                "            \"angle_deg\": %.2f,\n"
                "            \"distance_mm\": %u,\n"
                "            \"confidence\": %u\n"
                "        }%s\n",
                pt.angle_deg,
                static_cast<unsigned int>(pt.distance_mm),
                static_cast<unsigned int>(pt.intensity),
                (i + 1 < points.size()) ? "," : "");
        }

        Serial.printf("    ]\n");
        Serial.printf("}\n+\n");
    }
}
*/