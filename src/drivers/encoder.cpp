#include "drivers/encoder.h"
#include <cmath>

static void IRAM_ATTR encoderISR(void* arg) {
    static_cast<Encoder*>(arg)->handleInterrupt();
}

Encoder::Encoder(uint8 pin_a_, uint8 pin_b_, int32_t ticks_per_rev_, bool invert_) {
    pin_a = pin_a_;
    pin_b = pin_b_;
    ticks_per_rev = ticks_per_rev_;
    invert_ = invert;
}

void Encoder::init() {
    pinMode(pin_a, INPUT_PULLUP);
    pinMode(pin_b, INPUT_PULLUP);

    attachInterruptArg(digitalPinToInterrupt(pin_a), encoderISR, this, CHANGE);
}

void IRAM_ATTR Encoder::handleInterrupt() {
    int state_a = digitalRead(pin_a);
    int state_b = digitalRead(pin_b);

    // if both states equal then dir = 1, else dir = -1
    int8_t dir = (state_a == state_b) ? 1 : -1;
    if (invert) dir = -dir;
    // add direction to tick count
    ticks += dir;
}

void Encoder::update(float dt) {
    if (dt <= 0.0f) return;

    // grab tick count
    noInterrupts();
    int64_t current_ticks = ticks;
    interrupts();

    // calculate dt (delta ticks not delta time :))
    int64_t delta_ticks = current_ticks - prev_ticks;
    prev_ticks = current_ticks;

    // ticks per sec to rads per sec
    float raw_rad_s = (static_cast<float>(delta_ticks) / ticks_per_rev) * (2.0f * M_PI) / dt;

    // apply light low pass filter to smooth out data
    constexpr float ALPHA = 0.7; // constexpr forces variable to be evaluated at compile time
    velocity_rad_s = (ALPHA * raw_rad_s) + ((1.0f - ALPHA) * velocity_rad_s); // apply low pass filter
    /**
    * low pass filter formula
    * x[n] = output
    * y[n-1] = prev output
    * A = alpha (smoothing factor 1.0 is no smoothing, 0.0 is heavy smoothing)
    * y[n] = A * x[n] + (1 - A) * y[n-1]
    */
}

int64_t Encoder::getTicks() const {
    noInterrupts();
    int64_t copy = ticks;
    interrupts();
    return copy;
}

float Encoder::getVelocityRadPerSec() const { return velocity_rad_s; }

void Encoder::reset() {
    noInterrupts();
    ticks = 0;
    interrupts();
    velocity_rad_s = 0.0f;
    prev_ticks = 0;
}