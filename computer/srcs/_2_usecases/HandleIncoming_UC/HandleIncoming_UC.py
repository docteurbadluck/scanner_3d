from __future__ import annotations

from typing import Any, Callable

from srcs._2_usecases.HandleIncoming_UC.internal._handle_command import _handle_command
from srcs._2_usecases.HandleIncoming_UC.internal._handle_ping import _handle_ping
from srcs._2_usecases.HandleIncoming_UC.internal._parse_and_validate import _parse_and_validate
from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport


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
