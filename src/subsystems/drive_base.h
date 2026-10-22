#pragma once
#include "types.h"
#include "drivers/motor_driver.h"
#include "drivers/encoder.h"
#include <pid.h>

class DriveBase {
    public:
    DriveBase();
    void init();

    void setTargetVelocity(float vx, float omega);

    void update(float dt);

    Pose2D getPose() const;

    private:
    MotorDriver left_motor;
    MotorDriver right_motor;
    Encoder left_encoder;
    Encoder right_encoder;
    
};