#include "subsystems/drive_base.h"
#include "config.h"
#include <algorithm>
#include <cmath>

DriveBase::DriveBase()
    :   left_motor(PIN_MOTOR_L_PWM, PIN_MOTOR_L_DIR, MOTOR_L_PWM_CHANNEL),
        right_motor(PIN_MOTOR_R_PWM, PIN_MOTOR_R_DIR, MOTOR_R_PWM_CHANNEL),
        left_encoder(PIN_ENCODER_L_A, PIN_ENCODER_R_A, TICKS_PER_REV, false),
        right_encoder(PIN_ENCODER_R_A, PIN_ENCODER_R_B, TICKS_PER_REV, false),
        left_pid(KP, KI, KD, -1.0f, 1.0f),
        right_pid(KP, KI, KD, -1.0f, 1.0f)
{}

void DriveBase::init() {
    left_motor.init();
    right_motor.init();
    left_encoder.init();
    right_encoder.init();
    
    left_pid.reset();
    right_pid.reset();
}

void DriveBase::setTargetVelocities(float left_target_rad_s, float right_target_rad_s) {
    target_left_rad_s = left_target_rad_s;
    target_right_rad_s = right_target_rad_s;
}

void DriveBase::update(float dt) {

}

float DriveBase::getLeftVelocity() const {
    return target_left_rad_s;
}

float DriveBase::getRightVelocity() const {
    return target_right_rad_s;
}
