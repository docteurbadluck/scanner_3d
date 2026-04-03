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


# --- JSON transport parsing ---

@pytest.mark.parametrize("raw,expected_kind,expected_payload,expected_command", [
    ('{"type":"response","kind":"PONG","command":"PING"}', PiResponseKind.PONG, "PONG", "PING"),
    ('{"type":"response","kind":"DONE","command":"START_CAPTURE"}', PiResponseKind.DONE, "DONE", "START_CAPTURE"),
    ('{"type":"response","kind":"FAIL","command":"START_CAPTURE"}', PiResponseKind.FAIL, "FAIL", "START_CAPTURE"),
    ('{"type":"error","reason":"INVALID_CMD"}', PiResponseKind.INVALID_CMD, "INVALID_CMD", ""),
    ('{"type":"state","state":"INITIALIZATION"}', PiResponseKind.STATE, "INITIALIZATION", ""),
    ('{"type":"state","state":"READY"}', PiResponseKind.STATE, "READY", ""),
    ('{"type":"state","state":"PROCESSING"}', PiResponseKind.STATE, "PROCESSING", ""),
    ('{"type":"state","state":"SENDING"}', PiResponseKind.STATE, "SENDING", ""),
    ("GARBAGE", PiResponseKind.UNKNOWN, "GARBAGE", ""),
])
def test_transport_message_parse(raw: str, expected_kind: PiResponseKind, expected_payload: str, expected_command: str) -> None:
    r = PiResponse.parse(raw)
    assert r.kind    == expected_kind
    assert r.payload == expected_payload
    assert r.command == expected_command


def test_transport_message_to_json() -> None:
    r = PiResponse.parse('{"type":"response","kind":"DONE","command":"START_CAPTURE"}')
    data = json.loads(r.to_json())
    assert data["type"]    == "response"
    assert data["kind"]    == "DONE"
    assert data["command"] == "START_CAPTURE"


# --- forward_command_to_pi ---

async def test_forward_command_sends_json_command() -> None:
    pi = FakePi()
    await forward_command_to_pi(pi, "START_CAPTURE")
    assert pi.sent == ['{"type": "command", "command": "START_CAPTURE"}']


# --- handle_pi_message ---

async def test_handle_pi_message_broadcasts_to_all_browsers() -> None:
    b1, b2 = FakeBrowser(), FakeBrowser()
    await handle_pi_message('{"type":"response","kind":"DONE","command":"START_CAPTURE"}', [b1, b2])

    for browser in (b1, b2):
        assert len(browser.sent) == 1
        data = json.loads(browser.sent[0])
        assert data["type"]    == "response"
        assert data["kind"]    == "DONE"
        assert data["command"] == "START_CAPTURE"


async def test_handle_pi_message_no_browsers_does_not_raise() -> None:
    await handle_pi_message('{"type":"response","kind":"DONE","command":"START_CAPTURE"}', [])


async def test_handle_pi_message_state_forwarded() -> None:
    b = FakeBrowser()
    await handle_pi_message('{"type":"state","state":"READY"}', [b])
    data = json.loads(b.sent[0])
    assert data["type"] == "state"
    assert data["state"] == "READY"
