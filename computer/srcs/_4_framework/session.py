from __future__ import annotations

from websockets.asyncio.server import ServerConnection

from srcs._1_domain.system import State


class Session:
    def __init__(self) -> None:
        self._browsers: set[ServerConnection] = set()
        self._pi: ServerConnection | None = None
        self._state: State = State.INITIALIZATION
        self._pico_state: str | None = None

    def add_browser(self, ws: ServerConnection) -> None:
        self._browsers.add(ws)

    def remove_browser(self, ws: ServerConnection) -> None:
        self._browsers.discard(ws)

    def browsers(self) -> list[ServerConnection]:
        return list(self._browsers)

    def set_pi(self, ws: ServerConnection | None) -> None:
        self._pi = ws

    def pi(self) -> ServerConnection | None:
        return self._pi

    def set_state(self, state: State) -> None:
        self._state = state

    def state(self) -> State:
        return self._state

    def set_pico_state(self, pico_state: str | None) -> None:
        self._pico_state = pico_state

    def pico_state(self) -> str | None:
        return self._pico_state
