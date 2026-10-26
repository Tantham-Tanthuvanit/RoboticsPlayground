#pragma once
#include <Arduino.h>

typedef uint8_t uint8;

// IMU pin conffig
constexpr int IMU_SDA = 8;
constexpr int IMU_SCL = 9;

// ----------------------------------------------------------------
// < DRIVE MOTOR CONFIG >
// Left motor pins
constexpr int PIN_MOTOR_L_PWM = 0;
constexpr int PIN_MOTOR_L_DIR = 0;
constexpr int MOTOR_L_PWM_CHANNEL = 0;

// Right motor pins
constexpr int PIN_MOTOR_R_PWM = 0;
constexpr int PIN_MOTOR_R_DIR = 0;
constexpr int MOTOR_R_PWM_CHANNEL = 0;

// ----------------------------------------------------------------
// < ENCODER SETTINGS >

constexpr int TICKS_PER_REV = 0;

// Left encoder pins
constexpr int PIN_ENCODER_L_A = 0;
constexpr int PIN_ENCODER_L_B = 0;

// Right encoder pins
constexpr int PIN_ENCODER_R_A = 0;
constexpr int PIN_ENCODER_R_B = 0;

// ----------------------------------------------------------------
// < PID SETTINGS >
constexpr float KP = 0.1f;
constexpr float KI = 0.1f;
constexpr float KD = 0.1f;

// ----------------------------------------------------------------
// < LIDAR SETTINGS >
constexpr int LIDAR_RX_PIN = 18;
constexpr int LIDAR_MOTOR_PWM_PIN = 4;
unsigned long last_post_time = 0;
const unsigned long post_interval_ms = 3000;

