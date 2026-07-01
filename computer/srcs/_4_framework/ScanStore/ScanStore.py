from __future__ import annotations

from pathlib import Path
from typing import Any

from srcs._4_framework.ScanStore.internal._load import _load
from srcs._4_framework.ScanStore.internal._save import _save
from srcs._4_framework.ScanStore.internal._list_all import _list_all
from srcs._4_framework.ScanStore.internal._ensure_folders import _ensure_folders
from srcs._4_framework.ScanStore.internal._update_hardware_metrics import _update_hardware_metrics


class ScanStore:
    def __init__(self, photos_dir: Path) -> None:
        self._photos_dir = photos_dir

    def load(self, name: str) -> dict[str, Any] | None:
        return _load(self._photos_dir, name)

    def save(self, name: str, data: dict[str, Any]) -> None:
        _save(self._photos_dir, name, data)

    def list_all(self) -> list[dict[str, Any]]:
        return _list_all(self._photos_dir)

    def ensure_folders(self, name: str) -> None:
        _ensure_folders(self._photos_dir, name)

    def update_hardware_metrics(self, scan_name: str, duration_s: float) -> None:
        _update_hardware_metrics(self._photos_dir, scan_name, duration_s)
