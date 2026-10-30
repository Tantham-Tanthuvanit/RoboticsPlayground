#pragma once
#include <Arduino.h>

class Encoder {
    public:
        /**
         * @param pin_a          GPIO pin for encoder channel A
         * @param pin_b          GPIO pin for encoder channel B
         * @param ticks_per_rev  Total ticks per revolution
         * @param invert         Set true if spinning wheel forward decreases ticks
         */
        Encoder(uint8_t pin_a, uint8_t pin_b, int32_t ticks_per_rev, bool invert = false);

        void init();

        void update(float dt);

        int64_t getTicks() const;

        float getVelocityRadPerSec() const;

        // Reset tick count and velocity to zero
        void reset();

        void handleInterrupt();

    private:
        uint8_t pin_a;
        uint8_t pin_b;
        int32_t ticks_per_rev;
        bool invert;

        volatile int64_t ticks = 0;
        int64_t prev_ticks = 0;
        float velocity_rad_s = 0.0f;
};
