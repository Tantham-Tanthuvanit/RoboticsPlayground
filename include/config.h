#pragma once
#include <Arduino.h>

typedef uint8_t uint8;

// IMU pin conffig
constexpr int IMU_SDA = 8;
constexpr int IMU_SCL = 9;

// ----------------------------------------------------------------
// < DRIVE MOTOR CONFIG >
// Left motor pins
constexpr int PIN_MOTOR_L_PWM = 16;
constexpr int PIN_MOTOR_L_DIR = 15;
constexpr int MOTOR_L_PWM_CHANNEL = 0;

// Right motor pins
constexpr int PIN_MOTOR_R_PWM = 20;
constexpr int PIN_MOTOR_R_DIR = 19;
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
// unsigned long last_post_time = 0;
const unsigned long post_interval_ms = 3000;

// ----------------------------------------------------------------
// < TELEOP CONTROLLER SETTINGS >
// Modes:
// 0 = TELEOP_MODE_WIFI_UDP  (ESP32-S3 via Wi-Fi AP + UDP Bridge)
// 1 = TELEOP_MODE_BT_PS4    (Classic ESP32 via Bluetooth Classic)
#define TELEOP_MODE_WIFI_UDP 0
#define TELEOP_MODE_BT_PS4   1

#ifndef TELEOP_MODE
#define TELEOP_MODE TELEOP_MODE_WIFI_UDP
#endif

// Wi-Fi Access Point configuration (Mode 0)
constexpr char WIFI_AP_SSID[] = "Robot-S3";
constexpr char WIFI_AP_PASS[] = "12345678";
constexpr uint16_t TELEOP_UDP_PORT = 4210;

// Failsafe watchdog: stop motors if no packet received within timeout (ms)
constexpr unsigned long TELEOP_TIMEOUT_MS = 500;

// Bluetooth MAC address for classic PS4 pairing (Mode 1)
constexpr char PS4_BT_MAC[] = "1a:2b:3c:01:01:01";


