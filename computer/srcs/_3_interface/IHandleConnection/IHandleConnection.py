from __future__ import annotations

from typing import Callable, Protocol

from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport
from srcs._3_interface.IRunScan import IRunScan


class IHandleConnection(Protocol):
    async def __call__(
        self,
        connection: IConnection,
        pi: IPiTransport | None = ...,
        pi_provider: Callable[[], IPiTransport | None] | None = ...,
        state_provider: Callable[[], object] | None = ...,
        pico_state_provider: Callable[[], str | None] | None = ...,
        run_scan: IRunScan | None = ...,
        scan_list_provider: Callable[[], object] | None = ...,
    ) -> None: ...
