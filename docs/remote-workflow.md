# Remote development workflow

Code on your computer, build and flash remotely on the Pi — one command.

## Quick start

### 1. SSH key auth (one-time)

```bash
ssh-keygen -t ed25519        # if you don't have a key yet
ssh-copy-id pi@<pi-ip>       # copy public key to Pi
```

### 2. SSH alias + multiplexing (one-time, on your computer)

Add to `~/.ssh/config`:

```
Host pi
    HostName <pi-ip>
    User pi
    IdentityFile ~/.ssh/id_ed25519
    ControlMaster auto
    ControlPath ~/.ssh/cm-%r@%h:%p
    ControlPersist 60s
```

> `ControlMaster` reuses the TCP connection across commands — sync, build, and flash
> share one SSH handshake, making the full loop significantly faster.

### 3. Pi — install toolchain (one-time)

```bash
sudo apt update
sudo apt install cmake ninja-build gcc-arm-none-eabi openocd
# picotool — build from source or use package if available:
sudo apt install picotool
# Set PICO SDK path in ~/.bashrc:
export PICO_SDK_PATH=~/pico/pico-sdk
```

### 4. Verify everything

```bash
make check-ssh        # should print "ok"
make check-prereqs    # ✅/❌ report for all tools on Pi
```

---

## Daily workflow

```bash
make sync             # push local changes to Pi
make build-pico       # compile firmware on Pi
make flash-pico       # flash via USB (default)
```

Or in one command:

```bash
make all-remote       # sync → build-pi → test-pi → build-pico → flash-pico
```

### Override variables

```bash
make sync      PI_HOST=raspberrypi PI_REPO=/home/pi/scanner
make flash-pico FLASH_MODE=swd     # SWD via openocd (no button press)
```

### Sync variants

| Command | Behaviour |
|---------|-----------|
| `make sync` | Transfer changed files |
| `make sync-dry` | Show what would transfer, nothing sent |
| `make sync-delete` | Also delete files removed locally (careful) |

---

## Flash modes

| Mode | Command | Requires |
|------|---------|---------|
| USB (default) | `make flash-pico` | Pico connected via USB, `picotool` installed |
| SWD | `make flash-pico FLASH_MODE=swd` | 3-wire debug header wired (see below) |

### SWD wiring

```
Pi GPIO 24  →  Pico SWDCLK  (debug header pad)
Pi GPIO 25  →  Pico SWDIO   (debug header pad)
Pi GND      →  Pico SWD GND (debug header pad)
```

SWD flashes without pressing BOOTSEL — works while the Pico is running.

---

## Troubleshooting

### `ssh pi "echo ok"` prompts for password
- Run `ssh-copy-id pi@<pi-ip>` to install your public key.
- Check `~/.ssh/config` has the correct `HostName` and `User`.

### `rsync` transfers nothing but changes exist
- Run `make sync-dry` to see what rsync sees.
- Check `.rsync-exclude` isn't excluding files you changed.

### `picotool: device not found`
- Verify the USB cable supports data (not charge-only).
- Try `ssh pi "lsusb | grep Raspberry"` to check USB visibility.
- For automatic BOOTSEL-free flash, use SWD mode instead.

### `openocd: Error: unable to open MPSSE device`
- Verify the 3-wire SWD connection.
- Check `openocd` version: `openocd --version` (needs ≥ 0.11).
- Ensure Pi GPIO is not claimed by another process.

### `cmake: no such file` on Pi
- Run `make check-prereqs` and follow the ❌ items.
- Ensure `$PICO_SDK_PATH` is exported in `~/.bashrc` (not just `~/.profile`).

### Build fails on Pi but passes locally
- Likely a toolchain version mismatch — run `make check-prereqs` and compare versions.
- Run `make build-pico` alone first to isolate the failing step.
