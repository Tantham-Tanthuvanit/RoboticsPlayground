#include "drivers/lidar.h"

LidarSensor::LidarSensor(int rx_pin_, int motor_pwm_pin_, uint8_t uart_num_) :
    rx_pin(rx_pin_),
    motor_pwm_pin(motor_pwm_pin_),
    lidar_serial(uart_num_)
{
    current_points.reserve(360);
    scan_accumulator.reserve(360);
}

void LidarSensor::init() {
    // Increase RX buffer size to hold full revolution without dropping bytes during processing
    lidar_serial.setRxBufferSize(2048);

    // start serial with lidar at 115200 8N1
    lidar_serial.begin(115200, SERIAL_8N1, rx_pin, -1);

    // setup mosfet / motor pin
    pinMode(motor_pwm_pin, OUTPUT);
    ledcAttach(motor_pwm_pin, 20000, 8); // 20kHz freq with 8-bit resolution

    // turn off motor initially
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
                // Xiaomi LDS02RR start byte is 0xFA
                if (byte_in == 0xFA) {
                    buffer[0] = byte_in;
                    bytes_read = 1;
                    parse_state = WAITING_INDEX;
                }
                break;

            case WAITING_INDEX:
                // Index must be in the range 0xA0 to 0xF9 (packet 0 to 89)
                if (byte_in >= 0xA0 && byte_in <= 0xF9) {
                    buffer[1] = byte_in;
                    bytes_read = 2;
                    parse_state = READING_PACKET;
                } else if (byte_in == 0xFA) {
                    // Previous byte was a false alarm; this byte is a start candidate
                    buffer[0] = 0xFA;
                    bytes_read = 1;
                } else {
                    parse_state = WAITING_HEADER;
                    bytes_read = 0;
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

uint16_t LidarSensor::calculateChecksum(const uint8_t* packet) const {
    uint32_t chk32 = 0;

    // Group the first 20 bytes into 10 little-endian 16-bit words
    for (int i = 0; i < 10; ++i) {
        uint16_t word = static_cast<uint16_t>(packet[2 * i]) |
                        (static_cast<uint16_t>(packet[2 * i + 1]) << 8);
        chk32 = (chk32 << 1) + word;
    }

    // Wrap around to fit into 15 bits
    uint32_t checksum = (chk32 & 0x7FFF) + (chk32 >> 15);
    return static_cast<uint16_t>(checksum & 0x7FFF);
}

void LidarSensor::parsePacket() {
    // Checksum is located in bytes 20 and 21 (little-endian)
    uint16_t received_checksum = static_cast<uint16_t>(buffer[20]) |
                                 (static_cast<uint16_t>(buffer[21]) << 8);

    if (received_checksum != calculateChecksum(buffer)) {
        return; // Corrupt packet, drop
    }

    uint8_t packet_index = buffer[1];
    if (packet_index < 0xA0 || packet_index > 0xF9) {
        return;
    }

    uint32_t now = millis();
    if (last_packet_time_ms != 0 && (now - last_packet_time_ms > 500)) {
        // Stream interrupted (motor stopped or disconnect): reset sync and clear old data
        scan_accumulator.clear();
        is_synced = false;
        last_packet_index = -1;
    }
    last_packet_time_ms = now;

    // Speed in RPM: raw_speed represents 64ths of an RPM (target ~300 RPM)
    uint16_t raw_speed = static_cast<uint16_t>(buffer[2]) |
                         (static_cast<uint16_t>(buffer[3]) << 8);
    currentRPM = static_cast<float>(raw_speed) / 64.0f;

    // Revolution wrap-around detection (packet index wraps around from ~0xF9 to 0xA0)
    if (last_packet_index != -1 && packet_index < last_packet_index) {
        if (is_synced) {
            // Full 360-degree revolution completed!
            current_points = std::move(scan_accumulator);
            scan_accumulator.clear();
            scan_accumulator.reserve(360);
            new_scan_ready = true;
        } else {
            // Synchronized after the first wrap-around (discarding any initial partial revolution)
            scan_accumulator.clear();
            scan_accumulator.reserve(360);
            is_synced = true;
        }
    } else if (last_packet_index == -1 && packet_index == 0xA0) {
        // First packet happened to start exactly at packet 0 (0 degrees)
        is_synced = true;
    }

    last_packet_index = packet_index;

    // Base angle for this packet (0xA0 is packet 0 -> 0°, 0xF9 is packet 89 -> 356°)
    float base_angle = static_cast<float>(packet_index - 0xA0) * 4.0f;

    // 4 sample blocks per packet (4 bytes each)
    for (int i = 0; i < 4; ++i) {
        int idx = 4 + (i * 4);
        uint8_t byte0 = buffer[idx];
        uint8_t byte1 = buffer[idx + 1];
        uint8_t byte2 = buffer[idx + 2];
        uint8_t byte3 = buffer[idx + 3];

        // Byte 1 bit 7: warning flag, bit 6: invalid/unreliable flag
        bool invalid_flag = (byte1 & 0x80) != 0 || (byte1 & 0x40) != 0;

        // 14-bit distance measurement (mm)
        uint16_t distance = static_cast<uint16_t>(byte0) |
                            (static_cast<uint16_t>(byte1 & 0x3F) << 8);

        // 16-bit signal intensity
        uint16_t signal = static_cast<uint16_t>(byte2) |
                          (static_cast<uint16_t>(byte3) << 8);
        uint8_t intensity = static_cast<uint8_t>(signal > 255 ? 255 : signal);

        float angle = base_angle + static_cast<float>(i);
        if (angle >= 360.0f) {
            angle -= 360.0f;
        }

        if (!invalid_flag && distance > 0) {
            scan_accumulator.push_back({angle, distance, intensity});
        }
    }
}

bool LidarSensor::hasNewScan() const {
    return new_scan_ready;
}

const std::vector<LidarPoint>& LidarSensor::getLatestPoints() const {
    return current_points;
}

float LidarSensor::getLatestSpeedRPM() const {
    return currentRPM;
}
