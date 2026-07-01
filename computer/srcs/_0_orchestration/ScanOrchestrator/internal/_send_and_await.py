from __future__ import annotations

import asyncio

from srcs._0_orchestration.Session import Session
from srcs._1_domain.Messages import CommandMessage
from srcs._1_domain.PiMessages import PiResponse
from srcs._3_interface.IPiTransport import IPiTransport


class CommandTimeout(Exception):
    pass


async def send_and_await(
    session: Session,
    pi: IPiTransport,
    command: str,
    timeout_s: float,
) -> PiResponse:
    future = session.pending().arm(command)
    await pi.send(CommandMessage.build(command).to_json())
    try:
        return await asyncio.wait_for(future, timeout=timeout_s)
    except asyncio.TimeoutError as exc:
        raise CommandTimeout(command) from exc
