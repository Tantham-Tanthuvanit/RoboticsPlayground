#include "teleop_controller.h"
#include <algorithm>
#include <cmath>

TeleopController::TeleopController() 
    : last_packet_time(0), connected(false) {}

void TeleopController::init() {
#if TELEOP_MODE == TELEOP_MODE_WIFI_UDP
    Serial.println("[Teleop] Initializing Wi-Fi Access Point mode...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS);

    Serial.print("[Teleop] AP Created: ");
    Serial.println(WIFI_AP_SSID);
    Serial.print("[Teleop] AP IP Address: ");
    Serial.println(WiFi.softAPIP());

    udp.begin(TELEOP_UDP_PORT);
    Serial.print("[Teleop] Listening for UDP packets on port ");
    Serial.println(TELEOP_UDP_PORT);

#elif TELEOP_MODE == TELEOP_MODE_BT_PS4
    Serial.println("[Teleop] Initializing Bluetooth Classic PS4 controller...");
    PS4.begin(PS4_BT_MAC);
    Serial.print("[Teleop] PS4 Controller host initialized with MAC: ");
    Serial.println(PS4_BT_MAC);
#endif
}

void TeleopController::computeArcadeDrive(float throttle, float steering, float &out_left, float &out_right) {
    // Clamp inputs to [-1.0, 1.0]
    throttle = std::clamp(throttle, -1.0f, 1.0f);
    steering = std::clamp(steering, -1.0f, 1.0f);

    // Standard arcade mix: left = throttle + steering, right = throttle - steering
    float left = throttle + steering;
    float right = throttle - steering;

    // Normalize if magnitude exceeds 1.0f to maintain heading without clipping
    float max_mag = std::max(std::abs(left), std::abs(right));
    if (max_mag > 1.0f) {
        left /= max_mag;
        right /= max_mag;
    }

    out_left = left;
    out_right = right;
}

void TeleopController::update(float &out_left_speed, float &out_right_speed) {
    float throttle = 0.0f;
    float steering = 0.0f;

#if TELEOP_MODE == TELEOP_MODE_WIFI_UDP
    int packet_size = udp.parsePacket();
    if (packet_size >= (int)sizeof(TeleopControlPacket)) {
        TeleopControlPacket packet;
        udp.read((char*)&packet, sizeof(TeleopControlPacket));
        throttle = packet.throttle;
        steering = packet.steering;
        last_packet_time = millis();
        connected = true;
    }

    // Safety watchdog: stop robot if packets cease
    if (millis() - last_packet_time > TELEOP_TIMEOUT_MS) {
        connected = false;
        throttle = 0.0f;
        steering = 0.0f;
    }

#elif TELEOP_MODE == TELEOP_MODE_BT_PS4
    if (PS4.isConnected()) {
        connected = true;
        // PS4 sticks: Y axis (-128 to 127, inverted), X axis (-128 to 127)
        throttle = (float)PS4.LStickY() / 127.0f;
        steering = (float)PS4.RStickX() / 127.0f;

        // Apply deadzone for raw stick inputs
        if (std::abs(throttle) < 0.08f) throttle = 0.0f;
        if (std::abs(steering) < 0.08f) steering = 0.0f;
    } else {
        connected = false;
        throttle = 0.0f;
        steering = 0.0f;
    }
#endif

    if (connected) {
        computeArcadeDrive(throttle, steering, out_left_speed, out_right_speed);
    } else {
        out_left_speed = 0.0f;
        out_right_speed = 0.0f;
    }
}

bool TeleopController::isConnected() const {
    return connected;
}
