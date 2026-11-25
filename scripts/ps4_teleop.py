#!/usr/bin/env python3
"""
PS4 Controller to ESP32-S3 Teleoperation Bridge
Connects to a PS4 DualShock 4 controller (or keyboard fallback) and streams
arcade drive control packets (throttle, steering) over UDP to the ESP32-S3 robot.
"""

import argparse
import socket
import struct
import sys
import time

try:
    import pygame
except ImportError:
    print("Error: 'pygame' is not installed. Run: pip install pygame")
    sys.exit(1)


def parse_args():
    parser = argparse.ArgumentParser(description="PS4 Controller Teleoperation Bridge for ESP32-S3")
    parser.add_argument("--ip", default="192.168.4.1", help="ESP32 AP or Station IP (default: 192.168.4.1)")
    parser.add_argument("--port", type=int, default=4210, help="UDP destination port (default: 4210)")
    parser.add_argument("--rate", type=float, default=50.0, help="Packet transmission rate in Hz (default: 50)")
    parser.add_argument("--deadzone", type=float, default=0.08, help="Stick deadzone threshold (default: 0.08)")
    parser.add_argument("--max-speed", type=float, default=1.0, help="Max speed multiplier 0.1 - 1.0 (default: 1.0)")
    parser.add_argument("--keyboard", action="store_true", help="Force keyboard mode even if gamepad is attached")
    return parser.parse_args()


def apply_deadzone(value, deadzone):
    if abs(value) < deadzone:
        return 0.0
    # Rescale from deadzone to 1.0
    sign = 1.0 if value > 0 else -1.0
    return sign * (abs(value) - deadzone) / (1.0 - deadzone)


def main():
    args = parse_args()
    pygame.init()
    pygame.joystick.init()

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    dest_addr = (args.ip, args.port)

    # Check for joysticks
    joystick = None
    use_keyboard = args.keyboard

    if not use_keyboard:
        joystick_count = pygame.joystick.get_count()
        if joystick_count > 0:
            joystick = pygame.joystick.Joystick(0)
            joystick.init()
            print(f"[*] Detected Gamepad: {joystick.get_name()}")
            print(f"[*] Number of axes: {joystick.get_numaxes()}, buttons: {joystick.get_numbuttons()}")
        else:
            print("[!] No physical gamepad found.")
            print("[*] Switching to KEYBOARD mode (W/A/S/D or Arrow keys).")
            use_keyboard = True

    if use_keyboard:
        # Create a small pygame window to capture keyboard events
        pygame.display.set_caption("Robot Teleop (Keyboard Mode)")
        screen = pygame.display.set_mode((400, 200))
        print("--------------------------------------------------")
        print(" Keyboard Controls (Keep window focused):")
        print("   W / Up Arrow    : Forward")
        print("   S / Down Arrow  : Reverse")
        print("   A / Left Arrow  : Turn Left")
        print("   D / Right Arrow : Turn Right")
        print("   SPACE           : Emergency Stop")
        print("   Q / ESC         : Quit")
        print("--------------------------------------------------")
    else:
        print("--------------------------------------------------")
        print(" PS4 Controller Controls:")
        print("   Left Stick Y    : Forward / Reverse (Throttle)")
        print("   Right Stick X   : Turn Left / Right (Steering)")
        print("   L1 / R1         : Speed Limit Toggle")
        print("   Circle / Cross  : Emergency Stop")
        print("   Ctrl + C        : Quit")
        print("--------------------------------------------------")

    print(f"[*] Streaming UDP packets to {dest_addr[0]}:{dest_addr[1]} at {args.rate:.0f} Hz...")

    send_interval = 1.0 / args.rate
    speed_scale = max(0.1, min(1.0, args.max_speed))

    try:
        while True:
            t_start = time.time()
            pygame.event.pump()

            throttle = 0.0
            steering = 0.0

            if use_keyboard:
                keys = pygame.key.get_pressed()
                if keys[pygame.K_ESCAPE] or keys[pygame.K_q]:
                    break

                if keys[pygame.K_w] or keys[pygame.K_UP]:
                    throttle += 1.0
                if keys[pygame.K_s] or keys[pygame.K_DOWN]:
                    throttle -= 1.0
                if keys[pygame.K_a] or keys[pygame.K_LEFT]:
                    steering -= 1.0
                if keys[pygame.K_d] or keys[pygame.K_RIGHT]:
                    steering += 1.0
                if keys[pygame.K_SPACE]:
                    throttle = 0.0
                    steering = 0.0

                # Simple render
                screen.fill((30, 30, 30))
                pygame.display.flip()

            else:
                # PS4 Gamepad Axis Mapping:
                # Axis 1: Left Stick Y (-1.0 top, +1.0 bottom) -> Inverted for forward
                # Axis 2 (or 3 depending on OS): Right Stick X (-1.0 left, +1.0 right)
                raw_throttle = -joystick.get_axis(1)
                
                # Check axis count for right stick X (axis 2 or 3)
                if joystick.get_numaxes() > 2:
                    raw_steering = joystick.get_axis(2 if joystick.get_numaxes() <= 4 else 3)
                else:
                    raw_steering = joystick.get_axis(0)

                throttle = apply_deadzone(raw_throttle, args.deadzone)
                steering = apply_deadzone(raw_steering, args.deadzone)

                # Emergency stop buttons on PS4 (Cross = 0, Circle = 1 depending on driver)
                if joystick.get_numbuttons() > 1 and (joystick.get_button(0) or joystick.get_button(1)):
                    throttle = 0.0
                    steering = 0.0

            # Scale speed
            throttle *= speed_scale
            steering *= speed_scale

            # Send UDP packet: 2 floats (throttle, steering)
            packet = struct.pack("ff", float(throttle), float(steering))
            sock.sendto(packet, dest_addr)

            # Live status line
            sys.stdout.write(f"\rThrottle: {throttle:+5.2f} | Steering: {steering:+5.2f} | Target: {dest_addr[0]}:{dest_addr[1]}   ")
            sys.stdout.flush()

            # Maintain rate
            elapsed = time.time() - t_start
            sleep_time = send_interval - elapsed
            if sleep_time > 0:
                time.sleep(sleep_time)

    except KeyboardInterrupt:
        print("\n[*] Exiting teleop bridge...")
    finally:
        # Send zero failsafe packets on shutdown
        stop_packet = struct.pack("ff", 0.0, 0.0)
        for _ in range(5):
            sock.sendto(stop_packet, dest_addr)
            time.sleep(0.01)
        sock.close()
        pygame.quit()
        print("[*] Robot stopped. Goodbye!")


if __name__ == "__main__":
    main()
