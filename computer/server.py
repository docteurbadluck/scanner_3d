from __future__ import annotations

import asyncio
from pathlib import Path

from srcs._0_orchestration.ScanOrchestrator import ScanOrchestrator
from srcs._2_usecases.HandleConnection_UC import handle_connection
from srcs._2_usecases.HandlePiMessage_UC import handle_pi_message
from srcs._4_framework.HttpServer import start_http_server
from srcs._0_orchestration.Session import Session
from srcs._4_framework.WsHandlers import run_ws_server

WWW_DIR    = Path(__file__).parent / "www"
HTTP_PORT  = 8080
WS_PORT    = 8081
WS_PI_PORT = 8082


def main() -> None:
    session      = Session()
    orchestrator = ScanOrchestrator(session, handle_connection, handle_pi_message)
    start_http_server(HTTP_PORT, WWW_DIR)
    asyncio.run(run_ws_server(WS_PORT, WS_PI_PORT, orchestrator.handle_browser, orchestrator.handle_pi))


if __name__ == "__main__":
    main()
