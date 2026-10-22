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
}