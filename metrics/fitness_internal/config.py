from __future__ import annotations

from pathlib import Path

OUT_FILE = Path("computer/www/data/fitness.json")
COVERAGE_MIN = 80.0
NLOC_MAX = 15
CCN_MAX = 6

CPP_ROOTS: dict[str, Path] = {
    "pico": Path("pico/motor/srcs"),
    "pi": Path("pi/srcs"),
}
PY_ROOT = Path("computer/srcs")

COVERAGE_FILES: dict[str, Path] = {
    "pico": Path("computer/www/data/pico_coverage.json"),
    "pi": Path("computer/www/data/pi_coverage.json"),
    "computer": Path("computer/www/data/computer_srcs_coverage.json"),
}

CPP_LAYER_RULES: dict[str, list[str]] = {
    "1_domain": ["2_usecases", "3_interface", "4_drivers", "4_framework"],
    "2_usecases": ["4_drivers", "4_framework"],
    "3_interface": ["1_domain", "2_usecases", "4_drivers", "4_framework"],
    "4_drivers": ["1_domain", "2_usecases"],
    "4_framework": ["1_domain", "2_usecases"],
}
PY_LAYER_RULES: dict[str, list[str]] = {
    "_1_domain": ["_2_usecases", "_3_interface"],
    "_3_interface": ["_1_domain", "_2_usecases"],
}
