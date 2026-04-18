from __future__ import annotations

import asyncio

import websockets
from websockets.asyncio.server import ServerConnection

from srcs._1_domain.messages import CommandMessage, PiConnectionMessage, _DebugMessage
from srcs._1_domain.pi_messages import PiResponse, PiResponseKind
from srcs._1_domain.system import State
from srcs._2_usecases.handle_connection_uc import handle_connection
from srcs._2_usecases.handle_pi_message_uc import handle_pi_message
from srcs._4_framework.session import Session


async def _notify_browsers(session: Session, msg: str) -> None:
    for browser in session.browsers():
        await browser.send(msg)


async def handle_browser(websocket: ServerConnection, session: Session) -> None:
    session.add_browser(websocket)
    try:
        await handle_connection(
            websocket,
            pi_provider=session.pi,
            state_provider=session.state,
            pico_state_provider=session.pico_state,
        )
    finally:
        session.remove_browser(websocket)


async def _update_session_from_pi(session: Session, parsed: PiResponse) -> None:
    if parsed.kind == PiResponseKind.STATE:
        try:
            session.set_state(State(parsed.payload))
        except ValueError:
            pass
    if parsed.kind == PiResponseKind.PICO_STATUS:
        session.set_pico_state(parsed.payload)


async def handle_pi(websocket: ServerConnection, session: Session) -> None:
    session.set_pi(websocket)
    await _notify_browsers(session, PiConnectionMessage(connected=True).to_json())
    await websocket.send(CommandMessage.build("GET_PICO_STATUS").to_json())
    try:
        async for raw in websocket:
            await _notify_browsers(session, _DebugMessage(str(raw)).to_json())
            parsed = PiResponse.parse(str(raw))
            await _update_session_from_pi(session, parsed)
            await handle_pi_message(str(raw), session.browsers())
    finally:
        session.set_pi(None)
        session.set_pico_state(None)
        await _notify_browsers(session, PiConnectionMessage(connected=False).to_json())


async def run_ws_server(port: int, pi_port: int, session: Session) -> None:
    async with (
        websockets.serve(lambda ws: handle_browser(ws, session), "", port),
        websockets.serve(lambda ws: handle_pi(ws, session), "", pi_port, ping_interval=None),
    ):
        await asyncio.Future()
