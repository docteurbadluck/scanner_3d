from __future__ import annotations

import json
from pathlib import Path

_METRICS_FILE = Path("data") / "hardware_metrics.json"


def _update_hardware_metrics(photos_dir: Path, scan_name: str, duration_s: float) -> None:
    path = photos_dir.parent / _METRICS_FILE
    try:
        existing: dict = json.loads(path.read_text()) if path.exists() else {}
    except Exception:
        existing = {}
    existing["scan_completion_time_seconds"] = duration_s
    existing["last_scan"] = scan_name
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(existing, indent=2))
