from __future__ import annotations

from srcs._1_domain.Messages import AckMessage
from srcs._2_usecases.ForwardCommandToPi_UC import forward_command_to_pi
from srcs._3_interface.IConnection import IConnection
from srcs._3_interface.IPiTransport import IPiTransport


async def _send_command(connection: IConnection, active_pi: IPiTransport, command: str) -> None:
    await forward_command_to_pi(active_pi, command)
    await connection.send(AckMessage(command).to_json())
