# Pico — Motor controller

Controls 3 motors and 2 accelerometers. Receives commands from the Raspberry Pi via USB CDC and responds with status JSON.

## System states

```
LISTENING → INTERPRETING → EXECUTING
```

## Motors

| Motor | Driver | States | Constraint |
|-------|--------|--------|------------|
| Stepper NEMA 23 + DM556 | `StepperMotor_Driver` | position 0…N | Forward only, 10° per step |
| DC motor JGB37 | `MotorDC_Driver` | Top / Moving / Bottom | Adjacent transitions only |
| Servo MG995 | `ServoMotor_Driver` | UP_A / UP_B / DOWN_A / DOWN_B | Adjacent transitions only |
| Accelerometer MPU-6050 × 2 | `Accelerometer_Driver` | — | Magnitude threshold 0.05 g |

## Pin map

```
Physical  GPIO   Function
──────────────────────────────────────────────
  Pin  4   GP2   Servo MG995       PWM (slice 1A, 50 Hz)
  Pin  6   GP4   I2C0 SDA          MPU-6050 #1 (0x68) + #2 (0x69)
  Pin  7   GP5   I2C0 SCL
  Pin  9   GP6   DC Motor JGB37    IN1 PWM (slice 3A)
  Pin 10   GP7                     IN2 PWM (slice 3B)
  Pin 11   GP8                     BTN_UP  (active-low)
  Pin 12   GP9                     BTN_DOWN (active-low)
  Pin 14   GP10  Stepper DM556     STEP
  Pin 15   GP11                    DIR
  Pin 16   GP12                    ENA (active-low)
```

All signal groups are consecutive — use a shared GND bus.

## Communication with Pi

- **Transport**: USB CDC (`stdio_usb`) — Pi sees `/dev/ttyACM0`
- **Protocol**: newline-delimited JSON strings
- **Flashing**: SWD via openocd (no BOOTSEL button needed)
  ```
  Pi GPIO 24 → Pico SWDCLK
  Pi GPIO 25 → Pico SWDIO
  ```

## Architecture (4 layers)

```
0_orchestration/   config.hpp — all pins and configs in one place
1_domain/          System, JsonMessage
2_usecases/        PlateController, ArmController, HandController,
                   VibrationMonitor, CommandReceptor, SendToPi, ExecuteCommand
3_interface/       IStepperMotor, IMotorDC, IServoMotor, IAccelerometer,
                   IReceptor, ISender
4_drivers/         StepperMotor_Driver, MotorDC_Driver, ServoMotor_Driver,
                   Accelerometer_Driver, UsbReceptor_Driver, UsbSender_Driver
```

## Boot sequence

1. DC motor moves to top position (BTN_UP endstop) — exits on failure.
2. Servo moves to UP_A (0°).
3. Stepper sets current position as 0.
4. USB CDC ready — enter LISTENING loop.

## Build

```bash
cmake -B motor/build -S motor -G Ninja
cmake --build motor/build
```

## Metrics

- Time of answer (command received → response sent)
- Stability score (accelerometer magnitude during capture)
- Time per motor action
