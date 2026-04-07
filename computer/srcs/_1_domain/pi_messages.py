from __future__ import annotations

import json
from dataclasses import dataclass
from enum import Enum
from typing import Any


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

_STATE_MAPPING: dict[str, str] = {
    "LISTENING": "READY",
    "INTERPRETING": "PROCESSING",
    "EXECUTING": "PROCESSING",
    "INITIALIZATION": "INITIALIZATION",
    "READY": "READY",
    "PROCESSING": "PROCESSING",
    "SENDING": "SENDING",
}


def _normalize_state(raw_state: str) -> str:
    return _STATE_MAPPING.get(raw_state, raw_state)


def _parse_state_message(data: dict[str, Any]) -> PiResponse:
    state = _normalize_state(str(data.get("state", "")))
    return PiResponse(kind=PiResponseKind.STATE, payload=state)


def _parse_response_message(data: dict[str, Any]) -> PiResponse:
    kind_raw = str(data.get("kind", ""))
    command = str(data.get("command", ""))
    return PiResponse(
        kind=PiResponseKind.from_raw(kind_raw),
        payload=kind_raw,
        command=command,
    )


def _parse_error_message(data: dict[str, Any]) -> PiResponse:
    reason = str(data.get("reason", ""))
    return PiResponse(kind=PiResponseKind.INVALID_CMD, payload=reason)


def _parse_json_dict(data: dict[str, Any]) -> PiResponse | None:
    msg_type = str(data.get("type", ""))
    if msg_type == "state":
        return _parse_state_message(data)
    if msg_type == "response":
        return _parse_response_message(data)
    if msg_type == "error":
        return _parse_error_message(data)
    return None


def _parse_plain_text(stripped: str) -> PiResponse:
    normalized = _normalize_state(stripped)
    if stripped in _KNOWN_STATES or normalized != stripped:
        return PiResponse(kind=PiResponseKind.STATE, payload=normalized)
    kind = PiResponseKind.from_raw(stripped)
    return PiResponse(kind=kind, payload=stripped)


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
                result = _parse_json_dict(data)
                if result is not None:
                    return result
        except json.JSONDecodeError:
            pass
        return _parse_plain_text(stripped)

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
