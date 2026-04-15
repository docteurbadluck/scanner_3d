#!/usr/bin/env python3
"""Interactive UART console for the Pico motor firmware.

Usage:
    python3 scripts/pico_serial.py                   # auto-detect port
    python3 scripts/pico_serial.py /dev/ttyACM0      # USB CDC
    python3 scripts/pico_serial.py /dev/serial0      # TX/RX hardware UART
"""
from __future__ import annotations

import json
import sys
import time
import serial
import serial.tools.list_ports

BAUD         = 115200
TIMEOUT_S    = 3.0
MULTI_RESP   = 3          # Pico sends up to 3 lines per command (2x state + 1x response)
CANDIDATES   = ["/dev/serial0"]
COMMANDS     = [
    "PING", "GET_STATUS",
]


# ── port detection ─────────────────────────────────────────────────────────────

def _autodetect_port() -> str:
    for path in CANDIDATES:
        try:
            with serial.Serial(path, BAUD, timeout=0.1):
                return path
        except serial.SerialException:
            continue
    raise RuntimeError(f"No Pico found on {CANDIDATES}")


def open_port(path: str | None) -> serial.Serial:
    resolved = path or _autodetect_port()
    port = serial.Serial(resolved, BAUD, timeout=TIMEOUT_S)
    time.sleep(0.2)
    port.reset_input_buffer()
    print(f"[connected] {resolved}  {BAUD} baud")
    return port


# ── send / receive ─────────────────────────────────────────────────────────────

def send_command(port: serial.Serial, cmd: str) -> None:
    port.write((cmd.strip() + "\n").encode())
    port.flush()


def read_responses(port: serial.Serial) -> list[dict]:
    responses: list[dict] = []
    deadline = time.time() + TIMEOUT_S
    while time.time() < deadline and len(responses) < MULTI_RESP:
        line = port.readline().decode(errors="replace").strip()
        if not line:
            break
        try:
            responses.append(json.loads(line))
        except json.JSONDecodeError:
            responses.append({"raw": line})
    return responses


# ── display ────────────────────────────────────────────────────────────────────

def _color(msg_type: str) -> str:
    return {"response": "\033[92m", "state": "\033[94m", "error": "\033[91m"}.get(
        msg_type, "\033[0m"
    )


def print_responses(responses: list[dict]) -> None:
    reset = "\033[0m"
    for r in responses:
        color = _color(r.get("type", ""))
        print(f"  {color}{json.dumps(r)}{reset}")
    if not responses:
        print("  (no response — timeout)")


# ── REPL ───────────────────────────────────────────────────────────────────────

def _print_help() -> None:
    print("Commands:", ", ".join(COMMANDS))
    print("Meta:     help  exit  quit")


def run_repl(port: serial.Serial) -> None:
    _print_help()
    while True:
        try:
            raw = input("\npico> ").strip().upper()
        except (EOFError, KeyboardInterrupt):
            print("\n[exit]")
            break
        if not raw:
            continue
        if raw in ("EXIT", "QUIT"):
            break
        if raw == "HELP":
            _print_help()
            continue
        send_command(port, raw)
        print_responses(read_responses(port))


# ── main ───────────────────────────────────────────────────────────────────────

def main() -> None:
    path = sys.argv[1] if len(sys.argv) > 1 else None
    try:
        port = open_port(path)
    except RuntimeError as exc:
        print(f"[error] {exc}", file=sys.stderr)
        sys.exit(1)
    with port:
        run_repl(port)


if __name__ == "__main__":
    main()
