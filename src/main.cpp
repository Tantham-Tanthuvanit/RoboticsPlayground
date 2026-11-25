#include <config.h>
#include <Arduino.h>
#include "drivers/motor_driver.h"
#include "subsystems/teleop_controller.h"

MotorDriver motorR(PIN_MOTOR_R_PWM, PIN_MOTOR_R_DIR, MOTOR_R_PWM_CHANNEL);
MotorDriver motorL(PIN_MOTOR_L_PWM, PIN_MOTOR_L_DIR, MOTOR_L_PWM_CHANNEL);
TeleopController teleop;

void setup() {
    Serial.begin(115200);
    Serial.println("--- RoboticsPlayground Robot Initializing ---");

    motorR.init();
    motorL.init();
    teleop.init();

    Serial.println("Robot ready for teleoperation.");
}

void loop() {
    float left_speed = 0.0f;
    float right_speed = 0.0f;

    // Read controller state, handle failsafes, and calculate arcade drive speeds
    teleop.update(left_speed, right_speed);

    // Apply motor speeds
    motorL.setSpeed(left_speed);
    motorR.setSpeed(right_speed);

    // Telemetry output every 500ms
    static unsigned long last_telemetry = 0;
    if (millis() - last_telemetry >= 500) {
        last_telemetry = millis();
        if (teleop.isConnected()) {
            Serial.printf("[Teleop] Active - Left: %.2f | Right: %.2f\n", left_speed, right_speed);
        } else {
            Serial.println("[Teleop] Waiting for controller connection / packets...");
        }
    }

    delay(10); // Loop cadence (100 Hz)
}
