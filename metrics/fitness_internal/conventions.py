from __future__ import annotations

from typing import Any

import lizard

from .config import CCN_MAX, CPP_ROOTS, NLOC_MAX, PY_ROOT


def check_conventions() -> list[dict[str, Any]]:
    paths = [str(root) for root in CPP_ROOTS.values() if root.exists()]
    if PY_ROOT.exists():
        paths.append(str(PY_ROOT))
    functions = collect_all_functions(paths)
    return [violation for fn in functions for violation in check_function(fn)]


def collect_all_functions(paths: list[str]) -> list[Any]:
    results = lizard.analyze(paths=paths, exts=lizard.get_extensions([]))
    return [fn for result in results for fn in result.function_list]


def check_function(fn: Any) -> list[dict[str, Any]]:
    violations = []
    if fn.nloc > NLOC_MAX:
        violations.append({
            "file": fn.filename,
            "function": fn.name,
            "value": fn.nloc,
            "rule": f"nloc {fn.nloc} > {NLOC_MAX}",
        })
    if fn.cyclomatic_complexity > CCN_MAX:
        violations.append({
            "file": fn.filename,
            "function": fn.name,
            "value": fn.cyclomatic_complexity,
            "rule": f"ccn {fn.cyclomatic_complexity} > {CCN_MAX}",
        })
    return violations
