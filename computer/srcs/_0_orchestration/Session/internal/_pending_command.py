from __future__ import annotations

import asyncio

from srcs._1_domain.PiMessages import PiResponse, PiResponseKind

_RESOLVABLE: frozenset[PiResponseKind] = frozenset({PiResponseKind.DONE, PiResponseKind.FAIL})


class PendingCommand:
    def __init__(self) -> None:
        self._command: str | None = None
        self._future: asyncio.Future[PiResponse] | None = None

    def arm(self, command: str) -> asyncio.Future[PiResponse]:
        self._future = asyncio.get_event_loop().create_future()
        self._command = command
        return self._future

    def resolve_if_match(self, response: PiResponse) -> bool:
        if self._future is None or self._future.done():
            return False
        if response.command != self._command:
            return False
        if response.kind not in _RESOLVABLE:
            return False
        self._future.set_result(response)
        return True
