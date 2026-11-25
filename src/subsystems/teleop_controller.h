#pragma once
#include <Arduino.h>
#include "config.h"

#if TELEOP_MODE == TELEOP_MODE_WIFI_UDP
#include <WiFi.h>
#include <WiFiUdp.h>
#elif TELEOP_MODE == TELEOP_MODE_BT_PS4
#include <PS4Controller.h>
#endif

struct __attribute__((packed)) TeleopControlPacket {
    float throttle; // -1.0f (reverse) to +1.0f (forward)
    float steering; // -1.0f (full left) to +1.0f (full right)
};

class TeleopController {
public:
    TeleopController();
    void init();

    // Call inside loop(): updates left and right motor speeds [-1.0f, +1.0f]
    void update(float &out_left_speed, float &out_right_speed);

    // Returns true if connection is active and receiving packets
    bool isConnected() const;

private:
    void computeArcadeDrive(float throttle, float steering, float &out_left, float &out_right);

#if TELEOP_MODE == TELEOP_MODE_WIFI_UDP
    WiFiUDP udp;
#endif

    unsigned long last_packet_time;
    bool connected;
    float current_throttle;
    float current_steering;
};
