from __future__ import annotations

import json
from dataclasses import dataclass
from enum import Enum


class PiResponseKind(Enum):
    PONG         = "PONG"
    DONE         = "DONE"
    FAIL         = "FAIL"
    INVALID_CMD  = "INVALID_CMD"
    STATE        = "STATE"
    UNKNOWN      = "UNKNOWN"

    @staticmethod
    def from_raw(raw: str) -> PiResponseKind:
        try:
            return PiResponseKind(raw)
        except ValueError:
            return PiResponseKind.UNKNOWN


_KNOWN_STATES: frozenset[str] = frozenset(
    {"INITIALIZATION", "READY", "PROCESSING", "SENDING"}
)


@dataclass(frozen=True)
class PiResponse:
    kind:    PiResponseKind
    payload: str
    command: str = ""

    @staticmethod
    def parse(raw: str) -> PiResponse:
        stripped = raw.strip()
        try:
            data = json.loads(stripped)
            if isinstance(data, dict):
                msg_type = str(data.get("type", ""))
                if msg_type == "state":
                    state = str(data.get("state", ""))
                    return PiResponse(kind=PiResponseKind.STATE, payload=state)
                if msg_type == "response":
                    kind_raw = str(data.get("kind", ""))
                    command = str(data.get("command", ""))
                    return PiResponse(
                        kind=PiResponseKind.from_raw(kind_raw),
                        payload=kind_raw,
                        command=command,
                    )
                if msg_type == "error":
                    reason = str(data.get("reason", ""))
                    return PiResponse(kind=PiResponseKind.INVALID_CMD, payload=reason)
        except json.JSONDecodeError:
            pass

        if stripped in _KNOWN_STATES:
            return PiResponse(kind=PiResponseKind.STATE, payload=stripped)
        kind = PiResponseKind.from_raw(stripped)
        return PiResponse(kind=kind, payload=stripped)

    def to_json(self) -> str:
        if self.kind == PiResponseKind.STATE:
            return json.dumps({"type": "state", "state": self.payload})

        if self.kind == PiResponseKind.INVALID_CMD:
            return json.dumps({"type": "error", "reason": self.payload})

        data: dict[str, str] = {
            "type": "response",
            "kind": self.kind.value,
        }
        if self.command:
            data["command"] = self.command
        if self.kind == PiResponseKind.UNKNOWN:
            data["payload"] = self.payload
        return json.dumps(data)
