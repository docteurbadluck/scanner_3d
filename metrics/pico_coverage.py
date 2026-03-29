#!/usr/bin/env python3
from __future__ import annotations
import json
import subprocess
from pathlib import Path

PICO_DIR = Path("pico/motor")
OUT_FILE = Path("computer/www/data/pico_coverage.json")
_TMP = OUT_FILE.parent / "_gcovr_tmp.json"


def run_gcovr() -> dict:
    subprocess.run(
        ["gcovr",
         "--root", ".",
         "--filter", "srcs/",
         "--object-directory", "build_coverage/srcs",
         "--json-summary", str(_TMP.resolve())],
        cwd=str(PICO_DIR.resolve()),
        check=True,
    )
    data = json.loads(_TMP.read_text())
    _TMP.unlink()
    return data


def extract_pct(gcovr_data: dict) -> float:
    lines = gcovr_data.get("lines")
    if isinstance(lines, dict):
        return lines.get("percent", 0.0)
    return gcovr_data.get("line_percent", 0.0)


def main() -> None:
    data = run_gcovr()
    pct = extract_pct(data)
    OUT_FILE.write_text(json.dumps({"totals": {"percent_covered": pct}}, indent=2))
    print(f"Pico coverage: {pct:.1f}%")


if __name__ == "__main__":
    main()
