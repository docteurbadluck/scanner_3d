from __future__ import annotations

import json
from dataclasses import dataclass
from datetime import datetime, timezone
from typing import Any

from srcs._1_domain.System import Commands, State
from srcs._1_domain.Scan.Scan import ScanState, scan_state_to_dict


@dataclass(frozen=True)
class StatusMessage:
    state:     str
    timestamp: str

    @staticmethod
    def build(state: State) -> StatusMessage:
        return StatusMessage(
            state=state.value,
            timestamp=datetime.now(timezone.utc).isoformat(),
        )

    def to_json(self) -> str:
        return json.dumps({
            "type":      "status",
            "state":     self.state,
            "timestamp": self.timestamp,
        })


@dataclass(frozen=True)
class PongMessage:
    def to_json(self) -> str:
        return json.dumps({"type": "pong"})


@dataclass(frozen=True)
class CommandMessage:
    command: str

    @staticmethod
    def build(command: str) -> CommandMessage:
        if command not in Commands().valid:
            raise ValueError(f"Unknown command: {command!r}")
        return CommandMessage(command=command)

    def to_json(self) -> str:
        return json.dumps({"type": "command", "command": self.command})


@dataclass(frozen=True)
class AckMessage:
    command: str

    def to_json(self) -> str:
        return json.dumps({"type": "ack", "command": self.command})


@dataclass(frozen=True)
class ErrorMessage:
    reason: str

    def to_json(self) -> str:
        return json.dumps({"type": "error", "reason": self.reason})


@dataclass(frozen=True)
class PiConnectionMessage:
    connected: bool

    def to_json(self) -> str:
        return json.dumps({"type": "pi_connection", "connected": self.connected})


@dataclass(frozen=True)
class PicoStatusMessage:
    state: str

    def to_json(self) -> str:
        return json.dumps({"type": "pico_status", "state": self.state})


@dataclass(frozen=True)
class _DebugMessage:
    raw: str

    def to_json(self) -> str:
        return json.dumps({"type": "debug", "raw": self.raw})


@dataclass(frozen=True)
class ScanStateMessage:
    scan: ScanState

    def to_json(self) -> str:
        return json.dumps({"type": "scan_state", **scan_state_to_dict(self.scan)})


@dataclass(frozen=True)
class ScanListMessage:
    scans: list[ScanState]

    def to_json(self) -> str:
        return json.dumps({
            "type":  "scan_list",
            "scans": [scan_state_to_dict(s) for s in self.scans],
        })
