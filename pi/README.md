# Pi — Orchestrator & camera host

Captures photos of the object, commands the Pico via USB CDC, and sends data to the computer.

## System states

```
INITIALIZATION → READY → PROCESSING → SENDING
```

| State | Description |
|-------|-------------|
| INITIALIZATION | Verify Pico is ready, computer reachable, disk space available |
| READY | Wait for scan command from computer (HTTP 1.1) |
| PROCESSING | Send motor commands to Pico, wait for stability, capture photos |
| SENDING | Compress and upload images to computer |

## Communication

| Link | Transport | Details |
|------|-----------|---------|
| Pi → Computer | HTTP 1.1 | Image upload, scan metadata |
| Computer → Pi | HTTP 1.1 | Scan commands |
| Pi ↔ Pico | USB CDC | `/dev/ttyACM0` — newline-delimited JSON |

## Flashing the Pico from the Pi

SWD wiring (3 wires to Pico debug header):
```
Pi GPIO 24  →  Pico SWDCLK
Pi GPIO 25  →  Pico SWDIO
Pi GND      →  Pico SWD GND
```

```bash
openocd -f interface/raspberrypi-native.cfg \
        -f target/rp2040.cfg \
        -c "program motor.elf verify reset exit"
```

## Use cases

| Use case | Role |
|----------|------|
| `CheckConnection` | Verify Pico + computer reachability |
| `ListenForOrder` | Wait for scan command from computer |
| `CaptureData` | Command Pico motors, check stability, trigger camera |
| `SendData` | Compress images and upload via HTTP |

## Metrics

- Time of answer
- Processing time (motor positioning + capture)
- Image quality (blur, reflections)
- Sending time (including compression efficiency)
- Recovery rate
