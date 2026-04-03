from __future__ import annotations
from typing import Callable

from srcs._1_domain.messages import StatusMessage
from srcs._1_domain.system import State
from srcs._2_usecases.handle_incoming_uc import handle_incoming
from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport


async def handle_connection(
    connection: IConnection,
    pi: IPiTransport | None = None,
    pi_provider: Callable[[], IPiTransport | None] | None = None,
    state_provider: Callable[[], State] | None = None,
) -> None:
    current_state = state_provider() if state_provider is not None else State.INITIALIZATION
    await connection.send(StatusMessage.build(current_state).to_json())
    async for message in connection:
        await handle_incoming(connection, str(message), pi, pi_provider)
