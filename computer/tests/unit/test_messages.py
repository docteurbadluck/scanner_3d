from __future__ import annotations

import json
from typing import Any

from server import WWW_DIR
from srcs._4_framework.http_server import build_http_handler
from srcs._1_domain.messages import PongMessage, StatusMessage
from srcs._1_domain.system import State


def test_status_message_type() -> None:
    data: dict[str, Any] = json.loads(StatusMessage.build(State.INITIALIZATION).to_json())
    assert data["type"] == "status"


def test_status_message_has_state() -> None:
    data: dict[str, Any] = json.loads(StatusMessage.build(State.INITIALIZATION).to_json())
    assert "state" in data


def test_status_message_has_timestamp() -> None:
    data: dict[str, Any] = json.loads(StatusMessage.build(State.INITIALIZATION).to_json())
    assert "timestamp" in data


def test_pong_message_type() -> None:
    data: dict[str, Any] = json.loads(PongMessage().to_json())
    assert data["type"] == "pong"


def test_http_handler_builds_without_error() -> None:
    handler = build_http_handler(WWW_DIR)
    assert handler is not None


def test_www_dir_exists() -> None:
    assert WWW_DIR.exists()
