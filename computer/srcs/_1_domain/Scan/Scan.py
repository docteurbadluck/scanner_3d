from __future__ import annotations

import re
from dataclasses import dataclass, field
from enum import Enum
from typing import Any

POSITIONS: tuple[str, ...] = ("A", "B", "C", "D")
SHOTS_PER_POSITION: int = 10
POSITION_PAIRS: tuple[tuple[str, str], ...] = (("A", "B"), ("C", "D"))
STEPS_PER_PAIR: int = 10


class ScanStatus(Enum):
    IN_PROGRESS = "in_progress"
    FAILED      = "failed"
    DONE        = "done"


@dataclass
class ScanState:
    name:           str
    status:         ScanStatus = ScanStatus.IN_PROGRESS
    position_index: int = 0
    shot_index:     int = 0
    error:          str = ""


def is_valid_scan_name(name: str) -> bool:
    return bool(re.fullmatch(r"[A-Za-z0-9_-]{1,64}", name))


def photo_filename(position: str, shot: int) -> str:
    return f"{position}_{shot}.jpg"


def scan_state_to_dict(state: ScanState) -> dict[str, Any]:
    return {
        "name":           state.name,
        "status":         state.status.value,
        "position_index": state.position_index,
        "shot_index":     state.shot_index,
        "error":          state.error,
    }


def scan_state_from_dict(data: dict[str, Any]) -> ScanState:
    return ScanState(
        name=data["name"],
        status=ScanStatus(data["status"]),
        position_index=data["position_index"],
        shot_index=data["shot_index"],
        error=data.get("error", ""),
    )
