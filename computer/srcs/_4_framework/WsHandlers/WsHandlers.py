from __future__ import annotations

import asyncio
from typing import Awaitable, Callable

import websockets
from websockets.asyncio.server import ServerConnection

BrowserHandler = Callable[[ServerConnection], Awaitable[None]]
PiHandler = Callable[[ServerConnection], Awaitable[None]]


async def run_ws_server(
    port: int,
    pi_port: int,
    browser_handler: BrowserHandler,
    pi_handler: PiHandler,
) -> None:
    async with (
        websockets.serve(browser_handler, "", port),
        websockets.serve(pi_handler, "", pi_port, ping_interval=None),
    ):
        await asyncio.Future()
