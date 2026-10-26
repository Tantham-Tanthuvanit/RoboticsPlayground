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

}