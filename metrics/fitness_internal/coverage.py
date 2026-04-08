from __future__ import annotations

import json
from pathlib import Path
from typing import Any

from .config import COVERAGE_FILES, COVERAGE_MIN


def check_coverage() -> list[dict[str, Any]]:
    violations: list[dict[str, Any]] = []
    for component, path in COVERAGE_FILES.items():
        if not path.exists():
            continue
        pct = read_coverage_pct(path)
        if pct is not None and pct < COVERAGE_MIN:
            violations.append({
                "component": component,
                "coverage": round(pct, 1),
                "rule": f"coverage {pct:.1f}% < {COVERAGE_MIN}%",
            })
    return violations


def read_coverage_pct(path: Path) -> float | None:
    data: dict[str, Any] = json.loads(path.read_text())
    return data.get("totals", {}).get("percent_covered")
