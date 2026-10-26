#pragma once
#include "types.h"
#include "drivers/motor_driver.h"
#include "drivers/encoder.h"
#include <pid.h>

class DriveBase {
    public:
    DriveBase();
    void init();

    void setTargetVelocities(float left_target_rad_s, float right_target_rad_s);

    void update(float dt);

    float getLeftVelocity() const;
    float getRightVelocity() const;

    private:
    MotorDriver left_motor;
    MotorDriver right_motor;
    Encoder left_encoder;
    Encoder right_encoder;
    
    PIDController left_pid;
    PIDController right_pid;

    float target_left_rad_s = 0.0f;
    float target_right_rad_s = 0.0f;

};