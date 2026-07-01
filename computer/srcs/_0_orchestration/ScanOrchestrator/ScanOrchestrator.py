from __future__ import annotations

from typing import Any, Callable

from websockets.asyncio.server import ServerConnection

from srcs._1_domain.Messages import CommandMessage, PiConnectionMessage, _DebugMessage
from srcs._1_domain.PiMessages import PiResponse
from srcs._0_orchestration.ScanOrchestrator.internal._notify_browsers import _notify_browsers
from srcs._0_orchestration.ScanOrchestrator.internal._update_session import _update_session
from srcs._3_interface.IHandleConnection import IHandleConnection
from srcs._3_interface.IHandlePiMessage import IHandlePiMessage
from srcs._3_interface.IRunScan import IRunScan
from srcs._0_orchestration.Session import Session


class ScanOrchestrator:
    def __init__(
        self,
        session: Session,
        handle_connection: IHandleConnection,
        handle_pi_message: IHandlePiMessage,
        run_scan: IRunScan | None = None,
        scan_list_provider: Callable[[], list[Any]] | None = None,
    ) -> None:
        self._session = session
        self._handle_connection = handle_connection
        self._handle_pi_message = handle_pi_message
        self._run_scan = run_scan
        self._scan_list_provider = scan_list_provider

    async def handle_browser(self, websocket: ServerConnection) -> None:
        self._session.add_browser(websocket)
        try:
            await self._handle_connection(
                websocket,
                pi_provider=self._session.pi,
                state_provider=self._session.state,
                pico_state_provider=self._session.pico_state,
                run_scan=self._run_scan,
                scan_list_provider=self._scan_list_provider,
            )
        finally:
            self._session.remove_browser(websocket)

    async def handle_pi(self, websocket: ServerConnection) -> None:
        self._session.set_pi(websocket)
        await _notify_browsers(self._session, PiConnectionMessage(connected=True).to_json())
        await websocket.send(CommandMessage.build("GET_PICO_STATUS").to_json())
        try:
            async for raw in websocket:
                await _notify_browsers(self._session, _DebugMessage(str(raw)).to_json())
                parsed = PiResponse.parse(str(raw))
                await _update_session(self._session, parsed)
                await self._handle_pi_message(str(raw), self._session.browsers())
        finally:
            self._session.set_pi(None)
            self._session.set_pico_state(None)
            await _notify_browsers(self._session, PiConnectionMessage(connected=False).to_json())
