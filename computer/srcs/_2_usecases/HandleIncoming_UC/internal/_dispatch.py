from __future__ import annotations

from typing import Any, Callable

from srcs._2_usecases.HandleIncoming_UC.internal._handle_command import _handle_command
from srcs._2_usecases.HandleIncoming_UC.internal._handle_continue_scan import _handle_continue_scan
from srcs._2_usecases.HandleIncoming_UC.internal._handle_ping import _handle_ping
from srcs._2_usecases.HandleIncoming_UC.internal._handle_start_scan import _handle_start_scan
from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport
from srcs._3_interface.IRunScan import IRunScan


async def _dispatch(
    connection: IConnection,
    data: dict[str, Any],
    pi: IPiTransport | None,
    pi_provider: Callable[[], IPiTransport | None] | None,
    run_scan: IRunScan | None,
) -> None:
    t = data.get("type", "")
    if t == "ping": await _handle_ping(connection)
    elif t == "command": await _handle_command(connection, data, pi, pi_provider)
    elif t == "start_scan":
        await _handle_start_scan(connection, data, pi, pi_provider, run_scan)
    elif t == "continue_scan":
        await _handle_continue_scan(connection, data, pi, pi_provider, run_scan)
