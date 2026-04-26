from __future__ import annotations

from typing import Callable

from srcs._2_usecases.HandleConnection_UC.internal._send_initial_status import _send_initial_status
from srcs._2_usecases.HandleIncoming_UC import handle_incoming
from srcs._1_domain.System import State
from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport


async def handle_connection(
    connection: IConnection,
    pi: IPiTransport | None = None,
    pi_provider: Callable[[], IPiTransport | None] | None = None,
    state_provider: Callable[[], State] | None = None,
    pico_state_provider: Callable[[], str | None] | None = None,
) -> None:
    await _send_initial_status(connection, pi_provider, state_provider, pico_state_provider)
    async for message in connection:
        await handle_incoming(connection, str(message), pi, pi_provider)
