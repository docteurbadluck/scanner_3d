from __future__ import annotations

from datetime import datetime, timezone
from pathlib import Path


def _save_photo(
    photos_dir: Path,
    data: bytes,
    target: tuple[str, str, str] | None = None,
) -> None:
    if target is not None:
        scan_name, position, filename = target
        dest = photos_dir / scan_name / position
        dest.mkdir(parents=True, exist_ok=True)
        (dest / filename).write_bytes(data)
        return
    photos_dir.mkdir(exist_ok=True)
    timestamp: str = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%S%f")
    (photos_dir / f"{timestamp}.jpg").write_bytes(data)
