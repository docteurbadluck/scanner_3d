from __future__ import annotations

from srcs._1_domain.Messages import PongMessage
from srcs._3_interface.IConnection import IConnection


async def _handle_ping(connection: IConnection) -> None:
    await connection.send(PongMessage().to_json())
