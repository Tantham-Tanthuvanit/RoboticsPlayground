#pragma once
#include <Arduino.h>
#include <cmath>
#include <algorithm>

class MotorDriver {
    public:
        MotorDriver(int pwm_pin, int dir_pin, int pwm_channel);
        void init();

        // accepts speeds from -1.0f to +1.0f
        void setSpeed(float speed);
        void stop();

    private:
        int pwm_pin, dir_pin, pwm_channel;
};