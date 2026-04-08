#!/usr/bin/env python3
"""
Architectural fitness functions for the 3D Scanner project.

Checks:
  1. Layer violations  — no layer imports a layer it must not depend on
  2. Conventions       — function NLOC <= 15, CCN <= 6
  3. Coverage          — each component >= 80%

Exits with code 1 if any check fails (CI-friendly).

Usage:
  python3 metrics/fitness.py
"""
from __future__ import annotations

from typing import Any

from fitness_internal import (
    build_report,
    check_conventions,
    check_coverage,
    check_file_organisation,
    check_layer_violations,
    exit_with_status,
    print_table,
    write_json,
)
from fitness_internal.config import OUT_FILE


def main() -> None:
    results = run_all_checks()
    report = build_report(results)
import datetime
import json
import re
import sys
from pathlib import Path
from typing import Any

import lizard


# ── Configuration ─────────────────────────────────────────────────────────────

OUT_FILE       = Path("computer/www/data/fitness.json")
COVERAGE_MIN   = 80.0
NLOC_MAX       = 15
CCN_MAX        = 6

CPP_ROOTS: dict[str, Path] = {
    "pico": Path("pico/motor/srcs"),
    "pi":   Path("pi/srcs"),
}
PY_ROOT = Path("computer/srcs")

COVERAGE_FILES: dict[str, Path] = {
    "pico":     Path("computer/www/data/pico_coverage.json"),
    "pi":       Path("computer/www/data/pi_coverage.json"),
    "computer": Path("computer/www/data/computer_srcs_coverage.json"),
}

# layer_name → forbidden imported layers
CPP_LAYER_RULES: dict[str, list[str]] = {
    "1_domain":    ["2_usecases", "3_interface", "4_drivers", "4_framework"],
    "2_usecases":  ["4_drivers", "4_framework"],
    "3_interface": ["1_domain", "2_usecases", "4_drivers", "4_framework"],
    "4_drivers":   ["1_domain", "2_usecases"],
    "4_framework": ["1_domain", "2_usecases"],
}
PY_LAYER_RULES: dict[str, list[str]] = {
    "_1_domain":   ["_2_usecases", "_3_interface"],
    "_3_interface": ["_1_domain", "_2_usecases"],
}


# ── Entry point ────────────────────────────────────────────────────────────────

def main() -> None:
    results = run_all_checks()
    report  = build_report(results)
    print_table(report)
    write_json(report, OUT_FILE)
    exit_with_status(report)


def run_all_checks() -> dict[str, list[dict[str, Any]]]:
    return {
        "layer_violations": check_layer_violations(),
        "conventions": check_conventions(),
        "coverage": check_coverage(),
        "file_organisation": check_file_organisation(),
    }


if __name__ == "__main__":
    main()
