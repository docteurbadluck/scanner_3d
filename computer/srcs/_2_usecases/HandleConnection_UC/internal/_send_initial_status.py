from __future__ import annotations

from typing import Any, Callable

from srcs._1_domain.Messages import PiConnectionMessage, PicoStatusMessage, StatusMessage
from srcs._1_domain.Messages.Messages import ScanListMessage
from srcs._1_domain.Scan.Scan import ScanState, scan_state_from_dict
from srcs._1_domain.System import State
from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport


async def _send_pico_status(
    connection: IConnection,
    pico_state_provider: Callable[[], str | None] | None,
) -> None:
    if pico_state_provider is None:
        return
    pico_state = pico_state_provider()
    if pico_state is not None:
        await connection.send(PicoStatusMessage(state=pico_state).to_json())


async def _send_scan_list(
    connection: IConnection,
    scan_list_provider: Callable[[], list[Any]] | None,
) -> None:
    if scan_list_provider is None:
        return
    scans: list[ScanState] = [scan_state_from_dict(d) for d in scan_list_provider()]
    await connection.send(ScanListMessage(scans).to_json())


async def _send_initial_status(
    connection: IConnection,
    pi_provider: Callable[[], IPiTransport | None] | None,
    state_provider: Callable[[], State] | None,
    pico_state_provider: Callable[[], str | None] | None,
    scan_list_provider: Callable[[], list[Any]] | None = None,
) -> None:
    pi_connected = pi_provider is not None and pi_provider() is not None
    await connection.send(PiConnectionMessage(connected=pi_connected).to_json())
    current_state = state_provider() if state_provider is not None else State.INITIALIZATION
    await connection.send(StatusMessage.build(current_state).to_json())
    await _send_pico_status(connection, pico_state_provider)
    await _send_scan_list(connection, scan_list_provider)
