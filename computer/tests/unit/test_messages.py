from __future__ import annotations

import json
import sys
from pathlib import Path
from typing import Any

sys.path.insert(0, str(Path(__file__).parents[2]))

from server import WWW_DIR, build_http_handler, make_pong_message, make_status_message


def test_status_message_type() -> None:
    data: dict[str, Any] = json.loads(make_status_message())
    assert data["type"] == "status"


def test_status_message_has_state() -> None:
    data: dict[str, Any] = json.loads(make_status_message())
    assert "state" in data


def test_status_message_has_timestamp() -> None:
    data: dict[str, Any] = json.loads(make_status_message())
    assert "timestamp" in data


def test_pong_message_type() -> None:
    data: dict[str, Any] = json.loads(make_pong_message())
    assert data["type"] == "pong"


def test_http_handler_builds_without_error() -> None:
    handler = build_http_handler(WWW_DIR)
    assert handler is not None


def test_www_dir_exists() -> None:
    assert WWW_DIR.exists()
