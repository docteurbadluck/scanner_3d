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

    @staticmethod
    def parse(raw: str) -> PiResponse:
        stripped = raw.strip()
        if stripped in _KNOWN_STATES:
            return PiResponse(kind=PiResponseKind.STATE, payload=stripped)
        kind = PiResponseKind.from_raw(stripped)
        return PiResponse(kind=kind, payload=stripped)

    def to_json(self) -> str:
        return json.dumps({"type": "pi_response", "kind": self.kind.value, "payload": self.payload})
