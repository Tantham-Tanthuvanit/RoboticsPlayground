#pragma once
#include <Arduino.h>

typedef uint8_t uint8;

// IMU pin conffig
constexpr int IMU_SDA = 8;
constexpr int IMU_SCL = 9;

// ----------------------------------------------------------------
// < DRIVE MOTOR CONFIG >
// Left motor pins
constexpr int PIN_MOTOR_L_PWM;
constexpr int PIN_MOTOR_L_DIR;
constexpr int MOTOR_L_PWM_CHANNEL;

// Right motor pins
constexpr int PIN_MOTOR_R_PWM;
constexpr int PIN_MOTOR_R_DIR;
constexpr int MOTOR_R_PWM_CHANNEL;

// ----------------------------------------------------------------
// < ENCODER SETTINGS >

constexpr int TICKS_PER_REV;

// Left encoder pins
constexpr int PIN_ENCODER_L_A;
constexpr int PIN_ENCODER_L_B;

// Right encoder pins
constexpr int PIN_ENCODER_R_A;
constexpr int PIN_ENCODER_R_B;

// ----------------------------------------------------------------
// < PID SETTINGS >
constexpr float KP;
constexpr float KI;
constexpr float KD;

// ----------------------------------------------------------------
// < LIDAR SETTINGS >
constexpr int LIDAR_RX_PIN = 44;
constexpr int LIDAR_MOTOR_PWM_PIN = 4;
