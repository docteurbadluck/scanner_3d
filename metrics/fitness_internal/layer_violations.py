from __future__ import annotations

import re
from pathlib import Path
from typing import Any

from .config import CPP_LAYER_RULES, CPP_ROOTS, PY_LAYER_RULES, PY_ROOT


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
    for path in root.rglob("*.[ch]pp"):
        layer = detect_cpp_layer(path, root)
        if layer:
            violations += check_cpp_file(path, layer)
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
        match = re.match(r'\s*#include\s+"([^"]+)"', line)
        if not match:
            continue
        included = match.group(1)
        hit = next((name for name in forbidden if included.startswith(name + "/")), None)
        if hit:
            violations.append({
                "file": str(path),
                "line": lineno,
                "include": included,
                "rule": f"{layer} must not import {hit}",
            })
    return violations


def scan_py_violations(root: Path) -> list[dict[str, Any]]:
    violations: list[dict[str, Any]] = []
    for path in root.rglob("*.py"):
        layer = detect_py_layer(path, root)
        if layer:
            violations += check_py_file(path, layer)
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
        match = pattern.search(line)
        if not match:
            continue
        hit = next((name for name in forbidden if match.group(1).startswith(name.lstrip("_"))), None)
        if hit:
            violations.append({
                "file": str(path),
                "line": lineno,
                "include": match.group(1),
                "rule": f"{layer} must not import {hit}",
            })
    return violations
