from __future__ import annotations

from typing import Protocol

from srcs._3_interface.IPiTransport import IPiTransport


class IHandlePiMessage(Protocol):
    async def __call__(self, raw: str, browsers: list[IPiTransport]) -> None: ...
