from __future__ import annotations

import re
from pathlib import Path
from typing import Any

from .config import CPP_ROOTS, PY_LAYER_ROOTS

_UC_NAME_RE        = re.compile(r'^[A-Z][a-zA-Z0-9]+_UC$')
_CLASS_NAME_RE     = re.compile(r'^[A-Z][a-zA-Z0-9]+$')
_INTERFACE_NAME_RE = re.compile(r'^I[A-Z][a-zA-Z0-9]+$')


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
    for layer, root in PY_LAYER_ROOTS.items():
        violations += check_py_layer_structure(root, layer)
    return violations


def _name_pattern_for(layer: str) -> re.Pattern[str]:
    if layer == "_2_usecases":
        return _UC_NAME_RE
    if layer == "_3_interface":
        return _INTERFACE_NAME_RE
    return _CLASS_NAME_RE


def _check_flat_py_files(root: Path) -> list[dict[str, Any]]:
    violations: list[dict[str, Any]] = []
    for path in root.glob("*.py"):
        if path.name == "__init__.py":
            continue
        violations.append({
            "file": str(path),
            "line": 1,
            "rule": (
                f"'{path.name}' must be a folder PascalCase/ "
                f"(with __init__.py + PascalCase.py), not a flat file"
            ),
        })
    return violations


def _check_private_in_main(main_file: Path, folder_name: str) -> list[dict[str, Any]]:
    violations: list[dict[str, Any]] = []
    content = main_file.read_text(errors="ignore")
    pattern = re.compile(r'^(?:async\s+)?def\s+(_\w+)\s*\(', re.MULTILINE)
    for match in pattern.finditer(content):
        func_name = match.group(1)
        line = content[: match.start()].count("\n") + 1
        violations.append({
            "file": str(main_file),
            "line": line,
            "rule": (
                f"{folder_name}.py defines private function '{func_name}' "
                f"-- move it to internal/{func_name}.py"
            ),
        })
    return violations


def _check_internal_init(folder: Path, folder_name: str) -> list[dict[str, Any]]:
    internal = folder / "internal"
    if not internal.exists():
        return []
    if (internal / "__init__.py").exists():
        return []
    return [{
        "file": str(internal / "__init__.py"),
        "line": 1,
        "rule": f"{folder_name}/internal/ is missing __init__.py",
    }]


def _check_py_folder(folder: Path, name_pattern: re.Pattern[str]) -> list[dict[str, Any]]:
    violations: list[dict[str, Any]] = []
    name = folder.name
    if not name_pattern.match(name):
        violations.append({
            "file": str(folder),
            "line": 1,
            "rule": (
                f"Python folder '{name}' must follow PascalCase naming "
                f"({'PascalCase_UC' if name_pattern == _UC_NAME_RE else 'PascalCase'})"
            ),
        })
        return violations
    if not (folder / "__init__.py").exists():
        violations.append({
            "file": str(folder / "__init__.py"),
            "line": 1,
            "rule": f"{name}/ is missing __init__.py",
        })
    main_file = folder / f"{name}.py"
    if not main_file.exists():
        violations.append({
            "file": str(main_file),
            "line": 1,
            "rule": f"{name}/ is missing its main file {name}.py",
        })
    else:
        violations += _check_private_in_main(main_file, name)
    violations += _check_internal_init(folder, name)
    return violations


def check_py_layer_structure(root: Path, layer: str) -> list[dict[str, Any]]:
    if not root.exists():
        return []
    pattern = _name_pattern_for(layer)
    violations = _check_flat_py_files(root)
    for entry in sorted(root.iterdir()):
        if entry.is_dir() and not entry.name.startswith("__"):
            violations += _check_py_folder(entry, pattern)
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
