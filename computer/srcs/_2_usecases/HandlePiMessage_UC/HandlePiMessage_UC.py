from __future__ import annotations

from srcs._1_domain.PiMessages import PiResponse
from srcs._3_interface.IPiTransport import IPiTransport


async def handle_pi_message(raw: str, browsers: list[IPiTransport]) -> None:
    response = PiResponse.parse(raw)
    for browser in browsers:
        await browser.send(response.to_json())
