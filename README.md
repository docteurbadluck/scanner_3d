# 3D Scanner

PROJECT IN DEVELOPPMENT !

Automate 360° photo capture of a light object on a turntable, then reconstruct a 3D model with Meshroom.

## Architecture

Three independent binaries, each on a separate device:

| Binary | Device | Role |
|--------|--------|------|
| `pico/motor/` | Raspberry Pi Pico | Controls 3 motors + reads 2 accelerometers |
| `pi/` | Raspberry Pi 3B | Captures photos, commands the Pico, sends data |
| `computer/` | PC | Receives images, serves web monitor, runs Meshroom |

## Hardware

| Component | Part | Role |
|-----------|------|------|
| Microcontroller | Raspberry Pi Pico | Motor control |
| Single-board computer | Raspberry Pi 3B | Camera host, orchestrator |
| Camera | OV5647 | Photo capture |
| Stepper motor | NEMA 23 + DM556 (20–50V) | Rotates object platform |
| DC motor | JGB37 24V 45RPM | Moves camera arm (up/down) |
| Servo | MG995 | Tilts camera |
| Accelerometers | MPU-6050 × 2 | Stability check |
| Power | 24V PSU + LM2596 DC-DC | Motors at 24V; Pi/Pico at 5V |

## Communication

```
Computer  :3333
    │  HTTP/1.1 (image upload)
    │  WebSocket (real-time monitoring)
    ▼
Raspberry Pi 3B
    │  USB CDC / UART  (/dev/ttyACM0)
    ▼
Raspberry Pi Pico
    ├── DM556  stepper         (STEP/DIR/ENA pulses)      GP10–GP12
    ├── JGB37  DC motor driver (PWM IN1/IN2)              GP6–GP7
    ├── JGB37  endstops        (top/bottom limit inputs)  GP8–GP9
    ├── MG995  servo           (50 Hz PWM)                GP2
    └── MPU-6050 × 2           (I2C0)                     GP4–GP5
```

## Flashing the Pico from the Pi (SWD — no button press)

```
Pi GPIO 24  →  Pico SWDCLK  (debug header)
Pi GPIO 25  →  Pico SWDIO   (debug header)
Pi GND      →  Pico SWD GND (debug header)
```

```bash
openocd -f interface/raspberrypi-native.cfg \
        -f target/rp2040.cfg \
        -c "program motor.elf verify reset exit"
```

## Build

```bash
# Pico
cmake -B pico/motor/build -S pico/motor -G Ninja
cmake --build pico/motor/build

# Computer webserver
make -C computer/webserver/webserv
```

## Productivity

- 4-layer clean architecture (entities / use cases / interfaces / framework)
- 80% unit test coverage minimum
- CI/CD on every push to `main`
- Accelerate metrics: lead time, deployment frequency, time to restore, change failure rate
