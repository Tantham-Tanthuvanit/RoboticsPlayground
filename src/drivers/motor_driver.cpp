#include "motor_driver.h"

MotorDriver::MotorDriver(int pwm_pin_, int dir_pin_, int pwm_channel_) {
    pwm_pin = pwm_pin_;
    dir_pin = dir_pin_;    
    pwm_channel = pwm_channel_;
}

void MotorDriver::init() {
    pinMode(dir_pin, OUTPUT);

    // pin, freq, resolution
    ledcAttach(pwm_pin, 20000, 8);

    stop();
}

void MotorDriver::stop() {
    digitalWrite(dir_pin, LOW);
    // set pwm_pin cycle to zero or stop applying power
    ledcWrite(pwm_pin, 0);
}

void MotorDriver::setSpeed(float speed) {
    // normalize speed
    speed = std::clamp(speed, -1.0f, 1.0f);
    
    if (speed > 0.0f) {
        digitalWrite(dir_pin, LOW);
    } else {
        digitalWrite(dir_pin, HIGH);
    }

    // write pwm (power);
    uint8_t pwm_val = static_cast<uint8_t>(std::abs(speed) * 255.0f);
    ledcWrite(pwm_pin, pwm_val);
}