from __future__ import annotations

_KNOWN_STATES: frozenset[str] = frozenset(
    {"INITIALIZATION", "READY", "PROCESSING", "SENDING"}
)

_STATE_MAPPING: dict[str, str] = {
    "LISTENING":      "READY",
    "INITIALIZATION": "INITIALIZATION",
    "READY":          "READY",
    "PROCESSING":     "PROCESSING",
    "SENDING":        "SENDING",
}


def _normalize_state(raw_state: str) -> str:
    return _STATE_MAPPING.get(raw_state, raw_state)
