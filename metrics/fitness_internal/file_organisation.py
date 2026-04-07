from __future__ import annotations

import re
from pathlib import Path
from typing import Any

from .config import CPP_ROOTS


def _extract_method_definitions(content: str, class_name: str) -> list[tuple[str, int]]:
    pattern = re.compile(rf'\b{re.escape(class_name)}::(\w+)\s*\(')
    return [
        (match.group(1), content[:match.start()].count("\n") + 1)
        for match in pattern.finditer(content)
    ]


def _extract_all_method_definitions(content: str) -> list[tuple[str, str, int]]:
    pattern = re.compile(r'\b([A-Z]\w*)::(\w+)\s*\(')
    return [
        (match.group(1), match.group(2), content[:match.start()].count("\n") + 1)
        for match in pattern.finditer(content)
    ]


def _accessor_target(method_name: str) -> str | None:
    if method_name.startswith("get"):
        return "getters.cpp"
    if method_name.startswith("set"):
        return "setters.cpp"
    return None


def _internal_target(method_name: str) -> str | None:
    if method_name.startswith("_"):
        return f"{method_name}.cpp"
    return None


def check_file_organisation() -> list[dict[str, Any]]:
    violations: list[dict[str, Any]] = []
    for root in CPP_ROOTS.values():
        if root.exists():
            violations += scan_file_organisation(root)
    return violations


def scan_file_organisation(root: Path) -> list[dict[str, Any]]:
    violations: list[dict[str, Any]] = []
    for path in root.rglob("*.cpp"):
        violations += check_accessor_file(path)
        violations += check_internal_file(path)
        class_name = path.stem
        if not class_name or not class_name[0].isupper():
            continue
        if (path.parent / f"{class_name}.hpp").exists():
            violations += check_class_cpp(path, class_name)
    return violations


def check_accessor_file(path: Path) -> list[dict[str, Any]]:
    content = path.read_text(errors="ignore")
    violations: list[dict[str, Any]] = []
    for class_name, method_name, line in _extract_all_method_definitions(content):
        target = _accessor_target(method_name)
        if target is None:
            continue
        if path.parent.name == "accessors" and path.name == target:
            continue
        violations.append({
            "file": str(path),
            "line": line,
            "rule": (
                f"{class_name}::{method_name}() must live in accessors/{target}"
            ),
        })
    return violations


def check_internal_file(path: Path) -> list[dict[str, Any]]:
    content = path.read_text(errors="ignore")
    violations: list[dict[str, Any]] = []
    for class_name, method_name, line in _extract_all_method_definitions(content):
        target = _internal_target(method_name)
        if target is None:
            continue
        if path.parent.name == "internal" and path.name == target:
            continue
        violations.append({
            "file": str(path),
            "line": line,
            "rule": (
                f"{class_name}::{method_name}() must live in internal/{target}"
            ),
        })
    return violations


def check_class_cpp(path: Path, class_name: str) -> list[dict[str, Any]]:
    violations: list[dict[str, Any]] = []
    content = path.read_text(errors="ignore")
    for method_name, line in _extract_method_definitions(content, class_name):
        if method_name == class_name:
            continue
        target = _accessor_target(method_name)
        if target is not None:
            destination = f"accessors/{target}"
        elif _internal_target(method_name) is not None:
            destination = f"internal/{_internal_target(method_name)}"
        else:
            destination = f"{method_name}.cpp"
        violations.append({
            "file": str(path),
            "line": line,
            "rule": (
                f"{path.name} must only define the constructor; "
                f"found {class_name}::{method_name}() -- move it to {destination}"
            ),
        })
    return violations
