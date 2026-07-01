from __future__ import annotations

import asyncio
import json
from typing import AsyncIterator

from srcs._1_domain.System import State
from srcs._2_usecases.HandleConnection_UC import handle_connection
from srcs._2_usecases.HandleIncoming_UC import handle_incoming


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


class FakeRunScan:
    def __init__(self) -> None:
        self.started: list[str] = []
        self.resumed: list[str] = []

    async def start(self, name: str, pi: object) -> None:
        self.started.append(name)

    async def resume(self, name: str, pi: object) -> None:
        self.resumed.append(name)


async def test_handle_incoming_replies_pong_on_ping() -> None:
    connection = FakeConnection()
    await handle_incoming(connection, '{"type": "ping"}')

    assert len(connection.sent) == 1
    data = json.loads(connection.sent[0])
    assert data["type"] == "pong"


async def test_handle_connection_sends_status_on_connect() -> None:
    connection = FakeConnection()
    await handle_connection(connection)

    assert len(connection.sent) == 2
    pi_data = json.loads(connection.sent[0])
    status_data = json.loads(connection.sent[1])
    assert pi_data["type"] == "pi_connection"
    assert status_data["type"] == "status"
    assert status_data["state"] == "INITIALIZATION"
    assert "timestamp" in status_data


async def test_handle_connection_sends_runtime_state_on_connect() -> None:
    connection = FakeConnection()
    await handle_connection(connection, state_provider=lambda: State.READY)

    assert len(connection.sent) == 2
    assert json.loads(connection.sent[0])["type"] == "pi_connection"
    status_data = json.loads(connection.sent[1])
    assert status_data["type"] == "status"
    assert status_data["state"] == "READY"


async def test_handle_connection_handles_ping_messages() -> None:
    connection = FakeConnection(incoming=['{"type": "ping"}'])
    await handle_connection(connection)

    assert len(connection.sent) == 3
    assert json.loads(connection.sent[0])["type"] == "pi_connection"
    assert json.loads(connection.sent[1])["type"] == "status"
    assert json.loads(connection.sent[2])["type"] == "pong"


async def test_handle_incoming_acks_valid_command() -> None:
    pi = FakePi()
    for command in ("GET_STATUS", "GET_PICO_STATUS", "START_CAPTURE", "TAKE_PHOTO", "PING"):
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


async def test_handle_incoming_start_scan_acks() -> None:
    connection = FakeConnection()
    run_scan = FakeRunScan()
    pi = FakePi()
    await handle_incoming(
        connection,
        '{"type": "start_scan", "name": "my-scan"}',
        pi=pi,
        run_scan=run_scan,
    )
    await asyncio.sleep(0)
    assert any(json.loads(m)["type"] == "ack" for m in connection.sent)
    assert "my-scan" in run_scan.started


async def test_handle_incoming_start_scan_errors_without_pi() -> None:
    connection = FakeConnection()
    await handle_incoming(connection, '{"type": "start_scan", "name": "my-scan"}')
    data = json.loads(connection.sent[0])
    assert data["type"] == "error"


async def test_handle_incoming_start_scan_errors_on_invalid_name() -> None:
    connection = FakeConnection()
    await handle_incoming(
        connection,
        '{"type": "start_scan", "name": "bad name!"}',
        pi=FakePi(),
        run_scan=FakeRunScan(),
    )
    data = json.loads(connection.sent[0])
    assert data["type"] == "error"


async def test_handle_incoming_continue_scan_acks() -> None:
    connection = FakeConnection()
    run_scan = FakeRunScan()
    pi = FakePi()
    await handle_incoming(
        connection,
        '{"type": "continue_scan", "name": "my-scan"}',
        pi=pi,
        run_scan=run_scan,
    )
    await asyncio.sleep(0)
    assert any(json.loads(m)["type"] == "ack" for m in connection.sent)
    assert "my-scan" in run_scan.resumed


async def test_handle_connection_sends_scan_list_on_connect() -> None:
    connection = FakeConnection()
    scan_list_provider = lambda: [{"name": "s1", "status": "done", "position_index": 3, "shot_index": 10, "error": ""}]
    await handle_connection(connection, scan_list_provider=scan_list_provider)
    types = [json.loads(m)["type"] for m in connection.sent]
    assert "scan_list" in types
