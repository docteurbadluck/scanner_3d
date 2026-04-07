from __future__ import annotations

import json
from typing import Any, Callable

from srcs._1_domain.messages import AckMessage, CommandMessage, ErrorMessage, PongMessage
from srcs._2_usecases.forward_command_to_pi_uc import forward_command_to_pi
from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport


async def _parse_and_validate(connection: IConnection, raw: str) -> dict[str, Any] | None:
    try:
        decoded: Any = json.loads(raw)
    except json.JSONDecodeError:
        await connection.send(ErrorMessage("Invalid JSON message").to_json())
        return None
    if not isinstance(decoded, dict):
        await connection.send(ErrorMessage("Invalid message shape").to_json())
        return None
    return decoded


async def _handle_ping(connection: IConnection) -> None:
    await connection.send(PongMessage().to_json())


async def _send_command(connection: IConnection, active_pi: IPiTransport, command: str) -> None:
    cmd = CommandMessage.build(command)
    await forward_command_to_pi(active_pi, cmd.command)
    await connection.send(AckMessage(cmd.command).to_json())


async def _handle_command(
    connection: IConnection,
    data: dict[str, Any],
    pi: IPiTransport | None, pi_provider: Callable[[], IPiTransport | None] | None,
) -> None:
    command: str = str(data.get("command", ""))
    active_pi = pi_provider() if pi_provider is not None else pi
    try:
        if active_pi is None:
            raise ValueError("Pi unavailable")
        await _send_command(connection, active_pi, command)
    except ValueError as exc:
        await connection.send(ErrorMessage(str(exc)).to_json())
    except Exception as exc:
        await connection.send(ErrorMessage(f"Forward failed: {exc}").to_json())


async def handle_incoming(
    connection: IConnection,
    raw: str,
    pi: IPiTransport | None = None,
    pi_provider: Callable[[], IPiTransport | None] | None = None,
) -> None:
    data = await _parse_and_validate(connection, raw)
    if data is None:
        return
    msg_type: str = data.get("type", "")
    if msg_type == "ping":
        await _handle_ping(connection)
    elif msg_type == "command":
        await _handle_command(connection, data, pi, pi_provider)
