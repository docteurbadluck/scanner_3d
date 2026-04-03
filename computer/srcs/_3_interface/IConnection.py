from __future__ import annotations

from typing import AsyncIterator, Protocol, runtime_checkable


@runtime_checkable
class IConnection(Protocol):
    async def send(self, message: str) -> None:
        ...

    def __aiter__(self) -> AsyncIterator[object]:
        ...
