from __future__ import annotations

import asyncio
from typing import Any, Callable

from srcs._1_domain.Messages.Messages import AckMessage, ErrorMessage
from srcs._2_usecases.HandleIncoming_UC.internal._require_active_pi import _require_active_pi
from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport
from srcs._3_interface.IRunScan import IRunScan


async def _launch_continue_scan(
    connection: IConnection,
    name: str,
    pi: IPiTransport | None,
    pi_provider: Callable[[], IPiTransport | None] | None,
    run_scan: IRunScan | None,
) -> None:
    active_pi = _require_active_pi(pi, pi_provider)
    if run_scan is None:
        raise ValueError("Scan service unavailable")
    await connection.send(AckMessage("continue_scan").to_json())
    asyncio.create_task(run_scan.resume(name, active_pi))


async def _handle_continue_scan(
    connection: IConnection,
    data: dict[str, Any],
    pi: IPiTransport | None,
    pi_provider: Callable[[], IPiTransport | None] | None,
    run_scan: IRunScan | None,
) -> None:
    name: str = str(data.get("name", "")).strip()
    try:
        await _launch_continue_scan(connection, name, pi, pi_provider, run_scan)
    except ValueError as exc:
        await connection.send(ErrorMessage(str(exc)).to_json())
