#!/usr/bin/env python3
"""
Collects cyclomatic complexity metrics for all components using Lizard.

Analyzes pico/motor/srcs, pi/srcs, and computer/srcs then writes:
  - computer/www/data/complexity.json    (for web dashboard JSON)
  - computer/www/complexity/index.html   (browsable HTML report)

Usage:
  python3 metrics/collect_complexity.py
"""
from __future__ import annotations

import datetime
import json
import os
import re
import subprocess
import sys
from pathlib import Path
from typing import Any

import lizard


COMPONENTS: dict[str, str] = {
    "pico":     "pico/motor/srcs",
    "pi":       "pi/srcs",
    "computer": "computer/srcs",
}
CCN_WARN     = 5
CCN_CRITICAL = 10
OUT_FILE     = Path("computer/www/data/complexity.json")
HTML_DIR     = Path("computer/www/complexity")
TOP_N        = 5


def main() -> None:
    results  = analyze_all_components()
    summary  = build_summary(results)
    print_table(summary)
    write_json(summary, OUT_FILE)
    generate_html_report()


def analyze_all_components() -> dict[str, dict[str, Any]]:
    return {
        name: analyze_component(name, path)
        for name, path in COMPONENTS.items()
        if Path(path).exists()
    }


def analyze_component(name: str, path: str) -> dict[str, Any]:
    functions = collect_functions(path)
    if not functions:
        return empty_component()
    return build_component_stats(functions)


def collect_functions(path: str) -> list[Any]:
    results = lizard.analyze(
        paths=[path],
        exts=lizard.get_extensions([]),
    )
    return [fn for fr in results for fn in fr.function_list]


def build_component_stats(functions: list[Any]) -> dict[str, Any]:
    ccns  = [f.cyclomatic_complexity for f in functions]
    nlocs = [f.nloc for f in functions]
    return {
        "function_count":    len(functions),
        "nloc":              sum(nlocs),
        "avg_ccn":           round(sum(ccns) / len(ccns), 2),
        "max_ccn":           max(ccns),
        "violations_over_5":  sum(1 for c in ccns if c > CCN_WARN),
        "violations_over_10": sum(1 for c in ccns if c > CCN_CRITICAL),
        "top_complex":       build_top_complex(functions),
    }


def build_top_complex(functions: list[Any]) -> list[dict[str, Any]]:
    sorted_fns = sorted(
        functions,
        key=lambda f: f.cyclomatic_complexity,
        reverse=True,
    )
    return [
        {
            "name": f.name,
            "file": f.filename,
            "ccn":  f.cyclomatic_complexity,
            "nloc": f.nloc,
        }
        for f in sorted_fns[:TOP_N]
    ]


def empty_component() -> dict[str, Any]:
    return {
        "function_count": 0, "nloc": 0,
        "avg_ccn": 0.0, "max_ccn": 0,
        "violations_over_5": 0, "violations_over_10": 0,
        "top_complex": [],
    }


def build_summary(results: dict[str, dict[str, Any]]) -> dict[str, Any]:
    return {
        "computed_at": datetime.datetime.now(datetime.timezone.utc)
                           .strftime("%Y-%m-%dT%H:%M:%SZ"),
        "thresholds": {"warn": CCN_WARN, "critical": CCN_CRITICAL},
        "components": results,
    }


def print_table(summary: dict[str, Any]) -> None:
    print()
    print("Complexity Analysis (CCN = Cyclomatic Complexity Number)")
    print("=" * 48)
    header = f"  {'Component':<12} {'Functions':>9} {'Max CCN':>8} {'>5':>4} {'>10':>4}"
    print(header)
    print("  " + "-" * 44)
    for comp, data in summary["components"].items():
        print(
            f"  {comp:<12} {data['function_count']:>9} "
            f"{data['max_ccn']:>8} "
            f"{data['violations_over_5']:>4} {data['violations_over_10']:>4}"
        )
    print()


def write_json(data: dict[str, Any], path: Path) -> None:
    os.makedirs(path.parent, exist_ok=True)
    path.write_text(json.dumps(data, indent=2))
    print(f"Written to {path}")


def generate_html_report() -> None:
    sources = [p for p in COMPONENTS.values() if Path(p).exists()]
    if not sources:
        return
    HTML_DIR.mkdir(parents=True, exist_ok=True)
    out = HTML_DIR / "index.html"
    result = subprocess.run(
        [sys.executable, "-m", "lizard", "--html", *sources],
        capture_output=True, text=True,
    )
    out.write_text(_wrap_lizard_html(result.stdout))
    print(f"HTML report → {out}")


def _extract_lizard_body(html: str) -> str:
    match = re.search(r'<body[^>]*>(.*?)</body>', html, re.DOTALL | re.IGNORECASE)
    if not match:
        return html
    body = match.group(1).strip()
    return re.sub(r'<h2[^>]*>.*?</h2>', '', body, flags=re.DOTALL | re.IGNORECASE).strip()


def _wrap_lizard_html(lizard_html: str) -> str:
    body = _extract_lizard_body(lizard_html)
    return f"""<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>Complexity</title>
  <script src="https://cdn.tailwindcss.com"></script>
  <link rel="stylesheet" href="https://cdn.datatables.net/1.13.4/css/jquery.dataTables.min.css">
  <script src="https://code.jquery.com/jquery-3.7.0.min.js"></script>
  <script src="https://cdn.datatables.net/1.13.4/js/jquery.dataTables.min.js"></script>
  <style>
    table#complexityTable th {{background:#1e293b;color:#facc15;padding:10px;text-align:left;border:1px solid #334155;}}
    table#complexityTable td {{border:1px solid #334155;color:#e2e8f0;}}
    table#complexityTable tr:nth-child(even) td {{background:rgba(255,255,255,0.03);}}
    table#complexityTable tr:hover td {{background:rgba(250,204,21,0.05);}}
    td.greater-value {{background:rgba(239,68,68,0.2)!important;color:#fca5a5;}}
    td.lesser-value  {{background:rgba(34,197,94,0.2)!important;color:#86efac;}}
    td.file-header   {{background:#1e3a5f!important;font-weight:bold;color:#93c5fd;}}
    td.function-name {{background:#1e293b!important;color:#bae6fd;}}
    td.footer        {{text-align:center;color:#64748b;}}
    td.footer a      {{color:#64748b;}}
    td.value         {{padding:2px 10px;text-align:center;white-space:nowrap;}}
    .dataTables_wrapper {{color:#94a3b8;}}
    .dataTables_wrapper input,.dataTables_wrapper select {{background:#1e293b;color:#e2e8f0;border:1px solid #334155;border-radius:4px;padding:2px 6px;}}
    .dataTables_paginate .paginate_button {{color:#94a3b8!important;}}
    .dataTables_paginate .paginate_button.current {{background:#facc15!important;color:#0f172a!important;border-color:#facc15!important;}}
  </style>
</head>
<body style="margin:0" class="font-sans text-slate-200">
  <page-card title="Complexity" page="complexity">
    <div style="width:100%;overflow-x:auto;text-align:left">
      {body}
    </div>
  </page-card>
  <script type="module" src="/js/PageCard.js"></script>
</body>
</html>"""


if __name__ == "__main__":
    main()
