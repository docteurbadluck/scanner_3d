from __future__ import annotations

import json
from typing import AsyncIterator

from srcs._1_domain.system import State
from srcs._2_usecases.handle_connection_uc import handle_connection
from srcs._2_usecases.handle_incoming_uc import handle_incoming


class FakeConnection:
    def __init__(self, incoming: list[object] | None = None) -> None:
        self._incoming: list[object] = incoming or []
        self.sent: list[str] = []

    async def send(self, message: str) -> None:
        self.sent.append(message)

    def __aiter__(self) -> AsyncIterator[object]:
        return self._iter()

    async def _iter(self) -> AsyncIterator[object]:
        for message in self._incoming:
            yield message


class FakePi:
    def __init__(self) -> None:
        self.sent: list[str] = []

    async def send(self, message: str) -> None:
        self.sent.append(message)


async def test_handle_incoming_replies_pong_on_ping() -> None:
    connection = FakeConnection()
    await handle_incoming(connection, '{"type": "ping"}')

    assert len(connection.sent) == 1
    data = json.loads(connection.sent[0])
    assert data["type"] == "pong"


async def test_handle_connection_sends_status_on_connect() -> None:
    connection = FakeConnection()
    await handle_connection(connection)

    assert len(connection.sent) == 1
    data = json.loads(connection.sent[0])
    assert data["type"] == "status"
    assert data["state"] == "INITIALIZATION"
    assert "timestamp" in data


async def test_handle_connection_sends_runtime_state_on_connect() -> None:
    connection = FakeConnection()
    await handle_connection(connection, state_provider=lambda: State.READY)

    assert len(connection.sent) == 1
    data = json.loads(connection.sent[0])
    assert data["type"] == "status"
    assert data["state"] == "READY"


async def test_handle_connection_handles_ping_messages() -> None:
    connection = FakeConnection(incoming=['{"type": "ping"}'])
    await handle_connection(connection)

    assert len(connection.sent) == 2
    status_data = json.loads(connection.sent[0])
    pong_data = json.loads(connection.sent[1])
    assert status_data["type"] == "status"
    assert pong_data["type"] == "pong"


async def test_handle_incoming_acks_valid_command() -> None:
    pi = FakePi()
    for command in ("GET_STATUS", "GET_PICO_STATUS", "START_CAPTURE", "PING", "PONG"):
        connection = FakeConnection()
        await handle_incoming(connection, json.dumps({"type": "command", "command": command}), pi)

        assert len(connection.sent) == 1
        data = json.loads(connection.sent[0])
        assert data["type"] == "ack"
        assert data["command"] == command


async def test_handle_incoming_errors_when_pi_unavailable() -> None:
    connection = FakeConnection()
    await handle_incoming(connection, json.dumps({"type": "command", "command": "GET_STATUS"}))

    assert len(connection.sent) == 1
    data = json.loads(connection.sent[0])
    assert data["type"] == "error"
    assert data["reason"] == "Pi unavailable"


async def test_handle_incoming_errors_on_unknown_command() -> None:
    connection = FakeConnection()
    await handle_incoming(connection, '{"type": "command", "command": "EXPLODE"}')

    assert len(connection.sent) == 1
    data = json.loads(connection.sent[0])
    assert data["type"] == "error"
    assert "EXPLODE" in data["reason"]


async def test_handle_incoming_ignores_unknown_type() -> None:
    connection = FakeConnection()
    await handle_incoming(connection, '{"type": "unknown"}')
    assert len(connection.sent) == 0


async def test_handle_incoming_errors_on_invalid_json() -> None:
    connection = FakeConnection()
    await handle_incoming(connection, '{"type": "command",')

    assert len(connection.sent) == 1
    data = json.loads(connection.sent[0])
    assert data["type"] == "error"
    assert data["reason"] == "Invalid JSON message"


async def test_handle_incoming_errors_on_invalid_shape() -> None:
    connection = FakeConnection()
    await handle_incoming(connection, '["not-a-dict"]')

    assert len(connection.sent) == 1
    data = json.loads(connection.sent[0])
    assert data["type"] == "error"
    assert data["reason"] == "Invalid message shape"
