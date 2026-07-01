from __future__ import annotations

import json
from pathlib import Path
from typing import Any

from srcs._4_framework.ScanStore.internal._load import STATE_FILE


def _list_all(photos_dir: Path) -> list[dict[str, Any]]:
    if not photos_dir.exists():
        return []
    results: list[dict[str, Any]] = []
    for state_file in sorted(photos_dir.glob(f"*/{STATE_FILE}")):
        try:
            results.append(json.loads(state_file.read_text()))
        except Exception:
            pass
    return results
