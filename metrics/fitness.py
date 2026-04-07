#!/usr/bin/env python3
"""
Architectural fitness functions for the 3D Scanner project.

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
