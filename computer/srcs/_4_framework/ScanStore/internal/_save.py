from __future__ import annotations

import json
from pathlib import Path
from typing import Any

from srcs._4_framework.ScanStore.internal._load import STATE_FILE


def _save(photos_dir: Path, name: str, data: dict[str, Any]) -> None:
    scan_dir = photos_dir / name
    scan_dir.mkdir(parents=True, exist_ok=True)
    (scan_dir / STATE_FILE).write_text(json.dumps(data, indent=2))
