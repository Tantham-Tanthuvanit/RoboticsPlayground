#pragma once
#include <Arduino.h>

class MotorDriver {
    public:
        MotorDriver(int pwm_pin, int dir_pin_a, int dir_pin_b, int pwm_channel);
        void init();

        // accepts speeds from -1.0f to +1.0f
        void setSpeed(float speed);
        void stop();

    private:
        int pwm_pin_, dir_pin_a_, dir_pin_b_, pwm_channel_;
};