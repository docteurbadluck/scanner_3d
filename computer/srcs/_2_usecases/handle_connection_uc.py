from __future__ import annotations
from typing import Callable

from srcs._1_domain.messages import PiConnectionMessage, PicoStatusMessage, StatusMessage
from srcs._1_domain.system import State
from srcs._2_usecases.handle_incoming_uc import handle_incoming
from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport


async def _send_initial_status(
    connection: IConnection,
    pi_provider: Callable[[], IPiTransport | None] | None,
    state_provider: Callable[[], State] | None,
    pico_state_provider: Callable[[], str | None] | None,
) -> None:
    pi_connected = pi_provider is not None and pi_provider() is not None
    await connection.send(PiConnectionMessage(connected=pi_connected).to_json())
    current_state = state_provider() if state_provider is not None else State.INITIALIZATION
    await connection.send(StatusMessage.build(current_state).to_json())
    if pico_state_provider is not None:
        pico_state = pico_state_provider()
        if pico_state is not None:
            await connection.send(PicoStatusMessage(state=pico_state).to_json())


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
