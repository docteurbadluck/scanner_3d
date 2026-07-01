from __future__ import annotations

from typing import Callable

from srcs._2_usecases.HandleIncoming_UC.internal._dispatch import _dispatch
from srcs._2_usecases.HandleIncoming_UC.internal._parse_and_validate import _parse_and_validate
from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport
from srcs._3_interface.IRunScan import IRunScan


async def handle_incoming(
    connection: IConnection,
    raw: str,
    pi: IPiTransport | None = None,
    pi_provider: Callable[[], IPiTransport | None] | None = None,
    run_scan: IRunScan | None = None,
) -> None:
    data = await _parse_and_validate(connection, raw)
    if data is not None:
        await _dispatch(connection, data, pi, pi_provider, run_scan)
