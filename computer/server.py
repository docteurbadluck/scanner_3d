from __future__ import annotations

import asyncio
import threading
from http.server import HTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
from typing import Any

import websockets
from websockets.asyncio.server import ServerConnection

from srcs._1_domain.messages import PiConnectionMessage
from srcs._1_domain.pi_messages import PiResponse, PiResponseKind
from srcs._1_domain.system import State
from srcs._2_usecases.handle_connection_uc import handle_connection
from srcs._2_usecases.handle_pi_message_uc import handle_pi_message

WWW_DIR    = Path(__file__).parent / "www"
HTTP_PORT  = 8080
WS_PORT    = 8081
WS_PI_PORT = 8082

# shared state: active browser connections and pi connection
_browsers: set[ServerConnection] = set()
_pi_connection: ServerConnection | None = None
_current_state: State = State.INITIALIZATION


def build_http_handler(www_dir: Path) -> type[SimpleHTTPRequestHandler]:
    class Handler(SimpleHTTPRequestHandler):
        def __init__(self, *args: Any, **kwargs: Any) -> None:
            super().__init__(*args, directory=str(www_dir), **kwargs)

        def log_message(self, *_args: Any) -> None:
            pass

    return Handler


def start_http_server(port: int, www_dir: Path) -> HTTPServer:
    handler: type[SimpleHTTPRequestHandler] = build_http_handler(www_dir)
    server = HTTPServer(("", port), handler)
    thread = threading.Thread(target=server.serve_forever, daemon=True)
    thread.start()
    return server


async def _handle_browser(websocket: ServerConnection) -> None:
    global _pi_connection, _current_state
    _browsers.add(websocket)
    try:
        await handle_connection(
            websocket,
            pi_provider=lambda: _pi_connection,
            state_provider=lambda: _current_state,
        )
    finally:
        _browsers.discard(websocket)


async def _notify_browsers(msg: str) -> None:
    for browser in list(_browsers):
        await browser.send(msg)


async def _handle_pi(websocket: ServerConnection) -> None:
    global _pi_connection, _current_state
    _pi_connection = websocket
    await _notify_browsers(PiConnectionMessage(connected=True).to_json())
    try:
        async for raw in websocket:
            parsed = PiResponse.parse(str(raw))
            if parsed.kind == PiResponseKind.STATE:
                try:
                    _current_state = State(parsed.payload)
                except ValueError:
                    pass
            await handle_pi_message(str(raw), list(_browsers))
    finally:
        _pi_connection = None
        await _notify_browsers(PiConnectionMessage(connected=False).to_json())


async def run_ws_server(port: int) -> None:
    async with (
        websockets.serve(_handle_browser, "", port),
        websockets.serve(_handle_pi, "", WS_PI_PORT, ping_interval=None),
    ):
        await asyncio.Future()


def main() -> None:
    start_http_server(HTTP_PORT, WWW_DIR)
    asyncio.run(run_ws_server(WS_PORT))


if __name__ == "__main__":
    main()
