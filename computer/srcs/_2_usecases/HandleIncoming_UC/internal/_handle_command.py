from __future__ import annotations

from typing import Any, Callable

from srcs._1_domain.Messages import CommandMessage, ErrorMessage
from srcs._2_usecases.HandleIncoming_UC.internal._require_active_pi import _require_active_pi
from srcs._2_usecases.HandleIncoming_UC.internal._send_command import _send_command
from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport


async def _handle_command(
    connection: IConnection,
    data: dict[str, Any],
    pi: IPiTransport | None,
    pi_provider: Callable[[], IPiTransport | None] | None,
) -> None:
    command: str = str(data.get("command", ""))
    try:
        cmd = CommandMessage.build(command)
        active_pi = _require_active_pi(pi, pi_provider)
        await _send_command(connection, active_pi, cmd.command)
    except ValueError as exc:
        await connection.send(ErrorMessage(str(exc)).to_json())
    except Exception as exc:
        await connection.send(ErrorMessage(f"Forward failed: {exc}").to_json())
