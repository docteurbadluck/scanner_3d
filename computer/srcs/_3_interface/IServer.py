from __future__ import annotations

from typing import Protocol, runtime_checkable


@runtime_checkable
class IServer(Protocol):
    def start(self) -> None:
        ...

    def stop(self) -> None:
        ...
