from __future__ import annotations

from datetime import datetime, timezone
from pathlib import Path


def _save_photo(photos_dir: Path, data: bytes) -> None:
    photos_dir.mkdir(exist_ok=True)
    timestamp: str = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%S%f")
    (photos_dir / f"{timestamp}.jpg").write_bytes(data)
