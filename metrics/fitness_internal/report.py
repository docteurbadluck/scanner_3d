from __future__ import annotations

import datetime
import json
import sys
from pathlib import Path
from typing import Any


def build_report(results: dict[str, list[dict[str, Any]]]) -> dict[str, Any]:
    checks = {
        name: {
            "passed": len(violations) == 0,
            "violation_count": len(violations),
            "violations": violations,
        }
        for name, violations in results.items()
    }
    return {
        "computed_at": datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "passed": all(check["passed"] for check in checks.values()),
        "checks": checks,
    }


def print_table(report: dict[str, Any]) -> None:
    status = "PASSED" if report["passed"] else "FAILED"
    print()
    print(f"Fitness Functions — {status}")
    print("=" * 50)
    for name, check in report["checks"].items():
        icon = "OK" if check["passed"] else "FAIL"
        count = check["violation_count"]
        label = name.replace("_", " ").title()
        print(f"  [{icon:4}] {label:<25} {count} violation(s)")
        if not check["passed"]:
            for violation in check["violations"][:5]:
                desc = violation.get("rule", "")
                loc = violation.get("file", violation.get("component", ""))
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
