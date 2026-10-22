#pragma once
#include <Arduino.h>
#include <config.h>

class Encoder {
    public:
        /**
         * pin_a            GPIO pin for encoder channel A
         * pin_b            GPIO pin for encoder channel B
         * ticks_per_rec    Total ticks per revolution
         * invert           Set true if spinning wheel forward decrease ticks
         */

        Encoder(uint8 pin_a, uint8 pin_b, int32_t ticks_per_rec, bool invert = false);

        void init();

        void update(float dt);

        int64_t getTicks() const;

        float getVelocityRadPerSec() const;

        // reset tick count and velocity to zero
        void reset();

        void IRAM_ATTR handleInterrupt();

    private:
        uint8 pin_a;
        uint8 pin_b;
        int32_t ticks_per_rev;
        bool invert;

        volatile int64_t ticks = 0;
        int64_t prev_ticks = 0;
        float velocity_rad_s = 0.0f;

};