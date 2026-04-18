from __future__ import annotations

import asyncio
from pathlib import Path

from srcs._4_framework.http_server import start_http_server
from srcs._4_framework.session import Session
from srcs._4_framework.ws_handlers import run_ws_server

WWW_DIR    = Path(__file__).parent / "www"
HTTP_PORT  = 8080
WS_PORT    = 8081
WS_PI_PORT = 8082


def main() -> None:
    session = Session()
    start_http_server(HTTP_PORT, WWW_DIR)
    asyncio.run(run_ws_server(WS_PORT, WS_PI_PORT, session))


if __name__ == "__main__":
    main()
