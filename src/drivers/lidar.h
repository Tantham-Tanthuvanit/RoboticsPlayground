#pragma once
#include "config.h"
#include "types.h"
#include <Arduino.h>
#include <vector>

class LidarSensor {
    public:
        LidarSensor(int rx_pin, int pwm_pin, uint8_t uart_num = 1);
        void init();

        void setMotorSpeed(uint8_t duty_cycle);

        void update();

        bool hasNewScan() const;
        const std::vector<LidarPoint>& getLatestPoints() const;
        
    private:
        int rx_pin;
        int motor_pwm_pin;
        HardwareSerial lidar_serial;

        enum ParseState { WAITING_HEADER, READING_PACKET };
        ParseState parse_state = WAITING_HEADER;

        std::vector<LidarPoint> current_points;
        bool new_scan_ready = false;

        static constexpr size_t PACKET_SIZE = 47;
        uint8_t buffer[PACKET_SIZE];
        size_t bytes_read = 0;

        void parsePacket();

};