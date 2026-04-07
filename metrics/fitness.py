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
        "conventions":      check_conventions(),
        "coverage":         check_coverage(),
    }


# ── Checker 1: Layer violations ────────────────────────────────────────────────

def check_layer_violations() -> list[dict[str, Any]]:
    violations: list[dict[str, Any]] = []
    for root in CPP_ROOTS.values():
        if root.exists():
            violations += scan_cpp_violations(root)
    if PY_ROOT.exists():
        violations += scan_py_violations(PY_ROOT)
    return violations


def scan_cpp_violations(root: Path) -> list[dict[str, Any]]:
    violations: list[dict[str, Any]] = []
    for f in root.rglob("*.[ch]pp"):
        layer = detect_cpp_layer(f, root)
        if layer:
            violations += check_cpp_file(f, layer)
    return violations


def detect_cpp_layer(path: Path, root: Path) -> str | None:
    parts = path.relative_to(root).parts
    return parts[0] if parts else None


def check_cpp_file(path: Path, layer: str) -> list[dict[str, Any]]:
    forbidden = CPP_LAYER_RULES.get(layer, [])
    if not forbidden:
        return []
    violations = []
    for lineno, line in enumerate(path.read_text(errors="ignore").splitlines(), 1):
        m = re.match(r'\s*#include\s+"([^"]+)"', line)
        if not m:
            continue
        included = m.group(1)
        hit = next((f for f in forbidden if included.startswith(f + "/")), None)
        if hit:
            violations.append({
                "file":    str(path),
                "line":    lineno,
                "include": included,
                "rule":    f"{layer} must not import {hit}",
            })
    return violations


def scan_py_violations(root: Path) -> list[dict[str, Any]]:
    violations: list[dict[str, Any]] = []
    for f in root.rglob("*.py"):
        layer = detect_py_layer(f, root)
        if layer:
            violations += check_py_file(f, layer)
    return violations


def detect_py_layer(path: Path, root: Path) -> str | None:
    parts = path.relative_to(root).parts
    return parts[0] if parts else None


def check_py_file(path: Path, layer: str) -> list[dict[str, Any]]:
    forbidden = PY_LAYER_RULES.get(layer, [])
    if not forbidden:
        return []
    violations = []
    pattern = re.compile(r'(?:from|import)\s+srcs\.(\S+)')
    for lineno, line in enumerate(path.read_text(errors="ignore").splitlines(), 1):
        m = pattern.search(line)
        if not m:
            continue
        imported_layer = "_" + m.group(1).split(".")[0] if not m.group(1).startswith("_") else m.group(1).split(".")[0]
        hit = next((f for f in forbidden if m.group(1).startswith(f.lstrip("_"))), None)
        if hit:
            violations.append({
                "file":    str(path),
                "line":    lineno,
                "include": m.group(1),
                "rule":    f"{layer} must not import {hit}",
            })
    return violations


# ── Checker 2: Conventions ─────────────────────────────────────────────────────

def check_conventions() -> list[dict[str, Any]]:
    paths = [str(r) for r in CPP_ROOTS.values() if r.exists()]
    if PY_ROOT.exists():
        paths.append(str(PY_ROOT))
    functions = collect_all_functions(paths)
    return [v for fn in functions for v in check_function(fn)]


def collect_all_functions(paths: list[str]) -> list[Any]:
    results = lizard.analyze(paths=paths, exts=lizard.get_extensions([]))
    return [fn for fr in results for fn in fr.function_list]


def check_function(fn: Any) -> list[dict[str, Any]]:
    violations = []
    if fn.nloc > NLOC_MAX:
        violations.append({
            "file":     fn.filename,
            "function": fn.name,
            "value":    fn.nloc,
            "rule":     f"nloc {fn.nloc} > {NLOC_MAX}",
        })
    if fn.cyclomatic_complexity > CCN_MAX:
        violations.append({
            "file":     fn.filename,
            "function": fn.name,
            "value":    fn.cyclomatic_complexity,
            "rule":     f"ccn {fn.cyclomatic_complexity} > {CCN_MAX}",
        })
    return violations


# ── Checker 3: Coverage ────────────────────────────────────────────────────────

def check_coverage() -> list[dict[str, Any]]:
    violations = []
    for component, path in COVERAGE_FILES.items():
        if not path.exists():
            continue
        pct = read_coverage_pct(path, component)
        if pct is not None and pct < COVERAGE_MIN:
            violations.append({
                "component": component,
                "coverage":  round(pct, 1),
                "rule":      f"coverage {pct:.1f}% < {COVERAGE_MIN}%",
            })
    return violations


def read_coverage_pct(path: Path, component: str) -> float | None:
    data = json.loads(path.read_text())
    if component == "computer":
        return data.get("totals", {}).get("percent_covered")
    return data.get("totals", {}).get("percent_covered")


# ── Report ─────────────────────────────────────────────────────────────────────

def build_report(results: dict[str, list[dict[str, Any]]]) -> dict[str, Any]:
    checks = {
        name: {
            "passed":          len(vs) == 0,
            "violation_count": len(vs),
            "violations":      vs,
        }
        for name, vs in results.items()
    }
    return {
        "computed_at": datetime.datetime.now(datetime.timezone.utc)
                           .strftime("%Y-%m-%dT%H:%M:%SZ"),
        "passed":  all(c["passed"] for c in checks.values()),
        "checks":  checks,
    }


def print_table(report: dict[str, Any]) -> None:
    status = "PASSED" if report["passed"] else "FAILED"
    print()
    print(f"Fitness Functions — {status}")
    print("=" * 50)
    for name, check in report["checks"].items():
        icon   = "OK" if check["passed"] else "FAIL"
        count  = check["violation_count"]
        label  = name.replace("_", " ").title()
        print(f"  [{icon:4}] {label:<25} {count} violation(s)")
        if not check["passed"]:
            for v in check["violations"][:5]:
                desc = v.get("rule", "")
                loc  = v.get("file", v.get("component", ""))
                print(f"           ↳ {loc}: {desc}")
            if count > 5:
                print(f"           ↳ ... and {count - 5} more")
    print()


def write_json(data: dict[str, Any], path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, indent=2))
    print(f"Written to {path}")


def exit_with_status(report: dict[str, Any]) -> None:
    sys.exit(0 if report["passed"] else 1)


if __name__ == "__main__":
    main()
