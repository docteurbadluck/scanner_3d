from __future__ import annotations

from typing import Protocol

from srcs._3_interface.IPiTransport import IPiTransport


class IRunScan(Protocol):
    async def start(self, name: str, pi: IPiTransport) -> None: ...
    async def resume(self, name: str, pi: IPiTransport) -> None: ...
