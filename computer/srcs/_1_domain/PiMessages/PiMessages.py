from __future__ import annotations

import json
from dataclasses import dataclass
from enum import Enum
from typing import Any

from srcs._1_domain.PiMessages.internal._parse_json_dict import _parse_json_dict
from srcs._1_domain.PiMessages.internal._parse_plain_text import _parse_plain_text


class PiResponseKind(Enum):
    PONG        = "PONG"
    DONE        = "DONE"
    FAIL        = "FAIL"
    INVALID_CMD = "INVALID_CMD"
    STATE       = "STATE"
    PICO_STATUS = "PICO_STATUS"
    UNKNOWN     = "UNKNOWN"

    @staticmethod
    def from_raw(raw: str) -> PiResponseKind:
        try:
            return PiResponseKind(raw)
        except ValueError:
            return PiResponseKind.UNKNOWN


@dataclass(frozen=True)
class PiResponse:
    kind:    PiResponseKind
    payload: str
    command: str = ""
    ms:      int | None = None

    @staticmethod
    def parse(raw: str) -> PiResponse:
        stripped = raw.strip()
        try:
            data = json.loads(stripped)
            if isinstance(data, dict):
                result = _parse_json_dict(data)
                if result is not None:
                    return result
        except json.JSONDecodeError:
            pass
        return _parse_plain_text(stripped)

    def _build_response_data(self) -> dict[str, Any]:
        data: dict[str, Any] = {"type": "response", "kind": self.kind.value}
        if self.command:
            data["command"] = self.command
        if self.ms is not None:
            data["ms"] = self.ms
        if self.kind == PiResponseKind.UNKNOWN:
            data["payload"] = self.payload
        return data

    def to_json(self) -> str:
        if self.kind == PiResponseKind.STATE:
            return json.dumps({"type": "state", "state": self.payload})
        if self.kind == PiResponseKind.PICO_STATUS:
            return json.dumps({"type": "pico_status", "state": self.payload})
        if self.kind == PiResponseKind.INVALID_CMD:
            return json.dumps({"type": "error", "reason": self.payload})
        return json.dumps(self._build_response_data())
