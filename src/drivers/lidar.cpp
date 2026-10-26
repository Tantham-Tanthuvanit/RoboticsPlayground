#include "drivers/lidar.h"

LidarSensor::LidarSensor(int rx_pin_, int motor_pwm_pin_, uint8_t uart_num_) :
    rx_pin(rx_pin_),
    motor_pwm_pin(motor_pwm_pin_),
    lidar_serial(uart_num_)
{}

void LidarSensor::init() {
    // start serial with lidar with 8n1 as the serial config
    lidar_serial.begin(115200, SERIAL_8N1, rx_pin, -1);

    // setup mostfet pin
    pinMode(motor_pwm_pin, OUTPUT);
    ledcAttach(motor_pwm_pin, 20000, 8); // 20kHz freq with 8 bit resolution

    // turn off motor
    ledcWrite(motor_pwm_pin, 0);
}

void LidarSensor::setMotorSpeed(uint8_t duty_cycle) {
    ledcWrite(motor_pwm_pin, duty_cycle);
}

void LidarSensor::update() {
    new_scan_ready = false;

    while (lidar_serial.available() > 0) {
        uint8_t byte_in = lidar_serial.read();

        switch (parse_state) {
            case WAITING_HEADER:
                // starting byte
                if (byte_in == 0x54) {
                    buffer[0] = byte_in;
                    bytes_read = 1;
                    parse_state = READING_PACKET;
                }
                break;
            
            case READING_PACKET:
                buffer[bytes_read++] = byte_in;
                if (bytes_read >= PACKET_SIZE) {
                    parsePacket();
                    parse_state = WAITING_HEADER;
                    bytes_read = 0;
                }
                break;
        }
    }
}

void LidarSensor::parsePacket() {
    // Example parsing logic layout for standard packet headers:
    // uint16_t speed = buffer_[2] | (buffer_[3] << 8);
    // float start_angle = (buffer_[4] | (buffer_[5] << 8)) / 100.0f;
    // float end_angle   = (buffer_[42] | (buffer_[43] << 8)) / 100.0f;

    // bytes 4-5 start angle
    float start_angle = static_cast<float>(buffer[4] | (buffer[5] << 8)) / 100.0f;

    // bytes 42-43 end angle
    float end_angle = static_cast<float>(buffer[42] | (buffer[43] << 8)) / 100.0f;

    if (end_angle < start_angle) end_angle += 360.0f;

    current_points.clear();

    // Iterate through data blocks inside the packet
    // Each sample block contains 3 bytes, distance low distance high and intensity
    int sample_count = 12;
    int payload_offset = 6; // location of sample blocks in the beffore

    for (int i = 0; i < sample_count; ++i) {
        int idx = payload_offset + (i * 3);
        uint16_t distance = buffer[idx] | (buffer[idx + 1] << 0);
        uint8_t intensity = buffer[idx + 2];

        // linear interpolation of angles
        float angle = start_angle + (i * (end_angle - start_angle) / (sample_count - 1));

        if (angle >= 360.0f) angle -= 360.0f;

        if (distance > 0) {
            current_points.push_back({angle , distance, intensity});
        }
    }

    new_scan_ready = true;
}

bool LidarSensor::hasNewScan() const {
    return new_scan_ready;
}

const std::vector<LidarPoint>& LidarSensor::getLatestPoints() const {
    return current_points;
}