from __future__ import annotations

from pathlib import Path

import pytest

from srcs._4_framework.ScanStore import ScanStore


def test_load_missing_returns_none(tmp_path: Path) -> None:
    store = ScanStore(tmp_path)
    assert store.load("nonexistent") is None


def test_save_and_load_roundtrip(tmp_path: Path) -> None:
    store = ScanStore(tmp_path)
    data = {"name": "my-scan", "status": "in_progress", "position_index": 1, "shot_index": 3}
    store.save("my-scan", data)
    assert store.load("my-scan") == data


def test_ensure_folders_creates_position_dirs(tmp_path: Path) -> None:
    store = ScanStore(tmp_path)
    store.ensure_folders("test-scan")
    for pos in ("A", "B", "C", "D"):
        assert (tmp_path / "test-scan" / pos).is_dir()


def test_list_all_returns_all_saved(tmp_path: Path) -> None:
    store = ScanStore(tmp_path)
    store.save("scan1", {"name": "scan1", "status": "done"})
    store.save("scan2", {"name": "scan2", "status": "failed"})
    results = store.list_all()
    names = {r["name"] for r in results}
    assert names == {"scan1", "scan2"}


def test_list_all_empty_dir(tmp_path: Path) -> None:
    store = ScanStore(tmp_path / "nope")
    assert store.list_all() == []


def test_save_photo_with_target(tmp_path: Path) -> None:
    from srcs._4_framework.UploadHandler.internal._save_photo import _save_photo
    _save_photo(tmp_path, b"imgdata", target=("my-scan", "B", "B_3.jpg"))
    dest = tmp_path / "my-scan" / "B" / "B_3.jpg"
    assert dest.exists()
    assert dest.read_bytes() == b"imgdata"


def test_save_photo_without_target_uses_timestamp(tmp_path: Path) -> None:
    from srcs._4_framework.UploadHandler.internal._save_photo import _save_photo
    photos_dir = tmp_path / "photos"
    _save_photo(photos_dir, b"imgdata")
    files = list(photos_dir.glob("*.jpg"))
    assert len(files) == 1
