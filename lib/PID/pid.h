#pragma once
#include <cmath>
#include <algorithm>

class PIDController {
    /**
     *  kp          porportional gain
     *  ki          integral gain
     *  kd          derivative gain
     *  out_min     minimum allowed output
     *  out_max     maximum allowed output
    */
    public:
    PIDController(float kp_, float ki_, float kd_, float out_min_, float out_max_);

    // standard PID calculation
    float compute(float setpoint, float measurement, float dt, float feedforward = 0.0f);

    // angular PID calculation
    float computeAngle(float target_rad, float current_rad, float dt);

    // reset angle
    void reset();

    // update gain on the fly
    void setGains(float kp_, float ki_, float kd_);
    void setOutputLimits(float out_min_, float out_max_);

    // telemetry getters for plotting in serial
    float getLastP() const { return last_p; }
    float getLastI() const { return last_i; }
    float getLastD() const { return last_d; }
    float getLastError() const { return last_error; }

    private:
        float kp;
        float ki;
        float kd;

        float out_min;
        float out_max;

        float integral = 0.0f;
        float prev_measurement = 0.0f;
        bool first_run = true;

        float last_p = 0.0f;
        float last_i = 0.0f;
        float last_d = 0.0f;
        float last_error = 0.0f;

};