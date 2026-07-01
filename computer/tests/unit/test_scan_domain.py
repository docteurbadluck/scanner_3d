from __future__ import annotations

import pytest

from srcs._1_domain.Scan.Scan import (
    POSITIONS, SHOTS_PER_POSITION, ScanState, ScanStatus,
    is_valid_scan_name, photo_filename, scan_state_from_dict, scan_state_to_dict,
)


def test_valid_names_accepted() -> None:
    for name in ("scan1", "my-scan", "test_run", "A" * 64):
        assert is_valid_scan_name(name), f"Expected {name!r} to be valid"


def test_invalid_names_rejected() -> None:
    for name in ("", "a/b", "../etc", "a b", "x" * 65, "scan!", "a.b"):
        assert not is_valid_scan_name(name), f"Expected {name!r} to be invalid"


def test_photo_filename_format() -> None:
    assert photo_filename("A", 1) == "A_1.jpg"
    assert photo_filename("D", 10) == "D_10.jpg"


def test_scan_state_roundtrip() -> None:
    state = ScanState(
        name="my-scan", status=ScanStatus.FAILED,
        position_index=2, shot_index=5, error="timeout",
    )
    recovered = scan_state_from_dict(scan_state_to_dict(state))
    assert recovered == state


def test_scan_state_to_dict_fields() -> None:
    state = ScanState(name="test")
    d = scan_state_to_dict(state)
    assert d["name"] == "test"
    assert d["status"] == "in_progress"
    assert d["position_index"] == 0
    assert d["shot_index"] == 0
    assert d["error"] == ""


def test_constants() -> None:
    assert POSITIONS == ("A", "B", "C", "D")
    assert SHOTS_PER_POSITION == 10
