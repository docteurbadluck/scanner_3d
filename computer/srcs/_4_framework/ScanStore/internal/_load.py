from __future__ import annotations

import json
from pathlib import Path
from typing import Any

STATE_FILE = ".scan_state.json"


def _load(photos_dir: Path, name: str) -> dict[str, Any] | None:
    path = photos_dir / name / STATE_FILE
    if not path.exists():
        return None
    return json.loads(path.read_text())
