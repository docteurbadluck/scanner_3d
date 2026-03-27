from __future__ import annotations

import asyncio
import json
import threading
from datetime import datetime, timezone
from http.server import HTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
from typing import Any

import websockets
from websockets.asyncio.server import ServerConnection

WWW_DIR   = Path(__file__).parent / "www"
HTTP_PORT = 8080
WS_PORT   = 8081


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


def make_status_message() -> str:
    now: str = datetime.now(timezone.utc).isoformat()
    return json.dumps({"type": "status", "state": "INITIALIZATION", "timestamp": now})


def make_pong_message() -> str:
    return json.dumps({"type": "pong"})


async def handle_incoming(websocket: ServerConnection, raw: str) -> None:
    data: dict[str, Any] = json.loads(raw)
    if data.get("type") == "ping":
        await websocket.send(make_pong_message())


async def handle_connection(websocket: ServerConnection) -> None:
    await websocket.send(make_status_message())
    async for message in websocket:
        await handle_incoming(websocket, str(message))


async def run_ws_server(port: int) -> None:
    async with websockets.serve(handle_connection, "", port):
        await asyncio.Future()


def main() -> None:
    start_http_server(HTTP_PORT, WWW_DIR)
    asyncio.run(run_ws_server(WS_PORT))


if __name__ == "__main__":
    main()
