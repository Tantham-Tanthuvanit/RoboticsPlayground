#include "pid.h"

PIDController::PIDController(float kp_, float ki_, float kd_, float out_min_, float out_max_) {
    // init variables
    kp = kp_;
    ki = ki_;
    kd = kd_;
    out_min = out_min_;
    out_max = out_max_;

    last_p = 0.0f;
    last_i = 0.0f;
    last_d = 0.0f;
    last_error = 0.0f;
}

float PIDController::compute(float setpoint, float measurement, float dt, float feedforward = 0.0f) {
    if (dt <= 0.0f) return 0.0f;

    // calculations for porportional and integral term
    float error = setpoint - measurement;
    last_error = error;
    double porportional = error * kp;                            // porportional
    integral += error * dt * ki;                                 // summation of error over time
    integral = std::clamp(integral, out_min, out_max);          // clamp to value between min, max

    // avoid derivative kick on setpoint changes
    if (first_run) {
        last_d = 0.0f;
        first_run = false;
    } else {
        double derivative = -kd * (measurement - prev_measurement) / dt;
        last_d = derivative;
    }

    last_p = porportional;
    last_i = integral;
    prev_measurement = measurement;

    float output = porportional + integral + last_d + feedforward;
    return std::clamp(output, out_min, out_max);
}

float PIDController::computeAngle(float target_rad, float current_rad, float dt) {

}

void PIDController::reset() {
    integral = 0.0f;
    prev_measurement = 0.0f;
    last_p = 0.0f;
    last_i = 0.0f;
    last_d = 0.0f;
    last_error = 0.0f;
    first_run = true;
}

void PIDController::setGains(float kp_, float ki_, float kd_) {
    kp = kp_;
    ki = ki_;
    kd = kd_;
}

void PIDController::setOutputLimits(float out_min_, float out_max_) {
    out_min = out_min_;
    out_max = out_max_;
}