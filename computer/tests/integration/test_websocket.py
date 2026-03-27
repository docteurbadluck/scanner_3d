from __future__ import annotations

import asyncio
import json
from typing import Any

import websockets

WS_URL = "ws://localhost:8081"


async def test_ws_accepts_connection() -> None:
    async with websockets.connect(WS_URL) as ws:
        assert ws is not None


async def test_ws_sends_status_on_connect() -> None:
    async with websockets.connect(WS_URL) as ws:
        message: str         = await asyncio.wait_for(ws.recv(), timeout=3)
        data: dict[str, Any] = json.loads(message)
        assert data["type"] == "status"
        assert "state" in data
        assert "timestamp" in data


async def test_ws_responds_to_ping() -> None:
    async with websockets.connect(WS_URL) as ws:
        await ws.recv()
        await ws.send(json.dumps({"type": "ping"}))
        response: str        = await asyncio.wait_for(ws.recv(), timeout=3)
        data: dict[str, Any] = json.loads(response)
        assert data["type"] == "pong"
