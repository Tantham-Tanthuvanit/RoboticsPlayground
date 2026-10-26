#pragma once

struct WheelSpeeds {
    float left;     // rads/s
    float right;    // rads/s
};

struct Pose2D {
    float x;        // m
    float y;        // m
    float theta;    // radians
};

struct LidarPoint {
    float angle_deg;
    uint16_t distance_mm;
    uint8_t intensity;
};