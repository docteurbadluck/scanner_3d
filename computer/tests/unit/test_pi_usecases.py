from __future__ import annotations

import json

import pytest

from srcs._1_domain.PiMessages import PiResponse, PiResponseKind
from srcs._2_usecases.ForwardCommandToPi_UC import forward_command_to_pi
from srcs._2_usecases.HandlePiMessage_UC import handle_pi_message


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
    ('{"type":"pico_status","state":"LISTENING"}',    PiResponseKind.PICO_STATUS, "LISTENING",    ""),
    ('{"type":"pico_status","state":"INTERPRETING"}', PiResponseKind.PICO_STATUS, "INTERPRETING", ""),
    ('{"type":"pico_status","state":"EXECUTING"}',    PiResponseKind.PICO_STATUS, "EXECUTING",    ""),
    ('{"type":"response","kind":"PONG","command":"PING_PICO","ms":42}', PiResponseKind.PONG, "PONG", "PING_PICO"),
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

def test_ping_pico_response_preserves_ms() -> None:
    r = PiResponse.parse('{"type":"response","kind":"PONG","command":"PING_PICO","ms":42}')
    assert r.ms == 42
    data = json.loads(r.to_json())
    assert data["ms"] == 42
    assert data["command"] == "PING_PICO"


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


async def test_handle_pi_message_pico_status_forwarded() -> None:
    b = FakeBrowser()
    await handle_pi_message('{"type":"pico_status","state":"LISTENING"}', [b])
    data = json.loads(b.sent[0])
    assert data["type"]  == "pico_status"
    assert data["state"] == "LISTENING"
