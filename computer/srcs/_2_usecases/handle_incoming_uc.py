from __future__ import annotations

import json
from typing import Any, Callable

from srcs._1_domain.messages import AckMessage, CommandMessage, ErrorMessage, PongMessage
from srcs._2_usecases.forward_command_to_pi_uc import forward_command_to_pi
from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport


async def handle_incoming(
    connection: IConnection,
    raw: str,
    pi: IPiTransport | None = None,
    pi_provider: Callable[[], IPiTransport | None] | None = None,
) -> None:
    data: dict[str, Any] = json.loads(raw)
    msg_type: str = data.get("type", "")

    if msg_type == "ping":
        await connection.send(PongMessage().to_json())

    elif msg_type == "command":
        command: str = str(data.get("command", ""))
        try:
            cmd = CommandMessage.build(command)
            active_pi = pi_provider() if pi_provider is not None else pi
            if active_pi is None:
                await connection.send(ErrorMessage("Pi unavailable").to_json())
                return

            await forward_command_to_pi(active_pi, cmd.command)
            await connection.send(AckMessage(cmd.command).to_json())
        except ValueError as exc:
            await connection.send(ErrorMessage(str(exc)).to_json())
        except Exception as exc:
            await connection.send(ErrorMessage(f"Forward failed: {exc}").to_json())
