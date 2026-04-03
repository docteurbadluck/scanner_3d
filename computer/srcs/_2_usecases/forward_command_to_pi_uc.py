from __future__ import annotations

from srcs._3_interface.IPiTransport import IPiTransport


async def forward_command_to_pi(pi: IPiTransport, command: str) -> None:
    await pi.send(command)
