from __future__ import annotations

import json

import pytest

from srcs._1_domain.pi_messages import PiResponse, PiResponseKind
from srcs._2_usecases.forward_command_to_pi_uc import forward_command_to_pi
from srcs._2_usecases.handle_pi_message_uc import handle_pi_message


class FakePi:
    def __init__(self) -> None:
        self.sent: list[str] = []

    async def send(self, message: str) -> None:
        self.sent.append(message)


class FakeBrowser:
    def __init__(self) -> None:
        self.sent: list[str] = []

    async def send(self, message: str) -> None:
        self.sent.append(message)


# --- PiResponse parsing ---

@pytest.mark.parametrize("raw,expected_kind,expected_payload", [
    ("PONG",           PiResponseKind.PONG,        "PONG"),
    ("DONE",           PiResponseKind.DONE,        "DONE"),
    ("FAIL",           PiResponseKind.FAIL,        "FAIL"),
    ("INVALID_CMD",    PiResponseKind.INVALID_CMD, "INVALID_CMD"),
    ("INITIALIZATION", PiResponseKind.STATE,       "INITIALIZATION"),
    ("READY",          PiResponseKind.STATE,       "READY"),
    ("PROCESSING",     PiResponseKind.STATE,       "PROCESSING"),
    ("SENDING",        PiResponseKind.STATE,       "SENDING"),
    ("GARBAGE",        PiResponseKind.UNKNOWN,     "GARBAGE"),
])
def test_pi_response_parse(raw: str, expected_kind: PiResponseKind, expected_payload: str) -> None:
    r = PiResponse.parse(raw)
    assert r.kind    == expected_kind
    assert r.payload == expected_payload


def test_pi_response_to_json() -> None:
    r = PiResponse.parse("DONE")
    data = json.loads(r.to_json())
    assert data["type"]    == "pi_response"
    assert data["kind"]    == "DONE"
    assert data["payload"] == "DONE"


# --- forward_command_to_pi ---

async def test_forward_command_sends_raw_string() -> None:
    pi = FakePi()
    await forward_command_to_pi(pi, "START_CAPTURE")
    assert pi.sent == ["START_CAPTURE"]


# --- handle_pi_message ---

async def test_handle_pi_message_broadcasts_to_all_browsers() -> None:
    b1, b2 = FakeBrowser(), FakeBrowser()
    await handle_pi_message("DONE", [b1, b2])

    for browser in (b1, b2):
        assert len(browser.sent) == 1
        data = json.loads(browser.sent[0])
        assert data["type"]    == "pi_response"
        assert data["kind"]    == "DONE"


async def test_handle_pi_message_no_browsers_does_not_raise() -> None:
    await handle_pi_message("DONE", [])


async def test_handle_pi_message_state_forwarded() -> None:
    b = FakeBrowser()
    await handle_pi_message("READY", [b])
    data = json.loads(b.sent[0])
    assert data["kind"]    == "STATE"
    assert data["payload"] == "READY"
