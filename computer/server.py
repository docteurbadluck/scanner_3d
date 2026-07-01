from __future__ import annotations

import asyncio
from pathlib import Path

from srcs._0_orchestration.RunScan import RunScan
from srcs._0_orchestration.ScanOrchestrator import ScanOrchestrator
from srcs._0_orchestration.Session import Session
from srcs._2_usecases.HandleConnection_UC import handle_connection
from srcs._2_usecases.HandlePiMessage_UC import handle_pi_message
from srcs._4_framework.HttpServer import start_http_server
from srcs._4_framework.ScanStore import ScanStore
from srcs._4_framework.WsHandlers import run_ws_server

WWW_DIR    = Path(__file__).parent / "www"
PHOTOS_DIR = WWW_DIR / "photos"
HTTP_PORT  = 8080
WS_PORT    = 8081
WS_PI_PORT = 8082


def main() -> None:
    session  = Session()
    store    = ScanStore(PHOTOS_DIR)
    run_scan = RunScan(session, store)
    orchestrator = ScanOrchestrator(
        session,
        handle_connection,
        handle_pi_message,
        run_scan=run_scan,
        scan_list_provider=store.list_all,
    )
    start_http_server(
        HTTP_PORT,
        WWW_DIR,
        pending_upload_provider=session.take_pending_upload_target,
    )
    asyncio.run(run_ws_server(WS_PORT, WS_PI_PORT, orchestrator.handle_browser, orchestrator.handle_pi))


if __name__ == "__main__":
    main()
