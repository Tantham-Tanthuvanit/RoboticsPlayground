#include <config.h>
#include <Arduino.h>
#include "drivers/lidar.h"
#include "drivers/motor_driver.h"

MotorDriver motor(PIN_MOTOR_R_PWM, PIN_MOTOR_R_DIR, MOTOR_R_PWM_CHANNEL);

void setup()
{
    Serial.begin(115200);
    motor.init();
    motor.setSpeed(0.5);
}

void loop()
{
}
