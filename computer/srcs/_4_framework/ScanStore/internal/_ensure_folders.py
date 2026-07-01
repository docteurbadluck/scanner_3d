from __future__ import annotations

from pathlib import Path

POSITIONS = ("A", "B", "C", "D")


def _ensure_folders(photos_dir: Path, name: str) -> None:
    scan_dir = photos_dir / name
    for position in POSITIONS:
        (scan_dir / position).mkdir(parents=True, exist_ok=True)
