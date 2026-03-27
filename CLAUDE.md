# CLAUDE.md — 3D Scanner Project

## Project Overview

Automate 360° photo capture of a light object on a turntable, then reconstruct a 3D model with Meshroom.

Three independent C++ binaries, each on a separate device:

| Binary | Device | Role |
|--------|--------|------|
| `pico/` | Raspberry Pi Pico | SPI slave — controls 3 motors + reads 2 accelerometers |
| `pi/` | Raspberry Pi 3B | SPI master → Pico; captures photos; sends data to computer |
| `computer/` | PC | Receives images; serves web monitor; runs Meshroom 3D reconstruction |

Language: **C++ everywhere**. Pico uses `pico-sdk`. Pi uses Linux C++. Computer uses the existing 42-school webserver (Epoll, HTTP/1.1, WebSocket — already built in `computer/webserver/webserv/`).

---

## Hardware

| Component | Part | Role |
|-----------|------|------|
| Microcontroller | Raspberry Pi Pico | Motor control SPI slave |
| Single-board computer | Raspberry Pi 3B | SPI master, camera host |
| Camera | OV5647 | Photo capture (attached to Pi) |
| Stepper motor | NEMA 23 + DM556 (20–50V) | Rotates object platform |
| DC motor | JGB37 24V 45RPM | Moves camera arm (up/down) |
| Servo | MG995 | Tilts camera |
| Accelerometers | MPU-6050 × 2 | Stability check (one on platform, one near camera) |
| Power | 24V PSU + LM2596 DC-DC | Powers motors; Pi/Pico at 5V |

---

## Communication Topology

```
Computer  :3333
    │  HTTP/1.1 (image upload, scan control)
    │  WebSocket (real-time monitoring)
    ▼
Raspberry Pi 3B
    │  SPI  (Pi = master, Pico = slave)
    ▼
Raspberry Pi Pico
    ├── DM556  stepper driver  (step/dir pulses)
    ├── JGB37  DC motor        (PWM + direction)
    ├── MG995  servo           (PWM)
    └── MPU-6050 × 2          (I2C)
```

Shared contracts (documented in `docs/contracts/`):
- **SPI protocol**: command byte layout, response format, error codes
- **HTTP API**: upload endpoint, scan metadata JSON schema

---

## Component State Machines

**Pico**: `LISTENING` → `INTERPRETING` → `EXECUTING`

**Pi**: `INITIALIZATION` → `READY` → `PROCESSING` → `SENDING`

**Computer**: `INITIALIZATION` → `READING` → `WRITING` → `PROCESSING`

### Pico motor logical states

| Motor | States | Constraint |
|-------|--------|------------|
| Stepper (NEMA 23) | position 0…N | Forward only, 10° per step; position 0 at boot |
| DC motor (JGB37) | Top / Moving / Bottom | Only adjacent transitions |
| Servo (MG995) | HIGH_A / HIGH_B / LOW_A / LOW_B / Moving | Only adjacent transitions |

---

## 4-Layer Clean Architecture

Applies to **all three components**. Folder numbering is consistent everywhere:

```
1_entities/     domain rules — no external dependencies, no SDK calls
2_usecases/     orchestration — depends only on entities + interfaces
3_interfaces/   pure abstract classes (I-prefixed) — no implementation
4_framework/    concrete: SDK drivers, sockets, OS calls, hardware
```

**Dependency rule**: arrows point inward only.
`4_framework` → `3_interfaces` → `2_usecases` → `1_entities`

Hardware SDK calls (`gpio_put`, `spi_write`, `sleep_ms`, …) must **never** appear in layers 1–3.
Inject concrete implementations via constructor (constructor injection).

Use-case classes are named `PascalCase_UC`. Entry-point functions are short orchestrators:

```cpp
void execute()
{
    readSensors();
    evaluateStability();
    driveMotors();
}
```

---

## C++ Coding Conventions

Full reference: `pico/docs/Convention_CPP.md`

| Element | Convention | Example |
|---------|-----------|---------|
| Files | `snake_case` | `stepper_driver.hpp` |
| Classes / Structs | `PascalCase` | `StepperDriver` |
| Use cases | `PascalCase_UC` | `RotateObject_UC` |
| Interfaces | `I` + `PascalCase` | `IStepperDriver` |
| Public methods | `camelCase` | `stepForward()` |
| Private methods | `_camelCase` | `_sendPulse()` |
| Member variables | `_camelCase` | `_currentStep` |
| Constants / constexpr | `UPPER_SNAKE_CASE` | `STEP_ANGLE_DEG` |
| Free functions | `snake_case` | `compute_crc()` |
| Namespaces | `snake_case` | `use_cases`, `domain` |

**Rules:**
- Max **15 lines** per function. Max **100 chars** per line.
- Tabs (4 spaces). Always use `{}`.
- No comments — names must be self-explanatory.
- **Scripts (Python, shell)**: same 15-line limit per function. Every script must expose a
  minimal, highly abstract `main()` that reads like a table of contents — one call per
  logical step, no implementation detail visible at that level. Move all logic into
  well-named helper functions.
- Booleans read as questions: `isStable()`, `hasPhoto()`, `isAtTop()`.
- Use `[[nodiscard]]` on functions whose return value must be checked.
- Prefer composition over inheritance.
- Constructors leave the object in a valid, ready-to-use state.

---

## Build & Test Commands

```bash
# ── Computer webserver ──────────────────────────────────────────────────────
make -C computer/webserver/webserv                          # build → computer/webserver/webserv/webserv
make -C computer/webserver/testeur_webserver                # build integration tests

# Run integration tests (requires live server on :8080)
./computer/webserver/webserv/webserv computer/webserver/webserv/config.conf &
sleep 2
./computer/webserver/testeur_webserver/testeur_webserver
kill %1

# ── Pico (once pico/CMakeLists.txt exists) ──────────────────────────────────
cmake -B pico/build -S pico -G Ninja
cmake --build pico/build

# ── Pi (once pi/CMakeLists.txt exists) ──────────────────────────────────────
cmake -B pi/build -S pi -G Ninja
cmake --build pi/build
```

CI runs automatically on every push/PR to `main` via `.github/workflows/ci.yml`.
Pico and Pi build steps are **skipped** (not failed) when `CMakeLists.txt` does not exist yet.

**Important — config.conf**: `computer/webserver/webserv/config.conf` uses absolute paths
(`/home/docteurbadluck/Desktop/3D_scanner/…`). The CI generates a `ci.conf` via `sed` to replace
that prefix with `$GITHUB_WORKSPACE`. Do not change the original `config.conf` paths.

---

## Testing Requirements

- **80% unit test coverage** minimum for all components.
- **Unit tests**: pure C++, no hardware. Mock all interfaces via constructor injection.
- **Integration tests**: require live hardware or live server.
- **Computer**: Unity framework in `computer/webserver/testeur_webserver/`.
- **Pico**: unit tests with mock `IStepperDriver`, `IDcMotorDriver`, `IServo`, `IAccelerometer`.
- **Pi**: mock `IPicoClient`, `ICamera`, `IHttpUploader`.

---

## Metrics per Component

| Component | Metrics tracked |
|-----------|----------------|
| Pico | Time of answer, stability score, time per motor action |
| Pi | Answer time, processing time, image quality (blur/reflection), send time, recovery rate |
| Computer | Reconstruction success rate, model quality, scan completion time, processing time |
| CI/CD (Accelerate) | Lead time, deployment frequency, time to restore, change failure rate |

Accelerate metrics are stored as GitHub Actions artifacts (`accelerate-metrics-{run_number}`).
Aggregate them locally: `python3 metrics/aggregate.py` (needs `GITHUB_TOKEN` + `GITHUB_REPOSITORY`).

---

## Repository Layout

```
3D_scanner/
├── CLAUDE.md                        ← you are here
├── .github/workflows/ci.yml         ← CI/CD pipeline
├── metrics/
│   ├── aggregate.py                 ← cross-run Accelerate metrics
│   └── run_metrics.json             ← per-run artifact (generated by CI)
├── docs/
│   └── contracts/                   ← SPI protocol spec, HTTP API spec (create before parallel work)
├── pico/
│   ├── docs/Convention_CPP.md       ← C++ conventions (canonical reference)
│   └── src/                         ← pico-sdk C++ firmware (to be created)
├── pi/
│   └── src/                         ← Linux C++ software (to be created)
└── computer/
    └── webserver/
        ├── webserv/                 ← existing 42-school HTTP server (Epoll, C++98)
        │   ├── Makefile
        │   ├── config.conf
        │   └── srcs/                ← 4-layer architecture already in place
        └── testeur_webserver/       ← Unity integration test suite
```

---

## Branch Strategy

### Rule
`main` is always green (CI must pass to merge). No direct commits to `main`.

### Branch tree

```
main
│
├── feature/docs-contracts          ← Define SPI + HTTP contracts FIRST — unblocks all branches
│
├── feature/pico-stepper            ← StepperUC + IDm556Driver + Dm556Driver
├── feature/pico-dc-motor           ← DcMotorUC + IJgb37Driver + Jgb37Driver
├── feature/pico-servo              ← ServoUC + IMg995Driver + Mg995Driver
├── feature/pico-accelerometer      ← AccelerometerUC + IMpu6050 + Mpu6050
├── feature/pico-spi-slave          ← SPI slave API — merge AFTER all motor branches
│
├── feature/pi-spi-master           ← SPI master, Pico command client
├── feature/pi-camera               ← OV5647 capture, compression
├── feature/pi-http-client          ← HTTP upload to computer
│
├── feature/computer-scanner-routes ← New scan-control routes on existing webserver
├── feature/computer-websocket      ← WebSocket monitoring + dashboard pages
└── feature/computer-meshroom       ← Meshroom pipeline integration + cleanup
```

### Merge order

1. `feature/docs-contracts` → `main` (do this first)
2. All `feature/pico-{stepper,dc-motor,servo,accelerometer}` in parallel → `main`
3. `feature/pico-spi-slave` → `main` (after all motor branches)
4. All `feature/pi-*` and `feature/computer-*` in parallel → `main`
5. Integration testing on `main`

### Agent assignment
- Assign **one agent per leaf branch**. Each branch touches non-overlapping files.
- Each Pico motor branch lives entirely in `pico/src/2_usecases/{Motor}_UC/` +
  `pico/src/3_interfaces/I{Motor}Driver.hpp` + `pico/src/4_framework/{Motor}Driver/`.
- Always build (`make` / `cmake`) before marking a task complete.
- Always read `pico/docs/Convention_CPP.md` and this file before writing any code.
